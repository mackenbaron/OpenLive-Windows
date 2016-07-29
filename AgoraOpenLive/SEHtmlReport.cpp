#include "StdAfx.h"
#include "SystemEngineer.h"

#include "HtmlTable.h"
#include "SEHtmlReport.h"

#include "QueryOperatingSystemInfo.h"
#include "SMBQuerySystemInfo.h"

#include "CPUIdInfo.h"
#include "SMBQueryProcessorInfo.h"
#include "SMBQueryCacheInfo.h"

#include "SMBQueryBaseboardInfo.h"
#include "SMBQueryBIOSInfo.h"

#include "SMBMemCtrlInfo.h"
#include "SMBQueryMemModuleInfo.h"
#include "SMBMemDevice.h"
#include "SMBPhyMemArray.h"

#include "QueryVideoControllerInfo.h"
#include "QueryDesktopMonitorInfo.h"

#include "GetMediaInfo.h"

#include "GetDiskInfo.h"
#include "GetCDRomInfo.h"

#include "QueryNetworkAdapterInfo.h"

CSEHtmlReport::CSEHtmlReport(LPCTSTR lpFilePath)
{
	ASSERT(lpFilePath != NULL);
	_tfopen_s(&m_pFileStream, lpFilePath, _T("w+b"));

	CWMIQuery::InitCOMLibrary();
	CWMIQuery::InitCOMSecurity();
}


CSEHtmlReport::~CSEHtmlReport(void)
{
	if(m_pFileStream != NULL)
		fclose(m_pFileStream);

	CWMIQuery::UninitCOMLibrary();
}

void CSEHtmlReport::WriteHead()
{
	ASSERT(m_pFileStream != NULL);

	CStdioFile ReportFile(m_pFileStream);

	CString str;
	CString strInfo;
	SYSTEMTIME LocalTime;

	::GetLocalTime(&LocalTime);

#ifdef _UNICODE
	WORD wUFlag = 0xFEFF;
	ReportFile.Write(&wUFlag, sizeof(WORD));
#endif

	str = _T("<html>\r\n");
	ReportFile.WriteString(str);
	str = _T("<head>\r\n");
	ReportFile.WriteString(str);
	str = _T("<title>SystemEngineer系统信息报告</title>\r\n");
	ReportFile.WriteString(str);
	str = _T("<style type=\"text/css\">\r\n");
	ReportFile.WriteString(str);
	str = _T("<!--\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLECOLUMN {font-size:13px;BACKGROUND-COLOR:#CCCCCC;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLENORMAL {font-size:13px;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLEWARNING {font-size:13px;color:#FF0000;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLEVERIFY {font-size:13px;BACKGROUND-COLOR:#B0C9F1;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLETBITEM {font-size:13px;font-weight:bold;BACKGROUND-COLOR:#E5F0FF;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLETITLE {font-size:14px;font-weight:bold;}\r\n");
	ReportFile.WriteString(str);
	str = _T("-->\r\n");
	ReportFile.WriteString(str);
	str = _T("</style>\r\n");
	ReportFile.WriteString(str);
	str = _T("</head>\r\n");
	ReportFile.WriteString(str);
	str = _T("<body>\r\n");
	ReportFile.WriteString(str);
	str = _T("<p class=\"STYLETITLE\">SystemEngineer系统信息报告</p>\r\n");
	ReportFile.WriteString(str);

	str.Format(_T("日期: %d/%d/%d %d:%d <br>\r\n"), 
		LocalTime.wYear,
		LocalTime.wMonth,
		LocalTime.wDay,
		LocalTime.wHour,
		LocalTime.wMinute
		);
	ReportFile.WriteString(str);

	str  = _T("SystemEngineer版本: ");
	str += SE_VER;
	str += _T("</p>\r\n");
	ReportFile.WriteString(str);
}

void CSEHtmlReport::WriteTail()
{
	CStdioFile ReportFile(m_pFileStream);

	CString str;
	str = _T("</body>\r\n");
	ReportFile.WriteString(str);
	str = _T("</html>\r\n");
	ReportFile.WriteString(str);
}

