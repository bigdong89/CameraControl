#pragma once
#include <windows.h>
#include "HCNetSDK.h"

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


namespace VideoFindDown_HK {

	//�����Ԥ������
	typedef struct
	{
		LONG lChannel;//ͨ����
		LONG lLinkMode;//���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ�ӣ�0��30λ��ʾ�������ӷ�ʽ: 0��TCP��ʽ,1��UDP��ʽ,2���ಥ��ʽ,3 - RTP��ʽ��4-RTP/RTSP,5-RSTP/HTTP 
		IntPtr hPlayWnd;//���Ŵ��ڵľ��,ΪNULL��ʾ������ͼ��
		char* sMultiCastIP;//�ಥ���ַ																	    
	}NET_DVR_CLIENTINFO2, *LPNET_DVR_CLIENTINFO2;

	//¼�������Ϣ�ṹ��
	typedef struct 
	{
		char             sFileName[100];//�ļ�����������ѯʱ��Ч 
		NET_DVR_TIME     struStartTime;//�ļ��Ŀ�ʼʱ�� 
		NET_DVR_TIME     struStopTime;//�ļ��Ľ���ʱ��
		DWORD            dwFileSize;//�ļ���С
		char             sCardNum[32];//���� 
		BYTE             byLocked;//�ļ��Ƿ�������1���ļ���������0���ļ�δ����
		BYTE             byFileType;//�ļ����ͣ�������ѯʱ��Ч����0-��ʱ¼��1-�ƶ���⣬2-����������3-����|�ƶ���⣬4-����&�ƶ���⣬5-�������6-�ֶ�¼��7-�𶯱�����8-����������9-���ܱ�����10-PIR������11-���߱�����12-���ȱ�����13-�ƶ���⡢PIR�����ߡ����ȵ����б������͵�"��"��14-���ܽ�ͨ�¼���15-Խ����⣬16-�������֣�17-�����쳣��18-����������
		BYTE             byQuickSearch;//0- ��ͨ��ѯ�����1- ���٣���������ѯ��� 
		BYTE             byRes;
		DWORD            dwFileIndex;//�ļ������� 
		BYTE             byRes1[128];
	}NET_DVR_FINDDATA_V40,*LPNET_DVR_FINDDATA_V40;

	//�ļ�������Ϣ�ṹ�塣
	//struct{
	//	LONG                 lChannel;  
	//	DWORD                dwFileType;  
	//	DWORD                dwIsLocked;  
	//	DWORD                dwUseCardNo;  
	//	BYTE                 sCardNumber[CARDNUM_LEN_OUT];   
	//	NET_DVR_TIME         struStartTime;     
	//	NET_DVR_TIME         struStopTime; 
	//	BYTE                 byDrawFrame; 
	//	BYTE                 byFindType; 
	//	BYTE                 byQuickSearch; 
	//	BYTE                 byRes1; 
	//	DWORD                dwVolumeNum; 
	//	BYTE                 byWorkingDeviceGUID[GUID_LEN]; 
	//	BYTE                 byRes2[40]; 
	//}NET_DVR_FILECOND_V40, *LPNET_DVR_FILECOND_V40;

