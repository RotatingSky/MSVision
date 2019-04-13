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
	 *  \brief ����¼����ƶԻ���
	 *  \param [out]	pRecDlg	�Ի�����
	 *  \param [in]	hCam	������
	 *  \param [in]	pParentWnd	������ָ��
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgCreate( HANDLE* pRecDlg, HANDLE hCam, CWnd* pParentWnd = NULL );

	/*!
	 *  \brief ����¼����ƶԻ���
	 *  \param [out]	pRecDlg	�Ի�����
	 *  \param [in]	hCam	������
	 *  \param [in]	pParentWnd	������ָ��
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgCreateEx( HANDLE* pRecDlg, HANDLE hCam, HWND pParentWnd = NULL );

	/*!
	 *  \brief ���ٺ�ɾ��¼����ƶԻ���
	 *  \param [in]	hRecDlg	�Ի�����
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgDestroy( HANDLE hRecDlg );
	
	/*!
	 *  \brief	��ʾ������¼����ƶԻ���
	 *  \param [in]	hRecDlg	�Ի�����
	 *  \param [in]	nCmdShow SW_SHOW,SW_HIDE
	 *  \retval  	
	 */
	MVRECORDDLG_API BOOL __stdcall MVRecordDlgShow( HANDLE hRecDlg, int nCmdShow );

	/*!
	 *  \brief ֪ͨ�Ի��������ǰ����״̬
	 *  \param [in]	hRecDlg	�Ի�����
	 *  \param [in]	bCamRun �����ǰ�Ƿ��ڲɼ�״̬
	 *  \retval  	
	 */
	MVRECORDDLG_API void __stdcall MVRecordDlgCamRun(HANDLE hRecDlg, BOOL bCamRun );

	/*!
	 *  \brief ��¼����׷��һ֡ͼ��
	 *  \param [in]	hRecDlg	�Ի�����
	 *  \param [in]	hImage ͼ����
	 *  \param [in]	nBlockId ͼ�����
	 *  \retval  	
	 */
	MVRECORDDLG_API int __stdcall  MVRecordDlgRecord( HANDLE hRecDlg,HANDLE hImage, unsigned short nBlockId );
	
	/*!
	 *  \brief ���öԻ���ĸ�����
	 *  \param [in]	hRecDlg	�Ի�����
	 *  \param [in]	hParent	�����ھ��
	 *  \retval  	
	 */
	MVRECORDDLG_API int __stdcall  MVRecordDlgSetParent( HANDLE hRecDlg, HANDLE hParent );
#ifdef __cplusplus
}
#endif
#endif