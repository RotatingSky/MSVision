#pragma execution_character_set("utf-8")


#include "stdafx.h"
#include "MSVisionQt.h"
#include "LightControl.h"


MSVisionQt::MSVisionQt(QWidget *parent)
	: QMainWindow(parent),
	labelImgL(this),
	labelImgR(this),
	numCam(0),
	linkedNumCam(0),
	imgScale(-1),
	saveImgPath(""),
	mvInitLibFlag(false),
	linkedFlag(false),
	lightConnectFlag(false),
	lightFlag(false),
	loadFlag(false),
	sharpFlag(true),
	restoreFlag(false),
	showFlag(0),
	lightChannel(CHANNEL1),
	mvHoleType(CountersinkHole),
	fitMethod(FIT_ELLIPSE_DIRECT),
	scope(0.75f),
	pointsNum(53),
	bitAngle(100.0)
{
	ui.setupUi(this);

	// Initialize gui of images
	ui.horizontalLayoutImgL->addWidget(&labelImgL);
	ui.horizontalLayoutImgR->addWidget(&labelImgR);
	labelImgL.setText("Image 1");
	labelImgR.setText("Image 2");

	// Initialize gui of light channels
	channelBtns = new QButtonGroup();
	channelBtns->addButton(ui.channel1Btn, 0);
	channelBtns->addButton(ui.channel2Btn, 1);
	channelBtns->addButton(ui.channel3Btn, 2);
	channelBtns->addButton(ui.channel4Btn, 3);
	channelBtns->setExclusive(true);

	// Initialize gui of detection
	holeTypeRadBtns = new QButtonGroup();
	holeTypeRadBtns->addButton(ui.straightHole, 0);
	holeTypeRadBtns->addButton(ui.countersinkHole, 1);
	holeTypeRadBtns->setExclusive(true);
	fitMethodRadBtns = new QButtonGroup();
	fitMethodRadBtns->addButton(ui.radioButtonAMS, 0);
	fitMethodRadBtns->addButton(ui.radioButtonB2AC, 1);
	fitMethodRadBtns->addButton(ui.radioButtonRANSAC, 2);
	fitMethodRadBtns->setExclusive(true);
	measureRadBtns = new QButtonGroup();
	measureRadBtns->addButton(ui.radioButtonEpipolar, 0);
	measureRadBtns->addButton(ui.radioButtonRebuild, 1);

	// Initialize variables
	for (int i = 0; i < CAM_NUM; i++)
	{
		mvCamHandles[i] = NULL;
		mvProperties[i] = NULL;
	}

	// Initialize cameras
	initCam();

	// Qt Connection
	connect(ui.linkBtn, SIGNAL(clicked()), this, SLOT(on_linkCam()));
	connect(ui.sampleBtn, SIGNAL(clicked()), this, SLOT(on_startCam()));
	connect(ui.stopSampleBtn, SIGNAL(clicked()), this, SLOT(on_stopCam()));
	connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(on_saveImg()));
	connect(ui.proptyC1Btn, SIGNAL(clicked()), this, SLOT(on_setCam0()));
	connect(ui.proptyC2Btn, SIGNAL(clicked()), this, SLOT(on_setCam1()));
	connect(ui.comboBoxScale, SIGNAL(currentIndexChanged(int)), this, SLOT(on_changeScale(int)));
	connect(ui.checkBoxCross, SIGNAL(stateChanged(int)), this, SLOT(on_checkCross()));
	connect(ui.connectLightBtn, SIGNAL(clicked()), this, SLOT(on_connect()));
	connect(ui.spinBoxIndensity, SIGNAL(valueChanged(int)), this, SLOT(on_indensity()));
	connect(channelBtns, SIGNAL(buttonClicked(int)), this, SLOT(on_changeChannel(int)));
	connect(ui.spinCurrent, SIGNAL(valueChanged(int)), this, SLOT(on_multipier()));
	connect(ui.onOffBtn, SIGNAL(clicked()), this, SLOT(on_lightOnOff()));
	connect(ui.spinStrobeWidth, SIGNAL(valueChanged(int)), this, SLOT(on_strobeWidth()));
	connect(ui.loadDataBtn, SIGNAL(clicked()), this, SLOT(on_loadData()));
	connect(holeTypeRadBtns, SIGNAL(buttonClicked(int)), this, SLOT(on_holeType(int)));
	connect(fitMethodRadBtns, SIGNAL(buttonClicked(int)), this, SLOT(on_fitMethod(int)));
	connect(measureRadBtns, SIGNAL(buttonClicked(int)), this, SLOT(on_measureType(int)));
	connect(ui.checkBoxSharpen, SIGNAL(stateChanged(int)), this, SLOT(on_sharpen()));
	connect(ui.checkBoxRestore, SIGNAL(stateChanged(int)), this, SLOT(on_restore()));
	connect(ui.detectBtn, SIGNAL(clicked()), this, SLOT(on_detect()));
}

