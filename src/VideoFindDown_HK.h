#pragma once
#include <windows.h>
#include "HCNetSDK.h"

using namespace System;
using namespace System::Data;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
////process进程
//using namespace System::Diagnostics; 

//#include "vlc/vlc.h"
#include <string.h>

//#pragma comment(lib, "../libvlc.lib")
//#pragma comment(lib, "../libvlccore.lib")

//libvlc_instance_t *   vlc_ins    =nullptr;
//libvlc_media_player_t *  vlc_player =nullptr;
//libvlc_media_t *   vlc_media  =nullptr;

/************************************************************************/
/*       参数类型修改                     */
/*       将hPlayWnd的类型由HWND修改为IntPtr	  */
/************************************************************************/


namespace VideoFindDown_HK {

	//软解码预览参数
	typedef struct
	{
		LONG lChannel;//通道号
		LONG lLinkMode;//最高位(31)为0表示主码流，为1表示子，0－30位表示码流连接方式: 0：TCP方式,1：UDP方式,2：多播方式,3 - RTP方式，4-RTP/RTSP,5-RSTP/HTTP 
		IntPtr hPlayWnd;//播放窗口的句柄,为NULL表示不播放图象
		char* sMultiCastIP;//多播组地址																	    
	}NET_DVR_CLIENTINFO2, *LPNET_DVR_CLIENTINFO2;

	//录像查找信息结构体
	typedef struct 
	{
		char             sFileName[100];//文件名，日历查询时无效 
		NET_DVR_TIME     struStartTime;//文件的开始时间 
		NET_DVR_TIME     struStopTime;//文件的结束时间
		DWORD            dwFileSize;//文件大小
		char             sCardNum[32];//卡号 
		BYTE             byLocked;//文件是否被锁定，1－文件已锁定；0－文件未锁定
		BYTE             byFileType;//文件类型（日历查询时无效）：0-定时录像，1-移动侦测，2-报警触发，3-报警|移动侦测，4-报警&移动侦测，5-命令触发，6-手动录像，7-震动报警，8-环境报警，9-智能报警，10-PIR报警，11-无线报警，12-呼救报警，13-移动侦测、PIR、无线、呼救等所有报警类型的"或"，14-智能交通事件，15-越界侦测，16-区域入侵，17-声音异常，18-场景变更侦测
		BYTE             byQuickSearch;//0- 普通查询结果，1- 快速（日历）查询结果 
		BYTE             byRes;
		DWORD            dwFileIndex;//文件索引号 
		BYTE             byRes1[128];
	}NET_DVR_FINDDATA_V40,*LPNET_DVR_FINDDATA_V40;

	//文件查找信息结构体。
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

	//校时结构参数
	//typedef ref struct
	//{
	//	DWORD dwYear;		//年
	//	DWORD dwMonth;		//月
	//	DWORD dwDay;		//日
	//	DWORD dwHour;		//时
	//	DWORD dwMinute;		//分
	//	DWORD dwSecond;		//秒
	//}NET_DVR_TIME_managed, ^LPNET_DVR_TIME_managed;
	//录象文件参数(cvr)
	ref struct NET_DVR_FINDDATA_V40_managed
	{
		String^ sFileName;//文件名
		//NET_DVR_TIME struStartTime;//文件的开始时间
		//NET_DVR_TIME struStopTime;//文件的结束时间
		System::DateTime^ struStartTime;//文件的开始时间
		System::DateTime^ struStopTime;//文件的结束时间
		DWORD dwFileSize;//文件的大小
		//String^ sCardNum;
		//BYTE byLocked;//9000设备支持,1表示此文件已经被锁定,0表示正常的文件
		//BYTE byFileType;  //文件类型:0－定时录像,1-移动侦测 ，2－报警触发，
		////3-报警|移动侦测 4-报警&移动侦测 5-命令触发 6-手动录像,7－震动报警，8-环境报警，9-智能报警，10-PIR报警，11-无线报警，12-呼救报警,14-智能交通事件
		//BYTE byQuickSearch; //0:普通查询结果，1：快速（日历）查询结果
		//BYTE byRes;
		//DWORD dwFileIndex; //文件索引号
		////BYTE byRes1[128];	
		//ArrayList<BYTE>  byRes1[128];
	};//NET_DVR_FINDDATA_V40_managed, ^LPNET_DVR_FINDDATA_V40_managed;

	//海康DVR模块===========================================
    //原来realTime_Player加入的
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
	//董家泉：为了可以查找、下载录像文件加入的
	//time：2015.2.5.20:30
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
	//c接口：
	int saveRecordFile(int userId,char * srcfile,char * destfile);//作为sdk提供的例程，实际不使用
	int findRecordFile();//作为sdk提供的例程，实际不使用
	//cli接口


	//copied from fengyuan realtimeplayer
	char* ManagedString2UnmanagedCharPtr(String^ strIn);
	//将非托管的ANSI字符串转换成托管字符 串 
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
	//最后一个参数为下载进度
	bool isRealPlay();
	LONG getUserID();
	LONG getRealPlayHandle();
public:
	BOOL capturePicInRealPlay(String^ filename);
	//在实时播放中截图功能，filename为全路径名（后缀只能为jpg或bmp，填错则返回失败)，这个函数因为支持jpg额外慢一点
	BOOL capturePicInRealPlay_onlyBmp(String^ filename);
	//在实时播放中截图功能，filename为全路径名（后缀只能为bmp，，填错则安填错的文件名命名)，这个函数因为仅支持默认的bmp会快一些
	BOOL startSaveRealData(String^ sFileName);
	//在实时播放中开始保存实时数据，filename为全路径名
	BOOL stopSaveRealData();
	//在实时播放中停止保存实时数据
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

