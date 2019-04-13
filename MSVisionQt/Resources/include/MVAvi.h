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
	 *  \brief 创建AVI文件
	 *  \param [in]	 strFileName	文件名
	 *  \param [in]	 framesPerSecondPlayback	视频播放帧率fps
	 *  \retval  如果成功返回0
	 */
	HRESULT Create(char *strFileName, double framesPerSecondPlayback);
	
	/*!
	 *  \brief 设置压缩参数
	 *  \param [in]	 pImage 图像指针
	 *  \param [in]	 opts	压缩参数。可以为NULL
	 *  \param [in]	ShowDialog	是否显示压缩参数设置对话框
	 *  \param [in]	hparent	如果显示压缩参数设置对话框，给出父窗口句柄。否则可以为NULL
	 *  \retval  如果成功返回0
	 */
	HRESULT SetVideoCompression(MVImage *pImage, AVICOMPRESSOPTIONS *opts, bool ShowDialog, HWND hparent );
	/*!
	 *  \brief 向AVI中添加新的图片
	 *  \param [in]	 pImage	图像指针
	 *  \retval 如果成功返回0
	 */
	HRESULT AddFrame(MVImage *pImage);
	/*!
	 *  \brief	关闭文件 
	 *  \retval 如果成功返回0
	 */
	HRESULT Close();

	/*!
	 *  \brief	获取已经写入AVI文件的图像帧数
	 *  \retval  	返回已经写入AVI文件的帧数
	 */
	unsigned int GetCountOfAddedImages();

	/*!
	 *  \brief 获取已经写入AVI文件的图像字节数。
	 *  \retval  返回已经写入AVI文件的图像字节数。
	 */
	unsigned int GetImageDataBytesWritten();
private:
	HANDLE	m_hAvi;
	unsigned	int m_nFrames;
	unsigned	int m_nBytes;
};

#endif

