/**
 * Binocular Stereo Vision Realization based on MicroVision Cameras.
 * @filename:	MSVisionDLL.cpp
 * @author:		Sny
 * @version:	1.0
 * @since:		2018-12-12
 */

// INCLUDES
#include "stdafx.h"
#include "MSVisionDLL.h"

namespace ms
{
	MSInfoCode readBinoCalibParams(
		const std::string &fileName,
		cv::Mat (&intrinsics)[2],
		cv::Mat (&distCoeffs)[2],
		cv::Mat &R,
		cv::Mat &t)
	{
		cv::FileStorage fs(fileName, cv::FileStorage::READ);
		if (!fs.isOpened())
		{
			return MS_OPEN_FILE_ERROR;
		}
		fs["Intrinsics_Camera_Left"] >> intrinsics[0];
		fs["Distortion_Left"] >> distCoeffs[0];
		fs["Intrinsics_Camera_Right"] >> intrinsics[1];
		fs["Distortion_Right"] >> distCoeffs[1];
		fs["Rotated_Matrix"] >> R;
		fs["Translation"] >> t;
		fs.release();
		return MS_SUCCESS;
	}

	static MSInfoCode cvt2gray(
		const cv::Mat &src,
		cv::Mat &dst)
	{
		if (src.empty())
		{
			return  MS_ARGUMENT_ERROR;
		}
		if (src.channels() == 3)
		{
			cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
		}
		else if (src.channels() == 1)
		{
			dst = src.clone();
		}
		else
		{
			return MS_IMG_TYPE_ERROR;
		}
		return MS_SUCCESS;
	}

	static MSInfoCode getThresholds(
		const cv::Mat &src,
		double &threshold1,
		double &threshold2)
	{
		// Convert to gray image
		cv::Mat grayImg;
		MSInfoCode status = cvt2gray(src, grayImg);
		if (status != MS_SUCCESS)
		{
			return status;
		}

		// Get histogram of src
		cv::MatND dstHist;
		int channels = 0;
		int dims = 1;
		int size = 256;
		float hranges[] = { 0, 255 };
		const float *ranges[] = { hranges };
		cv::calcHist(&grayImg, 1, &channels, cv::Mat(), dstHist, dims, &size, ranges);

		// Compute prob[] and uT
		float sum = 0;
		for (int i = 0; i < 256; i++)
		{
			sum += dstHist.at<float>(i);
		}
		if (sum == 0)
		{
			return MS_DIVIDE_ZERO_ERROR;
		}
		std::vector<float> prob(256, 0);
		float uT = 0;
		for (int i = 0; i < 256; i++)
		{
			// By default, values greater than zero are returned here
			prob[i] = dstHist.at<float>(i) / sum;
			uT += i * prob[i];
		}

		// Compute thresholds
		float w[3] = { 0 };
		float u_[3] = { 0 };
		float u[3] = { 0 };
		float maxSigma2 = 0;
		for (int i = 0; i < 255; i++)
		{
			w[0] += prob[i];
			u_[0] += i * prob[i];
			if (w[0] != 0)
			{
				u[0] = u_[0] / w[0];
			}
			else
			{
				u[0] = 0;
			}
			for (int j = i + 1; j < 256; j++)
			{
				w[1] += prob[j];
				u_[1] += j * prob[j];
				if (w[1] != 0)
				{
					u[1] = u_[1] / w[1];
				}
				else
				{
					u[1] = 0;
				}
				w[2] = 1 - w[0] - w[1];
				u_[2] = uT - u_[0] - u_[1];
				if (w[2] != 0)
				{
					u[2] = u_[2] / w[2];
				}
				else
				{
					u[2] = 0;
				}
				float sigma2 = w[0] * (u[0] - uT) * (u[0] - uT)
					+ w[1] * (u[1] - uT) * (u[1] - uT)
					+ w[2] * (u[2] - uT) * (u[2] - uT);
				if (sigma2 > maxSigma2 && i > 0 && j < 255)
				{
					maxSigma2 = sigma2;
					threshold1 = (double)i;
					threshold2 = (double)j;
				}
			}
		}
		return MS_SUCCESS;
	}

	static void imgLinearTrans(
		const cv::Mat &src,
		cv::Mat &dst,
		float alpha,
		float beta)
	{
		cv::Mat lut(1, 256, CV_8U);
		uchar* pLut = lut.data;
		for (int i = 0; i < 256; i++)
		{
			pLut[i] = cv::saturate_cast<uchar>((float)i * alpha + beta);
		}
		cv::LUT(src, lut, dst);
	}

	static void imgGammaTrans(
		const cv::Mat &src,
		cv::Mat &dst,
		double fGamma)
	{
		cv::Mat lut(1, 256, CV_8U);
		uchar* pLut = lut.data;
		for (int i = 0; i < 256; i++)
		{
			pLut[i] = cv::saturate_cast<uchar>(pow((double)i / 255.0, fGamma) * 255.0);
		}
		cv::LUT(src, lut, dst);
	}

	static MSInfoCode adjustBright(
		const cv::Mat &src,
		cv::Mat &dst,
		float refBright)
	{
		// Conver to gray image
		cv::Mat grayImg;
		MSInfoCode status = cvt2gray(src, grayImg);
		if (status != MS_SUCCESS)
		{
			return status;
		}

		// Adjustment by linear method
		float sumBright = 0;
		for (int j = 0; j < grayImg.rows; j++) {
			uchar* pGray = grayImg.ptr<uchar>(j);
			for (int i = 0; i < grayImg.cols; i++) {
				sumBright += pGray[i];
			}
		}
		float aveBright = sumBright / (grayImg.rows * grayImg.cols);
		imgLinearTrans(src, dst, 1.f, refBright - aveBright);
		return MS_SUCCESS;
	}

	static float getFocusEntropy(
		const cv::Mat &src)
	{
		cv::Mat grayImg;
		if (src.channels() == 3)
		{
			cvtColor(src, grayImg, cv::COLOR_BGR2GRAY);
		}
		else if (src.channels() == 1)
		{
			grayImg = src.clone();
		}

		// Select horizontal and vertical pixels
		std::vector<cv::Mat> tempHoriImgs(2, cv::Mat(grayImg.rows, grayImg.cols / 2, CV_8UC1));
		std::vector<cv::Mat> tempVertImgs(2, cv::Mat(grayImg.rows / 2, grayImg.cols, CV_8UC1));
		cv::Mat mergedHoriImg = cv::Mat(grayImg.rows, grayImg.cols / 2, CV_8UC2);
		cv::Mat mergedVertImg = cv::Mat(grayImg.rows / 2, grayImg.cols, CV_8UC2);
		for (int j = 0; j < grayImg.rows; j++)
		{
			uchar* pGray = grayImg.ptr<uchar>(j);
			uchar* pHori1 = tempHoriImgs[0].ptr<uchar>(j);
			uchar* pHori2 = tempHoriImgs[1].ptr<uchar>(j);
			uchar* pVert1 = tempVertImgs[0].ptr<uchar>(j / 2);
			uchar* pVert2 = tempVertImgs[1].ptr<uchar>(j / 2);
			for (int i = 0; i < grayImg.cols; i++)
			{
				if (0 == i % 2)
				{
					pHori1[i / 2] = pGray[i];
				}
				else
				{
					pHori2[i / 2] = pGray[i];
				}
				if (0 == j % 2)
				{
					pVert1[i] = pGray[i];
				}
				else
				{
					pVert2[i] = pGray[i];
				}
			}
		}
		cv::merge(tempHoriImgs, mergedHoriImg);
		cv::merge(tempVertImgs, mergedVertImg);

		// Compute 2D histogram
		int level = 256;
		int histSize[] = { level, level };
		float histRange[] = { 0, 256 };
		const float* ranges[] = { histRange, histRange };
		cv::MatND dstHoriHist;
		cv::MatND dstVertHist;
		int channels[] = { 0, 1 };
		cv::calcHist(&mergedHoriImg, 1, channels, cv::Mat(), dstHoriHist, 2, histSize, ranges, true, false);
		cv::calcHist(&mergedVertImg, 1, channels, cv::Mat(), dstVertHist, 2, histSize, ranges, true, false);

		// Compute focus entropy
		float denomHori = cv::saturate_cast<float>(mergedHoriImg.rows * mergedHoriImg.cols);
		float denomVert = cv::saturate_cast<float>(mergedVertImg.rows * mergedVertImg.cols);
		float focusEntropy = 0;
		for (int i = 0; i < level; i++)
		{
			for (int j = 0; j < level; j++)
			{
				float binHoriValue = dstHoriHist.at<float>(i, j);
				float binVertValue = dstVertHist.at<float>(i, j);
				if (binHoriValue > 0)
				{
					float probHori = binHoriValue / denomHori;
					focusEntropy -= probHori * log(probHori);
				}
				if (binVertValue > 0)
				{
					float probVert = binVertValue / denomVert;
					focusEntropy -= probVert * log(probVert);
				}
			}
		}
		return focusEntropy;
	}