MSVisionQt::~MSVisionQt()
{
	for (int i = 0; i < CAM_NUM; i++)
	{
		MVCamProptySheetDestroy(mvProperties[i]);
	}
	on_stopCam();
	api_LE_SetCONSTOnOff(lightChannel, LIGHT_OFF);
	MVTerminateLib();
}

void MSVisionQt::initCam()
{
	// Initialization MV Camera library
	switch (MVInitLib())
	{
	case MVST_SUCCESS: mvInitLibFlag = true; break;
	case MVST_ERROR: WARNMSG("General initialization error"); break;
	case MVST_ERR_NOT_INITIALIZED: WARNMSG("Failed to initialization cameras"); break;
	case MVST_ERR_NOT_IMPLEMENTED: WARNMSG("Failed to realize when initialization"); break;
	case MVST_ERR_RESOURCE_IN_USE: WARNMSG("Resource in use when initialization"); break;
	case MVST_ACCESS_DENIED: WARNMSG("Access denied when initialization"); break;
	case MVST_INVALID_HANDLE: WARNMSG("Invalid handle"); break;
	case MVST_INVALID_ID: WARNMSG("Invalid camera ID"); break;
	case MVST_INVALID_PARAMETER: WARNMSG("Invalid parameters"); break;
	case MVST_INVALID_ADDRESS: WARNMSG("Invalid address"); break;
	case MVST_INVALID_BUFFER_SIZE: WARNMSG("Buffer size wrong"); break;
	case MVST_NO_DATA: WARNMSG("No data"); break;
	case MVST_FILE_IO: WARNMSG("IO wrong"); break;
	case MVST_TIMEOUT: WARNMSG("Time out"); break;
	case MVST_ERR_ABORT: WARNMSG("Abort and break"); break;
	case MVST_ERR_NOT_AVAILABLE: WARNMSG("Cannot available"); break;
	default: break;
	}

	if (mvInitLibFlag)
	{
		MVGetNumOfCameras(&numCam);
		if (numCam == CAM_NUM)
		{
			for (int i = 0; i < CAM_NUM; i++)
			{
				MVGetCameraInfo(i, &mvCamInfos[i]);
			}
			// Show information of cameras
			ui.camInfoC1->setText(QString(mvCamInfos[0].mModelName) + "  " + QString(mvCamInfos[0].mSerialNumber));
			ui.camInfoC2->setText(QString(mvCamInfos[1].mModelName) + "  " + QString(mvCamInfos[1].mSerialNumber));
		}
		else if(numCam <= 0)
		{
			WARNMSG("Cannot find cameras, please check the link or IP configuration");
			exit(-1);
		}
		else
		{
			WARNMSG("Cannot find all cameras, please check the linke or IP configuration");
			exit(-1);
		}
	}
	else
	{
		WARNMSG("Cannot initialize cameras");
		exit(-1);
	}
}

