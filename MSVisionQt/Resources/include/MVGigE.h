#ifndef MVGIGE_H_INCLUDE_
#define MVGIGE_H_INCLUDE_

#ifdef MVGIGE_EXPORTS
#define MVGIGE_API __declspec(dllexport)
#else
#define MVGIGE_API __declspec(dllimport)
#endif

#include "GigECamera_Types.h"

#define MSG_ID_LOST			0x1	//!< ��ϢID������Ͽ�
#define MSG_ID_RECONNECT	0x2	//!< ��ϢID���������������

#ifdef _MSC_VER_	// Modified by Sny
#include "MVimage.h"
#else
typedef void* HANDLE;
typedef unsigned int UINT;
#define FALSE 0
#define TRUE 1
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	/*!
	 *  \brief ��ʼ�������⡣�ڵ��ú������к���֮ǰ���á�
	  *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVInitLib();

	/*!
	 *  \brief �˳������⡣�ڳ����˳�ǰ���ã����ͷ���Դ��
	  *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVTerminateLib();

	/*! 
	 *  \brief	�������ӵ�������ϵ����
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVUpdateCameraList();

	/*! 
	 *  \brief	��ȡ���ӵ�������ϵ����������
	 *  \param [out]	pNumCams	�������
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetNumOfCameras(int* pNumCams);

	/*! 
	 *  \brief	�õ���idx���������Ϣ��
	 *  \param [in]	idx idx��0��ʼ�����������IP��ַ���򣬵�ַС������ǰ�档
	 *  \param [out]	pCamInfo  �������Ϣ (IP,MAC,SN,�ͺ�...) 
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetCameraInfo(unsigned char idx, MVCamInfo* pCamInfo);

	/*! 
	 *  \brief	�򿪵�idx�����
	 *  \param [in]	idx	idx��0��ʼ�����������IP��ַ���򣬵�ַС������ǰ�档
	 *  \param [out]	hCam ����ɹ�,���ص�������
	 *  \retval MVST_INVALID_PARAMETER : idxȡֵ����
	 *			MVST_ACCESS_DENIED		: ����޷����ʣ�������������������
	 *			MVST_ERROR				: ��������
	 *			MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVOpenCamByIndex(unsigned char idx, HANDLE* hCam);

	/*! 
	 *  \brief	��ָ��UserDefinedName�����
	 *  \param [in]	name UserDefinedName��
	 *  \param [out] hCam ����ɹ�,���ص������������ʧ�ܣ�ΪNULL��
	 *  \retval 
	 *			MVST_ACCESS_DENIED		: ����޷����ʣ�������������������
	 *			MVST_ERROR				: ��������
	 *			MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVOpenCamByUserDefinedName(char* name, HANDLE* hCam);


	/*! 
	 *  \brief	��ָ��IP�����
	 *  \param [in]	ip �����IP��ַ��
	 *  \param [out] hCam ����ɹ�,���ص������������ʧ�ܣ�ΪNULL��
	 *  \retval 
	 *			MVST_ACCESS_DENIED		: ����޷����ʣ�������������������
	 *			MVST_ERROR				: ��������
	 *			MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVOpenCamByIP( char *ip,HANDLE *hCam );

	/*! 
	 *  \brief	�ر�������Ͽ�����������ӡ�
	 *  \param [in]	hCam ����ľ��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVCloseCam(HANDLE hCam);

	/*!
	 * \brief	��ȡͼ����
	 * \param [in]	hCam		������
	 * \param [out]	pWidth	ͼ����[����]
	 * \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetWidth(HANDLE hCam, int* pWidth);

	/*!
	 *  \brief ��ȡͼ���ȿ����õķ�Χ
	 *  \param [in]	hCam		������
	 *  \param [out]	pWidthMin	ͼ���ȿ����õ���Сֵ
	 *  \param [out]	pWidthMax	ͼ���ȿ����õ����ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetWidthRange(HANDLE hCam, int* pWidthMin, int* pWidthMax);

	/*!
	 *  \brief ��ȡͼ���ȵ����Ĳ���
	 *  \param [in]	hCam		������
	 *  \param [out] pWidthInc	ͼ���ȵĵ����Ĳ�������ͼ��Ŀ�� = ��С��� + ���� x ����
	  *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetWidthInc(HANDLE hCam, int* pWidthInc);

	/*!
	 *  \brief ����ͼ��Ŀ��
	 *  \param [in]	hCam		������
	 *  \param [in]	nWidth	ͼ���ȣ�Ӧ���ڿ�ȿ����÷�Χ֮�ڣ����� = ��С��� + ���� x ����
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetWidth(HANDLE hCam, int nWidth);

	/*!
	 * \brief	��ȡͼ��߶�
	 * \param [in]	hCam		������
	 * \param [out]	 pHeight	ͼ��߶�[����]
	 * \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetHeight(HANDLE hCam, int* pHeight);

	/*!
	 *  \brief ��ȡͼ��߶ȿ����õķ�Χ
	 *  \param [in]	hCam		������
	 *  \param [out]	 pHeightMin	ͼ��߶ȿ����õ���Сֵ
	 *  \param [out]	 pHeightMax	ͼ��߶ȿ����õ����ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetHeightRange(HANDLE hCam, int* pHeightMin, int* pHeightMax);

	/*!
	 *  \brief ����ͼ��ĸ߶�
	 *  \param [in]	hCam		������
	 *  \param [in]	nHeight	ͼ��߶ȣ�Ӧ���ڸ߶ȿ����÷�Χ֮��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetHeight(HANDLE hCam, int nHeight);

	/*!
	 *  \brief ��ȡˮƽ����ƫ������ͼ�������õ�С�������ʱ�����Ե���ˮƽƫ���������òɼ����ڵ�ˮƽ��ʼλ�á�
	 *  \param [in]	hCam
	 *  \param [out]	 pOffsetX	ˮƽƫ����
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetX(HANDLE hCam, int* pOffsetX);

	/*!
	 *  \brief ��ȡˮƽ����ƫ����ȡֵ��Χ��
	 *  \param [in]	hCam
	 *  \param [out]	 pOffsetXMin	ˮƽƫ������Сֵ
	 *  \param [out]	 pOffsetXMax	ˮƽƫ�������ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetXRange(HANDLE hCam, int* pOffsetXMin, int* pOffsetXMax);

	/*!
	 *  \brief ����ˮƽ����ƫ������ͼ�������õ�С�������ʱ�����Ե���ˮƽƫ���������òɼ����ڵ�ˮƽ��ʼλ�á�
	 *  \param [in]	hCam
	 *  \param [in]	nOffsetX ˮƽƫ������Ӧ����ˮƽƫ��������ķ�Χ֮�ڡ�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetOffsetX(HANDLE hCam, int nOffsetX);

	/*!
	 *  \brief ��ȡ��ֱ����ƫ������ͼ�������õ�С�������ʱ�����Ե�����ֱƫ���������òɼ����ڵĴ�ֱ��ʼλ�á�
	 *  \param [in]	hCam
	 *  \param [out]	 pOffsetY	��ֱƫ����
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetY(HANDLE hCam, int* pOffsetY);

	/*!
	 *  \brief ��ȡ��ֱ����ƫ����ȡֵ��Χ��
	 *  \param [in]	hCam
	 *  \param [out]	 pOffsetYMin	��ֱƫ������Сֵ
	 *  \param [out]	 pOffsetYMax	��ֱƫ�������ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetOffsetYRange(HANDLE hCam, int* pOffsetYMin, int* pOffsetYMax);

	/*!
	 *  \brief ���ô�ֱ����ƫ������ͼ�������õ�С�������ʱ�����Ե�����ֱƫ���������òɼ����ڵĴ�ֱ��ʼλ�á�
	 *  \param [in]	hCam
	 *  \param [in]	nOffsetY ��ֱƫ������Ӧ���ڴ�ֱƫ��������ķ�Χ֮�ڡ�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetOffsetY(HANDLE hCam, int nOffsetY);	

	/*!
	 * \brief    ��ȡͼ������ظ�ʽ
	 * \param [in]	hCam		������
	 * \param [out]	pPixelFormat
	 * \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetPixelFormat(HANDLE hCam, MV_PixelFormatEnums* pPixelFormat);

	/*!
	 * \brief    ����ͼ������ظ�ʽ
	 * \param [in]	hCam		������
	 * \param [out]	PixelFormat
	 * \retval  	MVST_SUCCESS			: �ɹ�
	 */	MVGIGE_API MVSTATUS_CODES __stdcall MVSetPixelFormat( HANDLE hCam,MV_PixelFormatEnums PixelFormat );

	/*!
	 *  \brief	��ȡ��������ͨ����
	 *  \param [in]	hCam		������
	 *  \param [out] pSensorTaps
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetSensorTaps(HANDLE hCam, SensorTapsEnums* pSensorTaps);

	/*!
	 * \brief    ��ȡ��ǰ����ֵ
	 * \param [in]	hCam	������
	 * \param [out]	 pGain
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGain(HANDLE hCam, double* pGain);

	/*!
	 * \brief    ��ȡ����������õķ�Χ
	 * \param [in]	hCam	������
	 * \param [out]	 pGainMin	��Сֵ
	 * \param [out]	 pGainMax	���ֵ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainRange(HANDLE hCam, double* pGainMin, double* pGainMax);	

	/*!
	 * \brief    ��������
	 * \param [in]	hCam	������
	 * \param [in]	 fGain	����
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetGain(HANDLE hCam, double fGain);

	/*!
	 * \brief    �����������Ϊ��ͨ��ʱ������ĳ��ͨ��������
	 * \param [in]	hCam		������
	 * \param [in]	 fGain	����
	 * \param [in]	nTap		ͨ����˫ͨ��[0,1],��ͨ��[0,1,2,3]
	 *  \retval MVC_ST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetGainTaps(HANDLE hCam, double fGain, int nTap);

	/*!
	 * \brief    �����������Ϊ��ͨ��ʱ����ȡĳ��ͨ��������
	 * \param [in]	hCam		������
	 * \param [out]	 pGain
	 * \param [in]	nTap		ͨ����˫ͨ��[0,1],��ͨ��[0,1,2,3]
	 *  \retval MVC_ST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainTaps(HANDLE hCam, double* pGain, int nTap);

	/*!
	 * \brief    �����������Ϊ��ͨ��ʱ����ȡĳ��ͨ������������õķ�Χ
	 * \param [in]	hCam		������
	 * \param [out]	 pGainMin	������Сֵ
	 * \param [out]	 pGainMax	�������ֵ
	 * \param [in]	nTap		ͨ����˫ͨ��[0,1],��ͨ��[0,1,2,3]
	 *  \retval MVC_ST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainRangeTaps(HANDLE hCam, double* pGainMin, double* pGainMax, int nTap);

	/*!
	 * \brief    ��ȡ��ǰ��ƽ��ϵ��
	 * \param [in]	hCam		������
	 * \param [out]	 pRed	��ɫƽ��ϵ��
	 * \param [out]	 pGreen ��ɫƽ��ϵ��
	 * \param [out]	 pBlue	��ɫƽ��ϵ��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetWhiteBalance(HANDLE hCam, double* pRed, double* pGreen, double* pBlue);

	/*!
	 * \brief	��ȡ��ƽ�����õķ�Χ
	 * \param [in]	hCam		������
	 * \param [out]	 pMin	ϵ����Сֵ
	 * \param [out]	 pMax	ϵ�����ֵ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetWhiteBalanceRange(HANDLE hCam, double* pMin, double* pMax);

	/*!
	 * \brief    ���ð�ƽ��ϵ��
	 * \param [in]	hCam		������
	 * \param [in]	 fRed		��ɫƽ��ϵ��
	 * \param [in]	 fGreen	��ɫƽ��ϵ��
	 * \param [in]	 fBlue	��ɫƽ��ϵ��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetWhiteBalance(HANDLE hCam, double fRed, double fGreen, double fBlue);

	/*!
	 * \brief    ��ȡ�Ƿ�ͨ���Զ�ƽ��
	 * \param [in]	hCam		������
	 * \param [out]	pBalance	�Ƿ��Զ�ƽ��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainBalance(HANDLE hCam, int* pBalance);

	/*!
	 * \brief    �����Ƿ��Զ�ͨ��ƽ��
	 * \param [in]	hCam		������
	 * \param [in]	nBalance	�Ƿ��Զ�ͨ��ƽ��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetGainBalance(HANDLE hCam, int nBalance);

	/*! 
	 *  \brief	��ȡ��ǰ�ع�ʱ��
	 *  \param [in]	hCam
	 *  \param [in]	pExposuretime	��λus
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetExposureTime(HANDLE hCam, double* pExposuretime);

	/*!
	 * \brief  ��ȡ�ع�ʱ������÷�Χ  
	 * \param [in]	hCam		������
	 * \param [out]	 pExpMin	����ع�ʱ�� ��λΪus
	 * \param [out]	 pExpMax	��ع�ʱ�� ��λΪus
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetExposureTimeRange(HANDLE hCam, double* pExpMin, double* pExpMax);

	/*!
	 * \brief	�����ع�ʱ��
	 * \param [in]	hCam		������
	 * \param [in]	 nExp_us �ع�ʱ�� ��λΪus
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetExposureTime(HANDLE hCam,double nExp_us);

	/*!
	 * \brief	��ȡ֡�ʿ����õķ�Χ
	 * \param [in]	hCam		������
	 * \param [out]	 pFpsMin ���֡��
	 * \param [out]	 pFpsMax ���֡��	
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetFrameRateRange(HANDLE hCam, double* pFpsMin, double* pFpsMax);

	/*!
	 * \brief	��ȡ��ǰ֡��   
	 * \param [in]	hCam		������
	 * \param [out]	 fFPS	֡�� ֡/��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetFrameRate(HANDLE hCam, double* fFPS);

	/*!
	 * \brief    ����֡��
	 * \param [in]	hCam		������
	 * \param [in]	 fps	֡�� ֡/��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetFrameRate(HANDLE hCam, double fps);

	/*!
	 *  \brief ��ʼ�ɼ�ͼ��
	 *  \param [in]	hCam	������
	 *  \param [in] StreamCB	�ص�����ָ��
	 *  \param [in]	nUserVal	�û����ݣ����ݵ��ص��������β�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */

	MVGIGE_API MVSTATUS_CODES __stdcall MVStartGrab(HANDLE hCam, MVStreamCB StreamCB, ULONG_PTR nUserVal);


	/*!
	 *  \brief ֹͣ�ɼ�ͼ��
	 *  \param [in]	hCam	������
	 *  \retval  MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVStopGrab(HANDLE hCam);

	/*!
	 * \brief    ��ȡ����ģʽ
	 * \param [in]	hCam		������
	 * \param [out]	pMode	����ģʽ  TriggerMode_Off,TriggerMode_On
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerMode(HANDLE hCam, TriggerModeEnums* pMode);

	/*! 
	 *  \brief	���ô���ģʽ
	 * \param [in]	hCam		������
	 *  \param [in]	mode	����ģʽ
		TriggerMode_Off����������������ɼ�ģʽ��
		TriggerMode_On:��������ڴ���ģʽ����Ҫ���ⴥ���źŻ�����ָ�������
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerMode(HANDLE hCam, TriggerModeEnums mode);

	/*!
	 * \brief    ��ȡ����Դ
	 * \param [in]	hCam		������
	 * \param [out]	pSource	����Դ���������ⴥ��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerSource(HANDLE hCam, TriggerSourceEnums* pSource);

	/*! 
	 *  \brief	���ô���Դ
	 * \param [in]	hCam		������
	 *  \param [in]	source ����Դ
					TriggerSource_Software��ͨ��\c MVTriggerSoftware()����������
					TriggerSource_Line1��ͨ�����ӵĴ����ߴ�����
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerSource(HANDLE hCam, TriggerSourceEnums source);

	/*!
	 * \brief    ��ȡ��������
	 * \param [in]	hCam		������
	 * \param [out]	pAct
					TriggerActivation_RisingEdge: �����ش���
					TriggerActivation_FallingEdge: �½��ش���
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerActivation(HANDLE hCam, TriggerActivationEnums* pAct);

	/*! 
	 *  \brief	��ʹ�ô����ߴ���ʱ,�����������ش��������½��ش���
	 *  \param [in]	hCam
	 *  \param [in]	act �����ػ��½���
					TriggerActivation_RisingEdge: �����ش���
					TriggerActivation_FallingEdge: �½��ش���
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerActivation(HANDLE hCam, TriggerActivationEnums act);

	/*!
	 * \brief	��ȡ������ʱ
	 * \param [in]	hCam		������
	 * \param [out]	pDelay_us	������ʱ,��λus
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerDelay(HANDLE hCam, uint32_t* pDelay_us);

	/*!
	 * \brief    ��ȡ������ʱ��Χ
	 * \param [in]	hCam		������
	 * \param [out]	pMin	������ʱ��Сֵ,��λus	
	 * \param [out]	pMax ������ʱ���ֵ,��λus	
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTriggerDelayRange(HANDLE hCam, uint32_t* pMin, uint32_t* pMax);

	/*! 
	 *  \brief	��������ӵ������źź��ӳٶ���΢����ٿ�ʼ�ع⡣
	 *  \param [in]	hCam		������
	 *  \param [in]	nDelay_us
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetTriggerDelay(HANDLE hCam, uint32_t nDelay_us);

	/*! 
	 *  \brief	�����������ָ��
	 *  \param [in]	hCam		������
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVTriggerSoftware(HANDLE hCam);

	/*!
	 * \brief	��ȡ����ͬ���ź�Դ
	 * \param [in]	hCam		������
	 * \param [out]	pSource	����ͬ���ź�Դ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetStrobeSource(HANDLE hCam, LineSourceEnums* pSource);

	/*! 
	 *  \brief	����ͬ���ź�Դ
	 *  \param [in]	hCam
	 *  \param [in]	source
					LineSource_Off���ر�����ͬ��
					LineSource_ExposureActive���ع��ͬʱ����
					LineSource_Timer1Active���ɶ�ʱ������
					LineSource_UserOutput0�����û�ͨ��ָ�����
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetStrobeSource(HANDLE hCam, LineSourceEnums source);

	/*!
	 * \brief	��ȡ����ͬ���Ƿ�ת
	 * \param [in]	hCam		������
	 * \param [out]	pInvert
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetStrobeInvert(HANDLE hCam, bool* pInvert);

	/*! 
	 *  \brief	����ͬ���Ƿ�ת��������ͬ����Чʱ����ߵ�ƽ���ǵ͵�ƽ��
	 *  \param [in]	hCam		������
	 *  \param [in]	bInvert
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetStrobeInvert(HANDLE hCam, bool bInvert);

	/*!
	 * \brief	��ȡ�û����õ�����ͬ��
	 * \param [in]	hCam		������
	 * \param [out]	pSet
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetUserOutputValue0(HANDLE hCam, bool* pSet);

	/*! 
	 *  \brief	������ͬ��ԴѡΪUserOutputʱ
				��������ͨ��MVSetUserOutputValue0����������ͬ������ߵ�ƽ��͵�ƽ��
	 *  \param [in]	hCam		������
	 *  \param [in]	bSet ���õ�ƽ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetUserOutputValue0(HANDLE hCam, bool bSet);

	/*!
	 *  \brief ����������ʱʱ��
	 *  \param [in]	hCam	������
	 *  \param [in]	 nTimeOut	������ʱʱ�� ��λms
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 *  \note Ӧ�ó�����������������»�ȥ�ر����������ʱ����������жϣ�û�������ر������
	 *			���������������޷��ٴδ��ˣ������Ϊ���ڱ�ԭ����������ţ��� 
	 *			��������heartbeat��Ӧ�ó��������Ƿ��ڲɼ�ͼ��ÿ��һ��ʱ��(һ����1000ms)��Ҫȥ����һ�������
	 *			������HeartbeatTimeoutʱ��û������Ӧ�ó���ķ��ʣ��������ΪӦ�ó����ж��ˣ����Զ��ر�ԭ�����ӣ�
	 *			��ӭ���µ����ӡ�
	 *			��ʱ��������Ĳ�����SDK�ڲ��Ѿ�ʵ�֡����ʱ�����ٴ�ʵ�֡�
	 *			�����öϵ���Գ���ʱ�����Խ�nTimeOut���ó�һЩ������������Զ��ر����ӡ�
	 *			Release״̬�����ö�һЩ(3000ms),����Ӧ�ó��������˳�ʱ������᳤ʱ���޷����´򿪡�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetHeartbeatTimeout(HANDLE hCam, unsigned long nTimeOut);//unit ms

	/*!
	 * \brief	��ȡ�������ݰ���С
	 * \param [in]	hCam		������
	 * \param [out]	pPacketSize ���ݰ���С
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketSize(HANDLE hCam, unsigned int* pPacketSize);

	/*!
	 * \brief    ��ȡ�������ݰ���С�ķ�Χ��
	 * \param [in]	hCam			������
	 * \param [out]	pMin	�������ݰ���Сֵ	
	 * \param [out]	pMax �������ݰ����ֵ	
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketSizeRange(HANDLE hCam, unsigned int* pMin, unsigned int* pMax);

	/*! 
	 *  \brief	�����������ݰ��Ĵ�С��
	 *  \param [in]	hCam			������
	 *  \param [in]	nPacketSize �������ݰ���С(��λ:�ֽ�)���ô�С����С�������ܹ�֧�ֵ�������֡(Jumbo Frame)��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetPacketSize(HANDLE hCam, unsigned int nPacketSize);

	/*!
	 * \brief	��ȡ�������ݰ������
	 * \param [in]	hCam				������
	 * \param [out]	pDelay_us ���ݰ����ʱ�䣬��λus
	 *  \retval MVST_SUCCESS				: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketDelay(HANDLE hCam, unsigned int* pDelay_us);

	/*!
	 * \brief    ��ȡ�������ݰ������Χ
	 * \param [in]	hCam			������
	 * \param [out]	pMin	���ݰ����ʱ����Сֵ����λus	
	 * \param [out]	pMax ���ݰ����ʱ�����ֵ����λus	
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetPacketDelayRange(HANDLE hCam, unsigned int* pMin, unsigned int* pMax);


	/*! 
	 *  \brief	�����������ݰ�֮���ʱ�����������������Ե�����Ƿ�ѣ��޷�������ٵ�������ݰ����ᵼ�¶�ʧ���ݰ���
				�Ӷ�ʹͼ������������ͨ���������ݰ�֮���ʱ�����Ա�֤ͼ���䡣�������Ӹ�ֵ������ͼ���ʱ���ӳ٣�
				���п���Ӱ��֡�ʡ�
	 *  \param [in]	hCam
	 *  \param [in]	nDelay_us ʱ����(��λ:΢��)
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetPacketDelay(HANDLE hCam, unsigned int nDelay_us);

	/*!
	 * \brief    ��ȡ��ʱ����ʱ
	 * \param [in]	hCam		������
	 * \param [out]	pDelay	��ʱ����ʱ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDelay(HANDLE hCam, uint32_t* pDelay);

	/*!
	 * \brief	��ȡ��ʱ����ʱ�ķ�Χ
	 * \param [in]	hCam		������
	 * \param [out]	pMin	��ʱ����ʱ����Сֵ
	 * \param [out]	pMax ��ʱ����ʱ�����ֵ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDelayRange(HANDLE hCam, uint32_t* pMin, uint32_t* pMax);

	/*! 
	 *  \brief	������ͬ��ԴѡΪTimer1ʱMVSetStobeSource(hCam,LineSource_Timer1Active)
				����Timer1�ڽӵ������źź��ӳٶ���us��ʼ��ʱ
	 *  \param [in]	hCam		������
	 *  \param [in]	nDelay �ӵ������źź��ӳٶ���us��ʼ��ʱ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetTimerDelay(HANDLE hCam, uint32_t nDelay);

	/*!
	 * \brief    ��ȡ��ʱ����ʱʱ��
	 * \param [in]	hCam		������
	 * \param [out]	pDuration	��ʱ����ʱʱ��
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDuration(HANDLE hCam, uint32_t* pDuration);

	/*!
	 * \brief    ��ȡ��ʱ����ʱʱ��ȡֵ��Χ
	 * \param [in]	hCam		������
	 * \param [out]	pMin	��ʱ����ʱʱ����Сֵ
	 * \param [out]	pMax	��ʱ����ʱʱ�����ֵ
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetTimerDurationRange(HANDLE hCam, uint32_t* pMin, uint32_t* pMax);

	/*! 
	*  \brief	������ͬ��ԴѡΪTimer1ʱMVSetStobeSource(hCam,LineSource_Timer1Active)
				����Timer1�ڿ�ʼ��ʱ�󣬼�ʱ�೤ʱ�䡣
	 *  \param [in]	hCam
	 *  \param [in]	nDuration ����Timer1�ڿ�ʼ��ʱ�󣬼�ʱ�೤ʱ��(us)���������/�͵�ƽ�������ȡ�
	 *  \retval MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetTimerDuration(HANDLE hCam, uint32_t nDuration);

	/*!
	 *  \brief ��Bayer��ʽ��8bit��ͨ��ͼת��ΪBGR��ʽ��8Bit��ͨ��ͼ
	 *  \param [in]	hCam	������
	 *  \param [in]	psrc	��ͨ��ͼ���ָ��
	 *  \param [out] pdst	��ͨ��ͼ��ָ��
	 *  \param [in]	dststep	��ͨ��ͼ��һ��ͼ����ֽ�����ͨ��Ϊͼ����*3�����ǻ�Ϊ��4�ֽڶ���Ჹ�����ֽڡ�
	 *  \param [in]	width	ͼ����
	 *  \param [in]	height	ͼ��߶�
	 *  \param [in]	pixelformat	���ظ�ʽ����MVGetPixelFormatȡ��
	 *	\param [in] bMultiCores �Ƿ�ʹ��CPU��˼���
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVBayerToBGR(HANDLE hCam, void *psrc,void *pdst,unsigned int dststep,unsigned int width,unsigned int height,MV_PixelFormatEnums pixelformat,bool bMultiCores=FALSE);

	/*!
	 *  \brief ��Bayer��ʽ��16bit��ͨ��ͼת��ΪBGR��ʽ��16Bit��ͨ��ͼ
	 *  \param [in]	hCam	������
	 *  \param [in]	psrc	��ͨ��ͼ���ָ��
	 *  \param [out] pdst	��ͨ��ͼ��ָ��
	 *  \param [in]	dststep	��ͨ��ͼ��һ��ͼ����ֽ�����
	 *  \param [in]	width	ͼ����
	 *  \param [in]	height	ͼ��߶�
	 *  \param [in]	pixelformat	���ظ�ʽ����MVGetPixelFormatȡ��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVBayerToBGR16(HANDLE hCam, void *psrc,void *pdst,unsigned int dststep,unsigned int width,unsigned int height,MV_PixelFormatEnums pixelformat );
	

	/*!
	 *  \brief ��Bayer��ʽ��8bit��ͨ��ͼת��ΪRGB��ʽ��8Bit��ͨ��ͼ
	 *  \param [in]	hCam	������
	 *  \param [in]	psrc	��ͨ��ͼ���ָ��
	 *  \param [out] pdst	��ͨ��ͼ��ָ��
	 *  \param [in]	dststep	��ͨ��ͼ��һ��ͼ����ֽ�����ͨ��Ϊͼ����*3�����ǻ�Ϊ��4�ֽڶ���Ჹ�����ֽڡ�
	 *  \param [in]	width	ͼ����
	 *  \param [in]	height	ͼ��߶�
	 *  \param [in]	pixelformat	���ظ�ʽ����MVGetPixelFormatȡ��
	 *	\param [in] bMultiCores �Ƿ�ʹ��CPU��˼���
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVBayerToRGB(HANDLE hCam, void *psrc,void *pdst,unsigned int dststep,unsigned int width,unsigned int height,MV_PixelFormatEnums pixelformat,bool bMultiCores=FALSE);

	/*!
	 *  \brief ��Bayer��ʽ��16bit��ͨ��ͼת��ΪRGB��ʽ��16Bit��ͨ��ͼ
	 *  \param [in]	hCam	������
	 *  \param [in]	psrc	��ͨ��ͼ���ָ��
	 *  \param [out] pdst	��ͨ��ͼ��ָ��
	 *  \param [in]	dststep	��ͨ��ͼ��һ��ͼ����ֽ�����
	 *  \param [in]	width	ͼ����
	 *  \param [in]	height	ͼ��߶�
	 *  \param [in]	pixelformat	���ظ�ʽ����MVGetPixelFormatȡ��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVBayerToRGB16(HANDLE hCam, void *psrc,void *pdst,unsigned int dststep,unsigned int width,unsigned int height,MV_PixelFormatEnums pixelformat );

#ifdef _MSC_VER_	// Modified by Sny
	/*!
	 *  \brief ��Bayer��ʽ��8bit��ͨ��ͼת��ΪBGR��ʽ��8Bit��ͨ��ͼ
	 *  \param [in]	hCam	������
	 *  \param [in]	pInfo	�ɼ�Callback�����д�����ͼ����Ϣָ��
	 *  \param [out] pImage	ת�����ͼ���ָ��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVImageBayerToBGR(HANDLE hCam, MV_IMAGE_INFO* pInfo, MVImage* pImage);

	/*!
	 *  \brief ���ص������յ���ͼ����Ϣת��Ϊͼ��
	 *  \param [in]	hCam	������
	 *  \param [in]	pInfo	�ɼ�Callback�����д�����ͼ����Ϣָ��
	 *  \param [out]	pImage	ת�����ͼ���ָ��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVInfo2Image(HANDLE hCam, MV_IMAGE_INFO* pInfo, MVImage* pImage);

/*!
 *  \brief ��Bayer��ʽ��8bit��ͨ��ͼת��ΪBGR��ʽ��8Bit��ͨ��ͼ,ͬʱ����ͼ���GAMMA,��ɫ�ͷ���
 *  \param [in]	hCam	������
 *  \param [in]	pInfo	�ɼ�Callback�����д�����ͼ����Ϣָ��
 *  \param [out]	pImage	ת�����ͼ���ָ��
 *  \param [in]	 fGamma			GammaУ��ֵ��1Ϊ��У����<1ʱ��������������
 *  \param [in]	 bColorCorrect		�Ƿ������ɫУ����������ɫУ����ͼ����ø����ޡ�
 *  \param [in]	nContrast			�Ƿ���������ΧΪ0��50������ֵ����0ʱ��ͼ�񷴲���ǿ��
 *  \retval  	MVST_SUCCESS			: �ɹ�
 */
