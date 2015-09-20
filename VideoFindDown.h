#pragma once
#include <windows.h>
#include "VideoFindDown_DH.h"
#include "VideoFindDown_HK.h"

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


namespace VideoFindDown {

	ref struct COMMON_RECORDINFO
	{
		String^ sFileName;//文件名
		System::DateTime^ struStartTime;//文件的开始时间
		System::DateTime^ struStopTime;//文件的结束时间
		DWORD dwFileSize;//文件的大小

		//大华特有的类型
		unsigned int        ch;                         // 通道号
		unsigned int        framenum;                   // 文件总帧数
		unsigned int        driveno;                    // 磁盘号(区分网络录像和本地录像的类型，0－127表示本地录像,其中64表示光盘1，128表示网络录像)
		unsigned int        startcluster;               // 起始簇号
		BYTE                nRecordFileType;            // 录象文件类型  0：普通录象；1：报警录象；2：移动检测；3：卡号录象；4：图片, 5: 智能录像
		BYTE                bImportantRecID;            // 0:普通录像 1:重要录像
		BYTE                bHint;                      // 文件定位索引
		BYTE                bRecType;                   // 0-主码流录像 1-辅码1流录像 2-辅码流2 3-辅码流3录像

		//额外增加的属性
		//String^ sCamID;//用以唯一确定摄像机
		int indexofDev;//在设备表中的索引号
		
	};


	//////////////////////////////////////////////////////////////////////////
	//added function,copied from HK example
	//托管非托管转换函数
	//copied from fengyuan realtimeplayer
	char* ManagedString2UnmanagedCharPtr(String^ strIn);
	//将非托管的ANSI字符串转换成托管字符 串 
	String^ UnmanagedStringA2ManagedString(char *pIn);

	//////////////////////////////////////////////////////////////////////////
public ref class VideoFindDowner
{
public:
	VideoFindDowner(void);
	//VideoFindDown_HKer()
	//added 
public:
	//added by dongjiaquan
	int searchRecordList(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,ArrayList/*<NET_DVR_FINDDATA_V40^>*/^&  recordList,String^ devSupplier,int indexofDev);
	int startRealPlay(String ^ s_ip,String ^s_userName, String ^ s_userPassword,IntPtr render_wnd,WORD wDvrPort,String^ sChannel,String^ devSupplier);
	int stopRealPlay(void);
	int restartRealPlay();//执行过startRealPlay后可依据以前的信息重启播放
	//如果有查找到的录像相关信息，执行下行
	int downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG nChannel,COMMON_RECORDINFO^ recordInfo,String ^ destfile,String^ devSupplier,int* nPos);
	//最后一个参数为输出的下载百分比
	bool isRealPlay();
	LLONG getUserID();
	LLONG getRealPlayHandle();
public:
	BOOL capturePicInRealPlay(String^ sFilename);//仅支持bmp模式（后缀只能为bmp，，填错则安填错的文件名命名,实质仍是bmp)
	//仅在播放中有效，以下同上
	BOOL startSaverRealData(String^ sFilename);
	BOOL stopSaveRealData();
	bool IsSaveRealData();
public:
	//ArrayList^ DVR_AddTest(String ^ s_ip,WORD d_port, String ^s_userName, String ^ s_userPassword);
	ArrayList^ DVR_AddTestEx(String ^ s_ip, WORD w_port, String ^ & s_userName, String ^ s_userPassword, int & AnChanNum,int &  IPChanNum, int &  AnChanStartNum,int &  IPChanStartNum,int& nError,String^ sDevSupplier);
private:
	String^ m_sDevSupplier;
private:
	VideoFindDown_HK::VideoFindDown_HKer^ m_HK;
	VideoFindDown_DH::VideoFindDown_DHer^ m_DH;
private:
	String^ m_ip;
	String^ m_userName;
	String^ m_userPassword;
	WORD m_wDvrPort;
	String^ m_sChannel;
	IntPtr m_render_wnd;
};


}

