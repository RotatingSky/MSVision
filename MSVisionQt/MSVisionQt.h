/**
 * Header for MSVision Qt Application.
 * @filename:	MSVisionQt.h
 * @author:		Sny
 * @version:	1.0
 * @since:		2018-12-22
 */

#pragma once

// Standard and Windows library
#include <iostream>
#include <Windows.h>
// Qt libraries
#include <QtWidgets/QMainWindow>
#include <QMessagebox>
#include <QString>
#include <QImage>		//
#include <QPixmap>		//
#include <QButtonGroup>	//
#include <QFileDialog>	//
#include <QDateTime>	//
#include <QPainter>		//
#include "ui_MSVisionQt.h"
#include "MSLabel.h"
// MSVision library
#include "..\MSVisionDLL\MSVisionDLL.h"
#include "..\MSVisionDLL\MSVisionTypes.h"
// MV-camera libraries
#include "MVGigE.h"
#include "MVImage.h"
#include "MVCamProptySheet.h"

#define WARNINGS		("ERROR")
#define INFORMATION		("INFORMATION")
#define INFOMSG(str)	QMessageBox::information(this, INFORMATION, (str), QMessageBox::Ok);
#define WARNMSG(str)	QMessageBox::warning(this, WARNINGS, (str), QMessageBox::Ok);

namespace Ui {
	class MSVisionQt;
}

class MSVisionQt : public QMainWindow
{
	Q_OBJECT

public:
	MSVisionQt(QWidget *parent = Q_NULLPTR);
	~MSVisionQt();
	int onStreamCB(MV_IMAGE_INFO *, int);
	enum HoleType { StraightHole = 0, CountersinkHole };
	enum MeasureType { Reconstruction = 0, EpipolarMatching };

public slots:
	// For controlling camera
	void on_linkCam();
	void on_startCam();
	void on_stopCam();
	void on_setCam0();
	void on_setCam1();
	void on_saveImg();
	void on_changeScale(int);
	void on_checkCross();
	// For controlling light
	void on_connect();
	void on_changeChannel(int);
	void on_multipier();
	void on_indensity();
	void on_lightOnOff();
	void on_strobeWidth();
	// For calibration
	// For detection
	void on_loadData();
	void on_holeType(int);
	void on_fitMethod(int);
	void on_measureType(int);
	void on_detect();
	void on_sharpen();
	void on_restore();

protected:
	void initCam();
	void drawImgs(int);
	cv::Mat qImage2cvMat(QImage qtImg);
	QImage cvMat2qImage(cv::Mat matImg);
	ms::MSInfoCode detectImg(int);
	void measure();

private:
	Ui::MSVisionQtClass ui;
	MSLabel labelImgL;
	MSLabel labelImgR;
	int numCam;
	int linkedNumCam;
	int imgScale;
	HANDLE mvCamHandles[ms::CamsNum];
	HANDLE mvProperties[ms::CamsNum];
	MVCamInfo mvCamInfos[ms::CamsNum];
	MV_PixelFormatEnums mvPixelFormats[ms::CamsNum];
	MVImage mvImgs[ms::CamsNum];
	QImage qtImgs[ms::CamsNum];
	QImage showQtImgs[ms::CamsNum];
	QString saveImgPath;
	bool mvInitLibFlag;
	bool linkedFlag;
	bool lightConnectFlag;
	bool lightFlag;
	bool loadFlag;
	bool sharpFlag;
	bool restoreFlag;
	int showFlag;
	byte lightChannel;
	QButtonGroup *channelBtns;
	QButtonGroup *holeTypeRadBtns;
	QButtonGroup *fitMethodRadBtns;
	QButtonGroup *measureRadBtns;
	cv::Mat intrinsics[ms::CamsNum];
	cv::Mat distCoeffs[ms::CamsNum];
	cv::Mat R;
	cv::Mat t;
	HoleType mvHoleType;
	ms::MSFitMethod fitMethod;
	MeasureType measureType;
	std::vector<cv::RotatedRect> rRects[ms::CamsNum];
	float scope;
	int pointsNum;
	double bitAngle;
	double sTime;
};
