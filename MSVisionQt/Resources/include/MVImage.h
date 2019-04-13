#ifndef _MV_IMAGE_H
#define _MV_IMAGE_H

#include "GigECamera_Types.h"

#ifdef MVGIGE_EXPORTS
#define MVGIGE_API __declspec(dllexport)
#else
#define MVGIGE_API __declspec(dllimport)
#endif

class MVGIGE_API MVImage
{
public:
	MVImage();
	~MVImage();

	/*!
	 *  \brief 根据宽度、高度、每像素Bit数创建图像
	 *  \param [in]	nWidth	宽
	 *  \param [in]	nHeight	高
	 *  \param [in]	nBPP	每像素Bit数。取值范围为8(8bit灰度),16(16bit灰度),24(8bit彩色),48(16bit彩色)
	 *  \retval  	int
	 */
	int Create( int nWidth, int nHeight, int nBPP );

	/*!
	 *  \brief 根据宽度、高度、和像素格式创建图像
	 *  \param [in]	nWidth	宽度
	 *  \param [in]	nHeight	高度
	 *  \param [in]	PixelFormat	像素格式
	 *	\note
	 *	@verbatim 
	 *	------------------------------------------------
	 *	|Pixelformat			|	Image Format		|
	 *	------------------------------------------------
	 *	|PixelFormat_Mono8		|	8 Bits 灰度图		|
	 *	------------------------------------------------
	 *	|PixelFormat_Mono16		|	16 Bits 灰度图		|
	 *	------------------------------------------------
	 *	|PixelFormat_BayerBG8	|						|
	 *	|PixelFormat_BayerRG8	|						|
	 *	|PixelFormat_BayerGB8	|	24 Bits BGR彩色图	|
	 *	|PixelFormat_BayerGR8	|						|
	 *	------------------------------------------------
	 *	|PixelFormat_BayerBG16	|						|
	 *	|PixelFormat_BayerRG16	|						|
	 *	|PixelFormat_BayerGB16	|	48 Bits BGR彩色图	|
	 *	|PixelFormat_BayerGR16	|						|
	 *	------------------------------------------------
	 *	@endverbatim 
	 *  \retval  	int	
	 */
	int CreateByPixelFormat( int nWidth, int nHeight, MV_PixelFormatEnums PixelFormat );
	
	/*!
	 *  \brief 判断图像是否为空。在调用Create或CreateByPixelFormat之前，图像为空
	 *  \retval  	BOOL
	 */
	BOOL IsNull();
	
	/*!
	 *  \brief 获取图像宽度，单位为像素
	 *  \retval  	图像宽度
	 */
	int GetWidth();
	
	/*!
	 *  \brief 获取图像高度，单位为像素
	 *  \retval  	图像高度
	 */
	int GetHeight();
	
	/*!
	 *  \brief 获取图像缓冲区的指针
	 *  \retval  指向图像缓冲区的指针
	 */
	void* GetBits();

	/*!
	 *  \brief 获取一行图像的字节数
	 *  \retval  一行图像的字节数
	 */
	int	GetPitch();

	/*!
	 *  \brief 获取图像中每个像素所占位数
	 *  \retval  每个像素所占位数。
	 *  \note	8: 8Bit 灰度图，16: 16Bit 灰度图，24: 8Bit BGR彩色图，48: 16Bit BGR彩色图
	 */
	int	GetBPP();
	
	/*!
	 *  \brief 获取HBITMAP类型的句柄。用于绘图或生成AVI
	 *  \retval  HBITMAP类型的句柄
	 */
	HBITMAP GetHBitmap();
	
	/*!
	 *  \brief 在目标DC上的指定区域绘制本图像的全部或局部
	 *  \param [in]	hDestDC	目标DC的句柄
	 *  \param [in]	xDest	x坐标，目标矩形的左上角。
	 *  \param [in]	yDest	y坐标，目标矩形的左上角。
	 *  \param [in]	nDestWidth	宽度，目标矩形。
	 *  \param [in]	nDestHeight	高度，目标矩形。
	 *  \param [in]	xSrc	x坐标，源矩形的左上角。
	 *  \param [in]	ySrc	y坐标，源矩形的左上角。
	 *  \param [in]	nSrcWidth	宽度，源矩形。
	 *  \param [in]	nSrcHeight	高度，源矩形。
	 *  \retval  	
	 */
	BOOL Draw( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight );
	
	/*!
	 *  \brief 在目标DC上的指定区域绘制本图像的全部
	 *  \param [in]	hDestDC	目标DC的句柄
	 *  \param [in]	xDest	x坐标，目标矩形的左上角。
	 *  \param [in]	yDest	y坐标，目标矩形的左上角。
	 *  \param [in]	nDestWidth	宽度，目标矩形。
	 *  \param [in]	nDestHeight	高度，目标矩形。
	 *  \retval  	
	 */
	BOOL Draw( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight );
	
	/*!
	 *  \brief 在目标DC上的指定位置1:1绘制本图像
	 *  \param [in]	hDestDC	目标DC
	 *  \param [in]	xDest	图像绘制的起始水平坐标
	 *  \param [in]	yDest	图像绘制的起始垂直坐标
	 *  \retval  	
	 */
	BOOL Draw( HDC hDestDC, int xDest, int yDest );

	/*!
	 *  \brief 获取图像的DC，可用于在图像上绘制图形。
	 *  \retval  图像DC
	 *	\note	使用结束后应调用ReleaseDC进行释放
	 */
	HDC GetDC();

	/*!
	 *  \brief 释放使用GetDC获取的DC
	 */
	void ReleaseDC();

	/*!
	 *  \brief 保存图像
	 *  \param [in]	pszFileName	文件名。文件类型可以为.bmp,.jpg,.tif,.raw(仅支持位深为16位的图像)
	 *  \retval  	
	 *  \note 16位和48位图像仅在保存为.tif格式时，保留16位或48位位深。保存为bmp,jpg均被转换为8位或24位图像保存。
	 */
	int Save(LPCTSTR pszFileName);

	/*!
	 *  \brief 从文件读取图像
	 *  \param [in]	pszFileName	文件名。支持的文件类型为.bmp,.jpg,.tif,.raw
	 *  \retval  	
	 */
	int Load( LPCTSTR pszFileName );

	/*!
	 *  \brief 获取ATL::CImage
	 *  \retval  	
	 */
	void *GetCImageHandle();
	
	/*!
	 *  \brief 释放图像资源
	 *  \retval  	
	 */
	void Destroy();
	
	/*!
	 *  \brief 图像平滑
	 *  \retval  	
	 */
	int Smooth();
private:
	HANDLE m_hImage;
	HANDLE m_hImage16;
};

#endif
