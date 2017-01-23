#include "StdAfx.h"
#include "MemoryCtrl.h"
#include "LogVariable.h"
#include "XUnzip.h"
#include "XZip.h"
#include <io.h>

#define LOG_NAME_W			"LOG_SHAREMEM_W"
//#define LOG_NAME_R			"LOG_SHAREMEM_R"

const GUID CDECL _tlid = { 0x49CC2E36, 0xC7AB, 0x4227, { 0xA9, 0x88, 0xE7, 0xEA, 0xF0, 0xBD, 0x98, 0xB1 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

CMemoryCtrl clsMemCtrl;

CMemoryCtrl::CMemoryCtrl(void)
{
//	int nRet;

	m_hMemMapLogW	= NULL;
	m_pQueueLogW	= NULL;

/*
	m_hMemMapLog = CreateFileMapping(INVALID_HANDLE_VALUE,
									 NULL,
									 PAGE_READ,
									 0,
									 sizeof(LOG_QUEUE),
									(LPCWSTR)LOG_NAME);
*/
	m_hMemMapLogW = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, (LPCWSTR)LOG_NAME_W); // 공유파일 오픈후..핸들러 넘김..

	if (m_hMemMapLogW != NULL && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		m_pQueueLogW = (P_LOG_QUEUE_W)MapViewOfFile(m_hMemMapLogW, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	}
	else
	{
		m_hMemMapLogW = CreateFileMapping(INVALID_HANDLE_VALUE,
									      NULL,
									      PAGE_READWRITE,
									      0,
									      sizeof(LOG_QUEUE_W),
									      (LPCWSTR)LOG_NAME_W);

		if (m_hMemMapLogW != NULL && GetLastError() != ERROR_ALREADY_EXISTS)
		{
			m_pQueueLogW = (P_LOG_QUEUE_W)MapViewOfFile(m_hMemMapLogW, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		}
	}
/*
	m_hMemMapLogR = CreateFileMapping(INVALID_HANDLE_VALUE,
									 NULL,
									 PAGE_READWRITE,
									 0,
									 sizeof(LOG_NAME_R),
									 (LPCWSTR)LOG_NAME_R);

	if (m_hMemMapLogR != NULL)
	{
		nRet = GetLastError();
		if (nRet != ERROR_ALREADY_EXISTS)
		{	
			m_pQueueLogR = (P_LOG_QUEUE_R)MapViewOfFile(m_hMemMapLogR, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (m_pQueueLogR != NULL)
			{
				m_pQueueLogR->m_nFront	= 0;
				m_pQueueLogR->m_nRear		= 0;
				m_pQueueLogR->m_nCount	= 0;
			}
		}
		else
		{
			m_pQueueLogR = (P_LOG_QUEUE_R)MapViewOfFile(m_hMemMapLogR, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (m_pQueueLogR != NULL)
			{
				m_pQueueLogR->m_nFront	= 0;
				m_pQueueLogR->m_nRear	= 0;
				m_pQueueLogR->m_nCount	= 0;
			}
		}
	}
*/
}


CMemoryCtrl::~CMemoryCtrl(void)
{
	if (m_pQueueLogW != NULL)	UnmapViewOfFile(m_pQueueLogW);	
	if (m_hMemMapLogW != NULL)	CloseHandle(m_hMemMapLogW);

//	if (m_pQueueLogR != NULL)	UnmapViewOfFile(m_pQueueLogR);	
//	if (m_hMemMapLogR != NULL)	CloseHandle(m_hMemMapLogR);
}


CString	CMemoryCtrl::CharToString(char chData[], int nLength)
{
	CString strTmp;
	BSTR	buf;
	int		nCnt;

	nCnt	= MultiByteToWideChar(CP_ACP, 0, chData, nLength, NULL, NULL);
	buf		= SysAllocStringLen(NULL, nCnt);
	MultiByteToWideChar(CP_ACP, 0, chData, nLength, buf, nCnt);
	strTmp.Format(_T("%s"), buf);

	SysFreeString(buf);

	return strTmp;
}


void CMemoryCtrl::StringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}

void CMemoryCtrl::CreateFolder(CString strPath)
{
	int nRet = 0;
	char chfilename[100];

    do
	{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            CString strTemp = strPath.Left(nRet+1);
			
			StringToChar(strTemp, chfilename);
            if (_access(chfilename.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
}

void CMemoryCtrl::ConfigOpen()
{
	CString strLoadFile = _T("C:\\Interface\\LogCtrl.TXT");
	CString strTemp;

	TCHAR chData[100];

	GetPrivateProfileString(_T("PATH"), _T("strPathSamsung"), _T("C:\\Interface\\Samsung"), chData, sizeof(chData), strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_log_info.strPathSamsung = strTemp;

	CreateFolder(st_log_info.strPathSamsung);

	GetPrivateProfileString(_T("PATH"), _T("strPathTotal"), _T("C:\\Interface\\Total"), chData, sizeof(chData), strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_log_info.strPathTotal = strTemp;

	CreateFolder(st_log_info.strPathTotal);

	GetPrivateProfileString(_T("PATH"), _T("strPathCommunication"), _T("C:\\Interface\\Com"), chData, sizeof(chData), strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_log_info.strPathCommunication = strTemp;

	CreateFolder(st_log_info.strPathCommunication);

	GetPrivateProfileString(_T("PATH"), _T("strPathAlarm"), _T("C:\\Interface\\Alarm"), chData, sizeof(chData), strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_log_info.strPathAlarm = strTemp;

	strTemp		= st_log_info.strPathAlarm + _T("\\Daily\\");
	CreateFolder(strTemp);

	strTemp = st_log_info.strPathAlarm + _T("\\Monthly\\");
	CreateFolder(strTemp);

	GetPrivateProfileString(_T("LOG_FILE"), _T("OLD_NAME"), _T(""), chData, sizeof(chData), strLoadFile);
	strTemp.Format(_T("%s"), chData);
	m_strLogFile = strTemp;

	GetPrivateProfileString(_T("ALARM"), _T("nAlarmTime"), _T(""), chData, sizeof(chData), strLoadFile);
	strTemp.Format(_T("%s"), chData);
	st_log_info.nAlarmTime = _wtoi(strTemp);
}

void CMemoryCtrl::ConfigSave()
{
	CString strSaveFile = _T("C:\\Interface\\LogCtrl.TXT");

	:: WritePrivateProfileString(_T("LOG_FILE"), _T("OLD_NAME"), LPCTSTR(m_strLogFile), strSaveFile);
}

void CMemoryCtrl::LogZip(CString strFileName)
{
	CFileFind find;

	CString strZipFileName;

	 strZipFileName = strFileName;
	if (m_strLogFile != strFileName)
	{
		strZipFileName = m_strLogFile;
		strZipFileName.Replace(_T("TXT"), _T("ZIP"));

		if (!find.FindFile(strZipFileName))
		{
			LogZip(m_strLogFile, strZipFileName);
		}
		m_strLogFile = strFileName;
	}
}

BOOL CMemoryCtrl::LogZip(CString strOriginFile, CString strZipFile)
{
	if (_taccess(strOriginFile, 04) != 0)
	{
		return FALSE;
	}

	TCHAR * cp = (TCHAR *)_tcsrchr(strOriginFile, _T('\\'));
	if (cp == NULL)
		cp = (TCHAR *) (LPCTSTR)strOriginFile;
	else
		cp++;

	HZIP hz = CreateZip((void *)(LPCTSTR)strZipFile, 0, ZIP_FILENAME);

	if (hz)
	{
		ZRESULT zr = ZipAdd(hz, cp, (void *)(LPCTSTR)strOriginFile, 0, ZIP_FILENAME);

		CloseZip(hz);
	}
	else return FALSE;

	return TRUE;
}

CString CMemoryCtrl::LogWrite()
{
	CFileException e;
	CStdioFile file;

	SYSTEMTIME Time;	

	CString strFile;
	CString strLog = _T("");
	CString strMsg = _T("");


	GetLocalTime(&Time);

	strFile.Format(_T("%s\\%04d%02d%02d%02d.TXT"), st_log_info.strPathSamsung,
												   Time.wYear, 
											       Time.wMonth, 
											       Time.wDay, 
											       Time.wHour);

	if (m_hMemMapLogW != NULL && m_pQueueLogW != NULL)
	{
		strMsg = CharToString(m_pQueueLogW->m_chMsg[m_pQueueLogW->m_nFront], 1000);

		m_pQueueLogW->m_nFront++;
		if (m_pQueueLogW->m_nFront >= 100) m_pQueueLogW->m_nFront = 0;
			
		m_pQueueLogW->m_nCount--;

		strLog = strMsg;
		strLog += _T("\r\n");

		if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone, &e))
		{
			e.ReportError();
			return strMsg;
		}
		file.SeekToEnd();
		file.WriteString(strLog);
		file.Close();

		LogZip(strFile);
	}

	return strMsg;
}

