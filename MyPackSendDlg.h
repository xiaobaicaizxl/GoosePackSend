// MyPackSendDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMyPackSendDlg �Ի���
class CMyPackSendDlg : public CDialog
{
// ����
public:
	CMyPackSendDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYPACKSEND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
	CListBox CNetcardList;
	CString csPathName;
	afx_msg void OnBnClickedButtonFileopen();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonFileopenhex();
};
#define OPENED_FILE_TYPE_BIN 0x01
#define OPENED_FILE_TYPE_HEX 0x02
