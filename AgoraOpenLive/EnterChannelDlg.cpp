// EnterChannelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AgoraOpenLive.h"
#include "EnterChannelDlg.h"


#include "afxdialogex.h"


// CEnterChannelDlg 对话框

IMPLEMENT_DYNAMIC(CEnterChannelDlg, CDialogEx)

CEnterChannelDlg::CEnterChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnterChannelDlg::IDD, pParent)
{

}

CEnterChannelDlg::~CEnterChannelDlg()
{
}

void CEnterChannelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDCHNAME_CHANNEL, m_ctrChannel);
    DDX_Control(pDX, IDC_BTNTEST_CHANNEL, m_btnTest);
    DDX_Control(pDX, IDC_BTNJOIN_CHANNEL, m_btnJoin);
    DDX_Control(pDX, IDC_BTNSET_CHANNEL, m_btnSetup);
    DDX_Control(pDX, IDC_CMBROLE_CHANNEL, m_ctrRole);
    DDX_Control(pDX, IDC_CKENABLEDUL_CHANNEL, m_ckEnableDualStream);
}


BEGIN_MESSAGE_MAP(CEnterChannelDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNTEST_CHANNEL, &CEnterChannelDlg::OnBnClickedBtntestChannel)
	ON_BN_CLICKED(IDC_BTNJOIN_CHANNEL, &CEnterChannelDlg::OnBnClickedBtnjoinChannel)
	ON_BN_CLICKED(IDC_BTNSET_CHANNEL, &CEnterChannelDlg::OnBnClickedBtnsetChannel)
	ON_CBN_SELCHANGE(IDC_CMBROLE_CHANNEL, &CEnterChannelDlg::OnCbnSelchangeCmbRole)

END_MESSAGE_MAP()


// CEnterChannelDlg 消息处理程序
BOOL CEnterChannelDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_ESCAPE:
			return FALSE;
		case VK_RETURN:
			OnBnClickedBtnjoinChannel();
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CEnterChannelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ftHead.CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftDesc.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftBtn.CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_dlgDevice.Create(CDeviceDlg::IDD, this);
	m_dlgDevice.EnableDeviceTest(TRUE);

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CEnterChannelDlg::InitCtrls()
{
	CRect ClientRect;

	MoveWindow(0, 0, 320, 450, 1);
	GetClientRect(&ClientRect);

	m_ctrChannel.MoveWindow(ClientRect.Width()/2-150, ClientRect.Height() - 270, 300, 36, TRUE);
	m_ctrChannel.SetFont(&m_ftHead);
	m_ctrChannel.SetCaretPos(CPoint(12, 148));
	m_ctrChannel.ShowCaret();
	m_ctrChannel.SetBalloonTip(LANG_STR("IDS_CHN_CHTIP1"), LANG_STR("IDS_CHN_CHTIP2"), TTI_INFO);

	m_ctrRole.MoveWindow(ClientRect.Width() / 2 - 150, ClientRect.Height() - 200, 300, 48, TRUE);
	m_ctrRole.SetFont(&m_ftDesc);
	for (int nIndex = 0; nIndex < 2; nIndex++) {
		CStringA str;

		str.Format("IDS_CHN_ROLE%d", nIndex);
		m_ctrRole.InsertString(nIndex, LANG_STR(str));
	}
	m_ctrRole.SetCurSel(0);
	m_ckEnableDualStream.MoveWindow(ClientRect.Width() / 2 - 150, ClientRect.Height() - 165, 16, 16, TRUE);
	m_ckEnableDualStream.SetCheck(TRUE);

	m_btnTest.MoveWindow(ClientRect.Width()/2-150, ClientRect.Height() - 130, 144, 40, TRUE);
	m_btnJoin.MoveWindow(ClientRect.Width()/2+6, ClientRect.Height()-130, 144, 40, TRUE);
	m_btnSetup.MoveWindow(ClientRect.Width()/2-150, ClientRect.Height()-58, 300, 40, TRUE);

	m_btnTest.SetBackColor(RGB(0, 160, 239), RGB(0, 160, 239), RGB(0, 160, 239), RGB(192, 192, 192));
	m_btnTest.SetFont(&m_ftBtn);
	m_btnTest.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xC8, 0x64), RGB(0xFF, 0xC8, 0x64), RGB(0xCC, 0xCC, 0xCC));
	m_btnTest.SetWindowText(LANG_STR("IDS_CHN_BTTEST"));

	m_btnJoin.SetBackColor(RGB(248, 170, 31), RGB(248, 170, 31), RGB(248, 170, 31), RGB(0xCC, 0xCC, 0xCC));
	m_btnJoin.SetFont(&m_ftBtn);
	m_btnJoin.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0, 160, 239), RGB(0, 160, 239), RGB(0xCC, 0xCC, 0xCC));
	m_btnJoin.SetWindowText(LANG_STR("IDS_CHN_BTJOIN"));

	m_btnSetup.SetBackColor(RGB(0, 160, 239), RGB(0, 160, 239), RGB(0, 160, 239), RGB(0xCC, 0xCC, 0xCC));
	m_btnSetup.SetFont(&m_ftBtn);
	m_btnSetup.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xC8, 0x64), RGB(0xFF, 0xC8, 0x64), RGB(0xCC, 0xCC, 0xCC));
	m_btnSetup.SetWindowText(_T("1920*1080,15fps, 3mbps"));

	CMFCButton::EnableWindowsTheming(FALSE);
}

void CEnterChannelDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawClient(&dc);
}


void CEnterChannelDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;

	GetClientRect(&rcClient);

	CFont* defFont = lpDC->SelectObject(&m_ftHead);
	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpDC->SetTextColor(RGB(0x00, 0x9E, 0xEB));
	lpString = LANG_STR("IDS_CHN_TITLE1");
	lpDC->TextOut(12, 30, lpString, _tcslen(lpString));
	lpString = LANG_STR("IDS_CHN_TITLE2");
	lpDC->TextOut(12, 55, lpString, _tcslen(lpString));

	lpDC->SelectObject(&m_ftDesc);
	lpString = LANG_STR("IDS_CHN_ROLETITLE");
	lpDC->TextOut(12, 230, lpString);

	lpString = LANG_STR("IDS_CHN_CKENDAUL");
	lpDC->TextOut(28, 285, lpString);

	lpDC->SelectObject(&m_ftDesc);
	lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	lpString = LANG_STR("IDS_CHN_DSC1");
	lpDC->TextOut(12, 94, lpString, _tcslen(lpString));
	lpString = LANG_STR("IDS_CHN_DSC2");
	lpDC->TextOut(12, 112, lpString, _tcslen(lpString));
	lpString = LANG_STR("IDS_CHN_DSC3");
	lpDC->TextOut(12, 130, lpString, _tcslen(lpString));
	lpString = LANG_STR("IDS_CHN_DSC4");
	lpDC->TextOut(12, 148, lpString, _tcslen(lpString));

	lpDC->SelectObject(defFont);

	// Done with the font.  Delete the font object.
	//	font.DeleteObject();
}

void CEnterChannelDlg::OnBnClickedBtntestChannel()
{
	// TODO:  在此添加控件通知处理程序代码
	m_dlgDevice.ShowWindow(SW_SHOW);
	m_dlgDevice.CenterWindow();
}


void CEnterChannelDlg::OnBnClickedBtnjoinChannel()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strKey;

	GetParent()->SendMessage(WM_JOINCHANNEL, 0, 0);
}


void CEnterChannelDlg::OnBnClickedBtnsetChannel()
{
	// TODO:  在此添加控件通知处理程序代码

	GetParent()->SendMessage(WM_GONEXT, 0, 0);
}

void CEnterChannelDlg::OnCbnSelchangeCmbRole()
{
	int nSel = m_ctrRole.GetCurSel();
	CAgoraObject::GetAgoraObject()->SetClientRole(nSel);
}

CString CEnterChannelDlg::GetChannelName()
{
	CString strChannelName;

	m_ctrChannel.GetWindowText(strChannelName);

	return strChannelName;
}

void CEnterChannelDlg::SetVideoString(LPCTSTR lpVideoString)
{
	m_btnSetup.SetWindowText(lpVideoString);
}

void CEnterChannelDlg::SetDauleStream(BOOL bEnable)
{
	m_ckEnableDualStream.SetCheck(bEnable);
}

BOOL CEnterChannelDlg::IsDauleStream()
{
	return (BOOL)m_ckEnableDualStream.GetCheck();
}