	//Уʱ�ṹ����
	//typedef ref struct
	//{
	//	DWORD dwYear;		//��
	//	DWORD dwMonth;		//��
	//	DWORD dwDay;		//��
	//	DWORD dwHour;		//ʱ
	//	DWORD dwMinute;		//��
	//	DWORD dwSecond;		//��
	//}NET_DVR_TIME_managed, ^LPNET_DVR_TIME_managed;
	//¼���ļ�����(cvr)
	ref struct NET_DVR_FINDDATA_V40_managed
	{
		String^ sFileName;//�ļ���
		//NET_DVR_TIME struStartTime;//�ļ��Ŀ�ʼʱ��
		//NET_DVR_TIME struStopTime;//�ļ��Ľ���ʱ��
		System::DateTime^ struStartTime;//�ļ��Ŀ�ʼʱ��
		System::DateTime^ struStopTime;//�ļ��Ľ���ʱ��
		DWORD dwFileSize;//�ļ��Ĵ�С
		//String^ sCardNum;
		//BYTE byLocked;//9000�豸֧��,1��ʾ���ļ��Ѿ�������,0��ʾ�������ļ�
		//BYTE byFileType;  //�ļ�����:0����ʱ¼��,1-�ƶ���� ��2������������
		////3-����|�ƶ���� 4-����&�ƶ���� 5-����� 6-�ֶ�¼��,7���𶯱�����8-����������9-���ܱ�����10-PIR������11-���߱�����12-���ȱ���,14-���ܽ�ͨ�¼�
		//BYTE byQuickSearch; //0:��ͨ��ѯ�����1�����٣���������ѯ���
		//BYTE byRes;
		//DWORD dwFileIndex; //�ļ�������
		////BYTE byRes1[128];	
		//ArrayList<BYTE>  byRes1[128];
	};//NET_DVR_FINDDATA_V40_managed, ^LPNET_DVR_FINDDATA_V40_managed;

	//����DVRģ��===========================================
    //ԭ��realTime_Player�����
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_Init();
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_Cleanup();
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  LONG  NET_DVR_Login_V30(char *sDVRIP, WORD wDVRPort, char *sUserName, char *sPassword, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo); 	
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_Logout(LONG lUserID);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  DWORD NET_DVR_GetLastError();
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  char* NET_DVR_GetErrorMsg(LONG *pErrorNo = NULL);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  LONG  NET_DVR_RealPlay_V30(LONG lUserID, LPNET_DVR_CLIENTINFO2 lpClientInfo, void(CALLBACK *fRealDataCallBack_V30) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser) = NULL, void* pUser = NULL, BOOL bBlocked = FALSE);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  LONG  NET_DVR_StopRealPlay(LONG lRealHandle);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_SaveRealData(LONG lRealHandle,char *sFileName);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_StopSaveRealData(LONG lRealHandle );
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_GetDVRConfig(LONG lUserID, DWORD dwCommand,LONG lChannel, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned);
	//����Ȫ��Ϊ�˿��Բ��ҡ�����¼���ļ������
	//time��2015.2.5.20:30
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport) BOOL NET_DVR_StopGetFile(LONG lFileHandle);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  int  NET_DVR_GetDownloadPos(LONG lFileHandle);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  BOOL  NET_DVR_PlayBackControl_V40(LONG lPlayHandle,DWORD dwControlCode, LPVOID lpInBuffer = NULL, DWORD dwInLen = 0, LPVOID lpOutBuffer = NULL, DWORD *lpOutLen = NULL);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  LONG  NET_DVR_GetFileByName(LONG lUserID,char *sDVRFileName,char *sSavedFileName);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)   BOOL  NET_DVR_StopGetFile(LONG lFileHandle);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  int  NET_DVR_GetDownloadPos(LONG lFileHandle);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  BOOL  NET_DVR_PlayBackControl(LONG lPlayHandle,DWORD dwControlCode,DWORD dwInValue,DWORD *LPOutValue);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)   LONG  NET_DVR_FindNextFile_V30(LONG lFindHandle, LPNET_DVR_FINDDATA_V30 lpFindData);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  LONG  NET_DVR_FindNextFile_V40(LONG lFindHandle, VideoFindDown_HK::LPNET_DVR_FINDDATA_V40 lpFindData);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)   BOOL  NET_DVR_FindClose_V30(LONG lFindHandle);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport) BOOL  NET_DVR_SetReconnect(DWORD dwInterval = 30000, BOOL bEnableRecon = TRUE);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  BOOL  NET_DVR_SetConnectTime(DWORD dwWaitTime = 3000, DWORD dwTryTimes = 3);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)  BOOL  NET_DVR_FindClose_V30(LONG lFindHandle);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)   LONG  NET_DVR_FindFile_V40(LONG lUserID, LPNET_DVR_FILECOND_V40 pFindCond);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport) BOOL NET_DVR_SetCapturePictureMode(DWORD    dwCaptureMode);
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport) BOOL NET_DVR_CapturePicture(LONG lRealHandle, char *sPicFileName);

	/*[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)
	[DllImport("HCNetSDK.dll")]
	extern "C" _declspec(dllimport)*/


	//added function,copied from HK example
	//c�ӿڣ�
	int saveRecordFile(int userId,char * srcfile,char * destfile);//��Ϊsdk�ṩ�����̣�ʵ�ʲ�ʹ��
	int findRecordFile();//��Ϊsdk�ṩ�����̣�ʵ�ʲ�ʹ��
	//cli�ӿ�


	//copied from fengyuan realtimeplayer
	char* ManagedString2UnmanagedCharPtr(String^ strIn);
	//�����йܵ�ANSI�ַ���ת�����й��ַ� �� 
	String^ UnmanagedStringA2ManagedString(char *pIn);

