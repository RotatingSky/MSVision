#ifndef _MVSEQUENCEDLG_H_INCLUDE_

#define _MVSEQUENCEDLG_H_INCLUDE_

#ifdef MVSEQUENCEDLG_EXPORTS
#define MVSEQUENCEDLG_API __declspec(dllexport)
#else
#define MVSEQUENCEDLG_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \brief 创建序列帧控制对话框
 *  \param [out]	pSeqDlg 序列帧控制对话框句柄指针
 *  \param [in]	pParentWnd 父窗口指针
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgCreate( HANDLE* pSeqDlg, CWnd* pParentWnd = NULL );

/*!
 *  \brief 创建序列帧控制对话框
 *  \param [out]	pSeqDlg 序列帧控制对话框句柄指针
 *  \param [in]	pParentWnd 父窗口指针
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgCreateEx( HANDLE* pSeqDlg, HWND pParentWnd = NULL );

/*!
 *  \brief 销毁序列帧控制对话框
 *  \param [in]	hSeqDlg 序列帧控制对话框句柄
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgDestroy( HANDLE hSeqDlg );

/*!
 *  \brief	显示或隐藏录像控制对话框
 *  \param [in]	hSeqDlg	对话框句柄
 *  \param [in]	nCmdShow SW_SHOW,SW_HIDE
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgShow( HANDLE hSeqDlg, int nCmdShow );

/*!
 *  \brief 通知对话框相机当前运行状态
 *  \param [in]	hSeqDlg 对话框句柄
 *  \param [in]	bCamRun 相机当前是否在采集状态
 *  \retval  	
 */
MVSEQUENCEDLG_API void __stdcall MVSequenceDlgCamRun( HANDLE hSeqDlg, BOOL bCamRun );

/*!
 *  \brief 获取当前保存的文件名
 *  \param [in]	hSeqDlg 对话框句柄
 *  \param [out]	fname  文件名缓冲区
 *  \param [in]	szBuf 文件名缓冲区大小
 *  \retval  	
 */
MVSEQUENCEDLG_API int __stdcall  MVSequenceDlgGetFileName( HANDLE hSeqDlg, char *fname, int szBuf );

/*!
 *  \brief 设置对话框的父窗口
 *  \param [in]	hSeqDlg 对话框句柄
 *  \param [in]	hParent 父窗口句柄
 *  \retval  	
 */
MVSEQUENCEDLG_API int __stdcall  MVSequenceDlgSetParent( HANDLE hSeqDlg, HANDLE hParent  );

#ifdef __cplusplus
}
#endif

#endif	//_MVSEQUENCEDLG_H_INCLUDE_