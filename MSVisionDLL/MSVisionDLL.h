/**
 * Binocular Stereo Vision Realization based on MicroVision Cameras.
 * @filename:	MSVisionDLL.h
 * @author:		Sny
 * @version:	1.0
 * @since:		2018-12-12
 */

#pragma once

#ifndef EXPORT_DLL
#define EXPORT_API	__declspec(dllimport)
#else
#define EXPORT_API	__declspec(dllexport)
#endif

#ifndef __cplusplus
#error ms_visiondll.cpp must be compiled as C++
#endif

// INCLUDES
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "MSVisionTypes.h"

namespace ms
{
	/**
	 * Read binocular cameras calibration parameters.
	 * @params
	 *		fileName	- input XML file
	 *		intrinsics	- intrinsic paramters
	 *		distCoeffs	- distort coefficients
	 *		R			- rotation matrix of binocular vision
	 *		t			- translation vector of binocular vision
	 * @return
	 */
	EXPORT_API MSInfoCode readBinoCalibParams(
		const std::string &fileName,
		cv::Mat (&intrinsics)[2],
		cv::Mat (&distCoeffs)[2],
		cv::Mat &R,
		cv::Mat &t);

	/**
	 * Fit ellipse by RANSAC method.
	 * @params
	 *		points
	 *		rRect
	 *		fitMethod
	 * @return
	 */
	EXPORT_API MSInfoCode fitEllipse(
		std::vector<cv::Point> points,
		cv::RotatedRect &rRect,
		MSFitEllipseMethod fitMethod);

	/**
	 * Get ellipses from a gray image.
	 * @params
	 *		src
	 *		rRects
	 *		curveNum
	 *		curveSize
	 *		delta
	 * @return
	 */
	EXPORT_API MSInfoCode grabEllipse(
		const cv::Mat &src,
		std::vector<cv::RotatedRect> &rRects,
		float delta,
		int curveNum,
		int segNum,
		int curveSize,
		double angleThreshold = 15.0,
		double distThreshold = 4.0,
		MSFitEllipseMethod fitMethod = FIT_ELLIPSE_DIRECT);

	/**
	 * Gaussian Pyramid method for grabbing ellipses.
	 * @params
	 *		src
	 *		rRects
	 *		curveNum
	 *		layerNum
	 *		distThreshold
	 * @return
	 */
	EXPORT_API MSInfoCode pyrEllipse(
		const cv::Mat &src,
		std::vector<cv::RotatedRect> &rRects,
		int curveNum = 2,
		int layerNum = 4,
		int kernelSize = 3,
		int segNum = 10,
		float distThreshold = 2.f);

	/**
	 * Get the center and diameter of the hole.
	 * @params
	 *		rRects
	 *		planes
	 *		xyzs
	 *		diameters
	 *		pointsNum
	 *		intrinsics
	 *		R
	 *		t
	 * @return
	 */
	EXPORT_API void getCenterDiameter(
		cv::RotatedRect rRects[],
		cv::Vec4d planes[],
		cv::Point3d xyzs[],
		double &diameter,
		int pointsNum,
		cv::Mat intrinsics[],
		cv::Mat R,
		cv::Mat t);

	/**
	 * Get the delta angle and deep of the hole.
	 * @params
	 *		plane1
	 *		plane2
	 *		d1
	 *		d2
	 *		theta
	 *		delta
	 *		deep
	 * @return
	 */
	EXPORT_API void getDeltaDeep(
		cv::Vec4d plane1,
		cv::Vec4d plane2,
		double d1,
		double d2,
		double theta,
		double &delta,
		double &deep);

	/**
	 * @Override
	 *		Get the delta angle and deep of the hole.
	 * @params
	 *		R1
	 *		t1
	 *		delta
	 *		deep
	 * @return
	 */
	EXPORT_API void getVerticalityDepth(
		const cv::Mat(&R1)[2],
		const cv::Mat(&t1)[2],
		double &verticality,
		double &depth);

	/**
	 * Reconstruction an ellipse from 2 images.
	 * @params
	 *		rects
	 *		R1
	 *		t1
	 *		d
	 *		E
	 *		xyz
	 *		intrinsics
	 *		R
	 *		t
	 * @return
	 */
	EXPORT_API void conicReconstruction(
		const std::vector<cv::RotatedRect> &rRects,
		const cv::Mat(&intrinsics)[2],
		const cv::Mat &R,
		const cv::Mat &t,
		cv::Mat &R1,
		cv::Mat &t1,
		double &d,
		double &E,
		cv::Point3d(&xyz)[2]);

	/**
	 * Save the result of detecting
	 * @params
	 *		fileName
	 *		center
	 *		diameter
	 *		delta
	 *		deep
	 * @return
	 */
	EXPORT_API MSInfoCode saveHoleParameters(
		const std::string &fileName,
		cv::Point3d center[][2],
		double diameter[],
		double delta,
		double deep);
} // namespace ms