public ref class VideoFindDown_HKer
{
public:
	VideoFindDown_HKer(void);
	//VideoFindDown_HKer()
	//added 
public:
	void HKNet_DVR_Play(LONG lUserID,LONG nChan, IntPtr render_wnd);
	ArrayList^ HKDVR_AddTest(String ^ s_ip,String ^s_userName, String ^ s_userPassword);
	/*bool cmdIP(String ^strip);*/

	//added by dongjiaquan
	int searchRecordList(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,ArrayList/*<NET_DVR_FINDDATA_V40^>*/^&  recordList);
	int startRealPlay_HK(String ^ s_ip,String ^s_userName, String ^ s_userPassword,IntPtr render_wnd,WORD wDvrPort,String^ sChannel);
	int stopRealPlay_HK(void);
	//int restartRealPlay_HK();
	int downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,String ^ srcfile,String ^ destfile,int* nPos);
	//���һ������Ϊ���ؽ���
	bool isRealPlay();
	LONG getUserID();
	LONG getRealPlayHandle();
public:
	BOOL capturePicInRealPlay(String^ filename);
	//��ʵʱ�����н�ͼ���ܣ�filenameΪȫ·��������׺ֻ��Ϊjpg��bmp������򷵻�ʧ��)�����������Ϊ֧��jpg������һ��
	BOOL capturePicInRealPlay_onlyBmp(String^ filename);
	//��ʵʱ�����н�ͼ���ܣ�filenameΪȫ·��������׺ֻ��Ϊbmp��������������ļ�������)�����������Ϊ��֧��Ĭ�ϵ�bmp���һЩ
	BOOL startSaveRealData(String^ sFileName);
	//��ʵʱ�����п�ʼ����ʵʱ���ݣ�filenameΪȫ·����
	BOOL stopSaveRealData();
	//��ʵʱ������ֹͣ����ʵʱ����
	bool IsSaveRealData();
public:
	ArrayList^ HKDVR_AddTest(String ^ s_ip,WORD d_port, String ^s_userName, String ^ s_userPassword);
	ArrayList^ HKDVR_AddTestEx(String ^ s_ip, WORD w_port, String ^ & s_userName, String ^ s_userPassword, int & AnChanNum,int &  IPChanNum, int &  AnChanStartNum,int &  IPChanStartNum,int& nError);
private:
	int searchRecordList(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,/*std::vector<NET_DVR_FINDDATA_V40*>&*/ ArrayList^&  recordList);
	int startRealPlay_HK(char * ip,char * userName, char * userPassword,IntPtr render_wnd,WORD wDvrPort,int nChannel);
	int downRecordFile(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,char * srcfile,char * destfile,int* nPos);
private:
	LONG m_lUserID;
	LONG m_lRealPlayHandle;
	bool m_bIsRealPlay;
	bool m_bIsSaveRealData;
private:
	String^ m_ip;
	String^ m_userName;
	String^ m_userPassword;
	WORD m_wDvrPort;
	String^ m_sChannel;
	IntPtr m_render_wnd;
};

}

