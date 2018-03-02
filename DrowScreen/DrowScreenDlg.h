
// DrowScreenDlg.h : 头文件
//

#pragma once

#define WM_MYMSGKEY WM_USER + 306
// CDrowScreenDlg 对话框
class CDrowScreenDlg : public CDialogEx
{
private:
	bool m_bButtonStat;
	bool m_bLoadDll;
	HINSTANCE g_hInstDll;
// 构造
public:
	CDrowScreenDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DROWSCREEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
