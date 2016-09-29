#include "StdAfx.h"
#include "AgoraObject.h"
#include "AGResourceVisitor.h"

#include <stdio.h>


CAgoraObject *CAgoraObject::m_lpAgoraObject = NULL;
IRtcEngine *CAgoraObject::m_lpAgoraEngine = NULL;
CAGEngineEventHandler CAgoraObject::m_EngineEventHandler;
CString   CAgoraObject::m_strVendorKey;

CAgoraObject::CAgoraObject(void)
	: m_dwEngineFlag(0)
	, m_bVideoEnable(FALSE)
	, m_bLocalAudioMuted(FALSE)
	, m_bScreenCapture(FALSE)
	, m_nSelfUID(0)
	, m_nCodecType(0)
{
	m_strChannelName.Empty();
	m_bLocalVideoMuted = FALSE;
}

CAgoraObject::~CAgoraObject(void)
{
}

CString CAgoraObject::GetSDKVersion()
{
	int nBuildNumber = 0;
	const char *lpszEngineVer = getAgoraRtcEngineVersion(&nBuildNumber);

	CString strEngineVer;

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszEngineVer, -1, strEngineVer.GetBuffer(256), 256);
	strEngineVer.ReleaseBuffer();
#endif

	return strEngineVer;
}

IRtcEngine *CAgoraObject::GetEngine()
{
	if(m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	return m_lpAgoraEngine;
}

CAgoraObject *CAgoraObject::GetAgoraObject(LPCTSTR lpVendorKey)
{
	if(m_lpAgoraObject == NULL)
		m_lpAgoraObject = new CAgoraObject();

	if(m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	// 如果VendorKey为空则直接返回对象
	if (lpVendorKey == NULL)
		return m_lpAgoraObject;

	RtcEngineContext ctx;

	ctx.eventHandler = &m_EngineEventHandler;

#ifdef UNICODE
	char szVendorKey[128];

	::WideCharToMultiByte(CP_ACP, 0, lpVendorKey, -1, szVendorKey, 128, NULL, NULL);
	ctx.appId = szVendorKey;
#else
    ctx.appId = lpVendorKey;
#endif
	m_lpAgoraEngine->initialize(ctx);

	if (lpVendorKey != NULL)
		m_strVendorKey = lpVendorKey;

	return m_lpAgoraObject;
}

void CAgoraObject::CloseAgoraObject()
{
	if(m_lpAgoraEngine != NULL)
		m_lpAgoraEngine->release();

	if(m_lpAgoraObject != NULL)
		delete m_lpAgoraObject;

	m_lpAgoraEngine = NULL;
	m_lpAgoraObject = NULL;
}

void CAgoraObject::SetMsgHandlerWnd(HWND hWnd)
{
	m_EngineEventHandler.SetMsgReceiver(hWnd);
}

HWND CAgoraObject::GetMsgHandlerWnd()
{
	return m_EngineEventHandler.GetMsgReceiver();
}


void CAgoraObject::SetNetworkTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_ENNETTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_ENNETTEST);
}

BOOL CAgoraObject::GetNetworkTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ENNETTEST) != 0;
}

void CAgoraObject::SetEchoTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_ECHOTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_ECHOTEST);
}

BOOL CAgoraObject::GetEchoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_ECHOTEST) != 0;
}

void CAgoraObject::SetSpeakerphoneTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_SPKPHTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_SPKPHTEST);
}

BOOL CAgoraObject::GetSpeakerphoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_SPKPHTEST) != 0;
}

void CAgoraObject::SetMicrophoneTestFlag(BOOL bEnable)
{
	if(bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_MICPHTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_MICPHTEST);
}

BOOL CAgoraObject::GetMicrophoneTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_MICPHTEST) != 0;
}


void CAgoraObject::SetVideoTestFlag(BOOL bEnable)
{
	if (bEnable)
		m_dwEngineFlag |= AG_ENGFLAG_VIDEOTEST;
	else
		m_dwEngineFlag &= (~AG_ENGFLAG_VIDEOTEST);
}

