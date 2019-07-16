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
	 *		fileName	- input XML file of calibration
	 *		intrinsics	- intrinsic paramters of cameras
	 *		distCoeffs	- distort coefficients of cameras
	 *		R			- rotation matrix of binocular vision
	 *		t			- translation vector of binocular vision
	 * @return
	 *		MSInfoCode	- state of the code
	 */
	EXPORT_API MSInfoCode readBinoCalibParams(
		const std::string &fileName,
		cv::Mat (&intrinsics)[CamsNum],
		cv::Mat (&distCoeffs)[CamsNum],
		cv::Mat &R,
		cv::Mat &t);

	/**
	 * Fit ellipse by RANSAC method.
	 * @params
	 *		points		- input points for fitting ellipse
	 *		rRect		- output RotatedRect of the ellipse
	 *		delta		- threshold for finding compatible points
	 *		confidence	- confidence for RANSAC method
	 *		maxIters	- maximum iteration times of sampling
	 *		fitMethod	- interial fitting method
	 * @return
	 *		MSInfoCode	- state of the code
	 */
	EXPORT_API MSInfoCode fitEllipseRANSAC(
		const cv::Mat &src,
		const std::vector<cv::Point> &points,
		cv::RotatedRect &rRect,
		float delta,
		double confidence,
		int maxIters,
		MSFitMethod fitMethod);

	/**
	 * Get ellipses from a gray image.
	 * @params
	 *		src				- input gray image
	 *		rRects			- output RotatedRect set of ellipses
	 *		delta			- threshold for finding compatible points
	 *		curveNum		- ellipses' number to grab
	 *		segNum			- points number of a segment
	 *		curveSize		- threshold of curve size
	 *		angleThreshold	- threshold of curvature angle
	 *		distThreshold	- threshold of neighbor distance
	 *		fitMethod		- interial fitting method
	 * @return
	 *		MSInfoCode	- state of the code
	 */
	EXPORT_API MSInfoCode grabEllipse(
		const cv::Mat &src,
		std::vector<cv::RotatedRect> &rRects,
		float delta,
		int curveNum,
		int segNum,
		int curveSize,
		double angleThreshold,
		double distThreshold,
		MSFitMethod fitMethod);

	/**
	 * Linear transformation of an image.
	 * @params
	 *		src		- input image
	 *		dst		- output transformed image
	 *		alpha	- multiplier
	 *		beta	- constant offset
	 * @return
	 *		void
	 */
	EXPORT_API void imgLinearTrans(
		const cv::Mat &src,
		cv::Mat &dst,
		float alpha,
		float beta);

	/**
	 * Gamma transformation of an image.
	 * @params
	 *		src		- input image
	 *		dst		- output transformed image
	 *		fGamma	- gamma factor
	 * @return
	 *		void
	 */
	EXPORT_API void imgGammaTrans(
		const cv::Mat &src,
		cv::Mat &dst,
		double fGamma);

	/**
	 * Adjust the brightness of an image.
	 * @params
	 *		src			- input image
	 *		dst			- output transformed image
	 *		refBright	- referent brightness value
	 * @return
	 *		MSInfoCode	- state of the code
	 */
	EXPORT_API MSInfoCode adjustBright(
		const cv::Mat &src,
		cv::Mat &dst,
		float refBright);

	/**
	 * Get focus entropy of an image for autofocus.
	 * @params
	 *		src		- input image
	 * @return
	 *		float	- focus entropy value
	 */
	EXPORT_API float getFocusEntropy(
		const cv::Mat &src);

	/**
	 * Edge detection by Zernike moments.
	 * @params
	 *		src			- input image
	 *		dst			- output transformed image
	 * @return
	 *		MSInfoCode	- state of the code
	 */
	EXPORT_API MSInfoCode zernikeDetect(
		const cv::Mat &src,
		cv::Mat &dst);

	/**
	 * Gaussian Pyramid method for grabbing ellipses.
	 * @params
	 *		src				- input image
	 *		rRects			- output RotatedRect set of ellipses
	 *		curveNum		- ellipses' number
	 *		layerNum		- layers number
	 *		kernelSize		- size of Gaussian filter kernel
	 *		segNum			- points number of a segment
	 *		distThreshold	- threshold of neighbor distance
	 *		MSFitMethod		- internal fitting method
	 * @return
	 *		MSInfoCode		- state of the code
	 */
	EXPORT_API MSInfoCode pyrEllipse(
		const cv::Mat &src,
		std::vector<cv::RotatedRect> &rRects,
		int curveNum,
		int layerNum,
		int kernelSize,
		double fGamma,
		double fBilateral,
		int segNum,
		float distThreshold,
		int channelsFlag,
		MSFitMethod MSFitMethod);

	/**
	 * Get the center and diameter of the hole.
	 * @params
	 *		rRects		- output RotatedRect set of ellipses
	 *		intrinsics	- intrinsic paramters of cameras
	 *		R			- rotation matrix of binocular vision
	 *		t			- translation vector of binocular vision
	 *		planes		- planes of ellipses
	 *		xyzs		- coordinates of center in 2 cameras
	 *		diameter	- diameter of the hole
	 *		pointsNum	- points number for matching
	 * @return
	 *		void
	 */
	EXPORT_API void getCenterDiameter(
		const std::vector<cv::RotatedRect> &rRects,
		const cv::Mat (&intrinsics)[CamsNum],
		const cv::Mat &R,
		const cv::Mat &t,
		cv::Vec4d (&planes)[CamsNum],
		cv::Point3d (&xyzs)[CamsNum],
		double &diameter,
		int pointsNum);

	/**
	 * Get the delta angle and deep of the hole.
	 * @params
	 *		plane1	- plane of ellipse 1
	 *		plane2	- plane of ellipse 2
	 *		d1		- diameter of ellipse 1
	 *		d2		- diameter of ellipse 2
	 *		theta	- angle of the drilling bit
	 *		delta	- verticality of the hole
	 *		deep	- depth of the hole
	 * @return
	 *		void
	 */
	EXPORT_API void getDeltaDeep(
		const cv::Vec4d &plane1,
		const cv::Vec4d &plane2,
		double d1,
		double d2,
		double theta,
		double &delta,
		double &deep);

	/**
	 * @Override
	 *		Get the delta angle and deep of the hole.
	 * @params
	 *		R1		- rotation matries of camera 1
	 *		t1		- translation vectors of camera 1
	 *		delta	- verticality of the hole
	 *		deep	- depth of the hole
	 * @return
	 *		void
	 */
	EXPORT_API void getVerticalityDepth(
		const cv::Mat(&R1)[2],
		const cv::Mat(&t1)[2],
		double &verticality,
		double &depth);

	/**
	 * Reconstruction an ellipse from 2 images.
	 * @params
	 *		rRects		- output RotatedRect set of ellipses
	 *		intrinsics	- intrinsic paramters of cameras
	 *		R			- rotation matrix of binocular vision
	 *		t			- translation vector of binocular vision
	 *		R1			- rotation matrix of camera 1
	 *		t1			- translation vector of camera 1
	 *		d			- diameter of the ellipse
	 *		E			- error value of reconstruction
	 *		xyz			- coordinate of hole center
	 * @return
	 *		void
	 */
	EXPORT_API void conicReconstruction(
		const std::vector<cv::RotatedRect> &rRects,
		const cv::Mat(&intrinsics)[CamsNum],
		const cv::Mat &R,
		const cv::Mat &t,
		cv::Mat &R1,
		cv::Mat &t1,
		double &d,
		double &E,
		cv::Point3d(&xyz)[CamsNum]);

	/**
	 * Save the result of detecting
	 * @params
	 *		fileName	- input XML file of calibration
	 *		center		- centers of ellipses for 2 cameras
	 *		diameter	- diameters of ellipses
	 *		delta		- verticality of the hole
	 *		deep		- depth of the hole
	 * @return
	 *		MSInfoCode	- state of the code
	 */
	EXPORT_API MSInfoCode saveHoleParameters(
		const std::string &fileName,
		cv::Point3d center[2][CamsNum],
		double diameter[2],
		double delta,
		double deep);
} // namespace ms