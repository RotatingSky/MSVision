#ifndef _MVRECORDDLG_H_INCLUDE_
#define _MVRECORDDLG_H_INCLUDE_
#ifdef MVRECORDDLG_EXPORTS
#define MVRECORDDLG_API __declspec(dllexport)
#else
#define MVRECORDDLG_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 *  \brief 创建录像控制对话框
	 *  \param [out]	pRecDlg	对话框句柄
	 *  \param [in]	hCam	相机句柄
	 *  \param [in]	pParentWnd	父窗口指针
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgCreate( HANDLE* pRecDlg, HANDLE hCam, CWnd* pParentWnd = NULL );

	/*!
	 *  \brief 创建录像控制对话框
	 *  \param [out]	pRecDlg	对话框句柄
	 *  \param [in]	hCam	相机句柄
	 *  \param [in]	pParentWnd	父窗口指针
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgCreateEx( HANDLE* pRecDlg, HANDLE hCam, HWND pParentWnd = NULL );

	/*!
	 *  \brief 销毁和删除录像控制对话框
	 *  \param [in]	hRecDlg	对话框句柄
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgDestroy( HANDLE hRecDlg );
	
	/*!
	 *  \brief	显示或隐藏录像控制对话框
	 *  \param [in]	hRecDlg	对话框句柄
	 *  \param [in]	nCmdShow SW_SHOW,SW_HIDE
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgShow( HANDLE hRecDlg, int nCmdShow );

	/*!
	 *  \brief 通知对话框相机当前运行状态
	 *  \param [in]	hRecDlg	对话框句柄
	 *  \param [in]	bCamRun 相机当前是否在采集状态
	 *  \retval  	
	 */
	MVRECORDDLG_API void __stdcall MVRecordDlgCamRun(HANDLE hRecDlg, BOOL bCamRun );

	/*!
	 *  \brief 向录像中追加一帧图像
	 *  \param [in]	hRecDlg	对话框句柄
	 *  \param [in]	hImage 图像句柄
	 *  \param [in]	nBlockId 图像序号
	 *  \retval  	
	 */
	MVRECORDDLG_API int __stdcall  MVRecordDlgRecord( HANDLE hRecDlg,HANDLE hImage, unsigned short nBlockId );
	
	/*!
	 *  \brief 设置对话框的父窗口
	 *  \param [in]	hRecDlg	对话框句柄
	 *  \param [in]	hParent	父窗口句柄
	 *  \retval  	
	 */
	MVRECORDDLG_API int __stdcall  MVRecordDlgSetParent( HANDLE hRecDlg, HANDLE hParent );
#ifdef __cplusplus
}
#endif
#endif