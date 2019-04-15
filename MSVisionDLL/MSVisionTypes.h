/**
 * Types for error or exception handling.
 * @author:		Sny
 * @version:	1.0
 * @since:		2019-01-17
 */

#pragma once

#include <opencv2/opencv.hpp>

namespace ms
{
	// Constant variables
	const int CamsNum = 2;
	const int RansacBatch = 5;

	// An enum type for error or exceptions process
	enum MSInfoCode
	{
		MS_SUCCESS = 0,
		MS_ARGUMENT_ERROR = -1,
		MS_OPEN_FILE_ERROR = -2,
		MS_SAVE_FIEL_ERROR = -3,
		MS_FIT_ELLIPSE_ERROR = -4,
		MS_GET_ELLIPSE_ERROR = -5,
		MS_PYR_ELLIPSE_ERROR = -6,
		MS_ROI_SIZE_ERROR = -7,
		MS_IMG_TYPE_ERROR = -8,
		MS_DIVIDE_ZERO_ERROR = -9
	};

	// An enum for color selection
	enum MSChannel
	{
		MS_DEFAULT_CHANNEL = 0x00,
		MS_BLUE = 0x01,
		MS_GREEN = 0x02,
		MS_RED = 0x04
	};

	// An enum for fitting ellipse
	enum MSFitMethod
	{
		FIT_ELLIPSE_GENERAL = 0,
		FIT_ELLIPSE_AMS,
		FIT_ELLIPSE_DIRECT,
		FIT_ELLIPSE_RANSAC
	};

	// An enum for selecting light channel
	enum MSLightChannel
	{
		LIGHT_CHANNEL1 = 1,
		LIGHT_CHANNEL2,
		LIGHT_CHANNEL3,
		LIGHT_CHANNEL4
	};

	// An enum for light mode
	enum MSLightMode
	{
		LIGHT_CONSTANT = 0,
		LIGHT_TRIGGER = 1,
		LIGHT_AUTOSTROBE = 3
	};

	// An enum for light state
	enum MSLightState
	{
		LIGHT_ON = 1,
		LIGHT_OFF = 0
	};
} // namespace ms