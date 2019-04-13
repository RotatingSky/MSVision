/**
 * MSVision Qt Application.
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


#define	CAM_NUM			2
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
	enum MeasureType { EpipolarMatching = 0, Reconstruction };

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
	MSInfoCode detectImg(int);
	void measure();

private:
	Ui::MSVisionQtClass ui;
	MSLabel labelImgL;
	MSLabel labelImgR;
	int numCam;
	int linkedNumCam;
	int imgScale;
	HANDLE mvCamHandles[CAM_NUM];
	HANDLE mvProperties[CAM_NUM];
	MVCamInfo mvCamInfos[CAM_NUM];
	MV_PixelFormatEnums mvPixelFormats[CAM_NUM];
	MVImage mvImgs[CAM_NUM];
	QImage qtImgs[CAM_NUM];
	QImage showQtImgs[CAM_NUM];
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
	cv::Mat intrinsics[CAM_NUM];
	cv::Mat distCoeffs[CAM_NUM];
	cv::Mat R;
	cv::Mat t;
	HoleType mvHoleType;
	MSFitEllipseMethod fitMethod;
	MeasureType measureType;
	std::vector<cv::RotatedRect> rRects[CAM_NUM];
	float scope;
	int pointsNum;
	double bitAngle;
	double sTime;
};