void MSVisionQt::drawImgs(int index)
{
	// Calculate size parameters
	int wImg = 0;
	int hImg = 0;
	if (imgScale > 0)
	{
		if (index != -1)
		{
			wImg = qtImgs[index].width() / imgScale;
			hImg = qtImgs[index].height() / imgScale;
		}
		else
		{
			wImg = showQtImgs[index].width() / imgScale;
			hImg = showQtImgs[index].height() / imgScale;
		}
	}
	else
	{
		wImg = ui.scrollAreaL->width();
		hImg = ui.scrollAreaL->height();
	}
	// Draw images in labels
	QImage showImgs[CAM_NUM];
	if (index == 0)
	{
		showImgs[0] = qtImgs[0].scaled(wImg, hImg, Qt::KeepAspectRatio);
		labelImgL.setPixmap(QPixmap::fromImage(showImgs[0]));
	}
	else if (index == 1)
	{
		showImgs[1] = qtImgs[1].scaled(wImg, hImg, Qt::KeepAspectRatio);
		labelImgR.setPixmap(QPixmap::fromImage(showImgs[1]));
	}
	else if (index == -1)
	{
		showImgs[0] = showQtImgs[0].scaled(wImg, hImg, Qt::KeepAspectRatio);
		labelImgL.setPixmap(QPixmap::fromImage(showImgs[0]));
		showImgs[1] = showQtImgs[1].scaled(wImg, hImg, Qt::KeepAspectRatio);
		labelImgR.setPixmap(QPixmap::fromImage(showImgs[1]));
	}
}

cv::Mat MSVisionQt::qImage2cvMat(QImage qtImg)
{
	cv::Mat matImg;
	switch (qtImg.format())
	{
	case QImage::Format_RGB888:
		matImg = cv::Mat(qtImg.height(), qtImg.width(), CV_8UC3, (void*)qtImg.constBits(), qtImg.bytesPerLine());
		break;
	case QImage::Format_Indexed8:
		matImg = cv::Mat(qtImg.height(), qtImg.width(), CV_8UC1, (void*)qtImg.constBits(), qtImg.bytesPerLine());
		break;
	default:
		matImg = cv::Mat();
		break;
	}
	return matImg;
}

QImage MSVisionQt::cvMat2qImage(cv::Mat matImg)
{
	QImage qtImg;
	if (matImg.channels() == 1)
	{
		qtImg = QImage(matImg.cols, matImg.rows, QImage::Format_Indexed8);
		qtImg.setColorCount(256);
		for (int i = 0; i < 256; ++i)
		{
			qtImg.setColor(i, qRgb(i, i, i));
		}
		uchar *pSrc = matImg.data;
		for (int row = 0; row < matImg.rows; ++row)
		{
			uchar *pDest = qtImg.scanLine(row);
			memcpy(pDest, pSrc, matImg.step);
			pSrc += matImg.step;
		}
	}
	else if (matImg.channels() == 3)
	{
		qtImg = QImage(matImg.cols, matImg.rows, QImage::Format_RGB888);
		uchar *pSrc = matImg.data;
		for (int row = 0; row < matImg.rows; ++row)
		{
			uchar *pDest = qtImg.scanLine(row);
			memcpy(pDest, pSrc, matImg.step);
			pSrc += matImg.step;
		}
	}
	else
	{
		qtImg = QImage();
	}
	return qtImg;
}

