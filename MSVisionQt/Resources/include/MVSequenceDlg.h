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
 *  \brief ��������֡���ƶԻ���
 *  \param [out]	pSeqDlg ����֡���ƶԻ�����ָ��
 *  \param [in]	pParentWnd ������ָ��
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgCreate( HANDLE* pSeqDlg, CWnd* pParentWnd = NULL );

/*!
 *  \brief ��������֡���ƶԻ���
 *  \param [out]	pSeqDlg ����֡���ƶԻ�����ָ��
 *  \param [in]	pParentWnd ������ָ��
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgCreateEx( HANDLE* pSeqDlg, HWND pParentWnd = NULL );

/*!
 *  \brief ��������֡���ƶԻ���
 *  \param [in]	hSeqDlg ����֡���ƶԻ�����
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgDestroy( HANDLE hSeqDlg );

/*!
 *  \brief	��ʾ������¼����ƶԻ���
 *  \param [in]	hSeqDlg	�Ի�����
 *  \param [in]	nCmdShow SW_SHOW,SW_HIDE
 *  \retval  	
 */
MVSEQUENCEDLG_API BOOL __stdcall MVSequenceDlgShow( HANDLE hSeqDlg, int nCmdShow );

/*!
 *  \brief ֪ͨ�Ի��������ǰ����״̬
 *  \param [in]	hSeqDlg �Ի�����
 *  \param [in]	bCamRun �����ǰ�Ƿ��ڲɼ�״̬
 *  \retval  	
 */
MVSEQUENCEDLG_API void __stdcall MVSequenceDlgCamRun( HANDLE hSeqDlg, BOOL bCamRun );

/*!
 *  \brief ��ȡ��ǰ������ļ���
 *  \param [in]	hSeqDlg �Ի�����
 *  \param [out]	fname  �ļ���������
 *  \param [in]	szBuf �ļ�����������С
 *  \retval  	
 */
MVSEQUENCEDLG_API int __stdcall  MVSequenceDlgGetFileName( HANDLE hSeqDlg, char *fname, int szBuf );

/*!
 *  \brief ���öԻ���ĸ�����
 *  \param [in]	hSeqDlg �Ի�����
 *  \param [in]	hParent �����ھ��
 *  \retval  	
 */
MVSEQUENCEDLG_API int __stdcall  MVSequenceDlgSetParent( HANDLE hSeqDlg, HANDLE hParent  );

#ifdef __cplusplus
}
#endif

#endif	//_MVSEQUENCEDLG_H_INCLUDE_