void CSEHtmlReport::SystemInfoReport()		// 系统信息
{
	CSMBQuerySystemInfo QuerySystemInfo;
	CQueryOperatingSystemInfo	QueryInfo;
	CHtmlTable Table(m_pFileStream);

	CString str;
	CString strInfo;
	short shTimeZone = 0;

	QuerySystemInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	QueryInfo.Create();

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	Table.AddItemTitle(_T("PC制造商信息"), STYLE_TBITEM);

	Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetManufacturer());
	
	Table.InsertItem(_T("所属系列:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetFamily());

	Table.InsertItem(_T("产品型号:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetProductName());

	Table.InsertItem(_T("产品版本:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetVersion());

	Table.InsertItem(_T("产品ID:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetSerialNumber());

	Table.InsertItem(_T("UUID:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetUUID());

	Table.InsertItem(_T("SKU Number:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetSKUNumber());

	Table.AddItemTitle(_T("操作系统信息"), STYLE_TBITEM);
	QueryInfo.GetCaption(str);
	Table.InsertItem(_T("操作系统:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetRegisteredUser(str);
	Table.InsertItem(_T("注册用户:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetWindowsDirectory(str);
	Table.InsertItem(_T("windows目录:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetSystemDirectory(str);
	Table.InsertItem(_T("system32目录:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetSerialNumber(str);
	Table.InsertItem(_T("序列号:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetBuildNumber(str);
	Table.InsertItem(_T("BuildNumber:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetCSDVersion(str);
	Table.InsertItem(_T("补丁版本:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetBuildType(str);
	Table.InsertItem(_T("内核类型:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetCountryCode(str);
	Table.InsertItem(_T("国家码:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetCodeSet(str);
	Table.InsertItem(_T("字符集编号:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%d"), QueryInfo.GetOSLanguage());
	Table.InsertItem(_T("语言:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	shTimeZone = QueryInfo.GetCurrentTimeZone();
	if(shTimeZone >=0)
		str.Format(_T("GMT+%d"), shTimeZone/60);
	else
		str.Format(_T("GMT%d"), shTimeZone/60);
	Table.InsertItem(_T("当前时区:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetInstallDate(str);
	Table.InsertItem(_T("安装时间:"), STYLE_NORMAL);
	strInfo.Format(_T("%s-%s-%s %s:%s:%s"), 
		str.Left(4), str.Mid(4, 2), str.Mid(6,2),
		str.Mid(8, 2), str.Mid(10, 2), str.Mid(12, 2));
	Table.SetItemText(1, strInfo);

	QueryInfo.GetLastBootUpTime(str);
	Table.InsertItem(_T("上次启动时间:"), STYLE_NORMAL);
	strInfo.Format(_T("%s-%s-%s %s:%s:%s"), 
		str.Left(4), str.Mid(4, 2), str.Mid(6,2),
		str.Mid(8, 2), str.Mid(10, 2), str.Mid(12, 2));
	Table.SetItemText(1, strInfo);

	Table.AddItemTitle(_T("分页及内存信息"), STYLE_TBITEM);

	QueryInfo.GetTotalVirtualMemorySize(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("虚拟内存大小:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);
	
	QueryInfo.GetFreeVirtualMemory(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("可用虚拟内存大小:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QueryInfo.GetFreePhysicalMemory(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("可用物理内存大小:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QueryInfo.GetFreeSpaceInPagingFiles(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("分页文件可用空间:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QueryInfo.GetSizeStoredInPagingFiles(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("已分页存储大小:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QuerySystemInfo.DetachBuffer();
	QueryInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::CPUInfoReport()			// CPU信息
{
	CSMBQueryProcessorInfo	CPUInfo;
	CSMBQueryCacheInfo		CacheInfo;
	WORD					wCacheHandle = 0;
	ULARGE_INTEGER			ProcessorID;

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	static CONST TCHAR *lpInstrItem[16] = {
		_T("MMX: "),
		_T("SSE: "),
		_T("SSE2: "),
		_T("SSE3: "),
		_T("SSSE3: "),
		_T("SSE4.1: "),
		_T("SSE4.2: "),
		_T("MONITOR/MWAIT: "),
		_T("RDMSR/WRMSR: "),
		_T("SYSENTER/SYSEXIT: "),
		_T("FXSAVE/FXRSTOR: "),
		_T("POPCNT: "),
		_T("CMPXCHG8B: "),
		_T("CMPXCHG16B: "),
		_T("CMOV: "),
		_T("CLFSH: "),
	};

	static CONST TCHAR *lpCommonID[25] = {
		_T("x64 Technology: "),
		_T("xTPR Update Control: "),
		_T("Perf/Debug Capability MSR: "),
		_T("LAHF/SAHF Available In 64-bit Mode: "),
		_T("SYSCALL/SYSRET Available In 64-bit Mode: "),
		_T("Execute Disable Bit Available: "),
		_T("x87 FPU on Chip(FPU): "),
		_T("Virtual-8086 Mode Enhancement(VME): "),
		_T("Debugging Extensions(DE): "),
		_T("Page Size Extensions(PSE): "),
		_T("Time Stamp Counter(TSC): "),
		_T("Physical Address Extensions(PAE): "),
		_T("Machine Check Exception(MCE): "),
		_T("APIC on Chip(APIC): "),
		_T("Memory Type Range Registers(MTRR): "),
		_T("PTE Global Bit(PGE): "),
		_T("Machine Check Architecture(MCA): "),
		_T("Page Attribute Table(PAT): "),
		_T("36-bit Page Size Extension(PSE36): "),
		_T("Processor Serial Number(PSN): "),
		_T("Debug Store(DS): "),
		_T("Thermal Monitor and Clock Ctrl(ACPI)"),
		_T("Self Snoop(SS): "),
		_T("Thermal Monitor(TM): "),
		_T("Pend. Brk. En.(PBE): ")
	};

	static CONST TCHAR *lpIntelTech[5] = {
		_T("Multithreading(HTT): "),
		_T("Virtual Machine Extensions(VME): "),
		_T("Enhanced Intel SpeedStep Technology: "),
		_T("CPL Qualified Debug Store: "),
		_T("Thermal Monitor: ")
	};

	static CONST TCHAR *lpAMDTech[19] = {
		_T("MMXExt: "),
		_T("SSE4A: "),
		_T("3Dnow!: "),
		_T("3DnowExt: "),
		_T("AltMovCr8: "),
		_T("LZCNT: "),
		_T("FFXSR: "),
		_T("RDTSCP: "),

		_T("1GB Page:")
		_T("Nested Paging: "),
		_T("LBR Visualization: "),
		_T("FP128 Optimization: "),

		_T("Core Multi-Processing Legacy Mode(CmpLegacy): "),
		_T("Secure Virtual Machine(SVM): "),
		_T("Extended APIC Register Space(ExtApicSpace): "),

		_T("Misaligned SSE Support Mode Available: "),
		_T("PREFETCH/PREFETCHW Support: "),
		_T("SKINIT/DEV Support:"),
		_T("MOVU, MOVUPS, MOVUPD optimization over MOVL/MOVH, MOVHLPS/MOVHPS, MOVLPD/MOVHPD: ")

	};

	static _IsFunctionSupported pfnIsInstructionsSupported[16] = {
		&CCPUIdInfo::IsMMXSupported,
		&CCPUIdInfo::IsSSESupported,
		&CCPUIdInfo::IsSSE2Supported,
		&CCPUIdInfo::IsSSE3NewSupported,
		&CCPUIdInfo::IsSSSE3Supported,
		&CCPUIdInfo::IsSSE41ExtSupported,
		&CCPUIdInfo::IsSSE42ExtSupported,
		&CCPUIdInfo::IsMonMwaitSupported,
		&CCPUIdInfo::IsMSRSupported,
		&CCPUIdInfo::IsSEPSupported,
		&CCPUIdInfo::IsFXSRSupported,
		&CCPUIdInfo::IsPOPCNTSupported,
		&CCPUIdInfo::IsCX8Supported,
		&CCPUIdInfo::IsCX16Supported,
		&CCPUIdInfo::IsCMOVSupported,
		&CCPUIdInfo::IsCLFSHSupported
	};

	static _IsFunctionSupported pfnIsTechnologySupported[25] = {
		&CCPUIdInfo::Isx64Supported,
		&CCPUIdInfo::IsxTPRUpdateCtrlSupported,
		&CCPUIdInfo::IsPDMSRSupported,
		&CCPUIdInfo::IsLSAHF64Supported,
		&CCPUIdInfo::IsSYS64Supported,
		&CCPUIdInfo::IsEDBSupported,

		&CCPUIdInfo::IsFPUSupported,
		&CCPUIdInfo::IsVMESupported,
		&CCPUIdInfo::IsDESupported,
		&CCPUIdInfo::IsPSESupported,
		&CCPUIdInfo::IsTSCSupported,

		&CCPUIdInfo::IsPAESupported,
		&CCPUIdInfo::IsMCESupported,
		&CCPUIdInfo::IsAPICSupported,
		&CCPUIdInfo::IsMTRRSupported,
		&CCPUIdInfo::IsPGESupported,
		&CCPUIdInfo::IsMCASupported,

		&CCPUIdInfo::IsPATSupported,
		&CCPUIdInfo::IsPSE36Supported,
		&CCPUIdInfo::IsPSNSupported,
		&CCPUIdInfo::IsDSSupported,
		&CCPUIdInfo::IsACPISupported,
		&CCPUIdInfo::IsSSSupported,
		&CCPUIdInfo::IsTMSupported,
		&CCPUIdInfo::IsPBESupported
	};

	static _IsFunctionSupported pfnIsSupportedIntel[5] = {
		&CCPUIdInfo::IsHTTSupported,
		&CCPUIdInfo::IsVMExtSupported,
		&CCPUIdInfo::IsCPLQDSSupported,
		&CCPUIdInfo::IsEISTSupported,
		&CCPUIdInfo::IsThMonSupported
	};

	static _IsFunctionSupported pfnIsSupportedAMD[19] = {
		&CCPUIdInfo::IsEMMXSupported,	
		&CCPUIdInfo::IsSSE4ASupported,	
		&CCPUIdInfo::Is3DnowSupported,
		&CCPUIdInfo::Is3DnowExtSupported,	
		&CCPUIdInfo::IsAltMovCr8Supported,	
		&CCPUIdInfo::IsLZCNTSupported,		
		&CCPUIdInfo::IsFFXSRSupported,	
		&CCPUIdInfo::IsRDTSCPSupported,	

		&CCPUIdInfo::Is1GPageSupported,
		&CCPUIdInfo::IsNestedPagingSupported,
		&CCPUIdInfo::IsLBRVlSupported,	
		&CCPUIdInfo::IsFP128OptSupported,

		&CCPUIdInfo::IsCmpLegacySupported,
		&CCPUIdInfo::IsSVMSupported,
		&CCPUIdInfo::IsExtApicSpaceSupported,

		&CCPUIdInfo::IsMisAliSSESupported,
		&CCPUIdInfo::IsPreRWSupported,
		&CCPUIdInfo::IsSKINITDEVSupported,
		&CCPUIdInfo::IsOptMovSupported
	};

	CONST TCHAR **lppItem = NULL;
	_IsFunctionSupported *lppfn = NULL;
	int nCountAll = 0;

	CPUInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	Table.AddItemTitle(_T("CPU信息"), STYLE_TBITEM);
	CCPUIdInfo::GetCPUBandString(str);
	Table.InsertItem(_T("产品型号:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	Table.InsertItem(_T("所属系列:"), STYLE_NORMAL);
	Table.SetItemText(1, CCPUIdInfo::GetCPUInfoDescription());

	Table.InsertItem(_T("扩展标示:"), STYLE_NORMAL);
	Table.SetItemText(1, CCPUIdInfo::GetExtendedCPUInfoDescription());

	CCPUIdInfo::GetCPUIDString(str);
	Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	if(str == _T("GenuineIntel")){
		lppItem = lpIntelTech;
		lppfn = pfnIsSupportedIntel;
		nCountAll = 5;
	}
	else{
		lppItem = lpAMDTech;
		lppfn = pfnIsSupportedAMD;
		nCountAll = 19;
	}

	str.Format(_T("%dMHZ"), CPUInfo.GetProcessorMaxSpeed());
	Table.InsertItem(_T("CPU主频:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%dMHZ"), CPUInfo.GetProcessorCurrentSpeed());
	Table.InsertItem(_T("当前工作主频:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%d"), CCPUIdInfo::GetLogicalProcessors());
	Table.InsertItem(_T("逻辑核心数:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	CPUInfo.GetProcessorId(&ProcessorID);
	str.Format(_T("%08X%08X"), ProcessorID.HighPart, ProcessorID.LowPart);
	Table.InsertItem(_T("CPU特征ID:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	wCacheHandle = CPUInfo.GetL1CacheHandle();
	if(wCacheHandle != 0xFFFF){
		CacheInfo.AttachBuffer(CSMBQuerier::GetSMBiosData(), wCacheHandle);

		Table.InsertItem(_T("L1缓存状态:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetCacheConfigDescription());

		str.Format(_T("%dKB"), CacheInfo.GetInstalledSize());
		Table.InsertItem(_T("L1缓存大小:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dMHZ"), CacheInfo.GetCacheSpeed());
		Table.InsertItem(_T("L1缓存频率:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("L1缓存类型:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetTypeDescription());

		Table.InsertItem(_T("L1缓存纠错方式:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetErrorCorrectionTypeDescription());

		Table.InsertItem(_T("L1缓存系统类别:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetSystemCacheTypeDescription());

		Table.InsertItem(_T("L1缓存工作方式:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetAssociativityDescription());

		CacheInfo.DetachBuffer();
	}

	wCacheHandle = CPUInfo.GetL2CacheHandle();
	if(wCacheHandle != 0xFFFF){
		CacheInfo.AttachBuffer(CSMBQuerier::GetSMBiosData(), wCacheHandle);

		Table.InsertItem(_T("L2缓存状态:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetCacheConfigDescription());

		str.Format(_T("%dKB"), CacheInfo.GetInstalledSize());
		Table.InsertItem(_T("L2缓存大小:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dMHZ"), CacheInfo.GetCacheSpeed());
		Table.InsertItem(_T("L2缓存频率:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("L2缓存类型:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetTypeDescription());

		Table.InsertItem(_T("L2缓存纠错方式:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetErrorCorrectionTypeDescription());

		Table.InsertItem(_T("L2缓存系统类别:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetSystemCacheTypeDescription());

		Table.InsertItem(_T("L2缓存工作方式:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetAssociativityDescription());

		CacheInfo.DetachBuffer();
	}

	wCacheHandle = CPUInfo.GetL3CacheHandle();
	if(wCacheHandle != 0xFFFF){
		CacheInfo.AttachBuffer(CSMBQuerier::GetSMBiosData(), wCacheHandle);

		Table.InsertItem(_T("L3缓存状态:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetCacheConfigDescription());

		str.Format(_T("%dKB"), CacheInfo.GetInstalledSize());
		Table.InsertItem(_T("L3缓存大小:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dMHZ"), CacheInfo.GetCacheSpeed());
		Table.InsertItem(_T("L3缓存频率:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("L3缓存类型:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetTypeDescription());

		Table.InsertItem(_T("L3缓存纠错方式:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetErrorCorrectionTypeDescription());

		Table.InsertItem(_T("L3缓存系统类别:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetSystemCacheTypeDescription());

		Table.InsertItem(_T("L3缓存工作方式:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetAssociativityDescription());

		CacheInfo.DetachBuffer();
	}

	str.Format(_T("%dMHZ"), CPUInfo.GetProcessorExternalClock());
	Table.InsertItem(_T("外频:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%d位"), CCPUIdInfo::GetPhyAddrBits());
	Table.InsertItem(_T("物理寻址宽度:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%d位"), CCPUIdInfo::GetVirtAddrBits());
	Table.InsertItem(_T("虚拟寻址宽度:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%gV"), CPUInfo.GetProcessorVoltage());
	Table.InsertItem(_T("当前电压:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	Table.InsertItem(_T("CPU序列号:"), STYLE_NORMAL);
	Table.SetItemText(1, CPUInfo.GetSerialNumber());

	Table.InsertItem(_T("CPU Part号:"), STYLE_NORMAL);
	Table.SetItemText(1, CPUInfo.GetPartNumber());

	Table.InsertItem(_T("插槽类型:"), STYLE_NORMAL);
	Table.SetItemText(1, CPUInfo.GetProcessorUpgradeDescription());

	Table.AddItemTitle(_T("技术特征"), STYLE_TBITEM);
	for(int nIndex = 0; nIndex < 25; nIndex++){
		Table.InsertItem(*(lpCommonID+nIndex), STYLE_NORMAL);
		if(pfnIsTechnologySupported[nIndex]())
			strInfo = _T("支持");
		else
			strInfo = _T("不支持");

		Table.SetItemText(1, strInfo);
	}

	Table.AddItemTitle(_T("指令集特征"), STYLE_TBITEM);
	for(int nIndex = 0; nIndex < 16; nIndex++){
		Table.InsertItem(*(lpInstrItem+nIndex), STYLE_NORMAL);
		if(pfnIsInstructionsSupported[nIndex]())
			strInfo = _T("支持");
		else
			strInfo = _T("不支持");

		Table.SetItemText(1, strInfo);
	}

	Table.AddItemTitle(_T("专有技术"), STYLE_TBITEM);
	for(int nIndex = 0; nIndex < nCountAll; nIndex++){
		Table.InsertItem(*(lppItem+nIndex), STYLE_NORMAL);
		if(lppfn[nIndex]())
			strInfo = _T("支持");
		else
			strInfo = _T("不支持");

		Table.SetItemText(1, strInfo);
	}

	CPUInfo.DetachBuffer();
	Table.Close(TRUE);
}


void CSEHtmlReport::MainBoardInfoReport()		// 主板信息
{
	CSMBQueryBaseboardInfo	QueryBaseboardInfo;	
	CSMBQueryBIOSInfo		QueryBiosInfo;
	BOOL					bSuccess = FALSE;

	CString strInfo;
	CString str;

	ULARGE_INTEGER	uiCharacteristics;
	WORD			wExCharacteristics;
	CHtmlTable		Table(m_pFileStream);

	static CONST TCHAR *pszCharacteristicsItem[28] = {
		_T("ISA总线: "),
		_T("MCA总线: "),
		_T("EISA总线: "),
		_T("PCI总线: "),
		_T("PCMCIA接口: "),
		_T("即插即用: "),
		_T("APM(高级电源管理): "),
		_T("BIOS可升级(Flash): "),
		_T("BIOS可映射: "),
		_T("VL-VESA接口: "),
		_T("ESCD(扩展系统配置数据): "),
		_T("光驱引导: "),
		_T("启动项选择: "),
		_T("BIOS ROM is socketed: "),
		_T("PCMCIA启动: "),
		_T("EDD(增强磁盘驱动器)规范: "),
		_T("Int 13h/(NEC)3.5英寸1.2MB软驱: "),
		_T("Int 13h/(日立)3.5英寸1.2MB软驱: "),
		_T("Int 13h/5.25英寸360KB软驱: "),
		_T("Int 13h/5.25英寸1.2MB软驱: "),
		_T("Int 13h/3.5英寸720KB软驱: "),
		_T("Int 13h/3.5英寸2.88MB软驱: "),
		_T("Int 5h/屏幕打印: "),
		_T("Int 9h/8042键盘: "),
		_T("Int 14h/串口: "),
		_T("Int 17h/打印机接口: "),
		_T("Int 10h/CGA&单色显示: "),
		_T("NEC PC-98规范: ")
	};

	static CONST TCHAR *pszExItem[11] = {
		_T("ACPI: "),
		_T("USB: "),
		_T("AGP: "),
		_T("I2O启动: "),
		_T("LS-120启动: "),
		_T("ATAPI ZIP驱动器启动: "),
		_T("1394启动: "),
		_T("SmartBattery(UNOAV智能电池专利): "),
		_T("BIOS Boot Specification: "),
		_T("Function Key-Initiated Network Service Boot: "),
		_T("Targeted Content Distribution: ")
	};

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	bSuccess = QueryBaseboardInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	if(bSuccess){
		Table.AddItemTitle(_T("主板信息"), STYLE_TBITEM);
		Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetManufacturer());

		Table.InsertItem(_T("主板型号:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetProduct());

		Table.InsertItem(_T("序列号:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetSerialNumber());

		Table.InsertItem(_T("版本号:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetVersion());

		Table.InsertItem(_T("所有者标签:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetAssetTag());
		QueryBiosInfo.DetachBuffer();
	}

	bSuccess = QueryBiosInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	if(bSuccess){
		Table.AddItemTitle(_T("BIOS信息"), STYLE_TBITEM);
		Table.InsertItem(_T("发布厂商:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetVendor());

		Table.InsertItem(_T("版本号:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetVersion());

		Table.InsertItem(_T("发布日期:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetReleaseData());

		str.Format(_T("%dKB"), QueryBiosInfo.GetBIOSRomSize());
		Table.InsertItem(_T("BIOS容量:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("SystemBiosVer:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetSystemBIOSVer());

		Table.InsertItem(_T("ECFVer:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetECFVer());

		Table.AddItemTitle(_T("BIOS技术特征"), STYLE_TBITEM);
		QueryBiosInfo.GetBIOSCharacteristics(&uiCharacteristics);
		for(int nIndex = 0; nIndex < 28; nIndex++){
			Table.InsertItem(*(pszCharacteristicsItem+nIndex), STYLE_NORMAL);
			if(((SMBIOS_BIOS_ISA<<nIndex)&uiCharacteristics.LowPart) != 0)
				strInfo = (_T("支持"));
			else
				strInfo = (_T("不支持"));

			Table.SetItemText(1, strInfo);
		}

		Table.AddItemTitle(_T("BIOS技术特征(扩展)"), STYLE_TBITEM);
		wExCharacteristics = QueryBiosInfo.GetBIOSExCharacteristics();
		for(int nIndex = 0; nIndex < 11; nIndex++){
			Table.InsertItem(*(pszExItem+nIndex), STYLE_NORMAL);
			if(((SMBIOS_BIOSEX_ACPI<<nIndex)&wExCharacteristics) != 0)
				strInfo = (_T("支持"));
			else
				strInfo = (_T("不支持"));

			Table.SetItemText(1, strInfo);
		}
		QueryBiosInfo.DetachBuffer();
	}

	Table.Close(TRUE);
}

void CSEHtmlReport::MemoryInfoReport()		// 内存信息
{
	CSMBMemCtrlInfo MemCtrlInfo;
	CSMBMemDevice			MemDevice;
	CSMBPhyMemArray			PhyMemArray;
	ULARGE_INTEGER			uMemSize;

	CHtmlTable Table(m_pFileStream);

	BOOL	bQueryResult = FALSE;
	ULONG   ulIndex = 0;
	WORD    wTypeDetail = 1;
	DWORD   dwValue = 0;
	CString strInfo;
	CString str;

	CONST TCHAR *pszItem[23] = {
		_T("Unknown"),
		_T("Other"),
		_T("SiP"),
		_T("DIP"),
		_T("ZIP"),
		_T("SOJ"),
		_T("Proprietary"),
		_T("SIMM"),
		_T("DIMM"),
		_T("TSOPO"),
		_T("PGA"),
		_T("RIM"),
		_T("SODIMM"),
		_T("SRIMM"),
		_T("SMD"),
		_T("SSMP"),
		_T("QFP"),
		_T("TQFP"),
		_T("SOIC"),
		_T("LCC"),
		_T("PLCC"),
		_T("FPGA"),
		_T("LGA")
	};

	CONST TCHAR *pszMemType[22] =  {
		_T("未知类型"),
		_T("其他类型"),
		_T("DRAM"),
		_T("Synchronous DRAM"),
		_T("Cache DRAM"),
		_T("EDO"),
		_T("EDRAM"),
		_T("VRAM"),
		_T("SRAM"),
		_T("RAM"),
		_T("ROM"),
		_T("FLASH"),
		_T("EEPROM"),
		_T("FEPROM"),
		_T("EPROM"),
		_T("CDRAM"),
		_T("3DRAM"),
		_T("SDRAM"),
		_T("SGRAM"),
		_T("RDRAM"),
		_T("DDR"),
		_T("DDR2")
	};

	CONST TCHAR *pszErrorCorrecting[6] =  {
		_T("Other"),
		_T("Unknown"),
		_T("None"),
		_T("Single Bit Error Correcting"),
		_T("Double Bit Error Correcting"),
		_T("Error Scrubbing")
	};

	const TCHAR *pszVoltage[3] = {
		_T("5v"),
		_T("3.3v"),
		_T("2.9v")
	};	

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	bQueryResult = MemCtrlInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	if(bQueryResult){
		Table.AddItemTitle(_T("内存控制器信息"), STYLE_TBITEM);

		Table.InsertItem(_T("查错模式:"), STYLE_NORMAL);
		Table.SetItemText(1, MemCtrlInfo.GetErrorDetectingMethodDescrip());

		Table.InsertItem(_T("纠错支持:"), STYLE_NORMAL);
		dwValue = MemCtrlInfo.GetErrorCorrectingCapability();
		str = _T("");
		for(int nIndex = 0; nIndex < 6; nIndex++){
			if(((0x00000001<<nIndex)&dwValue) != 0){
				str += *(pszErrorCorrecting+nIndex);
				str += _T("\\");
			}
		}
		Table.SetItemText(1, str);

		Table.InsertItem(_T("交错式访问支持:"), STYLE_NORMAL);
		Table.SetItemText(1, MemCtrlInfo.GetSupportedInterleaveDescrip());

		Table.InsertItem(_T("当前启用交错式访问支持:"), STYLE_NORMAL);
		Table.SetItemText(1, MemCtrlInfo.GetCurrentInterleaveDescrip());

		if(PhyMemArray.AttachBuffer(CSMBQuerier::GetSMBiosData())){
			str.Format(_T("%dMB"), PhyMemArray.GetMaximumCapacity());
			Table.InsertItem(_T("最大总容量支持:"), STYLE_NORMAL);
			Table.SetItemText(1, str);
			PhyMemArray.DetachBuffer();
		}

		str.Format(_T("%dMB"), MemCtrlInfo.GetMaxMemModuleSize());
		Table.InsertItem(_T("单条最大容量支持:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("电压支持:"), STYLE_NORMAL);
		dwValue = MemCtrlInfo.GetMemModuleVoltage();
		str = _T("");
		for(int nIndex = 0; nIndex < 3; nIndex++){
			if(((0x00000001<<nIndex)&dwValue) != 0){
				str += *(pszVoltage+nIndex);
				str += _T("\\");
			}
		}
		Table.SetItemText(1, str);

		str.Format(_T("%d"), MemCtrlInfo.GetMemSlotsCount());
		Table.InsertItem(_T("插槽数:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		MemCtrlInfo.DetachBuffer();
	}

	if( MemDevice.AttachBuffer(CSMBQuerier::GetSMBiosData()) &&
		PhyMemArray.AttachBuffer(CSMBQuerier::GetSMBiosData()) ){
			do{
				if(MemDevice.GetPhyMemArrayHandle() != PhyMemArray.GetHandle())
					continue;

				MemDevice.GetSize(&uMemSize);
				str.Format(_T("插槽编号: %s %s(%dMB)"), MemDevice.GetDeviceLocator(), MemDevice.GetBankLocator(), uMemSize.LowPart);
				Table.AddItemTitle(str, STYLE_TBITEM);

				Table.InsertItem(_T("制造商编号:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetManufacturer());

				Table.InsertItem(_T("颗粒串号:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetAssetTag());

				Table.InsertItem(_T("序列号:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetSerialNumber());

				Table.InsertItem(_T("封装类型:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetMemoryTypeDescrip());

				Table.InsertItem(_T("内存类型:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetFormFactorDescrip());

				str.Format(_T("%d位"), MemDevice.GetTotalWidth());
				Table.InsertItem(_T("总线宽度:"), STYLE_NORMAL);
				Table.SetItemText(1, str);

				str.Format(_T("%d位"), MemDevice.GetDataWidth());
				Table.InsertItem(_T("数据位宽度:"), STYLE_NORMAL);
				Table.SetItemText(1, str);

				str.Format(_T("%dMHZ"), MemDevice.GetSpeed());
				Table.InsertItem(_T("工作频率:"), STYLE_NORMAL);
				Table.SetItemText(1, str);

			} while(MemDevice.FindNext());

			MemDevice.DetachBuffer();
			PhyMemArray.DetachBuffer();
	}

	Table.Close(TRUE);
}

void CSEHtmlReport::DisplayDeviceInfoReport()	// 显卡信息
{
	CQueryVideoControllerInfo QueryInfo;
	CQueryDesktopMonitorInfo  QueryMonitorInfo;
	BOOL	bQueryResult = TRUE;
	ULONG   ulIndex = 0;

	CONST TCHAR *pszScanMode[4] = {
		_T("其他"),
		_T("未知"),
		_T("交错扫描"),
		_T("逐行扫描"),
	};

	CONST TCHAR *pszMemoryType[13] = {
		_T("其他类型"),
		_T("未知类型"),
		_T("VRAM"),
		_T("DRAM"),
		_T("SRAM"),
		_T("WRAM"),
		_T("EDO RAM"),
		_T("Burst Synchronous DRAM"),
		_T("Pipelined Burst SRAM"),
		_T("CDRAM"),
		_T("3DRAM"),
		_T("SDRAM"),
		_T("SGRAM")
	};

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	Table.Open(TRUE);
	Table.SetColumnCount(2);
	bQueryResult = QueryInfo.Create();
	while(bQueryResult) {
		Table.AddItemTitle(_T("显卡信息"), STYLE_TBITEM);
		QueryInfo.GetVideoProcessor(str);
		Table.InsertItem(_T("GPU型号:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetCaption(str);
		Table.InsertItem(_T("显卡类型:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetAdapterCompatibility(str);
		Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetAdapterDACType(str);
		Table.InsertItem(_T("DAC型号:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d M"), QueryInfo.GetAdapterRAM()/1048576);
		Table.InsertItem(_T("显存大小:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str = *(pszMemoryType + QueryInfo.GetVideoMemoryType() - 1);
		Table.InsertItem(_T("显存类型:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetVideoModeDescription(str);
		Table.InsertItem(_T("当前显示模式:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d HZ"), QueryInfo.GetCurrentRefreshRate());
		Table.InsertItem(_T("当前刷新率:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d HZ"), QueryInfo.GetMaxRefreshRate());
		Table.InsertItem(_T("最大刷新率:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d HZ"), QueryInfo.GetMinRefreshRate());
		Table.InsertItem(_T("最小刷新率:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str = *(pszScanMode + QueryInfo.GetCurrentScanMode() - 1);
		Table.InsertItem(_T("当前扫描模式:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetInstalledDisplayDrivers(str);
		Table.InsertItem(_T("显卡驱动:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetDriverVersion(str);
		Table.InsertItem(_T("驱动版本号:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetDriverDate(strInfo);
		str.Format(_T("%s-%s-%s %s:%s:%s"), 
			strInfo.Left(4), strInfo.Mid(4, 2), strInfo.Mid(6,2),
			strInfo.Mid(8, 2), strInfo.Mid(10, 2), strInfo.Mid(12, 2));
		Table.InsertItem(_T("驱动发布日期:"), STYLE_NORMAL);
		Table.SetItemText(1, str);
		bQueryResult = QueryInfo.SetQueryIndex(&ulIndex);
	}

	ulIndex = 0;
	Table.AddItemTitle(_T("显示器信息"), STYLE_TBITEM);
	bQueryResult = QueryMonitorInfo.Create();
	while(bQueryResult){

		QueryMonitorInfo.GetDeviceID(str);
		Table.InsertItem(_T("设备ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryMonitorInfo.GetCaption(str);
		Table.InsertItem(_T("型号:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryMonitorInfo.GetMonitorManufacturer(str);
		Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dx%d 像素"), 
			QueryMonitorInfo.GetScreenWidth(),
			QueryMonitorInfo.GetScreenHeight());
		Table.InsertItem(_T("最大分辨率:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d像素点/英寸"), 
			QueryMonitorInfo.GetPixelsPerXLogicalInch());
		Table.InsertItem(_T("X轴向像素密度:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d像素点/英寸"), 
			QueryMonitorInfo.GetPixelsPerYLogicalInch());
		Table.InsertItem(_T("Y轴向像素密度:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		bQueryResult = QueryMonitorInfo.SetQueryIndex(&ulIndex);
	}

	QueryInfo.Close();
	QueryMonitorInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::AudioDeviceInfoReport()		// 声卡信息
{
	CGetMediaInfo QueryInfo;
	DWORD   dwIndex = 0;

	CString str;
	CHtmlTable Table(m_pFileStream);

	QueryInfo.Create();

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	while(QueryInfo.SetQueryIndex(dwIndex)){
		str.Format(_T("声卡型号: %s"), QueryInfo.GetDeviceDesc());
		Table.AddItemTitle(str, STYLE_TBITEM);
		
		Table.InsertItem(_T("设备类型:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetClassDescription());

		Table.InsertItem(_T("服务名:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetServiceName());

		Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetManufacturer());

		Table.InsertItem(_T("设备接口:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetDeviceInstanceId());
		
		dwIndex++;
	}

	QueryInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::InitialSmartItem()
{
	if(m_mapSmartItem.GetCount() > 0)
		return;

	m_mapSmartItem.SetAt(0x01, _T("底层数据读取错误率"));
	m_mapSmartItem.SetAt(0x02, _T("读写通量性能"));
	m_mapSmartItem.SetAt(0x03, _T("盘片启动时间"));
	m_mapSmartItem.SetAt(0x04, _T("电机起停次计数"));
	m_mapSmartItem.SetAt(0x05, _T("重定位扇区计数"));
	m_mapSmartItem.SetAt(0x06, _T("信道读取余量"));
	m_mapSmartItem.SetAt(0x07, _T("寻道错误率"));
	m_mapSmartItem.SetAt(0x08, _T("寻道性能"));
	m_mapSmartItem.SetAt(0x09, _T("硬盘加电时间"));
	m_mapSmartItem.SetAt(0x0a, _T("电机起转重试"));
	m_mapSmartItem.SetAt(0x0b, _T("磁头校准重试"));
	m_mapSmartItem.SetAt(0x0c, _T("设备开关计数"));
	m_mapSmartItem.SetAt(0x0d, _T("软件读取错误率"));
	m_mapSmartItem.SetAt(0xb7, _T("SATA降级运行计数"));
	m_mapSmartItem.SetAt(0xb8, _T("终端校验出错"));
	m_mapSmartItem.SetAt(0xb9, _T("磁头稳定性"));
	m_mapSmartItem.SetAt(0x07, _T("寻道错误率"));
	m_mapSmartItem.SetAt(0xbb, _T("报告不可纠正错误"));
	m_mapSmartItem.SetAt(0xbc, _T("通信超时"));
	m_mapSmartItem.SetAt(0xbd, _T("磁头写入高度"));
	m_mapSmartItem.SetAt(0xbe, _T("气流温度"));
	m_mapSmartItem.SetAt(0xbf, _T("加速度错误率"));
	m_mapSmartItem.SetAt(0xc0, _T("电源关闭磁头收回计数"));
	m_mapSmartItem.SetAt(0xc1, _T("磁头升降计数"));
	m_mapSmartItem.SetAt(0xc2, _T("温度"));
	m_mapSmartItem.SetAt(0xc3, _T("硬件ECC恢复"));
	m_mapSmartItem.SetAt(0xc4, _T("重定位事件计数"));
	m_mapSmartItem.SetAt(0xc5, _T("等候重定的扇区计数"));
	m_mapSmartItem.SetAt(0xc6, _T("无法校正的扇区计数"));
	m_mapSmartItem.SetAt(0xc7, _T("UltraDMA通讯CRC错误"));
	m_mapSmartItem.SetAt(0xc8, _T("多区域错误率"));
	m_mapSmartItem.SetAt(0xc9, _T("逻辑读取错误率"));
	m_mapSmartItem.SetAt(0xca, _T("数据地址标记错误"));
	m_mapSmartItem.SetAt(0xcb, _T("用完取消"));
	m_mapSmartItem.SetAt(0xcc, _T("逻辑ECC纠正"));
	m_mapSmartItem.SetAt(0xcd, _T("热嘈率"));
	m_mapSmartItem.SetAt(0xce, _T("飞行高度"));
	m_mapSmartItem.SetAt(0xcf, _T("主轴电机浪涌电流计数"));
	m_mapSmartItem.SetAt(0xd0, _T("磁头报警"));
	m_mapSmartItem.SetAt(0xd1, _T("离线寻址效能"));
	m_mapSmartItem.SetAt(0xd3, _T("写操作震动"));
	m_mapSmartItem.SetAt(0xd4, _T("写操作冲击"));
	m_mapSmartItem.SetAt(0xdc, _T("盘体偏移"));
	m_mapSmartItem.SetAt(0xdd, _T("加速计出错率"));
	m_mapSmartItem.SetAt(0xde, _T("数据加载时间"));
	m_mapSmartItem.SetAt(0xdf, _T("加载/卸载重试次数"));
	m_mapSmartItem.SetAt(0xe0, _T("负载摩擦"));
	m_mapSmartItem.SetAt(0xe1, _T("加载/卸载循环计数"));
	m_mapSmartItem.SetAt(0xe2, _T("磁头加载耗时"));
	m_mapSmartItem.SetAt(0xe3, _T("扭矩放大计数"));
	m_mapSmartItem.SetAt(0xe4, _T("断电缩回周期"));
	m_mapSmartItem.SetAt(0xe6, _T("GMR磁头振幅"));

	m_mapSmartItem.SetAt(0xe7, _T("硬盘温度"));
	m_mapSmartItem.SetAt(0xe8, _T("耐久性剩余"));
	m_mapSmartItem.SetAt(0xe9, _T("加电时间"));
	m_mapSmartItem.SetAt(0xf0, _T("磁头飞行时间"));
	m_mapSmartItem.SetAt(0xf1, _T("LBA写入总数"));
	m_mapSmartItem.SetAt(0xf2, _T("LBA读取总数"));
	m_mapSmartItem.SetAt(0xfa, _T("读取错误重试率"));
	m_mapSmartItem.SetAt(0xfe, _T("自由跌落保护"));
}

void CSEHtmlReport::DiskInfoReport()			// 磁盘信息
{
	CGetDiskInfo	GetDiskInfo;
	CGetCDRomInfo	GetCDRomInfo;
	IDSECTOR		IDSector;
	ULONG			ulDiskIndex = 0;
	ULARGE_INTEGER	uLargeInteger;

	LPSMART_INFO	lpSmartInfo = new SMART_INFO[256];
	SIZE_T			nSmartSize = sizeof(SMART_INFO)*256;

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	Table.Open(TRUE);
	Table.SetColumnCount(2);
	InitialSmartItem();

	Table.AddItemTitle(_T("物理存储器信息"), STYLE_TBITEM);
	while(GetDiskInfo.SetQueryIndex(&ulDiskIndex)){
		if(!GetDiskInfo.Create())
			break;

		Table.InsertItem(_T("物理设备名:"), STYLE_NORMAL);
		Table.SetItemText(1, GetDiskInfo.GetCurrentPhysicalDrive());
		
		memset(&IDSector, 0, sizeof(IDSECTOR));
		if(GetDiskInfo.GetIDSector(&IDSector)) {

			Table.InsertItem(_T("型号:"), STYLE_NORMAL);
			Table.SetItemText(1, CGetDiskInfo::BytesToString((const BYTE*)IDSector.szModelNumber, 40));

			Table.InsertItem(_T("序列号:"), STYLE_NORMAL);
			Table.SetItemText(1, GetDiskInfo.GetSerialNumber());

			Table.InsertItem(_T("固件号:"), STYLE_NORMAL);
			Table.SetItemText(1, CGetDiskInfo::BytesToString((const BYTE*)IDSector.szFirmwareRev, 8));

			strInfo.Format(_T("%dMB"), IDSector.wBufferSize/2048);
			Table.InsertItem(_T("缓存大小:"), STYLE_NORMAL);
			Table.SetItemText(1, strInfo);
		}
		
		GetDiskInfo.GetTotalSectors(&uLargeInteger);
		uLargeInteger.QuadPart*= GetDiskInfo.GetBytesPerSector();
		if(uLargeInteger.QuadPart >= 1000000000){
			uLargeInteger.QuadPart /= 1000000000;
			strInfo.Format(_T("%uGB"), uLargeInteger.QuadPart);
		}
		else if(uLargeInteger.QuadPart < 1000000000 && uLargeInteger.QuadPart >= 1000000){
			uLargeInteger.QuadPart /= 1000000;
			strInfo.Format(_T("%uMB"), uLargeInteger);
		}
		else
			strInfo.Format(_T("%uBytes"), uLargeInteger.QuadPart);
		Table.InsertItem(_T("容量:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		GetDiskInfo.GetTotalCylinders(&uLargeInteger);
		strInfo.Format(_T("%u"), uLargeInteger);
		Table.InsertItem(_T("柱面数:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		GetDiskInfo.GetTotalSectors(&uLargeInteger);
		strInfo.Format(_T("%u"), uLargeInteger);
		Table.InsertItem(_T("扇区数:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		GetDiskInfo.GetTotalTracks(&uLargeInteger);
		strInfo.Format(_T("%u"), uLargeInteger);
		Table.InsertItem(_T("磁轨数:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		Table.InsertItem(_T("每柱面磁轨数:"), STYLE_NORMAL);
		strInfo.Format(_T("%d"), GetDiskInfo.GetTracksPerCylinder());
		Table.SetItemText(1, strInfo);

		Table.InsertItem(_T("每磁轨扇区数:"), STYLE_NORMAL);
		strInfo.Format(_T("%d"), GetDiskInfo.GetSectorsPerTrack());
		Table.SetItemText(1, strInfo);

		Table.InsertItem(_T("每扇区字节数:"), STYLE_NORMAL);
		strInfo.Format(_T("%d"), GetDiskInfo.GetBytesPerSector());
		Table.SetItemText(1, strInfo);

		if(GetDiskInfo.IsSMARTSupported()){
			strInfo.Format(_T("%d摄氏度"), GetDiskInfo.GetCurrentTempurature());
			Table.InsertItem(_T("当前温度:"), STYLE_NORMAL);
			Table.SetItemText(1, strInfo);

			memset(lpSmartInfo, 0, nSmartSize);
			if(GetDiskInfo.GetSMARTInfo(lpSmartInfo, &nSmartSize)){
				Table.AddItemTitle(_T("S.M.A.R.T信息(当前值/最差值/阈值)"), STYLE_TBITEM);

				int nSmartCount = nSmartSize/sizeof(SMART_INFO);
				for(int nIndex = 0; nIndex < nSmartCount; nIndex++){
					m_mapSmartItem.Lookup(lpSmartInfo[nIndex].bAttribIndex, str);

					if(lpSmartInfo[nIndex].bWorst < lpSmartInfo[nIndex].bThreshold){
						str += _T("(警告):");
						Table.InsertItem(str, STYLE_WARNING);
					}
					else{
						str += _T("(正常):");
						Table.InsertItem(str, STYLE_NORMAL);
					}
					strInfo.Format(_T("%d/%d/%d"), lpSmartInfo[nIndex].bCurrent, lpSmartInfo[nIndex].bWorst, lpSmartInfo[nIndex].bThreshold);
					Table.SetItemText(1, strInfo);
				}
			}
		}

		GetDiskInfo.Close();
	}

	delete [] lpSmartInfo;

	Table.AddItemTitle(_T("光驱信息"), STYLE_TBITEM);
	if(GetCDRomInfo.Create()){
		ulDiskIndex = 0;
		while(GetCDRomInfo.SetQueryIndex(ulDiskIndex)){

			Table.InsertItem(_T("设备型号:"), STYLE_NORMAL);
			Table.SetItemText(1, GetCDRomInfo.GetFriendlyName());

			Table.InsertItem(_T("描述信息:"), STYLE_NORMAL);
			Table.SetItemText(1, GetCDRomInfo.GetClassDescription());

			Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
			Table.SetItemText(1, GetCDRomInfo.GetManufacturer());
			ulDiskIndex++;
		}
	}

	GetCDRomInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::NetDeviceInfoReport()		// 网络设备信息
{
	CQueryNetworkAdapterInfo QueryInfo;

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	BOOL  bQueryResult = TRUE;
	ULONG ulIndex = 0;

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	Table.AddItemTitle(_T("网络设备信息"), STYLE_TBITEM);
	bQueryResult = QueryInfo.Create();
	while(bQueryResult){
		QueryInfo.GetDescription(str);
		Table.InsertItem(_T("设备名称:"), STYLE_COLUMN);
		Table.SetItemText(1, str);

		QueryInfo.GetAdapterType(str);
		Table.InsertItem(_T("适配器类型:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetManufacturer(str);
		Table.InsertItem(_T("制造商:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetNetConnectionID(str);
		Table.InsertItem(_T("网络连接ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetDeviceID(str);
		Table.InsertItem(_T("设备ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetMACAddress(str);
		Table.InsertItem(_T("MAC地址:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetPNPDeviceID(str);
		Table.InsertItem(_T("即插即用设备ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		bQueryResult = QueryInfo.SetQueryIndex(&ulIndex);
	}

	QueryInfo.Close();
	Table.Close(TRUE);
}

UINT CSEHtmlReport::HtmlReportThreadProc(LPVOID lpParam)
{
	PREPORT_THREAD_PARAM pThreadParam = (PREPORT_THREAD_PARAM)lpParam;
	CSEHtmlReport Report(pThreadParam->pReportPath);

	::SendMessage(pThreadParam->hMainWnd, WM_THREAD_START, NULL, NULL);
	Report.WriteHead();

	pThreadParam->pDlgInformation->SetInfoText2(_T("系统信息检测"));
	Report.SystemInfoReport();
	
	pThreadParam->pDlgInformation->SetInfoText2(_T("CPU信息检测"));
	Report.CPUInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("主板信息检测"));
	Report.MainBoardInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("内存信息检测"));
	Report.MemoryInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("显卡信息检测"));
	Report.DisplayDeviceInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("声卡信息检测"));
	Report.AudioDeviceInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("磁盘信息"));
	Report.DiskInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("网络设备信息"));
	Report.NetDeviceInfoReport();
	
	Report.WriteTail();
	::SendMessage(pThreadParam->hMainWnd, WM_THREAD_FINISHED, NULL, NULL);

	delete pThreadParam;

	return 0;
}