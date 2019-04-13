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
*  \brief ����ͼ��
*  \param [in]	nWidth	ͼ����
*  \param [in]	nHeight	ͼ��߶�
*  \param [in]	nBPP	ÿ����Bit����ȡֵ��ΧΪ8(8bit�Ҷ�),16(16bit�Ҷ�),24(8bit��ɫ),48(16bit��ɫ)
*  \retval  ͼ����	
*/
MVGIGE_API void* __stdcall MVImageCreate(int nWidth, int nHeight, int nBPP);

/*!
*  \brief ���ݿ�ȡ��߶ȡ������ظ�ʽ����ͼ��
*  \param [in]	nWidth	���
*  \param [in]	nHeight	�߶�
*  \param [in]	PixelFormat	���ظ�ʽ
*	\note
*	@verbatim 
*	------------------------------------------------
*	|Pixelformat			|	Image Format		|
*	------------------------------------------------
*	|PixelFormat_Mono8		|	8 Bits �Ҷ�ͼ		|
*	------------------------------------------------
*	|PixelFormat_Mono16		|	16 Bits �Ҷ�ͼ		|
*	------------------------------------------------
*	|PixelFormat_BayerBG8	|						|
*	|PixelFormat_BayerRG8	|						|
*	|PixelFormat_BayerGB8	|	24 Bits BGR��ɫͼ	|
*	|PixelFormat_BayerGR8	|						|
*	------------------------------------------------
*	|PixelFormat_BayerBG16	|						|
*	|PixelFormat_BayerRG16	|						|
*	|PixelFormat_BayerGB16	|	48 Bits BGR��ɫͼ	|
*	|PixelFormat_BayerGR16	|						|
*	------------------------------------------------
*	@endverbatim 
*  \retval  	ͼ����	
*/
MVGIGE_API int __stdcall MVImageCreateByPixelFormat( void* hImage, int nWidth, int nHeight, MV_PixelFormatEnums PixelFormat );

/*!
*  \brief �ж�ͼ���Ƿ�Ϊ�ա��ڵ���Create��CreateByPixelFormat֮ǰ��ͼ��Ϊ��
*  \param [in]	hImage	������
*  \retval  	bool	TRUE:ͼ��δ����
*/
MVGIGE_API bool __stdcall MVImageIsNull(void* hImage);

/*!
*  \brief ��ȡͼ���ȣ���λΪ����
*  \param [in]	hImage	������
*  \retval  	ͼ����
*/
MVGIGE_API int __stdcall MVImageGetWidth(void* hImage);

/*!
*  \brief ��ȡͼ��߶ȣ���λΪ����
*  \param [in]	hImage	������
*  \retval  	ͼ��߶�
*/
MVGIGE_API int __stdcall MVImageGetHeight(void* hImage);

/*!
*  \brief ��ȡͼ�񻺳�����ָ��
*  \param [in]	hImage	������
*  \retval  ָ��ͼ�񻺳�����ָ��
*/
MVGIGE_API void* __stdcall MVImageGetBits(void* hImage);

/*!
*  \brief ��ȡһ��ͼ����ֽ���
*  \param [in]	hImage	������
*  \retval  һ��ͼ����ֽ���
*/
MVGIGE_API int __stdcall MVImageGetPitch(void* hImage);

/*!
*  \brief ��ȡͼ����ÿ��������ռλ��
*  \param [in]	hImage	������
*  \retval  ÿ��������ռλ����
*  \note	8: 8Bit �Ҷ�ͼ��16: 16Bit �Ҷ�ͼ��24: 8Bit BGR��ɫͼ��48: 16Bit BGR��ɫͼ
*/
MVGIGE_API int __stdcall MVImageGetBPP(void* hImage);

/*!
*  \brief ��ȡHBITMAP���͵ľ�������ڻ�ͼ������AVI
*  \param [in]	hImage	������
*  \retval  HBITMAP���͵ľ��
*/
MVGIGE_API HBITMAP __stdcall MVImageGetHBitmap(void* hImage);