MVGIGE_API MVSTATUS_CODES __stdcall MVImageBayerToBGREx( HANDLE hCam,MV_IMAGE_INFO *pInfo,MVImage *pImage,double fGamma,bool bColorCorrect,int nContrast );

	/*!
	 *  \brief BGR��ʽ��ͨ��ͼ������
	 *  \param [in]	hCam	������
	 *  \param [in]	 pSrc Դͼ��ָ��
	 *  \param [in]	srcWidth	Դͼ����
	 *  \param [in]	srcHeight	Դͼ��߶�
	 *  \param [out]	pDst	���ź�ͼ��ָ��
	 *  \param [in]	 fFactorX	���ű���
	 *  \param [in]	 fFactorY	���ű���
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVZoomImageBGR(HANDLE hCam, unsigned char* pSrc, int srcWidth, int srcHeight, unsigned char* pDst, double fFactorX, double fFactorY);
#else
	MVGIGE_API MVSTATUS_CODES __stdcall MVImageBayerToBGR(HANDLE hCam, MV_IMAGE_INFO* pInfo, void* pImage);
	MVGIGE_API MVSTATUS_CODES __stdcall MVInfo2Image(HANDLE hCam, MV_IMAGE_INFO* pInfo, void* pImage);
#endif
	/*!
	 *  \brief	��ȡ���ݴ����ͳ����Ϣ
	 *  \param [in]	hCam	������
	 *  \param [out]	pStatistic	ͳ����Ϣ
	 *  \retval  	JMVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetStreamStatistic(HANDLE hCam, MVStreamStatistic* pStatistic);



	/*!
	 *  \brief	��ȡ��Ӧ��ĳ���û�Ԥ��Ĳ���
	 *  \param [in]	hCam		������
	 *  \param [in]	userset
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVLoadUserSet(HANDLE hCam, UserSetSelectorEnums userset);

	/*!
	 *  \brief	����ǰ����Ĳ������浽�û������С�
	 *  \param [in]	hCam		������
	 *  \param [in]	userset
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSaveUserSet(HANDLE hCam, UserSetSelectorEnums userset);

	/*!
	 *  \brief	��������ϵ翪��ʱĬ�϶�ȡ��Ӧ����һ���û�����
	 *  \param [in]	hCam		������
	 *  \param [in]	userset
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetDefaultUserSet(HANDLE hCam, UserSetSelectorEnums userset);

	/*!
	 *  \brief	��ȡ����ϵ翪��ʱĬ�϶�ȡ��Ӧ����һ���û�����
	 *  \param [in]	hCam		������
	 *  \param [out]	pUserset	�û�����
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetDefaultUserSet(HANDLE hCam, UserSetSelectorEnums* pUserset);
#ifdef _MSC_VER_	// Modified by Sny
	/*!
	 *  \brief ͼ��ת
	 *  \param [in]	hCam	������
	 *  \param [in]	 pSrcImage	Դͼ��ָ��
	 *  \param [out] pDstImage	���ͼ��ָ�롣���ΪNULL����ת�Ľ������Դͼ���ڡ�
	 *  \param [in]	flipType	��ת���͡�FlipHorizontal:���ҷ�ת,FlipVertical:���·�ת,FlipBoth:��ת180��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVImageFlip(HANDLE hCam, MVImage* pSrcImage, MVImage* pDstImage, ImageFlipType flipType);


	/*!
	 *  \brief ͼ����ת
	 *  \param [in]	hCam	������
	 *  \param [in]	 pSrcImage	Դͼ��ָ��
	 *  \param [out] pDstImage	���ͼ��ָ��,����ΪNULL�����ͼ��Ŀ�Ⱥ͸߶�Ӧ�ú�Դͼ��Ŀ�Ⱥ͸߶Ȼ�����
	 *  \param [in]	roateType	��ת���ͣ�Rotate90DegCw, Rotate90DegCcw
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVImageRotate(HANDLE hCam, MVImage* pSrcImage, MVImage* pDstImage, ImageRotateType roateType);

	/*!
	 *  \brief ����ɫBGR��ͨ��24bitͼ��ת��Ϊ�Ҷȵ�ͨ��8bitͼ��
	 *  \param [in]	hCam	������
	 *  \param [in]	 psrc	��ɫBGR��ͨ��24bitͼ��ָ��
	 *  \param [out]	 pdst	�Ҷȵ�ͨ��8bitͼ��ָ��
	 *  \param [in]	 width	ͼ����
	 *  \param [in]	 height	ͼ��߶�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVBGRToGray(HANDLE hCam, unsigned char* psrc, unsigned char* pdst, unsigned int width, unsigned int height);

	/*!
	 *  \brief ����ɫBGR��ͨ��24bitͼ��ת��Ϊ�Ҷȵ�ͨ��8bitͼ��
	 *  \param [in]	hCam	������
	 *  \param [in]	 pSrcImage	��ɫBGR��ͨ��24bitͼ��ָ��
	 *  \param [out]	 pDstImage	�Ҷȵ�ͨ��8bitͼ��ָ�롣��ȸ߶ȱ����Դͼ��ͬ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVImageBGRToGray(HANDLE hCam, MVImage* pSrcImage, MVImage* pDstImage);


	/*!
	 *  \brief	����ɫBGR��ͨ��24bitͼ��ת��ΪYUVͼ�� 
	 *  \param [in]	hCam	������
	 *  \param [in]	 pSrcImage	��ɫBGR��ͨ��24bitͼ��ָ��
	 *  \param [out]	 pDst	YUVͼ��ָ�� (YUV422)
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVImageBGRToYUV(HANDLE hCam, MVImage* pSrcImage, unsigned char* pDst);

	/*!
	 *  \brief ���Ҷȵ�ͨ��8bitͼ��ת��Ϊ��ɫBGR��ͨ��24bitͼ��ת��������ͨ����ֵ����ͬ�ġ�
	 *  \param [in]	hCam	������
	 *  \param [in]	 pSrc	�Ҷȵ�ͨ��8bitͼ��ָ��
	 *  \param [out]	 pDst	��ɫBGR��ͨ��24bitͼ��ָ��
	 *  \param [in]	 width	ͼ����
	 *  \param [in]	 height	ͼ��߶�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGrayToBGR(HANDLE hCam, unsigned char* pSrc, unsigned char* pDst, int width, int height);
#endif
	/*!
	 *  \brief ��ȡ��ǰ�Զ��ع�ģʽ
	 *  \param [in]	hCam	������
	 *  \param [out]	pExposureAuto	��ǰ�Զ��ع�ģʽ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetExposureAuto(HANDLE hCam, ExposureAutoEnums* pExposureAuto);
	/*!
	 *  \brief �����Զ��ع�ģʽ
	 *  \param [in]	hCam	������
	 *  \param [in]	ExposureAuto	�Զ��ع�ģʽ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetExposureAuto(HANDLE hCam, ExposureAutoEnums ExposureAuto);

	/*!	
	 *  \brief ��ȡ��ǰ�Զ�����ģʽ
	 *  \param [in]	hCam	������
	 *  \param [out]	pGainAuto	��ǰ�Զ�����ģʽ��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGainAuto(HANDLE hCam, GainAutoEnums* pGainAuto);
	/*!
	 *  \brief ���õ�ǰ�Զ�����ģʽ
	 *  \param [in]	hCam	������
	 *  \param [in]	GainAuto	�Զ�����ģʽ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetGainAuto(HANDLE hCam, GainAutoEnums GainAuto);

	/*!
	 *  \brief ��ȡ��ǰ�Զ���ƽ��ģʽ
	 *  \param [in]	hCam	������
	 *  \param [out]	pBalanceWhiteAuto	��ǰ�Զ���ƽ��ģʽ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetBalanceWhiteAuto(HANDLE hCam, BalanceWhiteAutoEnums* pBalanceWhiteAuto);
	/*!
	 *  \brief �����Զ���ƽ��ģʽ
	 *  \param [in]	hCam	������
	 *  \param [in]	BalanceWhiteAuto	�Զ���ƽ��ģʽ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetBalanceWhiteAuto(HANDLE hCam, BalanceWhiteAutoEnums BalanceWhiteAuto);

	/*!
	 *  \brief ��ȡ�Զ���������ʱ�����������Χ����Сֵ
	 *  \param [in]	hCam	������
	 *  \param [out]	 pAutoGainLowerLimit	���������Χ����Сֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoGainLowerLimit(HANDLE hCam, double* pAutoGainLowerLimit);
	/*!
	 *  \brief �����Զ���������ʱ�����������Χ����Сֵ
	 *  \param [in]	hCam	������
	 *  \param [in]	 fAutoGainLowerLimit	���������Χ����Сֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoGainLowerLimit(HANDLE hCam, double fAutoGainLowerLimit);

	/*!
	 *  \brief ��ȡ�Զ���������ʱ�����������Χ�����ֵ
	 *  \param [in]	hCam	������
	 *  \param [out]	 pAutoGainUpperLimit	���������Χ�����ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoGainUpperLimit(HANDLE hCam, double* pAutoGainUpperLimit);
	/*!
	 *  \brief �����Զ���������ʱ�����������Χ�����ֵ
	 *  \param [in]	hCam	������
	 *  \param [in]	 fAutoGainUpperLimit	�ع�ʱ�������Χ����Сֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoGainUpperLimit(HANDLE hCam, double fAutoGainUpperLimit);

	/*!
	 *  \brief ��ȡ�Զ������ع�ʱ��ʱ���ع�ʱ�������Χ����Сֵ
	 *  \param [in]	hCam	������
	 *  \param [out]	 pAutoExposureTimeLowerLimit	�ع�ʱ�������Χ����Сֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoExposureTimeLowerLimit(HANDLE hCam, double* pAutoExposureTimeLowerLimit);
	/*!
	 *  \brief �����Զ������ع�ʱ��ʱ���ع�ʱ�������Χ����Сֵ
	 *  \param [in]	hCam	������
	 *  \param [in]	 fAutoExposureTimeLowerLimit	�ع�ʱ�������Χ�����ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoExposureTimeLowerLimit(HANDLE hCam, double fAutoExposureTimeLowerLimit);

	/*!
	 *  \brief ��ȡ�Զ������ع�ʱ��ʱ���ع�ʱ�������Χ�����ֵ
	 *  \param [in]	hCam	������
	 *  \param [out]	 pAutoExposureTimeUpperLimit	�ع�ʱ�������Χ�����ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoExposureTimeUpperLimit(HANDLE hCam, double* pAutoExposureTimeUpperLimit);
	/*!
	 *  \brief �����Զ������ع�ʱ��ʱ���ع�ʱ�������Χ�����ֵ
	 *  \param [in]	hCam	������
	 *  \param [in]	 fAutoExposureTimeUpperLimit
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoExposureTimeUpperLimit(HANDLE hCam, double fAutoExposureTimeUpperLimit);

	/*!
	 *  \brief ��ȡ�Զ���������(�ع⡢����)ʱ��������������ͼ������
	 *  \param [in]	hCam	������
	 *  \param [out]	 pAutoTargetValue	������������ͼ������
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoTargetValue(HANDLE hCam, int* pAutoTargetValue);
	/*!
	 *  \brief �����Զ���������(�ع⡢����)ʱ��������������ͼ������
	 *  \param [in]	hCam	������
	 *  \param [in]	nAutoTargetValue	������������ͼ������
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoTargetValue(HANDLE hCam, int nAutoTargetValue);

	/*!
	 *  \brief ���Զ�������Զ��ع�ʱ�䶼��ʱ����ȡ��һ��ֵ���ȵ���
	 *  \param [in]	hCam	������
	 *  \param [out]	pAutoFunctionProfile	�������Ȼ��ع�ʱ������
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoFunctionProfile(HANDLE hCam, AutoFunctionProfileEnums* pAutoFunctionProfile);
	/*!
	 *  \brief ���Զ�������Զ��ع�ʱ�䶼��ʱ��������һ��ֵ���ȵ���
	 *  \param [in]	hCam	������
	 *  \param [in]	AutoFunctionProfile	�������Ȼ��ع�ʱ������
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoFunctionProfile(HANDLE hCam, AutoFunctionProfileEnums AutoFunctionProfile);

	/*!
	 *  \brief	�Զ�������Զ��ع�ʱ��ͼ��������Ŀ�����Ȳ�����ݲ
	 *  \param [in]	hCam	������
	 *  \param [out]	 pAutoThreshold	ͼ��������Ŀ�����Ȳ�����ݲ�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetAutoThreshold(HANDLE hCam, int* pAutoThreshold);
	/*!
	 *  \brief �Զ�������Զ��ع�ʱ��ͼ��������Ŀ�����Ȳ�����ݲ
	 *  \param [in]	hCam	������
	 *  \param [in]	nAutoThreshold	ͼ��������Ŀ�����Ȳ�����ݲ�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetAutoThreshold(HANDLE hCam, int nAutoThreshold);

	/*!
	 *  \brief ��ȡ��ǰ٤��ֵ
	 *  \param [in]	hCam	������
	 *  \param [out]	 pGamma	��ǰ٤��ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGamma(HANDLE hCam, double* pGamma);
	/*!
	 *  \brief ��ȡ٤��ֵ�����õķ�Χ
	 *  \param [in]	hCam	������
	 *  \param [out]	 pGammaMin	٤����Сֵ
	 *  \param [out]	 pGammaMax	٤�����ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetGammaRange(HANDLE hCam, double* pGammaMin, double* pGammaMax);
	/*!
	 *  \brief ����٤��ֵ
	 *  \param [in]	hCam	������
	 *  \param [in]	 fGamma	٤��ֵ
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetGamma(HANDLE hCam, double fGamma);
	/*!
	 *  \brief ���ò��ұ�
	 *  \param [in]	hCam	������
	 *  \param [in]	pLUT	���ұ����飬unsigned long pLUT[1024];
	 *  \param [in]	nCnt	���ұ����鵥Ԫ������������1024
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetLUT(HANDLE hCam, unsigned long* pLUT, int nCnt);

	/*!
	 *  \brief��ʹ�ò��ұ�
	 *  \param [in]	hCam ������
	 *  \param [in]	 bEnable �Ƿ�����
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetEnableLUT(HANDLE hCam, bool bEnable);

	/*!
	 *  \brief ��ȡ��ǰ�Ƿ�ʹ�ò��ұ�״̬
	 *  \param [in]	hCam
	 *  \param [out]	bEnable	��ǰ�Ƿ�ʹ�ò��ұ�״̬
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetEnableLUT(HANDLE hCam, bool* bEnable);

	/*!
	 *  \brief �ɼ�һ֡ͼ��
	 *  \param [in]	hCam	������
	 *  \param [out]	hImage	ͼ����������ɼ�����ͼ��
	 *  \param [in]	 nWaitMs	�ȴ��೤ʱ�䣬��λms
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSingleGrab(HANDLE hCam, HANDLE hImage, unsigned long nWaitMs);

#ifdef _MSC_VER_	// Modified by Sny
	/*!
	 *  \brief ��ʼ�ɼ��������ɼ�����ͼ����ʾ��ָ������
	 *  \param [in]	hCam	������
	 *  \param [in]	hWnd	���ھ��
	 *  \param [in]	hWndMsg	��Ϣ����������ΪNULL,���µ�ͼ��ɼ����,�ᷢ����Ϣ(WM_USER+0x0200)���˴���
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVStartGrabWindow(HANDLE hCam, HWND hWnd, HWND hWndMsg);
	
	/*!
	 *  \brief	ֹͣ�ɼ�������
	 *  \param [in]	hCam	������
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVStopGrabWindow(HANDLE hCam);
#endif	
	/*!
	 *  \brief	���ɼ�������ʱ����ͣ������ɼ���
	 *  \param [in]	hCam	������
	 *  \param [in]	 bFreeze 
	 *				TRUE:��ͣ�ɼ�����ͣ����Ե���GetSampleGrab�����õ���ǰͼ��
	 *				FALSE:�����ɼ�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVFreezeGrabWindow(HANDLE hCam, bool bFreeze);
	/*!
	 *  \brief ���ɼ�������ʱ������ͼ����ʾ������ͱ�����
	 *  ��ͼ����(xSrc,ySrc,wSrc,hSrc)ָ����������ʾ��������ָ������(xDest,yDest,wDest,hDest)
	 *  \param [in]	hCam	������
	 *  \param [in]	xDest	ָ����ʾ������Ŀ��������Ͻǵ��߼�X����
	 *  \param [in]	yDest	ָ����ʾ������Ŀ��������Ͻǵ��߼�Y����
	 *  \param [in]	wDest	ָ����ʾ������Ŀ����εĿ��
	 *  \param [in]	hDest	ָ����ʾ������Ŀ����εĸ߶�
	 *  \param [in]	xSrc	ָ��ͼ��Դλͼ���Ͻǵ��߼�X����
	 *  \param [in]	ySrc	ָ��ͼ��Դλͼ���Ͻǵ��߼�Y����
	 *  \param [in]	wSrc	ָ��ͼ��Դλͼ���
	 *  \param [in]	hSrc	ָ��ͼ��Դλͼ�߶�
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetGrabWindow(HANDLE hCam, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc);

#ifdef _MSC_VER_	// Modified by Sny
	/*!
	 *  \brief ������MVFreezeGrabWindow(TRUE)�󣬵��ô˺������Ի�ȡ��ǰͼ��
	 *  \param [in]	hCam	������
	 *  \param [out]	 image	ͼ��
	 *  \param [out]	 nFrameID		ͼ���ID��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetSampleGrab(HANDLE hCam, MVImage* image, int* nFrameID);
#endif	

	/*!
	 *  \brief ��������յ��µ�ͼ�񣬶���һ֡��Callback������û��ִ���꣬SDK�л��ӵ��µ�һ֡ͼ�񡣴˺������Ի�ȡ�ӵ���֡����
	 *  \param [in]	hCam	������
	 *  \param [out]	pDroppedFrames	�ӵ���֡��
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetDroppedFrame(HANDLE hCam,unsigned long *pDroppedFrames);

	/*!
	 *  \brief ��ȡ�豸��������
	 *  \param [in]	hCam	������
	 *  \param [out]	pBuf	���ڱ������ƵĻ����������ڵ���32�ֽ�
	 *  \param [in,out]	szBuf ��������С
	 *  \retval		MVST_SUCCESS			: �ɹ�  	
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetDeviceVendorName(HANDLE hCam,char *pBuf,int *szBuf);

	/*!
	 *  \brief ��ȡ�豸���ͺ�
	 *  \param [in]	hCam	������
	 *  \param [out]	pBuf ���ڱ����ͺŵĻ����������ڵ���32�ֽ�
	 *  \param [in,out]	szBuf ��������С
	 *  \retval  	MVST_SUCCESS			: �ɹ�  	
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetDeviceModelName(HANDLE hCam,char *pBuf,int *szBuf);

	/*!
	 *  \brief ��ȡ�豸��ID�ţ������к�
	 *  \param [in]	hCam	������
	 *  \param [out]	pBuf	���ڱ������кŵĻ����������ڵ���16�ֽ�
	 *  \param [in,out]	szBuf	��������С
	 *  \retval  	MVST_SUCCESS			: �ɹ�  	
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetDeviceDeviceID(HANDLE hCam,char *pBuf,int *szBuf);

	/*!
	 *  \brief ����Ƿ����ڲɼ�ͼ��
	 *  \param [in]	hCam	������
	 *  \retval  	���ڲɼ�ͼ�񷵻�TRUE,���򷵻�FALSE
	 */
	MVGIGE_API bool __stdcall MVIsRunning(HANDLE hCam);
