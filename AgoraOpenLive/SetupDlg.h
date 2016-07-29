#pragma once
#include "AGButton.h"
#include "AGSliderCtrl.h"
#include "afxwin.h"

#include "AGConfig.h"
// CSetupDlg 对话框

class CSetupDlg : public CDialogEx
{

	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetupDlg();

    void SetVideoSolution(int nIndex);
    int GetVideoSolution();
    CString GetVideoSolutionDes();

// 对话框数据
	enum { IDD = IDD_SETUP_DIALOG };

	typedef struct _AGVIDEO_PARAM
	{
		int nWidth;
		int nHeight;
		int nMinKPB;
		int nMaxKPB;

	} AGNET_RATE_RANGE;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnconfirmSetup();

	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void InitData();
	void DrawClient(CDC *lpDC);

private:
	CAGButton		m_btnConfirm;
	CButton			m_ckSaveSettings;

	CComboBox		m_cbxVideoProfile;

	CFont			m_ftHead;		// title
	CFont			m_ftDes;		// text in ctrl
	CFont			m_ftBtn;		// button

	LPTSTR			    m_szProfileDes[64];
	AGNET_RATE_RANGE	m_agRateRange[13];
	
private:
	CAGConfig		m_agConfig;
};
