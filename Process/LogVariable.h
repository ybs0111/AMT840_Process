#define	WM_ICON_NOTIFY			WM_USER+10       // 사용자 정의 메세지

struct tagLOG_INFO
{
	CString			strPathSamsung;
	CString			strPathTotal;
	CString			strPathAlarm;
	CString			strPathCommunication;

	int				nAlarmTime;
};
extern struct tagLOG_INFO st_log_info;