MSInfoCode MSVisionQt::detectImg(int index)
{
	// Parameters for detection
	int rRectsNum = 0;
	switch (mvHoleType)
	{
	case StraightHole: rRectsNum = 1; break;
	case CountersinkHole: rRectsNum = 2; break;
	}
	// Convert image to OpenCV Mat type
	cv::Mat srcImg;
	cv::Mat bgrImg;
	srcImg = qImage2cvMat(qtImgs[index]);
	cv::cvtColor(srcImg, bgrImg, cv::COLOR_RGB2BGR);
	// Undistort the images
	cv::Mat rectifyImg;
	cv::Mat mapx, mapy;
	cv::Size imageSize = bgrImg.size();
	cv::initUndistortRectifyMap(
		intrinsics[index],
		distCoeffs[index],
		cv::Mat(),
		cv::getOptimalNewCameraMatrix(intrinsics[index], distCoeffs[index], imageSize, 1, imageSize, 0),
		imageSize,
		CV_16SC2,
		mapx,
		mapy);
	cv::remap(bgrImg, rectifyImg, mapx, mapy, cv::INTER_LINEAR);
	// Cut the image
	int roiW = int(rectifyImg.cols * scope);
	int roiH = int(rectifyImg.rows * scope);
	cv::Rect roiRect;
	roiRect.x = (rectifyImg.cols - roiW) / 2;
	roiRect.y = (rectifyImg.rows - roiH) / 2;
	roiRect.width = roiW;
	roiRect.height = roiH;
	cv::Mat roiImg = rectifyImg(roiRect);
	// Sharpen the image
	cv::Mat tempImg;
	if (sharpFlag)
	{
		cv::Mat kernel = (cv::Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		cv::filter2D(roiImg, tempImg, roiImg.depth(), kernel);
	}
	else
	{
		tempImg = roiImg.clone();
	}
	// Grab ellipses from image and draw
	MSInfoCode status;
	status = ms::pyrEllipse(tempImg, rRects[index], rRectsNum, 4, 3, 10, 2.f);
	if (status != MS_SUCCESS)
	{
		return status;
	}
	cv::Mat showImg = rectifyImg(roiRect).clone();
	for (size_t i = 0; i < rRects[index].size(); i++)
	{
		cv::ellipse(showImg, rRects[index][i], cv::Scalar(0, 255, 0), 1, 8);
		cv::circle(showImg, rRects[index][i].center, 2, cv::Scalar(255, 0, 255), -1, 8);
	}
	cv::cvtColor(showImg, showImg, cv::COLOR_BGR2RGB);
	showQtImgs[index] = cvMat2qImage(showImg);
	// Convert rects to the original image.
	for (size_t i = 0; i < rRects[index].size(); i++)
	{
		rRects[index][i].center.x += roiRect.x;
		rRects[index][i].center.y += roiRect.y;
	}
	return MS_SUCCESS;
}

void MSVisionQt::measure()
{
	// Calculate centers and diameters of the hole
	if (mvHoleType == CountersinkHole)
	{
		/*
		cv::Point3d xyzs[2][CAM_NUM];
		cv::RotatedRect tempRects[CAM_NUM];
		cv::Vec4d planes[2][CAM_NUM];
		double diameters[2];
		for (int k = 0; k < 2; k++)
		{
			tempRects[0] = rRects[0][k];
			tempRects[1] = rRects[1][k];
			getCenterDiameter(tempRects, planes[k], xyzs[k], diameters[k], pointsNum, intrinsics, R, t);
		}
		// Calculate delta and deep of the hole
		double deltas[2];
		double deeps[2];
		getDeltaDeep(planes[0][0], planes[1][0], diameters[0], diameters[1], bitAngle, deltas[0], deeps[0]);
		getDeltaDeep(planes[0][1], planes[1][1], diameters[0], diameters[1], bitAngle, deltas[1], deeps[1]);
		double delta = (deltas[0] + deltas[1]) / 2;
		double deep = (deeps[0] + deeps[1]) / 2;
		*/
		// Reconstruct the ellipses
		cv::Point3d xyzs[2][CAM_NUM];
		std::vector<cv::RotatedRect> tempRects(2);
		cv::Mat R1[2];
		cv::Mat t1[2];
		double diameters[2];
		double E[2];
		for (int k = 0; k < 2; k++)
		{
			tempRects[0] = rRects[0][k];
			tempRects[1] = rRects[1][k];
			ms::conicReconstruction(tempRects, intrinsics, R, t, R1[k], t1[k], diameters[k], E[k], xyzs[k]);
		}
		// Calculate verticality and depth of the hole
		double delta = 0;
		double deep = 0;
		ms::getVerticalityDepth(R1, t1, delta, deep);
		delta *= 180 / CV_PI;
		// Display result
		QString xyzStrOuterC1 = "[" +
			QString::number(xyzs[0][0].x, 10, 3) + "," +
			QString::number(xyzs[0][0].y, 10, 3) + "," +
			QString::number(xyzs[0][0].z, 10, 3) + "]";
		QString xyzStrOuterC2 = "[" +
			QString::number(xyzs[0][1].x, 10, 3) + "," +
			QString::number(xyzs[0][1].y, 10, 3) + "," +
			QString::number(xyzs[0][1].z, 10, 3) + "]";
		QString xyzStrInnerC1 = "[" +
			QString::number(xyzs[1][0].x, 10, 3) + "," +
			QString::number(xyzs[1][0].y, 10, 3) + "," +
			QString::number(xyzs[1][0].z, 10, 3) + "]";
		QString xyzStrInnerC2 = "[" +
			QString::number(xyzs[1][1].x, 10, 3) + "," +
			QString::number(xyzs[1][1].y, 10, 3) + "," +
			QString::number(xyzs[1][1].z, 10, 3) + "]";
		QString strOuterD = QString::number(diameters[0]-0.0237, 10, 3);
		QString strInnerD = QString::number(diameters[1]+0.0048, 10, 3);
		QString strDelta = QString::number(delta, 10, 3);
		QString strDeep = QString::number(deep, 10, 3);
		ui.centerOuterC1->setText(xyzStrOuterC1);
		ui.centerOuterC2->setText(xyzStrOuterC2);
		ui.centerInnerC1->setText(xyzStrInnerC1);
		ui.centerInnerC2->setText(xyzStrInnerC2);
		ui.outerDiameter->setText(strOuterD);
		ui.innerDiameter->setText(strInnerD);
		ui.delta->setText(strDelta);
		ui.deep->setText(strDeep);
	}
	else if (mvHoleType == StraightHole)
	{
		/*cv::Point3d xyzs[CAM_NUM];
		cv::RotatedRect tempRects[CAM_NUM];
		cv::Vec4d planes[CAM_NUM];
		double diameter;
		tempRects[0] = rRects[0][0];
		tempRects[1] = rRects[1][0];
		getCenterDiameter(tempRects, planes, xyzs, diameter, pointsNum, intrinsics, R, t);*/
		cv::Point3d xyzs[CAM_NUM];
		std::vector<cv::RotatedRect> tempRects(2);
		cv::Mat R1;
		cv::Mat t1;
		double diameter;
		double E;
		tempRects[0] = rRects[0][0];
		tempRects[1] = rRects[1][0];
		ms::conicReconstruction(tempRects, intrinsics, R, t, R1, t1, diameter, E, xyzs);
		// Display results
		QString xyzStrC1 = "[" +
			QString::number(xyzs[0].x, 10, 3) + "," +
			QString::number(xyzs[0].y, 10, 3) + "," +
			QString::number(xyzs[0].z, 10, 3) + "]";
		QString xyzStrC2 = "[" +
			QString::number(xyzs[1].x, 10, 3) + "," +
			QString::number(xyzs[1].y, 10, 3) + "," +
			QString::number(xyzs[1].z, 10, 3) + "]";
		QString strD = QString::number(diameter, 10, 3);
		ui.centerOuterC1->setText(xyzStrC1);
		ui.centerOuterC2->setText(xyzStrC2);
		ui.outerDiameter->setText(strD);
	}
}

void MSVisionQt::on_linkCam()
{
	std::vector<bool> mvLinkCamState(numCam, false);
	if (!mvInitLibFlag)
	{
		initCam();
	}
	if (!linkedFlag)
	{
		for (int i = 0; i < CAM_NUM; i++)
		{
			switch (MVOpenCamByIndex(i, &mvCamHandles[i]))
			{
			case MVST_SUCCESS: mvLinkCamState[i] = true; break;
			case MVST_ERROR: WARNMSG("General link error"); break;
			case MVST_ERR_NOT_INITIALIZED: WARNMSG("Have not been initialized"); break;
			case MVST_ERR_NOT_IMPLEMENTED: WARNMSG("Have not been realized"); break;
			case MVST_ERR_RESOURCE_IN_USE: WARNMSG("Resource in use"); break;
			case MVST_ACCESS_DENIED: WARNMSG("Cannot access, maybe controlled by other software"); break;
			case MVST_INVALID_ID: WARNMSG("Invalid Camera ID"); break;
			case MVST_INVALID_HANDLE: WARNMSG("Invalid handle"); break;
			case MVST_INVALID_ADDRESS: WARNMSG("Invalid address"); break;
			case MVST_INVALID_BUFFER_SIZE: WARNMSG("Invalid buffer size"); break;
			case MVST_INVALID_PARAMETER: WARNMSG("Invalid parameter"); break;
			case MVST_NO_DATA: WARNMSG("No data"); break;
			case MVST_FILE_IO: WARNMSG("IO wrong"); break;
			case MVST_TIMEOUT: WARNMSG("Time out"); break;
			case MVST_ERR_ABORT: WARNMSG("Abort and break"); break;
			case MVST_ERR_NOT_AVAILABLE: WARNMSG("Cannot available"); break;
			default: break;
			}
			if (mvLinkCamState[i])
			{
				// Create images
				int width, height;
				MVGetWidth(mvCamHandles[i], &width);
				MVGetHeight(mvCamHandles[i], &height);
				MVGetPixelFormat(mvCamHandles[i], &mvPixelFormats[i]);
				mvImgs[i].CreateByPixelFormat(width, height, mvPixelFormats[i]);
				// Create property sheet
				std::string proptyTitle = "Propty C" + std::to_string(i + 1);
				MVCamProptySheetInit(&mvProperties[i], mvCamHandles[i], 0, LPCTSTR(proptyTitle.c_str()));
				linkedNumCam++;
				if (linkedNumCam == CAM_NUM - 1)
				{
					linkedFlag = true;
					ui.linkBtn->setText(u8"断开连接");
					ui.sampleBtn->setEnabled(true);
				}
			}
		}
	}
	else
	{
		on_stopCam();
		for (int i = 0; i < CAM_NUM; i++)
		{
			if (mvCamHandles[i] != NULL)
			{
				MVCloseCam(mvCamHandles[i]);
			}
		}
		linkedNumCam = 0;
		mvInitLibFlag = false;
		linkedFlag = false;
		ui.linkBtn->setText(u8"连接相机");
		ui.sampleBtn->setDisabled(true);
	}
}

int MSVisionQt::onStreamCB(MV_IMAGE_INFO *pInfo, int index)
{
	int w = pInfo->nSizeX;
	int h = pInfo->nSizeY;
	// Get images data
	if (mvPixelFormats[index] == PixelFormat_Mono8)
	{
		MVInfo2Image(mvCamHandles[index], pInfo, &mvImgs[index]);
		qtImgs[index] = QImage((uchar*)(mvImgs[index].GetBits()), mvImgs[index].GetWidth(), mvImgs[index].GetHeight(), QImage::Format_Indexed8);
		// Qt must use "lut" to show gray Image. 
		QVector<QRgb> grayTable;
		for (int i = 0; i < 256; i++)
		{
			grayTable.push_back(qRgb(i, i, i));
		}
		qtImgs[index].setColorTable(grayTable);
	}
	else
	{
		MVBayerToRGB(mvCamHandles[index], pInfo->pImageBuffer, mvImgs[index].GetBits(), mvImgs[index].GetPitch(), w, h, mvPixelFormats[index]);
		qtImgs[index] = QImage((uchar*)(mvImgs[index].GetBits()), mvImgs[index].GetWidth(), mvImgs[index].GetHeight(), QImage::Format_RGB888);
	}
	if (showFlag > 0)
	{
		drawImgs(-1);
		if (showFlag < 50)
		{
			showFlag++;
		}
		else
		{
			showFlag = 0;
		}
	}
	else
	{
		drawImgs(index);
	}
	return 0;
}

int __stdcall streamCB0(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal)
{
	MSVisionQt *pMainWin = (MSVisionQt *)nUserVal;
	return (pMainWin->onStreamCB(pInfo, 0));
}

int __stdcall streamCB1(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal)
{
	MSVisionQt *pMainWin = (MSVisionQt *)nUserVal;
	return (pMainWin->onStreamCB(pInfo, 1));
}

void MSVisionQt::on_startCam()
{
	ui.stopSampleBtn->setEnabled(true);
	ui.saveBtn->setEnabled(true);
	// Start grabbing images
	if (numCam == 2)
	{
		MVStartGrab(mvCamHandles[0], streamCB0, (ULONG_PTR)this);
		MVStartGrab(mvCamHandles[1], streamCB1, (ULONG_PTR)this);
	}
	else if(numCam == 1)
	{
		MVStartGrab(mvCamHandles[0], streamCB0, (ULONG_PTR)this);
	}
}

void MSVisionQt::on_stopCam()
{
	ui.stopSampleBtn->setDisabled(true);
	ui.saveBtn->setDisabled(true);
	// Stop grabbing images
	for (int i = 0; i < CAM_NUM; i++)
	{
		MVStopGrab(mvCamHandles[i]);
	}
}

void MSVisionQt::on_setCam0()
{
	MVCamProptySheetShow(mvProperties[0]);
}

void MSVisionQt::on_setCam1()
{
	MVCamProptySheetShow(mvProperties[1]);
}

void MSVisionQt::on_saveImg()
{
	if (saveImgPath == "")
	{
		saveImgPath = QFileDialog::getExistingDirectory(
			this,
			tr(u8"选择图片保存路径"),
			"./");
	}
	QDateTime time = QDateTime::currentDateTime();
	QString timeStr = time.toString("yyyyMMdd-hhmmss");
	if (!saveImgPath.isNull())
	{
		QString filePath = QString("%1/%2_").arg(saveImgPath).arg(timeStr);
		qtImgs[0].save(filePath + QString::number(0) + ".BMP");
		qtImgs[1].save(filePath + QString::number(1) + ".BMP");
	}
}

void MSVisionQt::on_changeScale(int index)
{
	switch (index)
	{
	case 0: imgScale = -1; break;
	case 1: imgScale = 4; break;
	case 2: imgScale = 2; break;
	case 3: imgScale = 1; break;
	}
}

void MSVisionQt::on_checkCross()
{
	if (ui.checkBoxCross->isChecked())
	{
		labelImgL.showCross(true);
		labelImgR.showCross(true);
	}
	else
	{
		labelImgL.showCross(false);
		labelImgR.showCross(false);
	}
}

void MSVisionQt::on_connect()
{
	QString conType = ui.comboBoxConnect->currentText();
	byte errCode;
	if (!lightConnectFlag)
	{
		if (conType == "USB")
		{
			errCode = api_LE_USBConnect();
		}
		else if (conType == "ComPort")
		{
			errCode = api_LE_ComportConnect((byte)ui.comboBoxChannel->currentIndex());
		}
		errCode = api_LE_SetCHMode(lightChannel, CONSTANT);
		lightConnectFlag = true;
		lightFlag = true;
		ui.comboBoxConnect->setDisabled(true);
		ui.comboBoxChannel->setDisabled(true);
		ui.connectLightBtn->setText(u8"断开");
	}
	else
	{
		if (conType == "USB")
		{
			errCode = api_LE_USBDisconnect();
		}
		else if (conType == "ComPort")
		{
			errCode = api_LE_ComportDisConnect((byte)ui.comboBoxChannel->currentIndex());
		}
		lightConnectFlag = false;
		ui.comboBoxConnect->setEnabled(true);
		ui.comboBoxChannel->setEnabled(true);
		ui.connectLightBtn->setText(u8"连接");
	}
}

void MSVisionQt::on_changeChannel(int index)
{
	switch (index)
	{
	case 0: lightChannel = CHANNEL1; break;
	case 1: lightChannel = CHANNEL2; break;
	case 2: lightChannel = CHANNEL3; break;
	case 3: lightChannel = CHANNEL4; break;
	}
}

void MSVisionQt::on_multipier()
{
	byte errCode;
	errCode = api_LE_SetChMultipier(lightChannel, (byte)ui.spinCurrent->value());
}

void MSVisionQt::on_indensity()
{
	byte errCode;
	errCode = api_LE_SetConstInt(lightChannel, (byte)ui.spinBoxIndensity->value());
}

void MSVisionQt::on_lightOnOff()
{
	byte errCode;
	if (!lightFlag)
	{
		errCode = api_LE_SetCONSTOnOff(lightChannel, LIGHT_ON);
		lightFlag = true;
	}
	else
	{
		errCode = api_LE_SetCONSTOnOff(lightChannel, LIGHT_OFF);
		lightFlag = false;
	}
}

void MSVisionQt::on_strobeWidth()
{
	byte errCode;
	errCode = api_LE_SetStrobeWidth(lightChannel, (uint16)ui.spinStrobeWidth->value());
}

void MSVisionQt::on_loadData()
{
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr(u8"选择标定文件"),
		"./",
		tr("(*.xml)"));
	if (!fileName.isNull())
	{
		QFileInfo fileInfo = QFileInfo(fileName);
		QString filePath = fileInfo.absoluteFilePath();
		std::string filePathStr = filePath.toStdString();
		ms::readBinoCalibParams(filePathStr, intrinsics, distCoeffs, R, t);
		loadFlag = true;
	}
}

