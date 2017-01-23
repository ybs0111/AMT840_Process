#pragma once

typedef struct _LOG_QUEUE_W_
{
	int			m_nRear;
	int			m_nFront;
	int			m_nCount;

	char		m_chMsg[100][1000];
} LOG_QUEUE_W, *P_LOG_QUEUE_W;
/*
typedef struct _LOG_QUEUE_R_
{
	int			m_nRear;
	int			m_nFront;
	int			m_nCount;
} LOG_QUEUE_R, *P_LOG_QUEUE_R;
*/
/*
typedef struct _BARCODE_QUEUE_
{
	int			m_nTrigger[5];	// 

	char		m_chTrigger[5]; // trigger string.......
	char		m_chData[5][200];
	char		m_chErrorMsg[100];
} BARCODE_QUEUE, *P_BARCODE_QUEUE;

typedef struct _RFID_QUEUE_
{
	int			m_nTrigger; // 
	int			m_nReadWrite; // 0 Read 1 Write........

	char		m_chMsg[30][200];
	char		m_chErrorMsg[100];
} RFID_QUEUE, *P_RFID_QUEUE;

typedef struct _VNR_QUEUE_
{
	float		m_dVnR[10];			// VnR 데이터......

	char		m_chErrorMsg[100];
} VNR_QUEUE, *P_VNR_QUEUE;
*/
class CMemoryCtrl
{
public:
	// log file 관련 
	HANDLE				m_hMemMapLogW;
	P_LOG_QUEUE_W		m_pQueueLogW;
//	HANDLE				m_hMemMapLogR;
//	P_LOG_QUEUE_R		m_pQueueLogR;
/*
	// barcode 관련 
	HANDLE					m_hMemMapBarcode;
	P_BARCODE_QUEUE			m_pQueueBarcode;

	// rfid 관련 
	HANDLE					m_hMemMapRfid;
	P_RFID_QUEUE			m_pQueueRfid;

	// vnr 관련 
	HANDLE					m_hMemMapVnR;
	P_VNR_QUEUE				m_pQueueVnR;
*/
	CString					m_strLogFile;
	
	CString					CharToString(char chData[], int nLength);
	void					StringToChar(CString strData, char chData[]);
	void					CreateFolder(CString strPath);
	void					ConfigOpen();
	void					ConfigSave();
	void					LogZip(CString strFileName);
	BOOL					LogZip(CString strOriginFile, CString strZipFile);
	CString					LogWrite();
public:
	CMemoryCtrl(void);
	~CMemoryCtrl(void);
};

extern CMemoryCtrl clsMemCtrl;

