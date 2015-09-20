#pragma once
#include <windows.h>
#include "dhnetsdk.h"
#include "dhconfigsdk.h"

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


namespace VideoFindDown_DH {

	ref struct NET_RECORDFILE_INFO_Managed
	{
		unsigned int        ch;                         // 通道号
		//char              filename[124];              // 文件名
		String^             filename;                    // 文件名
		unsigned int        framenum;                   // 文件总帧数
		unsigned int        size;                       // 文件长度
		//NET_TIME            starttime;                  // 开始时间
		System::DateTime^   starttime;                   // 开始时间
		//NET_TIME            endtime;                   // 结束时间
		System::DateTime^   endtime;                    // 结束时间
		unsigned int        driveno;                    // 磁盘号(区分网络录像和本地录像的类型，0－127表示本地录像,其中64表示光盘1，128表示网络录像)
		unsigned int        startcluster;               // 起始簇号
		BYTE                nRecordFileType;            // 录象文件类型  0：普通录象；1：报警录象；2：移动检测；3：卡号录象；4：图片, 5: 智能录像
		BYTE                bImportantRecID;            // 0:普通录像 1:重要录像
		BYTE                bHint;                      // 文件定位索引
		BYTE                bRecType;                   // 0-主码流录像 1-辅码1流录像 2-辅码流2 3-辅码流3录像
	} ;


	//大华DVR模块导出接口===========================================
	/*[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  bool  NET_DVR_Init();*/
	// SDK初始化
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) BOOL CLIENT_Init(fDisConnect cbDisConnect, LDWORD dwUser);
	// SDK退出清理
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) void CLIENT_Cleanup();
	// 设置断线重连成功回调函数，设置后SDK内部断线自动重连
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  void  CLIENT_SetAutoReconnect(fHaveReConnect cbAutoConnect, LDWORD dwUser);
	// 设置动态子连接断线回调函数，目前SVR设备的监视和回放是短连接的。
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  void  CLIENT_SetSubconnCallBack(fSubDisConnect cbSubDisConnect, LDWORD dwUser);
	// 返回函数执行失败代码
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  DWORD  CLIENT_GetLastError(void);
	// 设置连接设备超时时间和尝试次数
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  void  CLIENT_SetConnectTime(int nWaitTime, int nTryTimes);
	// 向设备注册
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  LLONG  CLIENT_Login(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);
	// 多画面预览时nChannelID NVR设备填视频输出通道号
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  LLONG  CLIENT_RealPlayEx(LLONG lLoginID, int nChannelID, IntPtr hWnd, DH_RealPlayType rType = DH_RType_Realplay);
	// 停止实时预览--扩展
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_StopRealPlayEx(LLONG lRealHandle);
	// 向设备注销
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_Logout(LLONG lLoginID);
	// 按查询条件查询文件
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  LLONG  CLIENT_FindFileEx(LLONG lLoginID, EM_FILE_QUERY_TYPE emType, void* pQueryCondition, void *reserved, int waittime=1000);    
	// 查找文件:nFilecount:需要查询的条数, 返回值为媒体文件条数 返回值<nFilecount则相应时间段内的文件查询完毕
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  int   CLIENT_FindNextFileEx(LLONG lFindHandle, int nFilecount, void* pMediaFileInfo, int maxlen, void *reserved, int waittime=1000);
	// 结束录像文件查找
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_FindCloseEx(LLONG lFindHandle);
	// 获取符合查询条件的文件总数
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_GetTotalFileCount(LLONG lFindHandle, int* pTotalCount, void *reserved, int waittime=1000);
	// 结束录像文件查找
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_FindClose(LLONG lFindHandle);
	// 停止录像回放
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport)  BOOL  CLIENT_StopPlayBack(LLONG lPlayHandle);
	// 开始查找录像文件
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) LLONG  CLIENT_FindFile(LLONG lLoginID, int nChannelId, int nRecordFileType, char* cardid, LPNET_TIME time_start, LPNET_TIME time_end, BOOL bTime, int waittime);
	// 查找录像文件
	[DllImport("dhnetsdk.dll")]
	extern "C" _declspec(dllimport) int   CLIENT_FindNextFile(LLONG lFindHandle,LPNET_RECORDFILE_INFO lpFindData);
	// 下载录像文件
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
	//托管非托管转换函数
	//copied from fengyuan realtimeplayer
	char* ManagedString2UnmanagedCharPtr(String^ strIn);
	//将非托管的ANSI字符串转换成托管字符 串 
	String^ UnmanagedStringA2ManagedString(char *pIn);
	//c接口：
	//int saveRecordFile(int userId,char * srcfile,char * destfile);//作为sdk提供的例程，实际不使用
	//int findRecordFile();//作为sdk提供的例程，实际不使用
	//int findAndDownFileExample();//作为sdk提供的例程，实际不使用
	//int findAndDownFilebyTimeExample();
	//////////////////////////////////////////////////////////////////////////
	//大华的断线重连回调函数
	//断线回调函数
	void __stdcall DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);
	//自动重连回调函数
	void __stdcall HaveReConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);
	//子连接自动重连回调函数
	void __stdcall SubHaveReConnectFunc(EM_INTERFACE_TYPE emInterfaceType, BOOL bOnline, LLONG lOperateHandle, LLONG lLoginID, LDWORD dwUser);
	//以下为录像查询时用的回调函数
	void __stdcall DownLoadPosCallBackFunc(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser);
	int __stdcall DataCallBackFunc(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);
	//下载进度回调函数
	void __stdcall cbDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser);
	//cli接口

	


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
	//如果有查找到的录像相关信息，执行下行
	int downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,NET_RECORDFILE_INFO_Managed^ recordInfo,String ^ destfile,int* pnPos);
	//如果只有文件名（内部再执行一遍查找），执行下行----未写完，有更好的解决方法，放弃此方法
	//int downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,char* srcfile ,String ^ destfile);
	bool isRealPlay();
	LLONG getUserID();
	LLONG getRealPlayHandle();
public:
	BOOL capturePicInRealPlay(String^ sFilename);//仅支持bmp模式
	//仅在播放中有效，以下同上
	BOOL startSaverRealData(String^ sFilename);
	BOOL stopSaveRealData();
	bool IsSaveRealData();
public:
	//ArrayList^ DVR_AddTest(String ^ s_ip,WORD d_port, String ^s_userName, String ^ s_userPassword);
	ArrayList^ DVR_AddTestEx(String ^ s_ip, WORD w_port, String ^ & s_userName, String ^ s_userPassword, int & AnChanNum,int &  IPChanNum, int &  AnChanStartNum,int &  IPChanStartNum,int& nError);
private:
	int searchRecordList(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,/*std::vector<NET_DVR_FINDDATA_V40*>&*/ ArrayList^&  recordList);
	int startRealPlay(char * ip,char * userName, char * userPassword,IntPtr render_wnd,WORD wDvrPort,int nChannel);
	//如果有查找到的录像相关信息，执行下行
	int downRecordFile(char * ip,char * userName, char * userPassword,WORD wDvrPort,NET_RECORDFILE_INFO_Managed^ recordInfo,char * destfile,int* pnPos);
	//如果只有文件名（内部再执行一遍查找），执行下行----未写完，有更好的解决方法，放弃此方法
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