void MSVisionQt::on_holeType(int index)
{
	switch (index)
	{
	case 0: mvHoleType = StraightHole; break;
	case 1: mvHoleType = CountersinkHole; break;
	}
}

void MSVisionQt::on_fitMethod(int index)
{
	switch (index)
	{
	case 0: fitMethod = FIT_ELLIPSE_AMS;
	case 1: fitMethod = FIT_ELLIPSE_DIRECT;
	case 2: fitMethod = FIT_ELLIPSE_RANSAC;
	}
}

void MSVisionQt::on_measureType(int index)
{
	switch (index)
	{
	case 0: measureType = EpipolarMatching;
	case 1: measureType = Reconstruction;
	}
}

void MSVisionQt::on_sharpen()
{
	if (ui.checkBoxSharpen->isChecked())
	{
		sharpFlag = true;
	}
	else
	{
		sharpFlag = false;
	}
}

void MSVisionQt::on_restore()
{
	if (ui.checkBoxRestore->isChecked())
	{
		restoreFlag = true;
	}
	else
	{
		restoreFlag = false;
	}
}

void MSVisionQt::on_detect()
{
	// Check whether the .xml is load
	if (!loadFlag)
	{
		INFOMSG("Please load calibration file(.xml) first");
		return;
	}

	// Measure the hole
	sTime = (double)cv::getTickCount();
	MSInfoCode status;
	for (int i = 0; i < CAM_NUM; i++)
	{
		status = detectImg(i);
		if (status != MS_SUCCESS)
		{
			switch (status)
			{
			case MS_ARGUMENT_ERROR: WARNMSG("Argument error"); break;
			case MS_OPEN_FILE_ERROR: WARNMSG("Open .xml file error"); break;
			case MS_SAVE_FIEL_ERROR: WARNMSG("Save .xml file error"); break;
			case MS_FIT_ELLIPSE_ERROR: WARNMSG("Fit ellipse error"); break;
			case MS_GET_ELLIPSE_ERROR: WARNMSG("Get ellipse error"); break;
			case MS_PYR_ELLIPSE_ERROR: WARNMSG("Pyramid method error"); break;
			case MS_ROI_SIZE_ERROR: WARNMSG("ROI size error"); break;
			case MS_IMG_TYPE_ERROR: WARNMSG("Image type error"); break;
			}
			return;
		}
	}
	showFlag = 1;
	measure();
	double eTime = ((double)cv::getTickCount() - sTime) / cv::getTickFrequency();
	QString strTime = QString::number(eTime, 10, 3);
	ui.timeUsed->setText(strTime);
}
