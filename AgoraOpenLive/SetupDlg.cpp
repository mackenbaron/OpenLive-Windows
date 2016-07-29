// SetupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AgoraOpenLive.h"
#include "SetupDlg.h"
#include "afxdialogex.h"


// CSetupDlg 对话框

IMPLEMENT_DYNAMIC(CSetupDlg, CDialogEx)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupDlg::IDD, pParent)
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNCONFIRM_SETUP, m_btnConfirm);
	DDX_Control(pDX, IDC_CKSAVESETTING_SETUP, m_ckSaveSettings);
    DDX_Control(pDX, IDC_CMBVDOPRF_SETUP, m_cbxVideoProfile);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNCONFIRM_SETUP, &CSetupDlg::OnBnClickedBtnconfirmSetup)
//	ON_CBN_SELCHANGE(IDC_CMBRES_SETUP, &CSetupDlg::OnCbnSelchangeCmbresSetup)
//	ON_CBN_SELCHANGE(IDC_CMBCODEC_SETUP, &CSetupDlg::OnCbnSelchangeCmbresSetup)

	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()

END_MESSAGE_MAP()


// CSetupDlg 消息处理程序


BOOL CSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ftHead.CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftDes.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftBtn.CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

//	m_cbxVideoProfile.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, CRect(0, 0, 300, 40), this, IDC_CMBVDOPRF_SETUP);
	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF), TRUE);
    InitData();
    InitCtrls();

	if (m_agConfig.IsAutoSaveEnabled())
		m_ckSaveSettings.SetCheck(TRUE);
	else
		m_ckSaveSettings.SetCheck(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CSetupDlg::InitCtrls()
{
	CRect ClientRect;

	MoveWindow(0, 0, 320, 450, 1);
	GetClientRect(&ClientRect);

	int nResolutionIndex = 0;
	CString str;
	
    m_cbxVideoProfile.MoveWindow(10, 150, 300, 300, TRUE);
	m_ckSaveSettings.MoveWindow(10, ClientRect.Height() - 95, 20, 20, TRUE);

	m_btnConfirm.MoveWindow(10, ClientRect.Height() - 58, 300, 48, TRUE);

    for (int nIndex = 0; nIndex < 34; nIndex++) {
        m_cbxVideoProfile.InsertString(nIndex, m_szProfileDes[nIndex]);
        m_cbxVideoProfile.SetItemHeight(nIndex, 25);
    }

	m_btnConfirm.SetBackColor(RGB(0, 160, 239), RGB(0, 160, 239), RGB(0, 160, 239), RGB(192, 192, 192));
	m_btnConfirm.SetFont(&m_ftBtn);
	m_btnConfirm.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xC8, 0x64), RGB(0xFF, 0xC8, 0x64), RGB(0xCC, 0xCC, 0xCC));
	m_btnConfirm.SetWindowText(LANG_STR("IDS_SET_BTCONFIRM"));

    if (m_agConfig.IsAutoSaveEnabled())
        nResolutionIndex = m_agConfig.GetSolution();
    else
        nResolutionIndex = 0;
    
    m_cbxVideoProfile.SetCurSel(nResolutionIndex);
}

void CSetupDlg::InitData()
{
    m_szProfileDes[0] = _T("160x120 15fps");
    m_szProfileDes[1] = _T("160x160 15fps");
    m_szProfileDes[2] = _T("120x120 15fps");
    m_szProfileDes[3] = _T("320x180 15fps");
    m_szProfileDes[4] = _T("180x320 15fps");
    m_szProfileDes[5] = _T("180x180 15fps");
    m_szProfileDes[6] = _T("320x240 15fps");
    m_szProfileDes[7] = _T("240x320 15fps");
    m_szProfileDes[8] = _T("240x240 15fps");
    m_szProfileDes[9] = _T("640x360 15fps");
    m_szProfileDes[10] = _T("360x640 15fps");
    m_szProfileDes[11] = _T("360x360 15fps");
    m_szProfileDes[12] = _T("640x360 15fps");
    m_szProfileDes[13] = _T("360x640 30fps");
    m_szProfileDes[14] = _T("360x360 30fps");
    m_szProfileDes[15] = _T("640x480 15fps");
    m_szProfileDes[16] = _T("480x640 15fps");
    m_szProfileDes[17] = _T("480x480 15fps");
    m_szProfileDes[18] = _T("640x480 30fps");
    m_szProfileDes[19] = _T("480x640 30fps");
    m_szProfileDes[20] = _T("480x480 30fps");
    m_szProfileDes[21] = _T("640x480 15fps");
    m_szProfileDes[22] = _T("1280x720 15fps");
    m_szProfileDes[23] = _T("720x1280 15fps");
    m_szProfileDes[24] = _T("1280x720 30fps");
    m_szProfileDes[25] = _T("720x1280 30fps");
    m_szProfileDes[26] = _T("1920x1080 15fps");
    m_szProfileDes[27] = _T("1080x1920 15fps");
    m_szProfileDes[28] = _T("480x640 30fps");
    m_szProfileDes[29] = _T("480x480 30fps");
    m_szProfileDes[30] = _T("640x480 15fps");
    m_szProfileDes[31] = _T("1280x720 15fps");
    m_szProfileDes[32] = _T("720x1280 15fps");
    m_szProfileDes[33] = _T("1280x720 30fps");
    m_szProfileDes[34] = _T("720x1280 30fps");

}

void CSetupDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;

	GetClientRect(&rcClient);

	CFont* defFont = lpDC->SelectObject(&m_ftHead);
	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpDC->SetTextColor(RGB(0x00, 0x9E, 0xEB));

	lpString = LANG_STR("IDS_SET_RESOLUTION");
	lpDC->TextOut(12, 115, lpString, _tcslen(lpString));
//	lpString = LANG_STR("IDS_SET_MFR");
//	lpDC->TextOut(12, 205, lpString, _tcslen(lpString));
//	lpString = LANG_STR("IDS_SET_MR");
//	lpDC->TextOut(12, 295, lpString, _tcslen(lpString));

	lpString = LANG_STR("IDS_SET_SAVESETTING");
	lpDC->TextOut(30, rcClient.Height() - 95, lpString, _tcslen(lpString));

	lpDC->SelectObject(defFont);
}

void CSetupDlg::OnBnClickedBtnconfirmSetup()
{
	// TODO:  在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_GOBACK, 0, 0);

	if (m_ckSaveSettings.GetCheck() == TRUE) {
		m_agConfig.EnableAutoSave(TRUE);
        m_agConfig.SetSolution(m_cbxVideoProfile.GetCurSel());
	}
	else
		m_agConfig.EnableAutoSave(FALSE);
}

void CSetupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawClient(&dc);
}

int CSetupDlg::GetVideoSolution()
{
    return m_cbxVideoProfile.GetCurSel();
}

void CSetupDlg::SetVideoSolution(int nIndex)
{
	m_cbxVideoProfile.SetCurSel(nIndex);
}

CString CSetupDlg::GetVideoSolutionDes()
{
    int nIndex = m_cbxVideoProfile.GetCurSel();
    if (nIndex = -1)
        nIndex = 0;

    return (CString)m_szProfileDes[nIndex];
}