#ifdef _MSC_VER_	// Modified by Sny
	/*!
	 *  \brief ͼ���ʽת��
	 *  \param [in]	hCam	������
	 *  \param [in]	pImageSrc	Դͼ��ָ��
	 *  \param [out]	pImageDst	Ŀ��ͼ��ָ��
	 *  \retval  	
	 *  \note Ŀǰ��֧��Դͼ���Ŀ��ͼ��Ŀ����ͬ����16BitתΪ8Bit, ��48BitתΪ24Bit
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVConvertImage( HANDLE hCam, MVImage* pImageSrc,MVImage* pImageDst );


	/*!
	 *  \brief ֱ�Ӵӻص��������ص�ͼ����Ϣ�вü���ͼ���һ���֣��������֧��Ӳ��ROIʱ�������ô˺���ʵ�����ROI��
	 *  \param [in]	hCam	������
	 *  \param [in]	pInfoSrc	Դͼ��ָ�룬һ���ǻص��������ص�ͼ����Ϣָ��
	 *  \param [out]	pInfoDst	Ŀ��ͼ��ָ�룬nPixelTypeҪ��Դͼ�����ͬ����Ҫ��ǰ������ڴ档���ҿ�Ⱥ͸߶�Ҫ��roi�Ŀ����ͬ��
	 *  \param [in]	roi	����Ȥ������,roi��left,right,top,bottom������2��������
	 *  \retval  	
	 *  \note Ŀǰ��֧��Դͼ���Ŀ��ͼ��Ŀ����ͬ����16BitתΪ8Bit, ��48BitתΪ24Bit
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVCopyImageInfoROI( HANDLE hCam, MV_IMAGE_INFO* pInfoSrc, MV_IMAGE_INFO* pInfoDst, RECT roi );
#endif	
	/*!
	 *  \brief ���ڱ��Ͷ�
	 *  \param [in]	hCam	������
	 *  \param [in]	nSaturation	���Ͷ�,��Χ-100��100, -100Ϊ�ڰף�0Ϊԭͼ,100Ϊ������
	 *  \retval  	
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetSaturation(HANDLE hCam, int nSaturation );
	
	/*! 
	 *  \brief	��ȡ��ǰ���Ͷ�
	 *  \param [in]	hCam	������
	 *  \param [out]	nSaturation	���Ͷ�ָ��
	 *  \retval 
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetSaturation(HANDLE hCam, int *nSaturation );
	
	/*!
	 *  \brief ��ɫУ��
	 *  \param [in]	hCam	������
	 *  \param [in]	nColorCorrect, ��ɫУ��ģʽ��Ŀǰ��֧��0��1,0Ϊ��У����1ΪУ��
	 *  \retval  	
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetColorCorrect(HANDLE hCam, int nColorCorrect );

	/*! 
	 *  \brief����ȡ��ǰ��ɫУ��ģʽ
	 *  \param [in]	hCam	������
	 *  \param [out]	nColorCorrect	��ɫУ��ģʽָ��
	 *  \retval 
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetColorCorrect(HANDLE hCam, int *nColorCorrect );


#ifdef _MSC_VER_	// Modified by Sny
	/*!
	 *  \brief ע�����ڽ�����Ϣ�Ĵ��ھ������Ϣֵ��������Ͽ�����������ʱ�ᷢ����Ϣ���ô��ڡ�
	 *  \param [in]	hCam	������
	 *  \param [in]	hWnd	���ڽ�����Ϣ�Ĵ��ھ��
	 *  \param [in]	nMsg	��Ϣֵ
	 *  \retval  	
	 *  \sa	MVEnableMessage
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVRegisterMessage(HANDLE hCam, HWND hWnd, UINT nMsg);

	/*!
	 *  \brief �Ƿ�������ĳ����Ϣ
	 *  \param [in]	hCam	������
	 *  \param [in]	nMessageType	��Ϣ����, MSG_ID_LOST,MSG_ID_RECONNECT
	 *  \param [in]	bEnable	���ΪTRUE,���͸���Ϣ��ΪFALSE�򲻷��͸���Ϣ
	 *  \retval  	
	 *	\sa MVRegisterMessage
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVEnableMessage(HANDLE hCam, int nMessageType, bool bEnable);
#endif
	/*! 
	 *  \brief����ȡ�Զ�������
	 *  \param [in]	hCam	������
	 *  \param [out]	pBuf	������ƻ�����
	 *  \param [in/out]	szBuf	������ƻ���������ָ��
	 *  \retval 
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVGetUserDefinedName(HANDLE hCam, char *pBuf,int *szBuf );

	/*! 
	 *  \brief�������Զ�������
	 *  \param [in]	hCam	������
	 *  \param [out]	pBuf	������ƻ�����
	 *  \param [in]	szBuf	������ƻ���������ָ��
	 *  \retval 
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVSetUserDefinedName(HANDLE hCam, char *pBuf,int szBuf );
	
	/*!
	 *  \brief ��ֻ����ʽ�����
	 *  \param [in]	idx	idx��0��ʼ�����������IP��ַ���򣬵�ַС������ǰ�档
	 *  \param [out]	hCam ����ɹ�,���ص�������
	 *  \retval MVST_INVALID_PARAMETER : idxȡֵ����
	 *			MVST_ACCESS_DENIED		: ����޷����ʣ�������������������
	 *			MVST_ERROR				: ��������
	 *			MVST_SUCCESS			: �ɹ�
	 */
	MVGIGE_API MVSTATUS_CODES __stdcall MVOpenCamByIndexReadOnly(unsigned char idx,HANDLE *hCam);

	/*!
	 *  \brief ����Ӧ��ֵ�˲�������ȥ����ʱ���عⰵ������ɵ�ͼ�������㣬Ŀǰ��������8Bit�ڰ�ͼ��
	 *  \param [in]	hCam ������
	 *  \param [in/out]	pImage ͼ��
	 *  \param [in]	th ��ֵ������Χ������ڴ���ֵ�ı���Ϊ�����
	 *  \retval  	MVST_SUCCESS			: �ɹ�
	 */
	// Modified by Sny
	//MVGIGE_API MVSTATUS_CODES __stdcall MVFilterAdaptiveMedian(HANDLE hCam,MVImage *pImage,int th);
#ifdef __cplusplus
}
#endif

#endif //MVGIGE_H_INCLUDE_