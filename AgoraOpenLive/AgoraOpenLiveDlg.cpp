
// AgoraVideoCallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AgoraOpenLive.h"
#include "AgoraOpenLiveDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAgoraVideoCallDlg dialog



CAgoraOpenLiveDlg::CAgoraOpenLiveDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CAgoraOpenLiveDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_lpAgoraObject = NULL;
	m_lpRtcEngine = NULL;

	m_nNetworkQuality = 0;
}

void CAgoraOpenLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNMIN, m_btnMin);
	DDX_Control(pDX, IDC_BTNCLOSE, m_btnClose);

	DDX_Control(pDX, IDC_LINKAGORA, m_linkAgora);
}

BEGIN_MESSAGE_MAP(CAgoraOpenLiveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
    ON_MESSAGE(WM_GOBACK, &CAgoraOpenLiveDlg::OnBackPage)
    ON_MESSAGE(WM_GONEXT, &CAgoraOpenLiveDlg::OnNextPage)
    ON_MESSAGE(WM_JOINCHANNEL, &CAgoraOpenLiveDlg::OnJoinChannel)
    ON_MESSAGE(WM_LEAVECHANNEL, &CAgoraOpenLiveDlg::OnLeaveChannel)
	
    ON_BN_CLICKED(IDC_BTNMIN, &CAgoraOpenLiveDlg::OnBnClickedBtnmin)
    ON_BN_CLICKED(IDC_BTNCLOSE, &CAgoraOpenLiveDlg::OnBnClickedBtnclose)

    ON_MESSAGE(WM_MSGID(EID_NETWORK_QUALITY), &CAgoraOpenLiveDlg::OnNetworkQuality)

END_MESSAGE_MAP()


// CAgoraVideoCallDlg message handlers
BOOL CAgoraOpenLiveDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_ESCAPE:
		case VK_RETURN:
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CAgoraOpenLiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ftTitle.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftLink.CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftVer.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_lpAgoraObject = CAgoraObject::GetAgoraObject(VENDOR_KEY);
	m_lpRtcEngine = CAgoraObject::GetEngine();

    if (_tcslen(VENDOR_KEY) == 0) {
        MessageBox(_T("请在源码VENDOR_KEY宏定义中填上自己的KEY"), _T("提示"), MB_ICONINFORMATION);
        PostQuitMessage(0);
    }

	// m_lpRtcEngineEx->setClientRole(agora::rtc::CLIENT_ROLE_DUAL_STREAM_AUDIENCE);
	m_lpAgoraObject->SetLogFilePath(NULL);
	m_lpAgoraObject->SetMsgHandlerWnd(GetSafeHwnd());
	CAgoraObject::GetAgoraObject()->SetClientRole(0);

	SetBackgroundImage(IDB_DLG_MAIN);
	InitCtrls();
	InitChildDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAgoraOpenLiveDlg::InitCtrls()
{
	CRect ClientRect;
	CBitmap	bmpNetQuality;

	MoveWindow(0, 0, 600, 600, 1);
	GetClientRect(&ClientRect);

	bmpNetQuality.LoadBitmap(IDB_NETWORK_QUALITY);

	m_imgNetQuality.Create(32, 32, ILC_COLOR24 | ILC_MASK, 6, 1);
	m_imgNetQuality.Add(&bmpNetQuality, RGB(0xFF, 0, 0xFF));

	m_btnMin.MoveWindow(ClientRect.Width() - 46, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(ClientRect.Width() - 23, 1, 22, 22, TRUE);
	m_linkAgora.MoveWindow(ClientRect.Width() / 2 - 30, ClientRect.Height()-55, 80, 20, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0, 0xFF));

	m_linkAgora.SetFont(&m_ftLink);
	m_linkAgora.SetURL(_T("http://www.agora.io"));
	m_linkAgora.SetWindowText(LANG_STR("IDS_LOGO_AGORAWEB"));
	CMFCButton::EnableWindowsTheming(FALSE);
}

void CAgoraOpenLiveDlg::InitChildDialog()
{
	CString str;

	m_dlgEnterChannel.Create(CEnterChannelDlg::IDD, this);
	m_dlgSetup.Create(CSetupDlg::IDD, this);
	m_dlgVideo.Create(CVideoDlg::IDD, this);

	m_dlgEnterChannel.MoveWindow(140, 40, 320, 450, TRUE);
	m_dlgSetup.MoveWindow(140, 40, 320, 450, TRUE);

	m_dlgEnterChannel.ShowWindow(SW_SHOW);
	m_lpCurDialog = &m_dlgEnterChannel;

    m_dlgSetup.SetVideoSolution(15);
	m_dlgEnterChannel.SetVideoString(m_dlgSetup.GetVideoSolutionDes());
}

void CAgoraOpenLiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAgoraOpenLiveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawClient(&dc);
//		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAgoraOpenLiveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CAgoraOpenLiveDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
			lResult = HTCAPTION;
		
	return lResult;
}

void CAgoraOpenLiveDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;
	CFont* defFont = lpDC->SelectObject(&m_ftTitle);

	m_imgNetQuality.Draw(lpDC, m_nNetworkQuality, CPoint(16, 40), ILD_NORMAL);

	GetClientRect(&rcClient);
	lpDC->SetBkColor(RGB(0x00, 0x9E, 0xEB));
	lpDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = LANG_STR("IDS_TITLE");
	lpDC->TextOut(12, 3, lpString, _tcslen(lpString));
	
	lpDC->SelectObject(&m_ftVer);
	lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = AVC_VER;// _T("v1.3");
	
	rcText.SetRect(0, rcClient.Height() - 30, rcClient.Width(), rcClient.Height() - 5);
	lpDC->DrawText(lpString, _tcslen(lpString), &rcText, DT_CENTER | DT_SINGLELINE);
	lpDC->SelectObject(defFont);
}

void CAgoraOpenLiveDlg::OnBnClickedBtnmin()
{
	// TODO:  在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}


void CAgoraOpenLiveDlg::OnBnClickedBtnclose()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

LRESULT CAgoraOpenLiveDlg::OnBackPage(WPARAM wParam, LPARAM lParam)
{
	if (m_lpCurDialog == &m_dlgSetup) {
		m_lpCurDialog->ShowWindow(SW_HIDE);
		m_lpCurDialog = &m_dlgEnterChannel;
	}

    
    m_dlgEnterChannel.SetVideoString(m_dlgSetup.GetVideoSolutionDes());

	m_lpCurDialog->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CAgoraOpenLiveDlg::OnNextPage(WPARAM wParam, LPARAM lParam)
{
	m_lpCurDialog->ShowWindow(SW_HIDE);
	if (m_lpCurDialog == &m_dlgEnterChannel)
			m_lpCurDialog = &m_dlgSetup;

	m_lpCurDialog->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CAgoraOpenLiveDlg::OnJoinChannel(WPARAM wParam, LPARAM lParam)
{
	IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	CString strChannelName = m_dlgEnterChannel.GetChannelName();

	m_dlgVideo.MoveWindow(0, 0, 960, 720, 1);
	m_dlgVideo.ShowWindow(SW_SHOW);
	m_dlgVideo.CenterWindow();

	VideoCanvas vc;

	vc.uid = 0;
	vc.view = m_dlgVideo.GetLocalVideoWnd();
	vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;

    CAgoraObject::GetEngine()->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
	CAgoraObject::GetEngine()->setVideoProfile((VIDEO_PROFILE_TYPE)m_dlgSetup.GetVideoSolution(), m_dlgSetup.IsWHSwap());

	lpAgoraObject->EnableVideo(TRUE);
	CAgoraObject::GetAgoraObject()->EnableDauleStream(m_dlgEnterChannel.IsDauleStream());

	m_dlgVideo.SetWindowText(strChannelName);
	lpRtcEngine->setupLocalVideo(vc);
	lpRtcEngine->startPreview();

	lpAgoraObject->JoinChannel(strChannelName);
	lpAgoraObject->SetMsgHandlerWnd(m_dlgVideo.GetSafeHwnd());

	return 0;
}

LRESULT CAgoraOpenLiveDlg::OnLeaveChannel(WPARAM wParam, LPARAM lParam)
{
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	lpAgoraObject->LeaveCahnnel();

	return 0;
}

LRESULT CAgoraOpenLiveDlg::OnNetworkQuality(WPARAM wParam, LPARAM lParam)
{
	LPAGE_NETWORK_QUALITY lpData = (LPAGE_NETWORK_QUALITY)wParam;

	if (m_nNetworkQuality != lpData->quality) {
		m_nNetworkQuality = lpData->quality;
		InvalidateRect(CRect(16, 40, 48, 72), TRUE);
	}

	delete lpData;
	return 0;
}