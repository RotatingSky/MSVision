#ifndef _MVPROPERTY_H_INCLUDE_

#define _MVPROPERTY_H_INCLUDE_

#ifdef MVGIGE_EXPORTS
	#define MVPROPERTY_API __declspec(dllexport)
#else
	#define MVPROPERTY_API __declspec(dllimport)
#endif

#ifndef _MSC_VER_	// Modified by Sny
//typedef char* LPCTSTR;	// Modified by Sny
#define CWnd void
#define SW_SHOW 5
#endif

#include "GigECamera_Types.h"

#define PAGE_NONE			0x0000
#define PAGE_ALL			0xffff
#define PAGE_ACQUISITION	0x0001
#define PAGE_WHITE_BALANCE	0x0002
#define PAGE_TRANS_LAYER	0x0004
#define PAGE_TRIGGER		0x0008
#define PAGE_CAMERA_INFO	0x0010
#define PAGE_IMAGE_FORMAT	0x0020
#define PAGE_AUTOGE_CONTROL	0x0040
#define PAGE_USERSET		0x0080

typedef enum
{
	MVCameraRun_ON,
	MVCameraRun_OFF
}MVCameraRunEnums;

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus
	/*! 
	 *  \brief �����������ҳ
	 *  \param [out] phProptySheet	�����������ҳ���
	 *  \param [in] hCam	������
	 *  \param [in] pParentWnd	�����ھ��,����CWnd*
	 *  \param [in] lpszTitle	�������ҳ����������
	 *  \param [in] nStyle	�������ҳ���
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetInit( HANDLE* phProptySheet, HANDLE hCam, CWnd* pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL );
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetCreate( HANDLE* phProptySheet, HANDLE hCam, CWnd* pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL );
	/*! 
	 *  \brief �����������ҳ
	 *  \param [out] phProptySheet	�����������ҳ���
	 *  \param [in] hCam	������
	 *  \param [in] pParentWnd	�����ھ��,����HWND
	 *  \param [in] lpszTitle	�������ҳ����������
	 *  \param [in] nStyle	�������ҳ���
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetInitEx(HANDLE* phProptySheet, HANDLE hCam, HWND pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL);
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetCreateEx(HANDLE* phProptySheet, HANDLE hCam, HWND pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL);

	/*! 
	 *  \brief �����������ҳ
	 *  \param [in] hProptySheet	�������ҳ���
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetDestroy(HANDLE hProptySheet);

	/*! 
	 *  \brief �����������ҳ����������
	 *  \param [in] hProptySheet	�������ҳ���
	 *  \param [in] lpszTitle	�������ҳ����������
	 *  \param [in] nStyle	 ָ�����Ա�������ʽ�� ����ָ������ʽ��0����Ϊ PSH_PROPTITLE�� �������ʽ����Ϊ PSH_PROPTITLE�����ʡ����ԡ���Ϊ����ָ�����ı�֮�� ���磬���� SetTitle(���򵥡��� PSH_PROPTITLE)�ᵼ�¡������Ե����Ա���������

	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetSetTitle(HANDLE hProptySheet, LPCTSTR lpszTitle = NULL);

	/*! 
	 *  \brief �����������ҳ��Ӧ�������
	 *  \param [in] hProptySheet	�������ҳ���
	 *  \param [in] hCam	������
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetSetCamera(HANDLE hProptySheet, HANDLE hCam);

	/*! 
	 *  \brief ��ȡ�������ҳ��ǰ��Ӧ�������
	 *  \param [in] hProptySheet	�������ҳ���
	 *  \param [in] phCam	������
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetGetCamera(HANDLE hProptySheet, HANDLE *phCam);

	/*! 
	 *  \brief ������������Ƿ��������ڲɼ�ģʽ��
	 *  \param [in] hProptySheet	�������ҳ���
	 *  \param [in] Run	�������������ڲɼ�ģʽ�£�����ΪTRUE,��������ΪFALSE
	 *  \note ����������ڲɼ�ģʽ������ҳ�н�����һЩ�ɼ�״̬�²�����ı��������ԣ���ͼ���С�ȡ�
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetCameraRun( HANDLE hProptySheet, MVCameraRunEnums Run );

	/*! 
	 *  \brief	�Է�ģʽ��ʽ��ʾ��ر��������ҳ
	 *  \param [in] hProptySheet	�������ҳ���
	 *  \param [in] nCmdShow	SW_SHOW:��ʾ�� SW_HIDE:�ر�
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetShow(HANDLE hProptySheet, int nCmdShow = SW_SHOW);

	/*! 
	 *  \brief	��ģʽ��ʽ��ʾ�������ҳ
	 *  \param [in] hProptySheet	�������ҳ���
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetDoModal(HANDLE hProptySheet);
	/*! 
	 *  \brief ��������ҳ�ĸ�����
	 *  \param [out] hProptySheet	�������ҳ���
	 *  \param [in] pParentWnd	�����ھ��,����HWND
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetSetParent(HANDLE hProptySheet, HANDLE hParent);

	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetInsertPage(HANDLE hProptySheet, WORD nPageInsert);

	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetDeletePage(HANDLE hProptySheet, WORD nPageDelete);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//_MVPROPERTY_H_INCLUDE_
