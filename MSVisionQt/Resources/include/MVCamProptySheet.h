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
	 *  \brief 创建相机属性页
	 *  \param [out] phProptySheet	返回相机属性页句柄
	 *  \param [in] hCam	相机句柄
	 *  \param [in] pParentWnd	父窗口句柄,类型CWnd*
	 *  \param [in] lpszTitle	相机属性页标题栏文字
	 *  \param [in] nStyle	相机属性页风格
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetInit( HANDLE* phProptySheet, HANDLE hCam, CWnd* pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL );
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetCreate( HANDLE* phProptySheet, HANDLE hCam, CWnd* pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL );
	/*! 
	 *  \brief 创建相机属性页
	 *  \param [out] phProptySheet	返回相机属性页句柄
	 *  \param [in] hCam	相机句柄
	 *  \param [in] pParentWnd	父窗口句柄,类型HWND
	 *  \param [in] lpszTitle	相机属性页标题栏文字
	 *  \param [in] nStyle	相机属性页风格
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetInitEx(HANDLE* phProptySheet, HANDLE hCam, HWND pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL);
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetCreateEx(HANDLE* phProptySheet, HANDLE hCam, HWND pParentWnd = NULL, LPCTSTR lpszTitle = NULL, WORD nPageDisplay = PAGE_ALL);

	/*! 
	 *  \brief 销毁相机属性页
	 *  \param [in] hProptySheet	相机属性页句柄
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetDestroy(HANDLE hProptySheet);

	/*! 
	 *  \brief 设置相机属性页标题栏文字
	 *  \param [in] hProptySheet	相机属性页句柄
	 *  \param [in] lpszTitle	相机属性页标题栏文字
	 *  \param [in] nStyle	 指定属性表标题的样式。 必须指定该样式在0或作为 PSH_PROPTITLE。 如果该样式设置为 PSH_PROPTITLE，单词“属性”中为声明指定的文本之后。 例如，调用 SetTitle(“简单”， PSH_PROPTITLE)会导致“简单属性的属性表声明”。

	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetSetTitle(HANDLE hProptySheet, LPCTSTR lpszTitle = NULL);

	/*! 
	 *  \brief 设置相机属性页对应的相机。
	 *  \param [in] hProptySheet	相机属性页句柄
	 *  \param [in] hCam	相机句柄
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetSetCamera(HANDLE hProptySheet, HANDLE hCam);

	/*! 
	 *  \brief 获取相机属性页当前对应的相机。
	 *  \param [in] hProptySheet	相机属性页句柄
	 *  \param [in] phCam	相机句柄
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetGetCamera(HANDLE hProptySheet, HANDLE *phCam);

	/*! 
	 *  \brief 设置相机现在是否正工作在采集模式。
	 *  \param [in] hProptySheet	相机属性页句柄
	 *  \param [in] Run	如果相机正工作在采集模式下，设置为TRUE,否则设置为FALSE
	 *  \note 如果正工作在采集模式，属性页中将禁用一些采集状态下不允许改变的相机属性，如图像大小等。
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetCameraRun( HANDLE hProptySheet, MVCameraRunEnums Run );

	/*! 
	 *  \brief	以非模式框方式显示或关闭相机属性页
	 *  \param [in] hProptySheet	相机属性页句柄
	 *  \param [in] nCmdShow	SW_SHOW:显示， SW_HIDE:关闭
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetShow(HANDLE hProptySheet, int nCmdShow = SW_SHOW);

	/*! 
	 *  \brief	以模式框方式显示相机属性页
	 *  \param [in] hProptySheet	相机属性页句柄
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetDoModal(HANDLE hProptySheet);
	/*! 
	 *  \brief 设置属性页的父窗口
	 *  \param [out] hProptySheet	相机属性页句柄
	 *  \param [in] pParentWnd	父窗口句柄,类型HWND
	 *  \retval 
	 */
	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetSetParent(HANDLE hProptySheet, HANDLE hParent);

	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetInsertPage(HANDLE hProptySheet, WORD nPageInsert);

	MVPROPERTY_API MVSTATUS_CODES __stdcall MVCamProptySheetDeletePage(HANDLE hProptySheet, WORD nPageDelete);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//_MVPROPERTY_H_INCLUDE_
