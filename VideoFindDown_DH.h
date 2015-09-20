#pragma once
#include <windows.h>
#include "dhnetsdk.h"
#include "dhconfigsdk.h"

using namespace System;
using namespace System::Data;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
////process����
//using namespace System::Diagnostics; 

//#include "vlc/vlc.h"
#include <string.h>

//#pragma comment(lib, "../libvlc.lib")
//#pragma comment(lib, "../libvlccore.lib")

//libvlc_instance_t *   vlc_ins    =nullptr;
//libvlc_media_player_t *  vlc_player =nullptr;
//libvlc_media_t *   vlc_media  =nullptr;

/************************************************************************/
/*       ���������޸�                     */
/*       ��hPlayWnd��������HWND�޸�ΪIntPtr	  */
/************************************************************************/


namespace VideoFindDown_DH {

	ref struct NET_RECORDFILE_INFO_Managed
	{
		unsigned int        ch;                         // ͨ����
		//char              filename[124];              // �ļ���
		String^             filename;                    // �ļ���
		unsigned int        framenum;                   // �ļ���֡��
		unsigned int        size;                       // �ļ�����
		//NET_TIME            starttime;                  // ��ʼʱ��
		System::DateTime^   starttime;                   // ��ʼʱ��
		//NET_TIME            endtime;                   // ����ʱ��
		System::DateTime^   endtime;                    // ����ʱ��
		unsigned int        driveno;                    // ���̺�(��������¼��ͱ���¼������ͣ�0��127��ʾ����¼��,����64��ʾ����1��128��ʾ����¼��)
		unsigned int        startcluster;               // ��ʼ�غ�
		BYTE                nRecordFileType;            // ¼���ļ�����  0����ͨ¼��1������¼��2���ƶ���⣻3������¼��4��ͼƬ, 5: ����¼��
		BYTE                bImportantRecID;            // 0:��ͨ¼�� 1:��Ҫ¼��
		BYTE                bHint;                      // �ļ���λ����
		BYTE                bRecType;                   // 0-������¼�� 1-����1��¼�� 2-������2 3-������3¼��
	} ;


	//��DVRģ�鵼���ӿ�===========================================
	/*[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_Init();*/
	// SDK��ʼ��
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_Init(fDisConnect cbDisConnect, LDWORD dwUser);
	// SDK�˳�����
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) void CLIENT_Cleanup();
	// ���ö��������ɹ��ص����������ú�SDK�ڲ������Զ�����
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  void  CLIENT_SetAutoReconnect(fHaveReConnect cbAutoConnect, LDWORD dwUser);
	// ���ö�̬�����Ӷ��߻ص�������ĿǰSVR�豸�ļ��Ӻͻط��Ƕ����ӵġ�
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  void  CLIENT_SetSubconnCallBack(fSubDisConnect cbSubDisConnect, LDWORD dwUser);
	// ���غ���ִ��ʧ�ܴ���
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  DWORD  CLIENT_GetLastError(void);
	// ���������豸��ʱʱ��ͳ��Դ���
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  void  CLIENT_SetConnectTime(int nWaitTime, int nTryTimes);
	// ���豸ע��
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  LLONG  CLIENT_Login(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);
	// �໭��Ԥ��ʱnChannelID NVR�豸����Ƶ���ͨ����
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  LLONG  CLIENT_RealPlayEx(LLONG lLoginID, int nChannelID, IntPtr hWnd, DH_RealPlayType rType = DH_RType_Realplay);
	// ֹͣʵʱԤ��--��չ
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_StopRealPlayEx(LLONG lRealHandle);
	// ���豸ע��
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_Logout(LLONG lLoginID);
	// ����ѯ������ѯ�ļ�
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  LLONG  CLIENT_FindFileEx(LLONG lLoginID, EM_FILE_QUERY_TYPE emType, void* pQueryCondition, void *reserved, int waittime=1000);    
	// �����ļ�:nFilecount:��Ҫ��ѯ������, ����ֵΪý���ļ����� ����ֵ<nFilecount����Ӧʱ����ڵ��ļ���ѯ���
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  int   CLIENT_FindNextFileEx(LLONG lFindHandle, int nFilecount, void* pMediaFileInfo, int maxlen, void *reserved, int waittime=1000);
	// ����¼���ļ�����
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_FindCloseEx(LLONG lFindHandle);
	// ��ȡ���ϲ�ѯ�������ļ�����
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_GetTotalFileCount(LLONG lFindHandle, int* pTotalCount, void *reserved, int waittime=1000);
	// ����¼���ļ�����
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_FindClose(LLONG lFindHandle);
	// ֹͣ¼��ط�
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_StopPlayBack(LLONG lPlayHandle);
	// ��ʼ����¼���ļ�
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) LLONG  CLIENT_FindFile(LLONG lLoginID, int nChannelId, int nRecordFileType, char* cardid, LPNET_TIME time_start, LPNET_TIME time_end, BOOL bTime, int waittime);
	// ����¼���ļ�
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) int   CLIENT_FindNextFile(LLONG lFindHandle,LPNET_RECORDFILE_INFO lpFindData);
	// ����¼���ļ�
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) LLONG CLIENT_DownloadByRecordFile(LLONG lLoginID,LPNET_RECORDFILE_INFO lpRecordFile, char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, LDWORD dwUserData);
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_CapturePicture(LLONG  hPlayHandle,const char  *pchPicFileName);
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_SaveRealData(LLONG  lRealHandle,const char *pchFileName);
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_StopSaveRealData(LLONG lRealHandle);

	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_GetDevConfig(
		LLONG    lLoginID,
		DWORD    dwCommand,
		LONG     lChannel,
		LPVOID   lpOutBuffer,
		DWORD    dwOutBufferSize,
		LPDWORD  lpBytesReturned,
		int      waittime=500
		);

	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_GetNewDevConfig(
		LLONG     lLoginID,
		char     *szCommand,
		int       nChannelID,
		char     *szOutBuffer,
		DWORD     dwOutBufferSize,
		int      *error,
		int       waittime=500
		);

	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)BOOL CLIENT_QuerySystemInfo(
		LLONG  lLoginID,
		int    nSystemType,
		char   *pSysInfoBuffer,
		int    maxlen,
		int    *nSysInfolen,
		int    waittime=1000
		);


	[DllImport("dhconfigsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_ParseData(
		char                                    *szCommand ,
		char                                    *szInBuffer ,
		LPVOID                                  lpOutBuffer ,
		DWORD                                   dwOutBufferSize ,
		void                                    *pReserved
		);





	//////////////////////////////////////////////////////////////////////////
	//added function,copied from HK example
	//�йܷ��й�ת������
	//copied from fengyuan realtimeplayer
	char* ManagedString2UnmanagedCharPtr(String^ strIn);
	//�����йܵ�ANSI�ַ���ת�����й��ַ� �� 
	String^ UnmanagedStringA2ManagedString(char *pIn);
	//c�ӿڣ�
	//int saveRecordFile(int userId,char * srcfile,char * destfile);//��Ϊsdk�ṩ�����̣�ʵ�ʲ�ʹ��
	//int findRecordFile();//��Ϊsdk�ṩ�����̣�ʵ�ʲ�ʹ��
	//int findAndDownFileExample();//��Ϊsdk�ṩ�����̣�ʵ�ʲ�ʹ��
	//int findAndDownFilebyTimeExample();
	//////////////////////////////////////////////////////////////////////////
	//�󻪵Ķ��������ص�����
	//���߻ص�����
	void __stdcall DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);
	//�Զ������ص�����
	void __stdcall HaveReConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);
	//�������Զ������ص�����
	void __stdcall SubHaveReConnectFunc(EM_INTERFACE_TYPE emInterfaceType, BOOL bOnline, LLONG lOperateHandle, LLONG lLoginID, LDWORD dwUser);
	//����Ϊ¼���ѯʱ�õĻص�����
	void __stdcall DownLoadPosCallBackFunc(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser);
	int __stdcall DataCallBackFunc(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);
	//���ؽ��Ȼص�����
	void __stdcall cbDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
	//cli�ӿ�

	


	//////////////////////////////////////////////////////////////////////////

