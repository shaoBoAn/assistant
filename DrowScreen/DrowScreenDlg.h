
// DrowScreenDlg.h : ͷ�ļ�
//

#pragma once

#define WM_MYMSGKEY WM_USER + 306
// CDrowScreenDlg �Ի���
class CDrowScreenDlg : public CDialogEx
{
private:
	bool m_bButtonStat;
	bool m_bLoadDll;
	HINSTANCE g_hInstDll;
// ����
public:
	CDrowScreenDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DROWSCREEN_DIALOG };

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
	afx_msg void OnBnClickedStartStop();
	LRESULT OnMyKeyMsg(WPARAM wParam,LPARAM lParam);
public:
	bool StartKeyHook();
	bool StartMouseHook();
	bool StopKeyHook();
	bool StopMouseHook();
	bool LoadDll();
	void DelDLL();
	afx_msg void OnBnClickedLoaddll();
	afx_msg void OnBnClickedOk();
};
