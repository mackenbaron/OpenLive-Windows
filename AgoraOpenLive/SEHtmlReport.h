#pragma once

#include "DlgStatus.h"

typedef struct _REPORT_THREAD_PARAM
{
	HWND		hMainWnd;	// 主窗口句柄
	LPCTSTR		pReportPath;
	CDlgStatus *pDlgInformation;

} REPORT_THREAD_PARAM, *PREPORT_THREAD_PARAM;

class CSEHtmlReport
{
public:
	CSEHtmlReport(LPCTSTR lpFilePath);
	~CSEHtmlReport(void);

	void WriteHead();
	void WriteTail();

	void SystemInfoReport();		// 系统信息
	void CPUInfoReport();			// CPU信息
	void MainBoardInfoReport();		// 主板信息
	void MemoryInfoReport();		// 内存信息
	void DisplayDeviceInfoReport();	// 显卡信息
	void AudioDeviceInfoReport();	// 声卡信息
	void DiskInfoReport();			// 磁盘信息
	void NetDeviceInfoReport();		// 网络设备信息

	static UINT HtmlReportThreadProc(LPVOID lpParam);
protected:
	void InitialSmartItem();
	CMap<BYTE, BYTE, CString, CString> m_mapSmartItem;

private:
	FILE *m_pFileStream;
};