public ref class VideoFindDown_DHer
{
public:
	VideoFindDown_DHer(void);
	//VideoFindDown_HKer()
	//added 
public:
	
	void Net_DVR_Play(LONG lUserID,LONG nChan, IntPtr render_wnd);
	ArrayList^ DVR_AddTest(String ^ s_ip,String ^s_userName, String ^ s_userPassword);
	/*bool cmdIP(String ^strip);*/

	//added by dongjiaquan
	int searchRecordList(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,ArrayList/*<NET_DVR_FINDDATA_V40^>*/^&  recordList);
	int startRealPlay(String ^ s_ip,String ^s_userName, String ^ s_userPassword,IntPtr render_wnd,WORD wDvrPort,String^ sChannel);
	int stopRealPlay(void);
	//����в��ҵ���¼�������Ϣ��ִ������
	int downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,NET_RECORDFILE_INFO_Managed^ recordInfo,String ^ destfile,int* pnPos);
	//���ֻ���ļ������ڲ���ִ��һ����ң���ִ������----δд�꣬�и��õĽ�������������˷���
	//int downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,char* srcfile ,String ^ destfile);
	bool isRealPlay();
	LLONG getUserID();
	LLONG getRealPlayHandle();
public:
	BOOL capturePicInRealPlay(String^ sFilename);//��֧��bmpģʽ
	//���ڲ�������Ч������ͬ��
	BOOL startSaverRealData(String^ sFilename);
	BOOL stopSaveRealData();
	bool IsSaveRealData();
public:
	//ArrayList^ DVR_AddTest(String ^ s_ip,WORD d_port, String ^s_userName, String ^ s_userPassword);
	ArrayList^ DVR_AddTestEx(String ^ s_ip, WORD w_port, String ^ & s_userName, String ^ s_userPassword, int & AnChanNum,int &  IPChanNum, int &  AnChanStartNum,int &  IPChanStartNum,int& nError);
private:
	int searchRecordList(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,/*std::vector<NET_DVR_FINDDATA_V40*>&*/ ArrayList^&  recordList);
	int startRealPlay(char * ip,char * userName, char * userPassword,IntPtr render_wnd,WORD wDvrPort,int nChannel);
	//����в��ҵ���¼�������Ϣ��ִ������
	int downRecordFile(char * ip,char * userName, char * userPassword,WORD wDvrPort,NET_RECORDFILE_INFO_Managed^ recordInfo,char * destfile,int* pnPos);
	//���ֻ���ļ������ڲ���ִ��һ����ң���ִ������----δд�꣬�и��õĽ�������������˷���
	//int downRecordFile(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,char* srcfile,char * destfile);
private:
	//LONG m_lUserID;
	//LONG m_lRealPlayHandle;
	LLONG m_lLoginHandle;
	LLONG m_lRealHandle;
	bool m_bIsRealPlay;
	bool m_bIsSaveRealData;
};


}