	static MSInfoCode zernikeDetect(
		const cv::Mat &src,
		cv::Mat &dst)
	{
		// Parameters for Zernike detection
		const int gN = 7;
		cv::Mat M00 = (cv::Mat_<double>(7, 7) <<
			0, 0.0287, 0.0686, 0.0807, 0.0686, 0.0287, 0,
			0.0287, 0.0815, 0.0816, 0.0816, 0.0816, 0.0815, 0.0287,
			0.0686, 0.0816, 0.0816, 0.0816, 0.0816, 0.0816, 0.0686,
			0.0807, 0.0816, 0.0816, 0.0816, 0.0816, 0.0816, 0.0807,
			0.0686, 0.0816, 0.0816, 0.0816, 0.0816, 0.0816, 0.0686,
			0.0287, 0.0815, 0.0816, 0.0816, 0.0816, 0.0815, 0.0287,
			0, 0.0287, 0.0686, 0.0807, 0.0686, 0.0287, 0);

		cv::Mat M11R = (cv::Mat_<double>(7, 7) <<
			0, -0.015, -0.019, 0, 0.019, 0.015, 0,
			-0.0224, -0.0466, -0.0233, 0, 0.0233, 0.0466, 0.0224,
			-0.0573, -0.0466, -0.0233, 0, 0.0233, 0.0466, 0.0573,
			-0.069, -0.0466, -0.0233, 0, 0.0233, 0.0466, 0.069,
			-0.0573, -0.0466, -0.0233, 0, 0.0233, 0.0466, 0.0573,
			-0.0224, -0.0466, -0.0233, 0, 0.0233, 0.0466, 0.0224,
			0, -0.015, -0.019, 0, 0.019, 0.015, 0);

		cv::Mat M11I = (cv::Mat_<double>(7, 7) <<
			0, -0.0224, -0.0573, -0.069, -0.0573, -0.0224, 0,
			-0.015, -0.0466, -0.0466, -0.0466, -0.0466, -0.0466, -0.015,
			-0.019, -0.0233, -0.0233, -0.0233, -0.0233, -0.0233, -0.019,
			0, 0, 0, 0, 0, 0, 0,
			0.019, 0.0233, 0.0233, 0.0233, 0.0233, 0.0233, 0.019,
			0.015, 0.0466, 0.0466, 0.0466, 0.0466, 0.0466, 0.015,
			0, 0.0224, 0.0573, 0.069, 0.0573, 0.0224, 0);

		cv::Mat M20 = (cv::Mat_<double>(7, 7) <<
			0, 0.0225, 0.0394, 0.0396, 0.0394, 0.0225, 0,
			0.0225, 0.0271, -0.0128, -0.0261, -0.0128, 0.0271, 0.0225,
			0.0394, -0.0128, -0.0528, -0.0661, -0.0528, -0.0128, 0.0394,
			0.0396, -0.0261, -0.0661, -0.0794, -0.0661, -0.0261, 0.0396,
			0.0394, -0.0128, -0.0528, -0.0661, -0.0528, -0.0128, 0.0394,
			0.0225, 0.0271, -0.0128, -0.0261, -0.0128, 0.0271, 0.0225,
			0, 0.0225, 0.0394, 0.0396, 0.0394, 0.0225, 0);

		cv::Mat M31R = (cv::Mat_<double>(7, 7) <<
			0, -0.0103, -0.0073, 0, 0.0073, 0.0103, 0,
			-0.0153, -0.0018, 0.0162, 0, -0.0162, 0.0018, 0.0153,
			-0.0223, 0.0324, 0.0333, 0, -0.0333, -0.0324, 0.0223,
			-0.0190, 0.0438, 0.0390, 0, -0.0390, -0.0438, 0.0190,
			-0.0223, 0.0324, 0.0333, 0, -0.0333, -0.0324, 0.0223,
			-0.0153, -0.0018, 0.0162, 0, -0.0162, 0.0018, 0.0153,
			0, -0.0103, -0.0073, 0, 0.0073, 0.0103, 0);

		cv::Mat M31I = (cv::Mat_<double>(7, 7) <<
			0, -0.0153, -0.0223, -0.019, -0.0223, -0.0153, 0,
			-0.0103, -0.0018, 0.0324, 0.0438, 0.0324, -0.0018, -0.0103,
			-0.0073, 0.0162, 0.0333, 0.039, 0.0333, 0.0162, -0.0073,
			0, 0, 0, 0, 0, 0, 0,
			0.0073, -0.0162, -0.0333, -0.039, -0.0333, -0.0162, 0.0073,
			0.0103, 0.0018, -0.0324, -0.0438, -0.0324, 0.0018, 0.0103,
			0, 0.0153, 0.0223, 0.0190, 0.0223, 0.0153, 0);

		cv::Mat M40 = (cv::Mat_<double>(7, 7) <<
			0, 0.013, 0.0056, -0.0018, 0.0056, 0.013, 0,
			0.0130, -0.0186, -0.0323, -0.0239, -0.0323, -0.0186, 0.0130,
			0.0056, -0.0323, 0.0125, 0.0406, 0.0125, -0.0323, 0.0056,
			-0.0018, -0.0239, 0.0406, 0.0751, 0.0406, -0.0239, -0.0018,
			0.0056, -0.0323, 0.0125, 0.0406, 0.0125, -0.0323, 0.0056,
			0.0130, -0.0186, -0.0323, -0.0239, -0.0323, -0.0186, 0.0130,
			0, 0.013, 0.0056, -0.0018, 0.0056, 0.013, 0);

		cv::Mat grayImg;
		MSInfoCode status = cvt2gray(src, grayImg);
		if (status != MS_SUCCESS)
		{
			return status;
		}

		// Get Zernike moments
		cv::Mat zerImgM00;
		filter2D(grayImg, zerImgM00, CV_64F, M00, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
		cv::Mat zerImgM11R;
		filter2D(grayImg, zerImgM11R, CV_64F, M11R, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
		cv::Mat zerImgM11I;
		filter2D(grayImg, zerImgM11I, CV_64F, M11I, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
		cv::Mat zerImgM20;
		filter2D(grayImg, zerImgM20, CV_64F, M20, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
		cv::Mat zerImgM31R;
		filter2D(grayImg, zerImgM31R, CV_64F, M31R, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
		cv::Mat zerImgM31I;
		filter2D(grayImg, zerImgM31I, CV_64F, M31I, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
		cv::Mat zerImgM40;
		filter2D(grayImg, zerImgM40, CV_64F, M40, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

		std::vector<cv::Point2d> subEdgePoints;
		int rowNum = grayImg.rows;
		int colNum = grayImg.cols;
		for (int i = 0; i < rowNum; i++)
		{
			for (int j = 0; j < colNum; j++)
			{
				if (zerImgM00.at<double>(i, j) == 0)
				{
					continue;
				}

				// Compute theta
				double tempTheta = atan2(zerImgM31I.at<double>(i, j), zerImgM31R.at<double>(i, j));

				// Compute Z11 and Z31
				double rotatedZ11 = 0;
				rotatedZ11 = sin(tempTheta) * zerImgM11I.at<double>(i, j) + cos(tempTheta) * zerImgM11R.at<double>(i, j);
				double rotatedZ31 = 0;
				rotatedZ31 = sin(tempTheta) * zerImgM31I.at<double>(i, j) + cos(tempTheta) * zerImgM31R.at<double>(i, j);

				// Compute l
				double lMethod1 = sqrt((5 * zerImgM40.at<double>(i, j) + 3 * zerImgM20.at<double>(i, j)) / (8 * zerImgM20.at<double>(i, j)));
				double lMethod2 = sqrt((5 * rotatedZ31 + rotatedZ11) / (6 * rotatedZ11));
				double l = (lMethod1 + lMethod2) / 2;

				// Compute k and h
				double k = 3 * rotatedZ11 / 2 / pow((1 - lMethod2 * lMethod2), 1.5);
				double h = (zerImgM00.at<double>(i, j) - k * CV_PI / 2 + k * asin(lMethod2) + k * lMethod2 * sqrt(1 - lMethod2 * lMethod2)) / CV_PI;

				// Judge the edge
				double kValue = 30.0;
				double lValue = sqrt(2) / gN;
				double absl = abs(lMethod2 - lMethod1);
				if (k >= kValue && absl <= lValue)
				{
					cv::Point2d tempPoint;
					tempPoint.x = j + gN * l * cos(tempTheta) / 2;
					tempPoint.y = i + gN * l * sin(tempTheta) / 2;
					subEdgePoints.push_back(tempPoint);
				}
				else
				{
					continue;
				}
			}
		}
		return MS_SUCCESS;
	}

	static MSInfoCode preprocess(
		const cv::Mat &src,
		cv::Mat &dst,
		int kernelSize,
		double fBilateral,
		int channelsFlag)
	{
		// Check parmeters
		if (src.empty())
		{
			return MS_ARGUMENT_ERROR;
		}
		if (kernelSize % 2 != 1 || kernelSize <= 1)
		{
			kernelSize = 3;
		}

		// Convert the color image to gray image
		if (src.channels() == 3)
		{
			// Merge the edge images
			std::vector<cv::Mat> bgrChannels(3);
			cv::split(src, bgrChannels);
			std::vector<cv::Mat> filterImgs(3);
			std::vector<cv::Mat> midImgs(3);
			std::vector<cv::Mat> edgeImgs(3);
			for (int k = 0; k < 3; k++)
			{
				cv::GaussianBlur(bgrChannels[k], filterImgs[k], cv::Size(kernelSize, kernelSize), 0, 0);
				cv::bilateralFilter(filterImgs[k], midImgs[k], static_cast<int>(fBilateral), fBilateral * 2, fBilateral / 2);
				double threshold1 = 0;
				double threshold2 = 0;
				getThresholds(midImgs[k], threshold1, threshold2);
				cv::Canny(midImgs[k], edgeImgs[k], threshold1, threshold2);
			}
#ifdef MS_DEBUG
			cv::imshow("bImg", edgeImgs[0]);
			cv::imshow("gImg", edgeImgs[1]);
			cv::imshow("rImg", edgeImgs[2]);
			cv::waitKey(0);
#endif
			cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
			bool colorsFlag[3] = { false };
			if (channelsFlag <= MS_DEFAULT_CHANNEL || channelsFlag >= 0x07)
			{
				colorsFlag[0] = true;
				colorsFlag[1] = true;
				colorsFlag[2] = true;
			}
			else
			{
				if (channelsFlag & MS_BLUE)
				{
					colorsFlag[0] = true;
				}
				if (channelsFlag & MS_GREEN)
				{
					colorsFlag[1] = true;
				}
				if (channelsFlag & MS_RED)
				{
					colorsFlag[2] = true;
				}
			}
			for (int j = 0; j < dst.rows; j++)
			{
				uchar* edgePtr = dst.ptr<uchar>(j);
				uchar* pixelPtrB = edgeImgs[0].ptr<uchar>(j);
				uchar* pixelPtrG = edgeImgs[1].ptr<uchar>(j);
				uchar* pixelPtrR = edgeImgs[2].ptr<uchar>(j);
				for (int i = 0; i < dst.cols; i++)
				{
					if ((colorsFlag[0] && pixelPtrB[i] > 0) ||
						(colorsFlag[1] && pixelPtrG[i] > 0) ||
						(colorsFlag[2] && pixelPtrR[i] > 0))
					{
						edgePtr[i] = 255;
					}
					else
					{
						edgePtr[i] = 0;
					}
				}
			}
#ifdef MS_DEBUG
			cv::imshow("Edge", dst);
			cv::waitKey(0);
#endif
		}
		else if (src.channels() == 1)
		{
			// Get edge image
			cv::Mat filterImg;
			cv::Mat midImg;
			cv::GaussianBlur(src, filterImg, cv::Size(kernelSize, kernelSize), 0, 0);
			cv::bilateralFilter(filterImg, midImg, static_cast<int>(fBilateral), fBilateral * 2, fBilateral / 2);
			double threshold1 = 0;
			double threshold2 = 0;
			getThresholds(midImg, threshold1, threshold2);
			cv::Canny(midImg, dst, threshold1, threshold2);
		}
		else
		{
			return MS_IMG_TYPE_ERROR;
		}
		return MS_SUCCESS;
	}

	static void getCompatiblePoints(
		const std::vector<cv::Point> &points,
		const cv::RotatedRect &rRect,
		std::vector<int> &coIndexes,
		float delta)
	{
		// Check whether coIndexes is empty
		if (!coIndexes.empty())
		{
			coIndexes.clear();
		}

		// Parameters of the ellipse
		float cosA = (float)cos(rRect.angle / 180 * CV_PI);
		float sinA = (float)sin(rRect.angle / 180 * CV_PI);
		float a = rRect.size.height / 2;
		float b = rRect.size.width / 2;

		// Record the index of compatible point
		for (size_t i = 0; i < points.size(); ++i)
		{
			float x = (points[i].x - rRect.center.x) * sinA - (points[i].y - rRect.center.y) * cosA;
			float y = (points[i].x - rRect.center.x) * cosA + (points[i].y - rRect.center.y) * sinA;
			float distance = pow(x / a, 2) + pow(y / b, 2) - 1;
			if (abs(distance) < delta)
			{
				coIndexes.push_back((int)i);
			}
		}
	}

	static void getCompatiblePoints(
		const cv::Mat &src,
		const cv::RotatedRect &rRect,
		std::vector<cv::Point> &coPoints,
		float delta)
	{
		// Check whether coPoints is empty
		if (!coPoints.empty())
		{
			coPoints.clear();
		}

		// Parameters of the ellipse
		float cosA = (float)cos(rRect.angle / 180.f * CV_PI);
		float sinA = (float)sin(rRect.angle / 180.f * CV_PI);
		float a = rRect.size.height / 2.f;
		float b = rRect.size.width / 2.f;

		// Record the compatible point
		for (int j = 0; j < src.rows; j++)
		{
			const uchar* pSrc = src.ptr<uchar>(j);
			for (int i = 0; i < src.cols; i++)
			{
				if (pSrc[i] > 127)
				{
					float x = (i - rRect.center.x) * sinA - (j - rRect.center.y) * cosA;
					float y = (i - rRect.center.x) * cosA + (j - rRect.center.y) * sinA;
					float distance = pow(x / a, 2) + pow(y / b, 2) - 1.f;
					if (abs(distance) < delta)
					{
						coPoints.push_back(cv::Point(i, j));
					}
				}
			}
		}
	}

	static int getCompatiblePoints(
		const std::vector<cv::Point> &points,
		const cv::RotatedRect &rRect,
		float delta)
	{
		// Parameters of the ellipse
		float cosA = (float)cos(rRect.angle / 180.f * CV_PI);
		float sinA = (float)sin(rRect.angle / 180.f * CV_PI);
		float a = rRect.size.height / 2.f;
		float b = rRect.size.width / 2.f;

		// Record the index of compatible point
		int pointNum = 0;
		for (size_t i = 0; i < points.size(); i++)
		{
			float x = (points[i].x - rRect.center.x) * sinA - (points[i].y - rRect.center.y) * cosA;
			float y = (points[i].x - rRect.center.x) * cosA + (points[i].y - rRect.center.y) * sinA;
			float distance = pow(x / a, 2) + pow(y / b, 2) - 1.f;
			if (abs(distance) < delta)
			{
				pointNum++;
			}
		}
		return pointNum;
	}

	static MSInfoCode traceEdge(
		const cv::Mat &src,
		std::vector<std::vector<cv::Point>> &edges)
	{
		// Check parameters
		if (!edges.empty())
		{
			edges.clear();
		}
		cv::Mat edgeImg;
		MSInfoCode status = cvt2gray(src, edgeImg);
		if (status != MS_SUCCESS)
		{
			return status;
		}

		// Directions for a pixel
		const cv::Point directions[8] =
		{ {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };

		// Algorithm
		std::vector<cv::Point> tempEdge;
		for (int j = 0; j < edgeImg.rows; j++)
		{
			uchar* edgePtr = edgeImg.ptr<uchar>(j);
			for (int i = 0; i < edgeImg.cols; i++)
			{
				if (edgePtr[i] > 0)
				{
					// Initialization
					tempEdge.clear();
					bool traceFlag = false;
					cv::Point curPoint = cv::Point(i, j);

					// Record the point, and set it zero
					tempEdge.push_back(curPoint);
					edgePtr[i] = 0;

					int curDir = 5;
					int count = 0;
					cv::Point tempPoint = curPoint;
					while (!traceFlag)
					{
						// Find one edge point
						for (count = 0; count < 8; count++)
						{
							curPoint = tempPoint + directions[curDir];
							if ((curPoint.x >= 0) && (curPoint.x <= edgeImg.cols - 1) &&
								(curPoint.y >= 0) && (curPoint.y <= edgeImg.rows - 1))
							{
								if (edgeImg.at<uchar>(curPoint) > 0)
								{
									tempEdge.push_back(curPoint);
									edgeImg.at<uchar>(curPoint) = 0;
									tempPoint = curPoint;
									curDir -= 2;
									break;
								}
							}
							curDir++;
							if (curDir < 0)
							{
								curDir += 8;
							}
							if (curDir > 7)
							{
								curDir -= 8;
							}
						}

						// Found an edge
						if (count >= 8)
						{
							curDir = 5;
							traceFlag = true;
							edges.push_back(tempEdge);
							break;
						}
					}
				}
			}
		}
		return MS_SUCCESS;
	}

	static void cutAtCorner(
		const std::vector<cv::Point> &curve,
		std::vector<std::vector<cv::Point>> &curves,
		int segNum = 10)
	{
		// Check parameters
		if (segNum <= 1)
		{
			segNum = 10;
		}
		if (!curves.empty())
		{
			curves.clear();
		}

		// Return when size of arc is too small
		if (static_cast<int>(curve.size()) <= segNum)
		{
			curves.push_back(curve);
			return;
		}

		// Cut the curve at corners
		double distThreshold = segNum / 5.0;
		int lastIndex = 0;
		int curIndex = 0;
		int step = 1;
		int curveSize = static_cast<int>(curve.size());
		while (curIndex + segNum <= curveSize)
		{
			cv::Point lineVec = curve[curIndex + segNum - 1] - curve[curIndex];
			double lineMag = sqrt(lineVec.ddot(lineVec));
			double cosTheta = (double)lineVec.x / lineMag;
			double sinTheta = (double)lineVec.y / lineMag;
			double maxDist = 0;
			int maxDistIndex = -1;
			for (int k = curIndex + 1; k < curIndex + segNum - 1; k++)
			{
				cv::Point curVec = curve[k] - curve[curIndex];
				double distance = abs(curVec.y * cosTheta - curVec.x * sinTheta);
				if (distance > maxDist)
				{
					maxDist = distance;
					maxDistIndex = k;
				}
			}
			if (maxDist > distThreshold)
			{
				std::vector<cv::Point> segTemp(curve.begin() + lastIndex, curve.begin() + maxDistIndex);
				curves.push_back(segTemp);
				lastIndex = maxDistIndex;
				curIndex = maxDistIndex;
			}
			else
			{
				curIndex += step;
			}
		}

		// Add last arc into the segments
		if (lastIndex < curveSize)
		{
			std::vector<cv::Point> segTemp(curve.begin() + lastIndex, curve.end());
			curves.push_back(segTemp);
		}
	}

	static void linkCurves(
		std::vector<std::vector<cv::Point>> &srcCurves,
		std::vector<std::vector<cv::Point>> &dstCurves,
		int segNum = 10,
		double distThreshold = 5.0,
		double angleThreshold = 15)
	{
		// Check parameters
		if (segNum <= 1)
		{
			segNum = 10;
		}
		if (!dstCurves.empty())
		{
			dstCurves.clear();
		}

		double dbDistThreshold = distThreshold * distThreshold;
		double curvThreshold = angleThreshold * CV_PI / 180;
		while (!srcCurves.empty())
		{
			// Compute parameters of current arc
			cv::Point curHeadPoint = srcCurves[0].front();
			cv::Point curTailPoint = srcCurves[0].back();
			double curHeadAngle = 0;
			double curTailAngle = 0;
			if ((int)srcCurves[0].size() <= segNum)
			{
				cv::Point curVec = curTailPoint - curHeadPoint;
				curHeadAngle = atan2(curVec.y, curVec.x);
				curTailAngle = curHeadAngle;
			}
			else
			{
				cv::Point curHeadVec = *(srcCurves[0].begin() + segNum) - curHeadPoint;
				cv::Point curTailVec = curTailPoint - *(srcCurves[0].end() - segNum);
				curHeadAngle = atan2(curHeadVec.y, curHeadVec.x);
				curTailAngle = atan2(curTailVec.y, curTailVec.x);
			}

			// Link a complete arc
			bool linkFlag = false;
			std::vector<cv::Point> tempArc = srcCurves[0];
			srcCurves.erase(srcCurves.begin());
			while (!linkFlag)
			{
				double minHeadCurv = CV_PI;
				double minTailCurv = CV_PI;
				double bestHeadAngle = 0;
				double bestTailAngle = 0;
				int bestHeadIndex = -1;
				int bestTailIndex = -1;
				// Compute all distances and directions
				for (size_t k = 0; k < srcCurves.size(); k++)
				{
					cv::Point tempHeadPoint = srcCurves[k].front();
					cv::Point tempTailPoint = srcCurves[k].back();
					cv::Point vecChTt = curHeadPoint - tempTailPoint;
					cv::Point vecChTh = curHeadPoint - tempHeadPoint;
					cv::Point vecCtTh = curTailPoint - tempHeadPoint;
					cv::Point vecCtTt = curTailPoint - tempTailPoint;
					double dbDistChTt = vecChTt.ddot(vecChTt);
					double dbDistChTh = vecChTh.ddot(vecChTh);
					double dbDistCtTh = vecCtTh.ddot(vecCtTh);
					double dbDistCtTt = vecCtTt.ddot(vecCtTt);

					// Reverse the vector of points when hh or tt
					if (dbDistChTh < dbDistThreshold || dbDistCtTt < dbDistThreshold)
					{
						std::reverse(srcCurves[k].begin(), srcCurves[k].end());
					}

					double tempHeadAngle = 0;
					double tempTailAngle = 0;
					if ((int)srcCurves[k].size() <= segNum)
					{
						cv::Point tempVec = srcCurves[k].back() - srcCurves[k].front();
						tempHeadAngle = atan2(tempVec.y, tempVec.x);
						tempTailAngle = tempHeadAngle;
					}
					else
					{
						cv::Point tempHeadVec = *(srcCurves[k].begin() + segNum) - srcCurves[k].front();
						cv::Point tempTailVec = srcCurves[k].back() - *(srcCurves[k].end() - segNum);
						tempHeadAngle = atan2(tempHeadVec.y, tempHeadVec.x);
						tempTailAngle = atan2(tempTailVec.y, tempTailVec.x);
					}

					if (dbDistChTt < dbDistThreshold || dbDistChTh < dbDistThreshold)
					{
						double diffAngleTtCh = curHeadAngle - tempTailAngle;
						if (diffAngleTtCh < -CV_PI)
						{
							diffAngleTtCh += 2 * CV_PI;
						}
						else if (diffAngleTtCh > CV_PI)
						{
							diffAngleTtCh -= 2 * CV_PI;
						}
						if (abs(diffAngleTtCh) < curvThreshold && abs(diffAngleTtCh) < minHeadCurv)
						{
							minHeadCurv = abs(diffAngleTtCh);
							bestHeadIndex = (int)k;
							bestHeadAngle = tempHeadAngle;
						}
					}
					if (dbDistCtTh < dbDistThreshold || dbDistCtTt < dbDistThreshold)
					{
						double diffAngleCtTh = tempHeadAngle - curTailAngle;
						if (diffAngleCtTh < -CV_PI)
						{
							diffAngleCtTh += 2 * CV_PI;
						}
						else if (diffAngleCtTh > CV_PI)
						{
							diffAngleCtTh -= 2 * CV_PI;
						}
						if (abs(diffAngleCtTh) < curvThreshold && abs(diffAngleCtTh) < minTailCurv)
						{
							minTailCurv = abs(diffAngleCtTh);
							bestTailIndex = (int)k;
							bestTailAngle = tempTailAngle;
						}
					}
				}

				// Link the best fit arcs
				if (bestHeadIndex != -1)
				{
					tempArc.insert(tempArc.begin(), srcCurves[bestHeadIndex].begin(), srcCurves[bestHeadIndex].end());
					curHeadPoint = srcCurves[bestHeadIndex].front();
					curHeadAngle = bestHeadAngle;
				}
				if (bestTailIndex != -1)
				{
					tempArc.insert(tempArc.end(), srcCurves[bestTailIndex].begin(), srcCurves[bestTailIndex].end());
					curTailPoint = srcCurves[bestTailIndex].back();
					curTailAngle = bestTailAngle;
				}
				if (bestHeadIndex != -1 || bestTailIndex != -1)
				{
					if (bestHeadIndex > bestTailIndex)
					{
						srcCurves.erase(srcCurves.begin() + bestHeadIndex);
						if (bestTailIndex != -1)
						{
							srcCurves.erase(srcCurves.begin() + bestTailIndex);
						}
					}
					else if (bestHeadIndex < bestTailIndex)
					{
						srcCurves.erase(srcCurves.begin() + bestTailIndex);
						if (bestHeadIndex != -1)
						{
							srcCurves.erase(srcCurves.begin() + bestHeadIndex);
						}
					}
					else
					{
						srcCurves.erase(srcCurves.begin() + bestHeadIndex);
						linkFlag = true;
					}
				}
				else
				{
					linkFlag = true;
				}
			}
			dstCurves.push_back(tempArc);
		}
	}

	static void ellipticCut(
		const std::vector<cv::Point> &curve,
		std::vector<std::vector<cv::Point>> &curves,
		int segNum = 10,
		double angleThreshold = 25.0)
	{
		// Check paramters
		if (segNum <= 1)
		{
			segNum = 10;
		}
		if (!curves.empty())
		{
			curves.clear();
		}

		double curvThreshold = angleThreshold * CV_PI / 180;
		int lastIndex = 0;
		int curIndex = segNum;
		int sign = -1;
		int curveSize = static_cast<int>(curve.size());
		while (curIndex + segNum < curveSize)
		{
			// Compute curvature of two segments
			int beforeIndex = curIndex - segNum;
			int afterIndex = curIndex + segNum;
			cv::Point curVec = curve[curIndex] - curve[beforeIndex];
			cv::Point nextVec = curve[afterIndex] - curve[curIndex];
			double curAngle = atan2(curVec.y, curVec.x);
			double nextAngle = atan2(nextVec.y, nextVec.x);
			double curvature = nextAngle - curAngle;
			if (curvature < -CV_PI)
			{
				curvature += 2 * CV_PI;
			}
			else if (curvature > CV_PI)
			{
				curvature -= 2 * CV_PI;
			}

			// Check sign
			bool signFlag = false;
			if ((double)sign * curvature < 0)
			{
				signFlag = true;
			}
			sign = curvature > 0 ? 1 : -1;

			// Curvature conditions
			if (abs(curvature) > curvThreshold)
			{
				cv::Point lineVec = curve[afterIndex] - curve[beforeIndex];
				double lineMag = sqrt(lineVec.ddot(lineVec));
				double cosTheta = (double)lineVec.x / lineMag;
				double sinTheta = (double)lineVec.y / lineMag;
				double maxDist = 0;
				int maxDistIndex = -1;
				for (int k = beforeIndex + 1; k < afterIndex; k++)
				{
					cv::Point curVec = curve[k] - curve[beforeIndex];
					double distance = abs(curVec.y * cosTheta - curVec.x * sinTheta);
					if (distance > maxDist)
					{
						maxDist = distance;
						maxDistIndex = k;
					}
				}
				std::vector<cv::Point> segTemp(curve.begin() + lastIndex, curve.begin() + maxDistIndex);
				curves.push_back(segTemp);
				//curve.erase(curve.begin(), curve.begin() + maxDistIndex);
				lastIndex = maxDistIndex;
				curIndex = lastIndex + segNum;
			}
			else if (signFlag)
			{
				std::vector<cv::Point> segTemp(curve.begin() + lastIndex, curve.begin() + curIndex);
				curves.push_back(segTemp);
				//curve.erase(curve.begin(), curve.begin() + curIndex);
				lastIndex = curIndex;
				curIndex = lastIndex + segNum;
			}
			else
			{
				curIndex += segNum / 2;
			}
		}

		if (lastIndex < curveSize)
		{
			std::vector<cv::Point> segTemp(curve.begin() + lastIndex, curve.end());
			curves.push_back(segTemp);
		}
	}

	double computeSimilarity(
		const std::vector<cv::Point> &arcs1,
		const std::vector<cv::Point> &arcs2,
		float delta,
		double rateThreshold)
	{
		std::vector<cv::Point> tempArc = arcs1;
		tempArc.insert(tempArc.end(), arcs2.begin(), arcs2.end());
		cv::RotatedRect rRect;
		if (static_cast<int>(tempArc.size()) > 5)
		{
			rRect = cv::fitEllipseDirect(tempArc);
			int fitNum1 = getCompatiblePoints(arcs1, rRect, delta);
			int fitNum2 = getCompatiblePoints(arcs2, rRect, delta);
			double rate1 = (double)fitNum1 / static_cast<double>(arcs1.size());
			double rate2 = (double)fitNum2 / static_cast<double>(arcs2.size());
			if (rate1 > rateThreshold && rate2 > rateThreshold)
			{
				return (rate1 * rate2);
			}
		}
		return 0;
	}

	// @out-of-date
	static void ellipticGroup(
		std::vector<std::vector<cv::Point>> &srcCurves,
		std::vector<std::vector<cv::Point>> &dstCurves,
		float delta,
		double highRateThreshold = 0.9,
		double lowRateThreshold = 0.3)
	{
		// Check parameters
		if (!dstCurves.empty())
		{
			dstCurves.clear();
		}

		while (!srcCurves.empty())
		{
			std::vector<cv::Point> curArc = srcCurves[0];
			srcCurves.erase(srcCurves.begin());
			bool groupFlag = false;
			while (!groupFlag)
			{
				int bestFitIndex = -1;
				double maxRates = 0;
				for (size_t k = 0; k < srcCurves.size(); k++)
				{
					double curSimilarity = computeSimilarity(curArc, srcCurves[k], delta, highRateThreshold);
					if (curSimilarity > maxRates)
					{
						maxRates = curSimilarity;
						bestFitIndex = static_cast<int>(k);
					}
				}

				if (bestFitIndex != -1)
				{
					curArc.insert(curArc.end(), srcCurves[bestFitIndex].begin(), srcCurves[bestFitIndex].end());
					srcCurves.erase(srcCurves.begin() + bestFitIndex);
				}
				else
				{
					groupFlag = true;
				}
			}
			dstCurves.push_back(curArc);
		}

		// Delete the non-elliptic arcs
		for (size_t k = 0; k < dstCurves.size(); k++)
		{
			cv::RotatedRect rRect;
			rRect = cv::fitEllipseDirect(dstCurves[k]);
			int fitNum = getCompatiblePoints(dstCurves[k], rRect, delta);
			double ellipseLength = (rRect.size.width + rRect.size.height) * CV_PI / 2.0;
			double rate = (double)fitNum / ellipseLength;
			if (rate < lowRateThreshold)
			{
				dstCurves.erase(dstCurves.begin() + k);
			}
		}
	}

	static void ellipticCluster(
		std::vector<std::vector<cv::Point>> &srcCurves,
		std::vector<std::vector<cv::Point>> &dstCurves,
		int clusterNum,
		float delta,
		double highRateThreshold = 0.9,
		double lowRateThreshold = 0.3)
	{
		// Check parameters
		if (!dstCurves.empty())
		{
			dstCurves.clear();
		}

		// Clustering
		int curClustNum = static_cast<int>(srcCurves.size());
		while (curClustNum > clusterNum)
		{
			double maxSimilarity = 0;
			int index1 = -1;
			int index2 = -1;
			for (int i = 0; i < static_cast<int>(srcCurves.size()) - 1; i++)
			{
				for (int j = i + 1; j < static_cast<int>(srcCurves.size()); j++)
				{
					double curSimilarity = computeSimilarity(srcCurves[i], srcCurves[j], delta, highRateThreshold);
					if (curSimilarity > maxSimilarity)
					{
						maxSimilarity = curSimilarity;
						index1 = i;
						index2 = j;
					}
				}
			}
			// Merge the arcs
			if (index1 != -1 && index2 != -1)
			{
				srcCurves[index1].insert(srcCurves[index1].end(), srcCurves[index2].begin(), srcCurves[index2].end());
				srcCurves.erase(srcCurves.begin() + index2);
				curClustNum--;
			}
			else
			{
				break;
			}
		}

		// Delete the non-elliptic arcs
		for (size_t k = 0; k < srcCurves.size(); k++)
		{
			cv::RotatedRect rRect;
			rRect = cv::fitEllipseDirect(srcCurves[k]);
			int fitNum = getCompatiblePoints(srcCurves[k], rRect, delta);
			double ellipseLength = (rRect.size.width + rRect.size.height) * CV_PI / 2.0;
			double rate = (double)fitNum / ellipseLength;
			if (rate < lowRateThreshold)
			{
				srcCurves.erase(srcCurves.begin() + k);
			}
		}
		dstCurves = srcCurves;
	}

	// @out-of-date
	MSInfoCode fitEllipse(
		std::vector<cv::Point> points,
		cv::RotatedRect &rRect,
		MSFitEllipseMethod fitMethod = FIT_ELLIPSE_DIRECT)
	{
		// Check points number of the ellipse
		if (points.size() < 5)
		{
			return MS_FIT_ELLIPSE_ERROR;
		}

		// Set index of all points
		std::vector<int> indexPoints(points.size());
		for (int i = 0; i < (int)points.size(); ++i)
		{
			indexPoints[i] = i;
		}

		// Initialize parameters
		// K = ln(1-Pc) / ln(1-Pk)
		// Pk = q^m
		int epoch = 50;
		int batchSize = 5;
		size_t maxCoNum = 0;
		float disThreshold = 0.01f;
		std::vector<int> bestIndexes;
		cv::RotatedRect tempRect;
		std::vector<cv::Point> tempPoints;

		// Get the best randomly selected points
		for (int k = 0; k < epoch; ++k)
		{
			size_t curSize = indexPoints.size();
			srand((unsigned)time(NULL));
			std::vector<cv::Point> selectPoints;
			for (int i = 0; i < batchSize; ++i)
			{
				int index = rand() % curSize;
				selectPoints.push_back(points[indexPoints[index]]);
				int temp = indexPoints[curSize - 1];
				indexPoints[curSize - 1] = indexPoints[index];
				indexPoints[index] = temp;
				curSize--;
			}
			switch (fitMethod)
			{
			case FIT_ELLIPSE_GENERAL:
				tempRect = cv::fitEllipse(selectPoints); break;
			case FIT_ELLIPSE_AMS:
				tempRect = cv::fitEllipseAMS(selectPoints); break;
			case FIT_ELLIPSE_DIRECT:
			default:
				tempRect = cv::fitEllipseDirect(selectPoints); break;
			}
			// Compute number of compatible points
			std::vector<int> coIndexes;
			getCompatiblePoints(points, tempRect, coIndexes, disThreshold);
			if (coIndexes.size() > maxCoNum)
			{
				maxCoNum = coIndexes.size();
				bestIndexes = coIndexes;
				tempPoints = selectPoints;
			}
		}

		// Fit ellipse with more points
		size_t curSize = indexPoints.size() - batchSize;
		int epochAdd = MIN((int)indexPoints.size() / batchSize - 1, 50);
		for (int k = 0; k < epochAdd; ++k)
		{
			srand((unsigned)time(NULL));
			for (int i = 0; i < batchSize; ++i)
			{
				int index = rand() % curSize;
				tempPoints.push_back(points[indexPoints[index]]);
				int temp = indexPoints[curSize - 1];
				indexPoints[curSize - 1] = indexPoints[index];
				indexPoints[index] = temp;
				curSize--;
			}
			switch (fitMethod)
			{
			case FIT_ELLIPSE_GENERAL:
				tempRect = cv::fitEllipse(tempPoints); break;
			case FIT_ELLIPSE_AMS:
				tempRect = cv::fitEllipseAMS(tempPoints); break;
			case FIT_ELLIPSE_DIRECT:
			default:
				tempRect = cv::fitEllipseDirect(tempPoints); break;
			}
			// Compute number of compatible points
			std::vector<int> coIndexes;
			getCompatiblePoints(points, tempRect, coIndexes, disThreshold);
			if (coIndexes.size() > maxCoNum)
			{
				maxCoNum = coIndexes.size();
				bestIndexes = coIndexes;
			}
			else
			{
				for (int i = 0; i < batchSize; ++i)
				{
					tempPoints.pop_back();
				}
				curSize += batchSize;
			}
		}

		// Get the final result
		std::vector<cv::Point> bestPoints;
		for (size_t i = 0; i < bestIndexes.size(); ++i)
		{
			bestPoints.push_back(points[bestIndexes[i]]);
		}
		rRect = cv::fitEllipseDirect(bestPoints);
		return MS_SUCCESS;
	}

	static int updateNumIters(
		double confidence,
		double ratio,
		int batchSize,
		int numIters)
	{
		if (batchSize <= 0)
		{
			batchSize = 5;
		}

		confidence = MAX(confidence, 0.);
		confidence = MIN(confidence, 1.);
		ratio = MAX(ratio, 0.);
		ratio = MIN(ratio, 1.);

		double num = MAX(1. - confidence, DBL_MIN);
		double denom = 1. - pow(1. - ratio, batchSize);
		if (denom < DBL_MIN)
		{
			return 0;
		}

		num = log(num);
		denom = log(denom);

		return denom >= 0 || -num >= numIters * (-denom) ?
			numIters : cvRound(num / denom);
	}

	MSInfoCode fitEllipseRANSAC(
		const std::vector<cv::Point> &points,
		cv::RotatedRect &rRect,
		float threshold,
		double confidence = 0.995,
		int maxIters = 1000,
		MSFitEllipseMethod fitMethod = FIT_ELLIPSE_DIRECT)
	{
		// Too few points return error.
		if (static_cast<int>(points.size()) < 5)
		{
			return MS_FIT_ELLIPSE_ERROR;
		}

		// Get index of all points.
		std::vector<int> indexPoints(points.size());
		for (int i = 0; i < points.size(); ++i)
		{
			indexPoints[i] = i;
		}

		// Initialize parameters.
		// K = ln(1-Pc) / ln(1-Pk)
		// Pk = q^m
		// q = In / N
		int numIters = maxIters;
		int batchSize = 5;
		int pointsNum = static_cast<int>(points.size());
		size_t maxCoNum = 0;
		std::vector<int> bestIndexes;
		cv::RotatedRect tempRect;

		// Get the best randomly selected points.
		for (int k = 0; k < numIters; ++k)
		{
			// Select points from original set.
			int tempNum = pointsNum;
			srand((unsigned)time(NULL));
			std::vector<cv::Point> selectPoints;
			for (int i = 0; i < batchSize; ++i)
			{
				int index = rand() % tempNum;
				selectPoints.push_back(points[indexPoints[index]]);
				int temp = indexPoints[tempNum - 1];
				indexPoints[tempNum - 1] = indexPoints[index];
				indexPoints[index] = temp;
				tempNum--;
			}
			// Compute the model.
			switch (fitMethod)
			{
			case FIT_ELLIPSE_GENERAL:
				tempRect = cv::fitEllipse(selectPoints); break;
			case FIT_ELLIPSE_AMS:
				tempRect = cv::fitEllipseAMS(selectPoints); break;
			case FIT_ELLIPSE_DIRECT:
			default:
				tempRect = cv::fitEllipseDirect(selectPoints); break;
			}
			// Compute number of compatible points.
			std::vector<int> coIndexes;
			getCompatiblePoints(points, tempRect, coIndexes, threshold);
			if (coIndexes.size() > maxCoNum)
			{
				maxCoNum = coIndexes.size();
				bestIndexes = coIndexes;
				numIters = updateNumIters(
					confidence,
					(double)(pointsNum - static_cast<int>(coIndexes.size())) / pointsNum,
					batchSize,
					numIters);
			}
		}

		// Get the final result.
		if (bestIndexes.size() < 5)
		{
			return MS_FIT_ELLIPSE_ERROR;
		}
		std::vector<cv::Point> bestPoints;
		for (size_t i = 0; i < bestIndexes.size(); ++i)
		{
			bestPoints.push_back(points[bestIndexes[i]]);
		}
		rRect = cv::fitEllipseDirect(bestPoints);
		return MS_SUCCESS;
	}

	// @out-of-date
	static void getDistances(
		const cv::RotatedRect &rRect,
		std::vector<cv::Point> points,
		std::vector<float> &distances)
	{
		// Parameter of ellipse
		float cosA = (float)cos(rRect.angle / 180 * CV_PI);
		float sinA = (float)sin(rRect.angle / 180 * CV_PI);
		float a = rRect.size.height / 2;
		float b = rRect.size.width / 2;

		// Compute distances
		for (size_t i = 0; i < points.size(); i++)
		{
			float x = (points[i].x - rRect.center.x) * sinA - (points[i].y - rRect.center.y) * cosA;
			float y = (points[i].x - rRect.center.x) * cosA + (points[i].y - rRect.center.y) * sinA;
			float distance = pow(x / a, 2) + pow(y / b, 2) - 1;
			distances.push_back(distance);
		}
	}

	// @out-of-date
	static void kMeansEllipse(
		const std::vector<cv::Point> &points,
		std::vector<int> &labels,
		cv::RotatedRect rRect,
		int maxEpoches = 5)
	{
		if (labels.empty())
		{
			labels = std::vector<int>(points.size());
		}
		if (maxEpoches <= 0)
		{
			maxEpoches = 5;
		}

		// Get distance
		std::vector<float> distances;
		getDistances(rRect, points, distances);

		// Initialize the centers
		int clusterNum = 2;
		std::vector<double> center(clusterNum);
		std::vector<double> d(clusterNum);
		center[0] = -0.1;
		center[1] = 0.1;

		// K-means method
		for (int k = 0; k < maxEpoches; k++)
		{
			std::vector<double> sumD(clusterNum, 0);
			std::vector<int> countNum(clusterNum, 0);
			for (size_t i = 0; i < distances.size(); i++)
			{
				d[0] = abs(center[0] - distances[i]);
				d[1] = abs(center[1] - distances[i]);
				if (d[0] < d[1])
				{
					labels[i] = 0;
				}
				else
				{
					labels[i] = 1;
				}
				sumD[labels[i]] += distances[i];
				countNum[labels[i]]++;
			}
			center[0] = countNum[0] == 0 ? center[0] : sumD[0] / countNum[0];
			center[1] = countNum[1] == 0 ? center[1] : sumD[1] / countNum[1];
		}
	}

	// @out-of-date
	static MSInfoCode getEllipse(
		cv::Mat src,
		std::vector<cv::RotatedRect> &rRectSet,
		int curveNum,
		int contourSize,
		MSFitEllipseMethod fitMethod = FIT_ELLIPSE_RANSAC)
	{
		// Grab contours from the edge image
		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(src, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

		// Delete undesirable contours
		std::vector<std::vector<cv::Point>>::iterator itr = contours.begin();
		while (itr != contours.end())
		{
			if (itr->size() < (size_t)contourSize)
			{
				itr = contours.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		// Get ellipses of the hole contours
		if (curveNum == 2)
		{
			// Fit initial ellipse
			std::vector<std::vector<cv::Point>> resultContours(curveNum);
			std::vector<cv::Point> tempContour;
			cv::RotatedRect rRect;
			for (size_t j = 0; j < contours.size(); j++)
			{
				tempContour.insert(tempContour.end(), contours[j].begin(), contours[j].end());
			}
			if (tempContour.size() > 4)
			{
				rRect = cv::fitEllipseDirect(tempContour);
			}
			else
			{
				return MS_GET_ELLIPSE_ERROR;
			}

			// Remove the bad points
			// It needs some modification
			std::vector<float> distances;
			getDistances(rRect, tempContour, distances);
			float ThresholdPD = 0.5;
			std::vector<cv::Point> midContour = tempContour;
			std::vector<cv::Point>::iterator itrMid = midContour.begin();
			for (size_t i = 0; i < midContour.size(); i++)
			{
				if (abs(distances[i]) > ThresholdPD)
				{
					itrMid = midContour.erase(itrMid);
				}
				else
				{
					itrMid++;
				}
			}
			if (midContour.size() < size_t(0.9 * tempContour.size()))
			{
				rRect.size.height *= 0.9f;
				getDistances(rRect, tempContour, distances);
				std::vector<cv::Point>::iterator itrTemp = tempContour.begin();
				for (size_t i = 0; i < tempContour.size(); i++)
				{
					if (abs(distances[i]) > ThresholdPD)
					{
						itrTemp = tempContour.erase(itrTemp);
					}
					else
					{
						itrTemp++;
					}
				}
			}
			else
			{
				tempContour = midContour;
			}

			// Devide the rest points into 2 clusters
			std::vector<int> labels(tempContour.size());
			kMeansEllipse(tempContour, labels, rRect, 15);
			for (size_t i = 0; i < tempContour.size(); i++)
			{
				resultContours[labels[i]].push_back(tempContour[i]);
			}

			// Fit the points.
			for (int k = 0; k < curveNum; k++)
			{
				if (resultContours[k].size() > 4)
				{
					switch (fitMethod)
					{
					case FIT_ELLIPSE_GENERAL:
						rRect = cv::fitEllipse(resultContours[k]); break;
					case FIT_ELLIPSE_AMS:
						rRect = cv::fitEllipseAMS(resultContours[k]); break;
					case FIT_ELLIPSE_DIRECT:
						rRect = cv::fitEllipseDirect(resultContours[k]); break;
					case FIT_ELLIPSE_RANSAC:
					default:
						fitEllipse(resultContours[k], rRect, FIT_ELLIPSE_DIRECT); break;
					}
					rRectSet.push_back(rRect);
				}
				else
				{
					return MS_GET_ELLIPSE_ERROR;
				}
			}
		}
		else if (curveNum == 1)
		{
			std::vector<cv::Point> tempContour;
			cv::RotatedRect rRect;
			for (size_t j = 0; j < contours.size(); j++)
			{
				tempContour.insert(tempContour.end(), contours[j].begin(), contours[j].end());
			}
			if (tempContour.size() > 4)
			{
				switch (fitMethod)
				{
				case FIT_ELLIPSE_GENERAL:
					rRect = cv::fitEllipse(tempContour); break;
				case FIT_ELLIPSE_AMS:
					rRect = cv::fitEllipseAMS(tempContour); break;
				case FIT_ELLIPSE_DIRECT:
					rRect = cv::fitEllipseDirect(tempContour); break;
				case FIT_ELLIPSE_RANSAC:
				default:
					fitEllipse(tempContour, rRect, FIT_ELLIPSE_DIRECT); break;
				}
				rRectSet.push_back(rRect);
			}
			else
			{
				return MS_GET_ELLIPSE_ERROR;
			}
		}

		return MS_SUCCESS;
	}

	MSInfoCode grabEllipse(
		const cv::Mat &src,
		std::vector<cv::RotatedRect> &rRects,
		float delta,
		int curveNum,
		int segNum,
		int curveSize,
		double angleThreshold,
		double distThreshold,
		MSFitEllipseMethod fitMethod)
	{
		// Trace all edges
		std::vector<std::vector<cv::Point>> edges;
		MSInfoCode status = traceEdge(src, edges);
		if (status != MS_SUCCESS)
		{
			return status;
		}

		// Cut at corners
		std::vector<std::vector<cv::Point>> cutEdges;
		for (size_t k = 0; k < edges.size(); k++)
		{
			std::vector<std::vector<cv::Point>> segments;
			cutAtCorner(edges[k], segments, segNum);
			cutEdges.insert(cutEdges.end(), segments.begin(), segments.end());
		}

		// Link edges 1
		std::vector<std::vector<cv::Point>> linkEdges;
		linkCurves(cutEdges, linkEdges, segNum, distThreshold, angleThreshold);

		// Remove too small edges
		std::vector<std::vector<cv::Point>>::iterator itr1 = linkEdges.begin();
		while (itr1 != linkEdges.end())
		{
			if (static_cast<int>(itr1->size()) < segNum)
			{
				itr1 = linkEdges.erase(itr1);
			}
			else
			{
				itr1++;
			}
		}

		// Elliptic cutting
		std::vector<std::vector<cv::Point>> arcs;
		for (size_t k = 0; k < linkEdges.size(); k++)
		{
			std::vector<std::vector<cv::Point>> segments;
			ellipticCut(linkEdges[k], segments, segNum, angleThreshold);
			arcs.insert(arcs.end(), segments.begin(), segments.end());
		}

		// Link edges 2
		std::vector<std::vector<cv::Point>> linkArcs;
		linkCurves(arcs, linkArcs, segNum, distThreshold, angleThreshold);

		// Remove non-elliptic arcs
		std::vector<std::vector<cv::Point>>::iterator itr2 = linkArcs.begin();
		while (itr2 != linkArcs.end())
		{
			if (static_cast<int>(itr2->size()) < curveSize)
			{
				itr2 = linkArcs.erase(itr2);
			}
			else
			{
				itr2++;
			}
		}

		// Group elliptic arcs
		std::vector<std::vector<cv::Point>> contours;
		//ellipticGroup(linkArcs, contours, delta, 0.9, 0.2);
		ellipticCluster(linkArcs, contours, curveNum + 1, delta, 0.9, 0.2);
		if (static_cast<int>(contours.size()) < curveNum)
		{
			return MS_GET_ELLIPSE_ERROR;
		}

		// Get RotatedRect of contours
		int count = 0;
		while (count < curveNum)
		{
			int maxLength = 0;
			int bestIndex = -1;
			for (size_t k = 0; k < contours.size(); k++)
			{
				if (static_cast<int>(contours[k].size()) > maxLength)
				{
					maxLength = static_cast<int>(contours[k].size());
					bestIndex = static_cast<int>(k);
				}
			}
			cv::RotatedRect tempRect;
			if (static_cast<int>(contours[bestIndex].size()) > 5)
			{
				switch (fitMethod)
				{
				case FIT_ELLIPSE_GENERAL:
					tempRect = cv::fitEllipse(contours[bestIndex]); break;
				case FIT_ELLIPSE_AMS:
					tempRect = cv::fitEllipseAMS(contours[bestIndex]); break;
				case FIT_ELLIPSE_RANSAC:
					fitEllipse(contours[bestIndex], tempRect, FIT_ELLIPSE_DIRECT); break;
				case FIT_ELLIPSE_DIRECT:
				default:
					tempRect = cv::fitEllipseDirect(contours[bestIndex]); break;
				}
				rRects.push_back(tempRect);
				contours.erase(contours.begin() + bestIndex);
				count++;
			}
			else
			{
				return MS_GET_ELLIPSE_ERROR;
			}
		}

		// Sort the ellipses
		for (int i = 0; i < curveNum - 1; i++)
		{
			int maxIndex = -1;
			float maxValue = 0;
			for (int j = i; j < curveNum; j++)
			{
				if (rRects[j].size.height > maxValue)
				{
					maxValue = rRects[j].size.height;
					maxIndex = j;
				}
			}
			cv::RotatedRect tempRect;
			tempRect = rRects[i];
			rRects[i] = rRects[maxIndex];
			rRects[maxIndex] = tempRect;
		}

		return MS_SUCCESS;
	}

	MSInfoCode pyrEllipse(
		const cv::Mat &src,
		std::vector<cv::RotatedRect> &rRects,
		int curveNum,
		int layerNum,
		int kernelSize,
		int segNum,
		float distThreshold)
	{
		// Check parameters
		if (src.empty())
		{
			return MS_ARGUMENT_ERROR;
		}

		// Generate images of all layers
		std::vector<cv::Mat> G(layerNum);
		G[0] = src;
		for (int l = 0; l < layerNum - 1; l++)
		{
			cv::pyrDown(G[l], G[l + 1], cv::Size(G[l].cols / 2, G[l].rows / 2));
		}

		// Parameters for image process
		double fBilateral = 1.0;
		double ffB = 2.0;

		// Detect ellipses in bottom layer
		MSInfoCode status;
		cv::Mat btmEdge;
		status = preprocess(G[layerNum - 1], btmEdge, kernelSize, fBilateral, MS_GREEN | MS_RED);
		if (status != MS_SUCCESS)
		{
			return status;
		}
		std::vector<cv::RotatedRect> btmrRects;
		float delta = 16.f * distThreshold / (btmEdge.cols + btmEdge.rows);
		int curveSize = 25;
		double angleThreshold = 15;
		status = grabEllipse(
			btmEdge,
			btmrRects,
			delta,
			curveNum,
			segNum,
			curveSize,
			angleThreshold,
			distThreshold * 2.5);
		if (status != MS_SUCCESS)
		{
			return status;
		}

#ifdef MS_DEBUG
		cv::Mat testImg;
		cv::imshow("TEST", btmEdge);
		cv::waitKey(0);
		cv::cvtColor(btmEdge, testImg, cv::COLOR_GRAY2BGR);
		for (int k = 0; k < curveNum; k++)
		{
			cv::ellipse(testImg, btmrRects[k], cv::Scalar(0, k * 127, 255), 1, 8);
		}
		cv::imshow("TEST", testImg);
		cv::waitKey(0);
#endif

		// Get ROI of all images and edge images
		float btmMaxH = 0;
		float btmMaxW = 0;
		int indexMaxH = -1;
		int indexMaxW = -1;
		for (int k = 0; k < curveNum; k++)
		{
			float tempH = btmrRects[k].size.height;
			float tempW = btmrRects[k].size.width;
			if (tempH > btmMaxH)
			{
				btmMaxH = tempH;
				indexMaxH = k;
			}
			if (tempW > btmMaxW)
			{
				btmMaxW = tempW;
				indexMaxW = k;
			}
		}
		if (indexMaxH != indexMaxW)
		{
			return MS_GET_ELLIPSE_ERROR;
		}
		int fSize = 30;
		int btmBorder = ((int)btmMaxH / fSize + 1) * fSize;
		std::vector<cv::Rect> rectG(layerNum);
		rectG[layerNum - 1].x = static_cast<int>(btmrRects[indexMaxH].center.x - btmBorder / 2);
		rectG[layerNum - 1].y = static_cast<int>(btmrRects[indexMaxH].center.y - btmBorder / 2);
		rectG[layerNum - 1].width = btmBorder;
		rectG[layerNum - 1].height = btmBorder;
		if ((rectG[layerNum - 1].x < 0) ||
			(rectG[layerNum - 1].y < 0) ||
			(rectG[layerNum - 1].x + rectG[3].width >= G[3].cols) ||
			(rectG[layerNum - 1].y + rectG[3].height >= G[3].rows))
		{
			return MS_ROI_SIZE_ERROR;
		}
		std::vector<cv::Mat> roiG(layerNum);
		std::vector<cv::Mat> roiEdges(layerNum);
		roiG[layerNum - 1] = G[layerNum - 1](rectG[layerNum - 1]);
		roiEdges[layerNum - 1] = btmEdge(rectG[layerNum - 1]);

		// Grab points of ellipses in each layer
		float rangeThreshold = 4 * sqrt(1 / (btmMaxH * btmMaxH) + 1 / (btmMaxW * btmMaxW)) * distThreshold;
		float fRange = 2.0;
		std::vector<cv::RotatedRect> temprRects(curveNum);
		for (size_t k = 0; k < curveNum; k++)
		{
			temprRects[k].center.x = btmrRects[k].center.x - rectG[layerNum - 1].x;
			temprRects[k].center.y = btmrRects[k].center.y - rectG[layerNum - 1].y;
			temprRects[k].size.width = btmrRects[k].size.width;
			temprRects[k].size.height = btmrRects[k].size.height;
			temprRects[k].angle = btmrRects[k].angle;
		}
		for (int l = layerNum - 1; l >= 0; l--)
		{
			if (l < layerNum - 1)
			{
				rectG[l].x = rectG[l + 1].x * 2;
				rectG[l].y = rectG[l + 1].y * 2;
				rectG[l].width = rectG[l + 1].width * 2;
				rectG[l].height = rectG[l + 1].height * 2;
				roiG[l] = G[l](rectG[l]);
				fBilateral *= ffB;
				status = preprocess(roiG[l], roiEdges[l], kernelSize, fBilateral, MS_GREEN | MS_RED);
				if (status != MS_SUCCESS)
				{
					return status;
				}
				for (size_t k = 0; k < curveNum; k++)
				{
					temprRects[k].center.x *= 2;
					temprRects[k].center.y *= 2;
					temprRects[k].size.width *= 2;
					temprRects[k].size.height *= 2;
				}
				rangeThreshold /= fRange;
			}
			std::vector<std::vector<cv::Point>> curves(curveNum);
			for (size_t k = 0; k < temprRects.size(); k++)
			{
				float cosA = (float)cos(temprRects[k].angle / 180 * CV_PI);
				float sinA = (float)sin(temprRects[k].angle / 180 * CV_PI);
				float a = temprRects[k].size.height / 2;
				float b = temprRects[k].size.width / 2;
				cv::Mat tempImg = roiEdges[l];
				getCompatiblePoints(tempImg, temprRects[k], curves[k], rangeThreshold);
				/*for (int i = 0; i < tempImg.rows; i++)
				{
					uchar* data = tempImg.ptr<uchar>(i);
					for (int j = 0; j < tempImg.cols; j++)
					{
						if (data[j] > 128)
						{
							float x = (j - temprRects[k].center.x) * sinA - (i - temprRects[k].center.y) * cosA;
							float y = (j - temprRects[k].center.x) * cosA + (i - temprRects[k].center.y) * sinA;
							float distance = pow(x / a, 2) + pow(y / b, 2) - 1;
							if (abs(distance) < rangeThreshold)
							{
								curves[k].push_back(cv::Point(j, i));
							}
						}
					}
				}*/
				if (curves[k].size() > 4)
				{
					cv::RotatedRect rRect = cv::fitEllipseDirect(curves[k]);
					temprRects[k] = rRect;
				}
				else
				{
					return MS_PYR_ELLIPSE_ERROR;
				}
			}
#ifdef MS_DEBUG
			cv::Mat drawImg = roiG[l].clone();
			for (size_t k = 0; k < temprRects.size(); k++)
			{
				cv::ellipse(drawImg, temprRects[k], cv::Scalar(0, 255, 0), 1, 8);
			}
			cv::imshow("TEST", drawImg);
			cv::waitKey(0);
#endif
		}
		for (size_t k = 0; k < temprRects.size(); k++)
		{
			temprRects[k].center.x += rectG[0].x;
			temprRects[k].center.y += rectG[0].y;
		}
		rRects = temprRects;
		return MS_SUCCESS;
	}

	static void selectPoints(
		cv::RotatedRect rRect,
		std::vector<cv::Point2f> &points,
		int pointNum)
	{
		float cosA = (float)cos(rRect.angle / 180 * CV_PI);
		float sinA = (float)sin(rRect.angle / 180 * CV_PI);
		float a = rRect.size.height / 2;
		float b = rRect.size.width / 2;
		float dA = float(2 * CV_PI / pointNum);
		cv::Point2f tempPoint;
		cv::Point2f resPoint;
		for (int i = 0; i < pointNum; i++)
		{
			tempPoint.x = a * cos(dA * i);
			tempPoint.y = b * sin(dA * i);
			resPoint.x = tempPoint.y * cosA + tempPoint.x * sinA + rRect.center.x;
			resPoint.y = tempPoint.y * sinA - tempPoint.x * cosA + rRect.center.y;
			points.push_back(resPoint);
		}
	}

	static void getTarPoint(
		cv::RotatedRect refRect,
		cv::RotatedRect tarRect,
		cv::Vec3f lineCoeff,
		cv::Point2f refPoint,
		cv::Point2f &tarPoint)
	{
		// Compute theta0 of reference point
		cv::Point2f refPoint_;
		float cosA = cosf(float(refRect.angle / 180 * CV_PI));
		float sinA = sinf(float(refRect.angle / 180 * CV_PI));
		refPoint_.x = (refPoint.x - refRect.center.x) * sinA - (refPoint.y - refRect.center.y) * cosA;
		refPoint_.y = (refPoint.x - refRect.center.x) * cosA + (refPoint.y - refRect.center.y) * sinA;
		float theta0 = 0;
		theta0 = atan2f(2 * refPoint_.y / refRect.size.height, 2 * refPoint_.x / refRect.size.width);

		// Convert to ellipse coordinate
		cosA = cosf(float(tarRect.angle / 180 * CV_PI));
		sinA = sinf(float(tarRect.angle / 180 * CV_PI));
		float A = lineCoeff[0] * sinA - lineCoeff[1] * cosA;
		float B = lineCoeff[0] * cosA + lineCoeff[1] * sinA;
		float C = lineCoeff[0] * tarRect.center.x + lineCoeff[1] * tarRect.center.y + lineCoeff[2];

		// Compute crossover points
		// A * x + B * y + C = 0
		// x = a * cos(theta);
		// y = b * sin(theta);
		float a = tarRect.size.height / 2;
		float b = tarRect.size.width / 2;
		float a_ = (A * a) * (A * a) + (B * b) * (B * b);
		float b_ = 2 * A * a * C;
		float c_ = (C * C) - (B * b) * (B * b);
		float delta = b_ * b_ - 4 * a_ * c_;
		cv::Point2f tempPoint;
		if (delta > 0)
		{
			float theta1 = 0;
			float theta2 = 0;
			float cosT1, sinT1;
			float cosT2, sinT2;
			cosT1 = (-b_ - sqrt(delta)) / (2 * a_);
			sinT1 = -(C + A * a * cosT1) / (B * b);
			theta1 = atan2f(sinT1, cosT1);
			cosT2 = (-b_ + sqrt(delta)) / (2 * a_);
			sinT2 = -(C + A * a * cosT2) / (B * b);
			theta2 = atan2f(sinT2, cosT2);
			if (theta0 < 0)
			{
				theta0 = float(theta0 - CV_PI);
				if (abs(theta1 - theta0) < abs(theta2 - theta0))
				{
					tempPoint.x = a * cosT1;
					tempPoint.y = b * sinT1;
				}
				else
				{
					tempPoint.x = a * cosT2;
					tempPoint.y = b * sinT2;
				}
			}
			else if (theta0 > 0)
			{
				theta0 = float(theta0 + CV_PI);
				if (abs(theta1 - theta0) < abs(theta2 - theta0))
				{
					tempPoint.x = a * cosT1;
					tempPoint.y = b * sinT1;
				}
				else
				{
					tempPoint.x = a * cosT2;
					tempPoint.y = b * sinT2;
				}
			}
			else
			{
				if (abs(theta1) > abs(theta2))
				{
					tempPoint.x = a * cosT1;
					tempPoint.y = b * sinT1;
				}
				else
				{
					tempPoint.x = a * cosT2;
					tempPoint.y = b * sinT2;
				}
			}
		}
		else if (delta = 0)
		{
			float cosT = -b_ / (2 * a_);
			float sinT = -(C + A * a * cosT) / (B * b);
			tempPoint.x = a * cosT;
			tempPoint.y = b * sinT;
		}
		else
		{
			return;
		}

		// Transform the points to original coordinate
		tarPoint.x = tempPoint.y * cosA + tempPoint.x * sinA + tarRect.center.x;
		tarPoint.y = tempPoint.y * sinA - tempPoint.x * cosA + tarRect.center.y;
	}

	static void computeF(
		cv::Mat Al,
		cv::Mat Ar,
		cv::Mat R,
		cv::Mat t,
		cv::Mat &F)
	{
		// E = R * S
		// F = Ar' \ S * R / Al
		// S = [0, -t(3), t(2); t(3), 0, -t(1); -t(2), t(1), 0]
		cv::Mat S = (cv::Mat_<double>(3, 3) <<
			0, -t.at<double>(2, 0), t.at<double>(1, 0),
			t.at<double>(2, 0), 0, -t.at<double>(0, 0),
			-t.at<double>(1, 0), t.at<double>(0, 0), 0);
		F = Ar.t().inv() * S * R * Al.inv();
	}

	static void computeXYZ(
		cv::Point pl,
		cv::Point pr,
		cv::Mat Al,
		cv::Mat Ar,
		cv::Mat R,
		cv::Mat t,
		cv::Point3d &xyzl,
		cv::Point3d &xyzr)
	{
		// v0 = -Ar * R \ Al * Pl
		// v1 = Ar * T
		// A = [v0, Pr]
		// zs = [zl, zr]'
		// A * zs = v1
		// zs = (A' * A) \ A' * b
		cv::Mat v0(3, 1, CV_32FC1);
		cv::Mat v1(3, 1, CV_32FC1);
		cv::Mat Pl = (cv::Mat_<double>(3, 1) << pl.x, pl.y, 1);
		cv::Mat Pr = (cv::Mat_<double>(3, 1) << pr.x, pr.y, 1);
		v0 = -Ar * R * Al.inv() * Pl;
		v1 = Ar * t;
		cv::Mat A(3, 2, CV_32FC1);
		cv::Mat zs(2, 1, CV_32FC1);
		hconcat(v0, Pr, A);
		solve(A, v1, zs, cv::DECOMP_SVD);
		double zl = zs.at<double>(0, 0);
		double zr = zs.at<double>(1, 0);
		// zl * Pl = Al * xyzl
		// zr * Pr = Ar * xyzr
		cv::Mat PcL(3, 1, CV_32FC1);
		cv::Mat PcR(3, 1, CV_32FC1);
		PcL = zl * Al.inv() * Pl;
		PcR = zr * Ar.inv() * Pr;
		xyzl = cv::Point3d(PcL.at<double>(0, 0), PcL.at<double>(1, 0), PcL.at<double>(2, 0));
		xyzr = cv::Point3d(PcR.at<double>(0, 0), PcR.at<double>(1, 0), PcR.at<double>(2, 0));
	}

	static void fitPlane(
		std::vector<cv::Point3d> points,
		cv::Vec4d& planeCoeff)
	{
		// A * x + B * y + C * z + D = 0
		// M * [A, B, C, D]' = 0
		// M' * M * [A, B, C, D]' = 0
		// v = [A, B, C, D]'
		// [U, S, V] = SVD(M);
		// v = V(:, 4)

		// Initialize the points matrix
		std::vector<cv::Mat> matPoints;
		for (size_t i = 0; i < points.size(); i++)
		{
			cv::Mat tempMat = (cv::Mat_<double>(1, 4) << points[i].x, points[i].y, points[i].z, 1.0);
			matPoints.push_back(tempMat);
		}
		cv::Mat M;
		cv::vconcat(matPoints, M);

		// Compute coefficients by LSM
		cv::Mat v(4, 1, CV_32FC1);
		cv::SVD::solveZ(M.t() * M, v);
		planeCoeff[0] = v.at<double>(0, 0);
		planeCoeff[1] = v.at<double>(1, 0);
		planeCoeff[2] = v.at<double>(2, 0);
		planeCoeff[3] = v.at<double>(3, 0);

		// Compute coefficients by SVD method
		//Mat W, U, Vt;
		//SVDecomp(M, W, U, Vt, 0);
		//planeCoeff[0] = Vt.at<double>(3, 0);
		//planeCoeff[1] = Vt.at<double>(3, 1);
		//planeCoeff[2] = Vt.at<double>(3, 2);
		//planeCoeff[3] = Vt.at<double>(3, 3);

		// Normalization of first 3 coefficients
		double denominator = -sqrt(planeCoeff[0] * planeCoeff[0] + planeCoeff[1] * planeCoeff[1] + planeCoeff[2] * planeCoeff[2]);
		planeCoeff[0] /= denominator;
		planeCoeff[1] /= denominator;
		planeCoeff[2] /= denominator;
		planeCoeff[3] /= denominator;
	}

	static void computeCenterDiameter(
		std::vector<cv::Point3d> points,
		cv::Vec4d plane,
		cv::Point3d &center,
		double &diameter)
	{
		std::vector<cv::Point2f> holePoints;

		// Project points.
		// theta_ = k x n / |k x n| * theta
		// k = [0, 0, 1]'
		// n = [A, B, C]'
		// A^2 + B^2 + C^2 = 1
		// s = k x n = [-B, A, 0]'
		// R = rodrigues(theta_)
		// p1 = R' * p2
		// p2 = R * p1

		// Compute rotated matrix
		double A = plane[0];
		double B = plane[1];
		double C = plane[2];
		double angle = acos(C);
		double factor = angle / sqrt(B * B + A * A);
		cv::Mat s = (cv::Mat_<double>(3, 1) << -B * factor, A * factor, 0);
		cv::Mat R(3, 3, CV_32FC1);
		cv::Rodrigues(s, R);

		// Transformate points
		for (size_t i = 0; i < points.size(); i++)
		{
			cv::Mat tempMat = (cv::Mat_<double>(3, 1) << points[i].x, points[i].y, points[i].z);
			tempMat = R * tempMat;
			cv::Point2f tempPoint;
			tempPoint.x = (float)tempMat.at<double>(0, 0);
			tempPoint.y = (float)tempMat.at<double>(1, 0);
			holePoints.push_back(tempPoint);
		}

		// Get coordinate of hole center
		if (holePoints.size() > 4)
		{
			cv::RotatedRect holeRect = fitEllipse(holePoints);
			diameter = (holeRect.size.height + holeRect.size.width) / 2;
			// n' * p1 = -D
			// p1 = R' * p2
			// n' * R' * p2 = -D
			cv::Point3d holeCenter;
			cv::Mat rowMat = (cv::Mat_<double>(1, 3) << A, B, C);
			rowMat = rowMat * R.t();
			holeCenter.x = holeRect.center.x;
			holeCenter.y = holeRect.center.y;
			holeCenter.z = -(rowMat.at<double>(0) * holeCenter.x + rowMat.at<double>(1) * holeCenter.y + plane[3]) / rowMat.at<double>(2);
			cv::Mat colMat = (cv::Mat_<double>(3, 1) << holeCenter.x, holeCenter.y, holeCenter.z);
			colMat = R.t() * colMat;
			center.x = colMat.at<double>(0);
			center.y = colMat.at<double>(1);
			center.z = colMat.at<double>(2);
		}
	}

	void getCenterDiameter(
		cv::RotatedRect rRects[],
		cv::Vec4d planes[],
		cv::Point3d xyzs[],
		double &diameter,
		int pointsNum,
		cv::Mat intrinsics[],
		cv::Mat R,
		cv::Mat t)
	{
		// Compute fundamental matrix
		cv::Mat F;
		computeF(intrinsics[0], intrinsics[1], R, t, F);
		// Select points of the left image
		std::vector<cv::Point2f> points;
		selectPoints(rRects[0], points, pointsNum);
		// Compute correspond epipolar lines
		std::vector<cv::Vec3f> lines;
		cv::computeCorrespondEpilines(points, 1, F, lines);
		// Compute crossover points
		std::vector<cv::Point2f> pointsL;
		std::vector<cv::Point2f> pointsR;
		for (int i = 0; i < pointsNum; i++)
		{
			// Compute crossover points and 3D coordinate
			cv::Point2f interPoint;
			getTarPoint(rRects[0], rRects[1], lines[i], points[i], interPoint);
			if (interPoint.x != 0 && interPoint.y != 0)
			{
				pointsL.push_back(points[i]);
				pointsR.push_back(interPoint);
			}
		}
		// compute 3D coordinates
		std::vector<cv::Point3d> points3DL;
		std::vector<cv::Point3d> points3DR;
		for (size_t i = 0; i < pointsL.size(); i++)
		{
			cv::Point3d xyzl;
			cv::Point3d xyzr;
			computeXYZ(pointsL[i], pointsR[i], intrinsics[0], intrinsics[1], R, t, xyzl, xyzr);
			points3DL.push_back(xyzl);
			points3DR.push_back(xyzr);
		}
		fitPlane(points3DL, planes[0]);
		fitPlane(points3DR, planes[1]);
		double tempD[2] = { 0 };
		computeCenterDiameter(points3DL, planes[0], xyzs[0], tempD[0]);
		computeCenterDiameter(points3DR, planes[1], xyzs[1], tempD[1]);
		diameter = (tempD[0] + tempD[1]) / 2;
	}

	void getDeltaDeep(
		cv::Vec4d plane1,
		cv::Vec4d plane2,
		double d1,
		double d2,
		double theta,
		double &delta,
		double &deep)
	{
		// Compute verticality of the hole
		double n1n2 = plane1[0] * plane2[0] + plane1[1] * plane2[1] + plane1[2] * plane2[2];
		delta = acos(n1n2) * 180 / CV_PI;

		// Compute deep of the hole
		theta = theta / 360 * CV_PI;
		if (d2 < d1)
		{
			std::swap(d1, d2);
		}
		deep = (d2 - d1) / (2 * tan(theta));
	}

	void getVerticalityDepth(
		const cv::Mat(&R1)[2],
		const cv::Mat(&t1)[2],
		double &verticality,
		double &depth)
	{
		// Compute verticality of the hole
		cv::Mat nw = (cv::Mat_<double>(3, 1) << 0, 0, 1.0);
		cv::Mat n1 = R1[0] * nw;
		cv::Mat n2 = R1[1] * nw;
		double n1n2 = n1.dot(n2);
		verticality = acos(n1n2);

		// Compute deep of the hole
		cv::Mat dt = t1[0] - t1[1];
		depth = sqrt(dt.dot(dt));
	}

	void conicReconstruction(
		const std::vector<cv::RotatedRect> &rRects,
		const cv::Mat(&intrinsics)[2],
		const cv::Mat &R,
		const cv::Mat &t,
		cv::Mat &R1,
		cv::Mat &t1,
		double &d,
		double &E,
		cv::Point3d(&xyz)[2])
	{
		// Reference: S.D.Ma, Conics-Based Stereo, Motion Estimation, and Pose Determination
		// Compute Q of 2 ellipses in the images
		cv::RotatedRect rect;
		std::vector<cv::Mat> Q(rRects.size());
		for (size_t i = 0; i < rRects.size(); i++)
		{
			rect = rRects[i];
			double cosA = cos(rect.angle / 180 * CV_PI);
			double sinA = sin(rect.angle / 180 * CV_PI);
			double a = rect.size.height / 2;
			double b = rect.size.width / 2;
			double c2A = cosA * cosA;
			double s2A = sinA * sinA;
			double scA = sinA * cosA;
			double _a2 = 1 / (a * a);
			double _b2 = 1 / (b * b);
			double A = s2A * _a2 + c2A * _b2;
			double B = 2 * scA * (_b2 - _a2);
			double C = c2A * _a2 + s2A * _b2;
			double D = -2 * rect.center.x * A - rect.center.y * B;
			double E = -2 * rect.center.y * C - rect.center.x * B;
			double F = rect.center.x * rect.center.x * A + rect.center.y * rect.center.y * C + rect.center.x * rect.center.y * B - 1;
			Q[i] = (cv::Mat_<double>(3, 3) <<
				A, B / 2, D / 2,
				B / 2, C, E / 2,
				D / 2, E / 2, F);
		}
		Q[0] = intrinsics[0].t() * Q[0] * intrinsics[0];
		Q[1] = intrinsics[1].t() * Q[1] * intrinsics[1];

		// Compute eigenvalues and eigenvectors of M
		cv::Mat M = (R.t() * Q[1] * R).inv() * Q[0];
		cv::Mat eigenvaluesM;
		cv::Mat eigenvectorsM;
		cv::eigenNonSymmetric(M, eigenvaluesM, eigenvectorsM);
		double k = eigenvaluesM.at<double>(0);

		// C = Q1 - k * R' * Q2 * R
		// Compute eigenvalues and eigenvectors of C
		// Suppose that l1, l2 are 2 nonzero eigenvalues of C
		// s1, s2 are the correspondent eigenvectors
		// The third column of R1 is given by:
		// r13 = +-norm[(sqrt(|l1|) * s1 +- sqrt(|l2|) * s2)]
		cv::Mat C = Q[0] - k * R.t() * Q[1] * R;
		cv::Mat eigenvaluesC;
		cv::Mat eigenvectorsC;
		cv::eigen(C, eigenvaluesC, eigenvectorsC);
		std::vector<cv::Mat> R1s(3);
		std::vector<cv::Mat> tempR1r1s(2);
		tempR1r1s[0] = sqrt(abs(eigenvaluesC.at<double>(0))) * eigenvectorsC.row(0).t() - sqrt(abs(eigenvaluesC.at<double>(2))) * eigenvectorsC.row(2).t();
		tempR1r1s[1] = sqrt(abs(eigenvaluesC.at<double>(0))) * eigenvectorsC.row(0).t() + sqrt(abs(eigenvaluesC.at<double>(2))) * eigenvectorsC.row(2).t();
		cv::normalize(tempR1r1s[0], tempR1r1s[0], 1, 0, cv::NORM_L2);
		cv::normalize(tempR1r1s[1], tempR1r1s[1], 1, 0, cv::NORM_L2);

		// H = Q1 - r13 * r13' * Q1.
		// Because H * r11 = (r11' * Q1 * r11) * r11
		// and H * r12 = (r12' * Q1 * r12) * r12
		// Then r11, r12 are 2 eigenvectors of H
		// lh1 = r11' * Q1 * r11, lh2 = r12' * Q1 * r12 are correspondent eigenvalues
		std::vector<cv::Mat> Hs(2);
		std::vector<cv::Mat> eigenvaluesHs(2);
		std::vector<cv::Mat> eigenvectorsHs(2);
		Hs[0] = Q[0] - tempR1r1s[0] * tempR1r1s[0].t() * Q[0];
		Hs[1] = Q[0] - tempR1r1s[1] * tempR1r1s[1].t() * Q[0];
		cv::eigenNonSymmetric(Hs[0], eigenvaluesHs[0], eigenvectorsHs[0]);
		cv::eigenNonSymmetric(Hs[1], eigenvaluesHs[1], eigenvectorsHs[1]);
		std::vector<double> diffEigenValues(2);
		diffEigenValues[0] = eigenvaluesHs[0].at<double>(0) - eigenvaluesHs[0].at<double>(1);
		diffEigenValues[1] = eigenvaluesHs[1].at<double>(0) - eigenvaluesHs[1].at<double>(1);
		cv::Mat eigenvectorH;
		if (diffEigenValues[0] < diffEigenValues[1])
		{
			R1s[0] = eigenvectorsHs[0].row(0).t();
			R1s[1] = eigenvectorsHs[0].row(1).t();
			R1s[2] = tempR1r1s[0];
			eigenvectorH = eigenvaluesHs[0];
		}
		else
		{
			R1s[0] = eigenvectorsHs[1].row(0).t();
			R1s[1] = eigenvectorsHs[1].row(1).t();
			R1s[2] = tempR1r1s[1];
			eigenvectorH = eigenvaluesHs[1];
		}

		// Obtain R2 by R2 = R * R1
		cv::Mat R2;
		cv::hconcat(R1s, R1);
		R2 = R * R1;

		// Because it is ellipse, we can get 3 equations:
		// t1' * Q1 * r11 = 0
		// t1' * Q1 * r12 = 0
		// t2' * Q2 * r21 = 0
		// And we already have t2 = R * t1 + t
		// k1 = -t1' * Q1 * t1
		// k2 = k / k1
		// a^2 = k1 / (r11' * Q1 * r11)
		// b^2 = k1 / (r12' * Q1 * r12)
		cv::Mat A(4, 3, CV_64FC1);
		A.row(0) = R1s[0].t() * Q[0];
		A.row(1) = R1s[1].t() * Q[0];
		A.row(2) = R2.col(0).t() * Q[1] * R;
		A.row(3) = R2.col(1).t() * Q[1] * R;
		cv::Mat beta_2 = -R2.col(0).t() * Q[1] * t;
		cv::Mat beta_3 = -R2.col(1).t() * Q[1] * t;
		cv::Mat beta = (cv::Mat_<double>(4, 1) << 0, 0, beta_2.at<double>(0), beta_3.at<double>(0));
		t1 = (A.t() * A).inv() * A.t() * beta;
		cv::Mat t2 = R * t1 + t;
		cv::Mat temp = -t1.t() * Q[0] * t1;
		double k1 = temp.at<double>(0);
		double k2 = k1 / k;
		/*temp = R1s[0].t() * Q[0] * R1s[0];
		double a = sqrt(k1 / temp.at<double>(0));*/
		double a = sqrt(k1 / eigenvectorH.at<double>(0));
		/*temp = R1s[1].t() * Q[0] * R1s[1];
		double b = sqrt(k1 / temp.at<double>(0));*/
		double b = sqrt(k1 / eigenvectorH.at<double>(1));
		d = a + b;
		xyz[0] = cv::Point3d(t1.at<double>(0), t1.at<double>(1), t1.at<double>(2));
		xyz[1] = cv::Point3d(t2.at<double>(0), t2.at<double>(1), t2.at<double>(2));

		// Compute errors
		std::vector<cv::Mat> G1s;
		G1s.push_back(R1s[0]);
		G1s.push_back(R1s[1]);
		G1s.push_back(t1);
		cv::Mat G1;
		cv::hconcat(G1s, G1);
		std::vector<cv::Mat> G2s;
		G2s.push_back(R2.col(0));
		G2s.push_back(R2.col(1));
		G2s.push_back(t2);
		cv::Mat G2;
		cv::hconcat(G2s, G2);
		cv::Mat E1 = G1.t() * Q[0] * G1;
		cv::Mat E2 = G2.t() * Q[1] * G2;
		cv::Mat Qs = (cv::Mat_<double>(3, 3) <<
			1 / (a * a), 0, 0,
			0, 1 / (b * b), 0,
			0, 0, -1);
		E = cv::norm(E1 - k1 * Qs) + cv::norm(E2 - k2 * Qs);
	}

	MSInfoCode saveHoleParameters(
		const std::string& fileName,
		cv::Point3d center[][2],
		double diameter[],
		double verticality,
		double depth)
	{
		// Save calibration parameters
		cv::FileStorage fs(fileName, cv::FileStorage::WRITE || cv::FileStorage::APPEND);
		if (!fs.isOpened())
		{
			return MS_SAVE_FIEL_ERROR;
		}
		// Here should not exist spaces in the name.
		fs << "Left_Center_1" << center[0][0]
			<< "Left_Center_2" << center[1][0]
			<< "Right_Center_1" << center[0][1]
			<< "Right_Center_2" << center[1][1];
		fs << "Diameter_1" << diameter[0]
			<< "Diameter_2" << diameter[1]
			<< "Delta" << verticality
			<< "Deep" << depth;
		fs.release();
		return MS_SUCCESS;
	}
} // namespace ms