/*!
*  \brief ��Ŀ��DC�ϵ�ָ��������Ʊ�ͼ���ȫ����ֲ�
*  \param [in]	hImage	������
*  \param [in]	hDestDC	Ŀ��DC�ľ��
*  \param [in]	xDest	x���꣬Ŀ����ε����Ͻǡ�
*  \param [in]	yDest	y���꣬Ŀ����ε����Ͻǡ�
*  \param [in]	nDestWidth	��ȣ�Ŀ����Ρ�
*  \param [in]	nDestHeight	�߶ȣ�Ŀ����Ρ�
*  \param [in]	xSrc	x���꣬Դ���ε����Ͻǡ�
*  \param [in]	ySrc	y���꣬Դ���ε����Ͻǡ�
*  \param [in]	nSrcWidth	��ȣ�Դ���Ρ�
*  \param [in]	nSrcHeight	�߶ȣ�Դ���Ρ�
*  \retval  	
*/
MVGIGE_API bool __stdcall MVImageDrawEx(void* hImage, HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);

/*!
*  \brief ��Ŀ��DC�ϵ�ָ��λ��1:1���Ʊ�ͼ��
*  \param [in]	hImage	������
*  \param [in]	hDestDC	Ŀ��DC
*  \param [in]	xDest	ͼ����Ƶ���ʼˮƽ����
*  \param [in]	yDest	ͼ����Ƶ���ʼ��ֱ����
*  \retval  	
*/
MVGIGE_API bool __stdcall MVImageDraw(void* hImage, HDC hDestDC, int xDest, int yDest);

#ifndef _MSC_VER
/*!
*  \brief ��Ŀ�괰���ϵ�ָ��λ��1:1���Ʊ�ͼ��
*  \param [in]	hImage	������
*  \param [in]	hWnd	Ŀ�괰��
*  \param [in]	xDest	ͼ����Ƶ���ʼˮƽ����
*  \param [in]	yDest	ͼ����Ƶ���ʼ��ֱ����
*  \retval  	
*/
MVGIGE_API bool __stdcall MVImageDrawHwnd(void* hImage, HWND hWnd, int xDest, int yDest);

/*!
*  \brief ��Ŀ�괰���ϵ�ָ��������Ʊ�ͼ���ȫ����ֲ�
*  \param [in]	hImage	������
*  \param [in]	hWnd	Ŀ�괰��ľ��
*  \param [in]	xDest	x���꣬Ŀ����ε����Ͻǡ�
*  \param [in]	yDest	y���꣬Ŀ����ε����Ͻǡ�
*  \param [in]	nDestWidth	��ȣ�Ŀ����Ρ�
*  \param [in]	nDestHeight	�߶ȣ�Ŀ����Ρ�
*  \param [in]	xSrc	x���꣬Դ���ε����Ͻǡ�
*  \param [in]	ySrc	y���꣬Դ���ε����Ͻǡ�
*  \param [in]	nSrcWidth	��ȣ�Դ���Ρ�
*  \param [in]	nSrcHeight	�߶ȣ�Դ���Ρ�
*  \retval  	
*/
MVGIGE_API bool __stdcall MVImageDrawHwndEx( void* hImage, HWND hWnd, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight );

#endif
/*!
*  \brief ��ȡͼ���DC����������ͼ���ϻ���ͼ�Ρ�
*  \param [in]	hImage	������
*  \retval  ͼ��DC
*	\note	ʹ�ý�����Ӧ����ReleaseDC�����ͷ�
*/
MVGIGE_API HDC __stdcall MVImageGetDC(void* hImage);

/*!
*  \brief �ͷ�ʹ��GetDC��ȡ��DC
*  \param [in]	hImage	������
*/
MVGIGE_API void __stdcall MVImageReleaseDC(void* hImage);

/*!
*  \brief ����ͼ��
*  \param [in]	hImage	������
*  \param [in]	pszFileName	�ļ������ļ����Ϳ���Ϊ.bmp,.jpg,.tif,.raw(��֧��λ��Ϊ16λ��ͼ��)
*  \retval  	
*  \note 16λ��48λͼ����ڱ���Ϊ.tif��ʽʱ������16λ��48λλ�����Ϊbmp,jpg����ת��Ϊ8λ��24λͼ�񱣴档
*/
MVGIGE_API int __stdcall MVImageSave(void* hImage, LPCSTR pszFileName);

/*!
*  \brief ���ļ���ȡͼ��
*  \param [in]	hImage	������
*  \param [in]	pszFileName	�ļ�����֧�ֵ��ļ�����Ϊ.bmp,.jpg,.tif,.raw
*  \retval  	
*/
MVGIGE_API int __stdcall MVImageLoad( void* hImage,LPCSTR pszFileName );

/*!
*  \brief �ͷ�ͼ����Դ
*  \param [in]	hImage	������
*  \retval  	
*/
MVGIGE_API void __stdcall MVImageDestroy(void* hImage);

#ifdef __cplusplus
}
#endif

#endif