#ifndef _MV_AVI_H
#define _MV_AVI_H

#ifdef MVGIGE_EXPORTS
#define MVGIGE_API __declspec(dllexport)
#else
#define MVGIGE_API __declspec(dllimport)
#endif

#include <mmreg.h>
#include <vfw.h>

#include "MVImage.h"

class MVGIGE_API MVAvi
{
public:
	MVAvi();
	~MVAvi();

	/*!
	 *  \brief ����AVI�ļ�
	 *  \param [in]	 strFileName	�ļ���
	 *  \param [in]	 framesPerSecondPlayback	��Ƶ����֡��fps
	 *  \retval  ����ɹ�����0
	 */
	HRESULT Create(char *strFileName, double framesPerSecondPlayback);
	
	/*!
	 *  \brief ����ѹ������
	 *  \param [in]	 pImage ͼ��ָ��
	 *  \param [in]	 opts	ѹ������������ΪNULL
	 *  \param [in]	ShowDialog	�Ƿ���ʾѹ���������öԻ���
	 *  \param [in]	hparent	�����ʾѹ���������öԻ��򣬸��������ھ�����������ΪNULL
	 *  \retval  ����ɹ�����0
	 */
	HRESULT SetVideoCompression(MVImage *pImage, AVICOMPRESSOPTIONS *opts, bool ShowDialog, HWND hparent );
	/*!
	 *  \brief ��AVI������µ�ͼƬ
	 *  \param [in]	 pImage	ͼ��ָ��
	 *  \retval ����ɹ�����0
	 */
	HRESULT AddFrame(MVImage *pImage);
	/*!
	 *  \brief	�ر��ļ� 
	 *  \retval ����ɹ�����0
	 */
	HRESULT Close();

	/*!
	 *  \brief	��ȡ�Ѿ�д��AVI�ļ���ͼ��֡��
	 *  \retval  	�����Ѿ�д��AVI�ļ���֡��
	 */
	unsigned int GetCountOfAddedImages();

	/*!
	 *  \brief ��ȡ�Ѿ�д��AVI�ļ���ͼ���ֽ�����
	 *  \retval  �����Ѿ�д��AVI�ļ���ͼ���ֽ�����
	 */
	unsigned int GetImageDataBytesWritten();
private:
	HANDLE	m_hAvi;
	unsigned	int m_nFrames;
	unsigned	int m_nBytes;
};

#endif