BOOL CAgoraObject::GetVideoTestFlag()
{
	return (m_dwEngineFlag & AG_ENGFLAG_VIDEOTEST) != 0;
}

BOOL CAgoraObject::SetLogFilePath(LPCTSTR lpLogPath)
{
	ASSERT(m_lpAgoraEngine != NULL);

	CHAR szLogPathA[MAX_PATH];
	CHAR szLogPathTrans[MAX_PATH];

	int ret = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (::GetFileAttributes(lpLogPath) == INVALID_FILE_ATTRIBUTES) {
		::GetModuleFileNameA(NULL, szLogPathA, MAX_PATH);
		LPSTR lpLastSlash = strrchr(szLogPathA, '\\')+1;
		strcpy_s(lpLastSlash, 64, "AgoraSDK.log");
	}
	else {
#ifdef UNICODE
		::WideCharToMultiByte(CP_UTF8, 0, lpLogPath, -1, szLogPathA, MAX_PATH, NULL, NULL);
#else
		::MultiByteToWideChar(CP_UTF8, 0, lpLogPath, -1, (WCHAR *)szLogPathA, MAX_PATH, NULL, NULL);
#endif
	}

	CAGResourceVisitor::TransWinPathA(szLogPathA, szLogPathTrans, MAX_PATH);

	ret = rep.setLogFile(szLogPathTrans);

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::JoinChannel(LPCTSTR lpChannelName, UINT nUID)
{
	int nRet = 0;

//	m_lpAgoraEngine->setVideoProfile(VIDEO_PROFILE_720P);
	
#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_ACP, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	nRet = m_lpAgoraEngine->joinChannel(NULL, szChannelName, NULL, nUID);
#else
	nRet = m_lpAgoraEngine->joinChannel(NULL, lpChannelName, NULL, nUID);
#endif

	if (nRet == 0)
		m_strChannelName = lpChannelName;
	
	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::LeaveCahnnel()
{
	m_lpAgoraEngine->stopPreview();
	int nRet = m_lpAgoraEngine->leaveChannel();

	return nRet == 0 ? TRUE : FALSE;
}

CString CAgoraObject::GetChanelName()
{
	return m_strChannelName;
}

CString CAgoraObject::GetCallID()
{
	agora::util::AString uid;
	CString strUID;

	m_lpAgoraEngine->getCallId(uid);

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, 0, uid->c_str(), -1, strUID.GetBuffer(128), 128);
	strUID.ReleaseBuffer();
#else
	strUID = uid->c_str();
#endif

	return strUID;
}

BOOL CAgoraObject::EnableVideo(BOOL bEnable)
{
	int nRet = 0;

	if (bEnable)
		nRet = m_lpAgoraEngine->enableVideo();
	else
		nRet = m_lpAgoraEngine->disableVideo();

	if (nRet == 0)
		m_bVideoEnable = bEnable;

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsVideoEnabled()
{
	return m_bVideoEnable;
}

BOOL CAgoraObject::MuteLocalAudio(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalAudioStream((bool)bMuted);
	if (ret == 0)
		m_bLocalAudioMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}
/*
BOOL CAgoraObject::SetVideoResolution(int nWidth, int nHeight)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

//	int ret = rep.setVideoResolution(nWidth, nHeight);

	return ret == 0 ? TRUE : FALSE;
}
*/

BOOL CAgoraObject::MuteLocalVideo(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalVideoStream((bool)bMuted);
	if (ret == 0)
		m_bLocalVideoMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}
/*
BOOL CAgoraObject::SetVideoMaxBitrate(int nBitrate)
{
	ASSERT(m_lpAgoraEngineEx != NULL);

	int ret = 0;
//	RtcEngineParametersEx repEx(*m_lpAgoraEngineEx);
//	int ret = repEx.setVideoMaxBitrate(nBitrate);

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetVideoMaxFrameRate(int nFrameRate)
{
	ASSERT(m_lpAgoraEngineEx != NULL);

	int ret = 0;
//	RtcEngineParameters rep(*m_lpAgoraEngine);
//	int ret = rep.setVideoMaxFrameRate(nFrameRate);

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetCodec(int nCodecType)
{
	ASSERT(m_lpAgoraEngineEx != NULL);

	char szCodec[128];
	RtcEngineParametersEx repEx(*m_lpAgoraEngineEx);

	memset(szCodec, 0, 128);
	switch (nCodecType)
	{
	case 0:
		strcpy_s(szCodec, 128, "{\"rtc.video.codec\":\"e264\"}");
		break;
	case 1:
		strcpy_s(szCodec, 128, "{\"rtc.video.codec\":\"default\"}");
		break;
	case 2:
		strcpy_s(szCodec, 128, "{\"rtc.video.codec\":\"vp8\"}");
		break;
	default:
		strcpy_s(szCodec, 128, "{\"rtc.video.codec\":\"default\"}");
		break;
	}

	int ret = m_lpAgoraEngineEx->setParameters(szCodec);
	if (ret == 0)
		m_nCodecType = nCodecType;

	return ret == 0 ? TRUE : FALSE;
}
*/

BOOL CAgoraObject::SetClientRole(int nIndex)
{
	int nRet = 0;

	switch (nIndex)
	{
	case 0:
		nRet = m_lpAgoraEngine->setClientRole(CLIENT_ROLE_BROADCASTER);
		break;
	case 1:
		nRet = m_lpAgoraEngine->setClientRole(CLIENT_ROLE_AUDIENCE);
		break;
	default:
		nRet = m_lpAgoraEngine->setClientRole(CLIENT_ROLE_BROADCASTER);
		nIndex = 0;
		break;
	}

	if (nRet == 0)
		m_nRoleIndex = nIndex;

	return nRet == 0 ? TRUE : FALSE;
}


BOOL CAgoraObject::EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath)
{
	int ret = 0;

	RtcEngineParameters rep(*m_lpAgoraEngine);

	if (bEnable) {
#ifdef UNICODE
		CHAR szFilePath[MAX_PATH];
		::WideCharToMultiByte(CP_ACP, 0, lpFilePath, -1, szFilePath, MAX_PATH, NULL, NULL);
		ret = rep.startAudioRecording(szFilePath);
#else
		ret = rep.startAudioRecording(lpFilePath);
#endif
	}
	else
		ret = rep.stopAudioRecording();

	return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableDauleStream(BOOL bEnable)
{
	RtcEngineParameters rep(*m_lpAgoraEngine);

	int nRet = rep.enableDualStreamMode(bEnable);

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetRemoteStreamType(UINT nUID, REMOTE_VIDEO_STREAM_TYPE nType)
{
	RtcEngineParameters rep(*m_lpAgoraEngine);

	int nRet = rep.setRemoteVideoStreamType(nUID, nType);

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn)
{
	int nRet = 0;

	if (bPreviewOn) {
		VideoCanvas vc;

		vc.uid = 0;
		vc.view = hVideoWnd;
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN;

		m_lpAgoraEngine->setupLocalVideo(vc);
		nRet = m_lpAgoraEngine->startPreview();
	}
	else
		nRet = m_lpAgoraEngine->stopPreview();

	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetLogFilter(LOG_FILTER_TYPE logFilterType, LPCTSTR lpLogPath)
{
	int nRet = 0;
	RtcEngineParameters rep(*m_lpAgoraEngine);

	nRet = rep.setLogFilter(logFilterType);

#ifdef UNICODE
	CHAR szFilePath[MAX_PATH];
	::WideCharToMultiByte(CP_ACP, 0, lpLogPath, -1, szFilePath, MAX_PATH, NULL, NULL);
	nRet |= rep.setLogFile(szFilePath);
#else
	nRet |= repEx.setLogFile(lpLogPath);
#endif

	return nRet == 0 ? TRUE : FALSE;
}
