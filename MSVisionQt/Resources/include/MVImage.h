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
	 *  \brief ���ݿ�ȡ��߶ȡ�ÿ����Bit������ͼ��
	 *  \param [in]	nWidth	��
	 *  \param [in]	nHeight	��
	 *  \param [in]	nBPP	ÿ����Bit����ȡֵ��ΧΪ8(8bit�Ҷ�),16(16bit�Ҷ�),24(8bit��ɫ),48(16bit��ɫ)
	 *  \retval  	int
	 */
	int Create( int nWidth, int nHeight, int nBPP );

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
	 *  \retval  	int	
	 */
	int CreateByPixelFormat( int nWidth, int nHeight, MV_PixelFormatEnums PixelFormat );
	
	/*!
	 *  \brief �ж�ͼ���Ƿ�Ϊ�ա��ڵ���Create��CreateByPixelFormat֮ǰ��ͼ��Ϊ��
	 *  \retval  	BOOL
	 */
	BOOL IsNull();
	
	/*!
	 *  \brief ��ȡͼ���ȣ���λΪ����
	 *  \retval  	ͼ����
	 */
	int GetWidth();
	
	/*!
	 *  \brief ��ȡͼ��߶ȣ���λΪ����
	 *  \retval  	ͼ��߶�
	 */
	int GetHeight();
	
	/*!
	 *  \brief ��ȡͼ�񻺳�����ָ��
	 *  \retval  ָ��ͼ�񻺳�����ָ��
	 */
	void* GetBits();

	/*!
	 *  \brief ��ȡһ��ͼ����ֽ���
	 *  \retval  һ��ͼ����ֽ���
	 */
	int	GetPitch();

	/*!
	 *  \brief ��ȡͼ����ÿ��������ռλ��
	 *  \retval  ÿ��������ռλ����
	 *  \note	8: 8Bit �Ҷ�ͼ��16: 16Bit �Ҷ�ͼ��24: 8Bit BGR��ɫͼ��48: 16Bit BGR��ɫͼ
	 */
	int	GetBPP();
	
	/*!
	 *  \brief ��ȡHBITMAP���͵ľ�������ڻ�ͼ������AVI
	 *  \retval  HBITMAP���͵ľ��
	 */
	HBITMAP GetHBitmap();
	
	/*!
	 *  \brief ��Ŀ��DC�ϵ�ָ��������Ʊ�ͼ���ȫ����ֲ�
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
	BOOL Draw( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight );
	
	/*!
	 *  \brief ��Ŀ��DC�ϵ�ָ��������Ʊ�ͼ���ȫ��
	 *  \param [in]	hDestDC	Ŀ��DC�ľ��
	 *  \param [in]	xDest	x���꣬Ŀ����ε����Ͻǡ�
	 *  \param [in]	yDest	y���꣬Ŀ����ε����Ͻǡ�
	 *  \param [in]	nDestWidth	��ȣ�Ŀ����Ρ�
	 *  \param [in]	nDestHeight	�߶ȣ�Ŀ����Ρ�
	 *  \retval  	
	 */
	BOOL Draw( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight );
	
	/*!
	 *  \brief ��Ŀ��DC�ϵ�ָ��λ��1:1���Ʊ�ͼ��
	 *  \param [in]	hDestDC	Ŀ��DC
	 *  \param [in]	xDest	ͼ����Ƶ���ʼˮƽ����
	 *  \param [in]	yDest	ͼ����Ƶ���ʼ��ֱ����
	 *  \retval  	
	 */
	BOOL Draw( HDC hDestDC, int xDest, int yDest );

	/*!
	 *  \brief ��ȡͼ���DC����������ͼ���ϻ���ͼ�Ρ�
	 *  \retval  ͼ��DC
	 *	\note	ʹ�ý�����Ӧ����ReleaseDC�����ͷ�
	 */
	HDC GetDC();

	/*!
	 *  \brief �ͷ�ʹ��GetDC��ȡ��DC
	 */
	void ReleaseDC();

	/*!
	 *  \brief ����ͼ��
	 *  \param [in]	pszFileName	�ļ������ļ����Ϳ���Ϊ.bmp,.jpg,.tif,.raw(��֧��λ��Ϊ16λ��ͼ��)
	 *  \retval  	
	 *  \note 16λ��48λͼ����ڱ���Ϊ.tif��ʽʱ������16λ��48λλ�����Ϊbmp,jpg����ת��Ϊ8λ��24λͼ�񱣴档
	 */
	int Save(LPCTSTR pszFileName);

	/*!
	 *  \brief ���ļ���ȡͼ��
	 *  \param [in]	pszFileName	�ļ�����֧�ֵ��ļ�����Ϊ.bmp,.jpg,.tif,.raw
	 *  \retval  	
	 */
	int Load( LPCTSTR pszFileName );

	/*!
	 *  \brief ��ȡATL::CImage
	 *  \retval  	
	 */
	void *GetCImageHandle();
	
	/*!
	 *  \brief �ͷ�ͼ����Դ
	 *  \retval  	
	 */
	void Destroy();
	
	/*!
	 *  \brief ͼ��ƽ��
	 *  \retval  	
	 */
	int Smooth();
private:
	HANDLE m_hImage;
	HANDLE m_hImage16;
};

#endif
