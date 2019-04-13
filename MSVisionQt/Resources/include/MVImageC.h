#ifndef _MV_IMAGEC_H
#define _MV_IMAGEC_H

#ifdef MVGIGE_EXPORTS
#define MVGIGE_API __declspec(dllexport)
#else
#define MVGIGE_API __declspec(dllimport)
#endif

#ifndef _MSC_VER
typedef char* LPCSTR ;
typedef void* CWnd ;
typedef void* HANDLE; 
typedef unsigned short WORD ;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*!
*  \brief 创建图像
*  \param [in]	nWidth	图像宽度
*  \param [in]	nHeight	图像高度
*  \param [in]	nBPP	每像素Bit数。取值范围为8(8bit灰度),16(16bit灰度),24(8bit彩色),48(16bit彩色)
*  \retval  图像句柄	
*/
MVGIGE_API void* __stdcall MVImageCreate(int nWidth, int nHeight, int nBPP);

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
*  \retval  	图像句柄	
*/
MVGIGE_API int __stdcall MVImageCreateByPixelFormat( void* hImage, int nWidth, int nHeight, MV_PixelFormatEnums PixelFormat );

/*!
*  \brief 判断图像是否为空。在调用Create或CreateByPixelFormat之前，图像为空
*  \param [in]	hImage	相机句柄
*  \retval  	bool	TRUE:图像还未创建
*/
MVGIGE_API bool __stdcall MVImageIsNull(void* hImage);

/*!
*  \brief 获取图像宽度，单位为像素
*  \param [in]	hImage	相机句柄
*  \retval  	图像宽度
*/
MVGIGE_API int __stdcall MVImageGetWidth(void* hImage);

/*!
*  \brief 获取图像高度，单位为像素
*  \param [in]	hImage	相机句柄
*  \retval  	图像高度
*/
MVGIGE_API int __stdcall MVImageGetHeight(void* hImage);

/*!
*  \brief 获取图像缓冲区的指针
*  \param [in]	hImage	相机句柄
*  \retval  指向图像缓冲区的指针
*/
MVGIGE_API void* __stdcall MVImageGetBits(void* hImage);

/*!
*  \brief 获取一行图像的字节数
*  \param [in]	hImage	相机句柄
*  \retval  一行图像的字节数
*/
MVGIGE_API int __stdcall MVImageGetPitch(void* hImage);

/*!
*  \brief 获取图像中每个像素所占位数
*  \param [in]	hImage	相机句柄
*  \retval  每个像素所占位数。
*  \note	8: 8Bit 灰度图，16: 16Bit 灰度图，24: 8Bit BGR彩色图，48: 16Bit BGR彩色图
*/
MVGIGE_API int __stdcall MVImageGetBPP(void* hImage);

/*!
*  \brief 获取HBITMAP类型的句柄。用于绘图或生成AVI
*  \param [in]	hImage	相机句柄
*  \retval  HBITMAP类型的句柄
*/
MVGIGE_API HBITMAP __stdcall MVImageGetHBitmap(void* hImage);

/*!
*  \brief 在目标DC上的指定区域绘制本图像的全部或局部
*  \param [in]	hImage	相机句柄
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
MVGIGE_API bool __stdcall MVImageDrawEx(void* hImage, HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);

/*!
*  \brief 在目标DC上的指定位置1:1绘制本图像
*  \param [in]	hImage	相机句柄
*  \param [in]	hDestDC	目标DC
*  \param [in]	xDest	图像绘制的起始水平坐标
*  \param [in]	yDest	图像绘制的起始垂直坐标
*  \retval  	
*/
MVGIGE_API bool __stdcall MVImageDraw(void* hImage, HDC hDestDC, int xDest, int yDest);

#ifndef _MSC_VER
/*!
*  \brief 在目标窗体上的指定位置1:1绘制本图像
*  \param [in]	hImage	相机句柄
*  \param [in]	hWnd	目标窗体
*  \param [in]	xDest	图像绘制的起始水平坐标
*  \param [in]	yDest	图像绘制的起始垂直坐标
*  \retval  	
*/
MVGIGE_API bool __stdcall MVImageDrawHwnd(void* hImage, HWND hWnd, int xDest, int yDest);

/*!
*  \brief 在目标窗体上的指定区域绘制本图像的全部或局部
*  \param [in]	hImage	相机句柄
*  \param [in]	hWnd	目标窗体的句柄
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
MVGIGE_API bool __stdcall MVImageDrawHwndEx( void* hImage, HWND hWnd, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight );

#endif
/*!
*  \brief 获取图像的DC，可用于在图像上绘制图形。
*  \param [in]	hImage	相机句柄
*  \retval  图像DC
*	\note	使用结束后应调用ReleaseDC进行释放
*/
MVGIGE_API HDC __stdcall MVImageGetDC(void* hImage);

/*!
*  \brief 释放使用GetDC获取的DC
*  \param [in]	hImage	相机句柄
*/
MVGIGE_API void __stdcall MVImageReleaseDC(void* hImage);

/*!
*  \brief 保存图像
*  \param [in]	hImage	相机句柄
*  \param [in]	pszFileName	文件名。文件类型可以为.bmp,.jpg,.tif,.raw(仅支持位深为16位的图像)
*  \retval  	
*  \note 16位和48位图像仅在保存为.tif格式时，保留16位或48位位深。保存为bmp,jpg均被转换为8位或24位图像保存。
*/
MVGIGE_API int __stdcall MVImageSave(void* hImage, LPCSTR pszFileName);

/*!
*  \brief 从文件读取图像
*  \param [in]	hImage	相机句柄
*  \param [in]	pszFileName	文件名。支持的文件类型为.bmp,.jpg,.tif,.raw
*  \retval  	
*/
MVGIGE_API int __stdcall MVImageLoad( void* hImage,LPCSTR pszFileName );

/*!
*  \brief 释放图像资源
*  \param [in]	hImage	相机句柄
*  \retval  	
*/
MVGIGE_API void __stdcall MVImageDestroy(void* hImage);

#ifdef __cplusplus
}
#endif

#endif