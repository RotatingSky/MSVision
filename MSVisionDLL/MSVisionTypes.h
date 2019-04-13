/**
 * Types for error or exception handling.
 * @author:		Sny
 * @version:	1.0
 * @since:		2019-01-17
 */

#pragma once

#include <opencv2/opencv.hpp>

// Constant variables
const int CamsNum = 2;
const int CurvesNum = 2;
const int RansacBatch = 5;

// An enum type for error or exceptions process
typedef enum
{
	MS_SUCCESS				= 0,
	MS_ARGUMENT_ERROR		= -1,
	MS_OPEN_FILE_ERROR		= -2,
	MS_SAVE_FIEL_ERROR		= -3,
	MS_FIT_ELLIPSE_ERROR	= -4,
	MS_GET_ELLIPSE_ERROR	= -5,
	MS_PYR_ELLIPSE_ERROR	= -6,
	MS_ROI_SIZE_ERROR		= -7,
	MS_IMG_TYPE_ERROR		= -8,
	MS_DIVIDE_ZERO_ERROR	= -9
} MSInfoCode;

// Const expressions for color selection
typedef enum
{
	MS_DEFAULT_CHANNEL = 0x00,
	MS_BLUE = 0x01,
	MS_GREEN = 0x02,
	MS_RED = 0x04
} MSVChannel;

typedef enum
{
	FIT_ELLIPSE_GENERAL = 0,
	FIT_ELLIPSE_AMS,
	FIT_ELLIPSE_DIRECT,
	FIT_ELLIPSE_RANSAC
}MSFitEllipseMethod;
