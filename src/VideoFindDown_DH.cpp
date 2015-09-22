#include "StdAfx.h"
#include "VideoFindDown_DH.h"
using namespace System::Windows::Forms;
using namespace System::IO;

//////////////////////////////////////////////////////////////////////////
//命名空间中的局部全局方法


//////////////////////////////////////////////////////////////////////////
///类成员函数
VideoFindDown_DH::VideoFindDown_DHer::VideoFindDown_DHer(void)
{
	m_bIsRealPlay=false;
	m_bIsSaveRealData=false;
	m_lLoginHandle=0;
	m_lRealHandle=0;

}


/************************************************************************/
/*        托管string^  转换成非托管const  char*                         */    
/************************************************************************/
char* VideoFindDown_DH::ManagedString2UnmanagedCharPtr(String^ strIn)
{
	System::IntPtr ip = Marshal::StringToHGlobalAnsi(strIn);
	const char* pTemp = static_cast<const char*>(ip.ToPointer());
	char *pOut = new char[strlen(pTemp)+1];
	strcpy(pOut, pTemp);
	Marshal::FreeHGlobal(ip);
	return pOut;
} 
String^ VideoFindDown_DH::UnmanagedStringA2ManagedString(char *pIn)
{
	String^ strOut = Marshal::PtrToStringAnsi(static_cast<IntPtr>(pIn));
	return strOut;
}
/************************************************************************/
		 /*       海康DVR:播放视频，供dragdrop掉用                   */
		 /************************************************************************/
void VideoFindDown_DH::VideoFindDown_DHer::Net_DVR_Play(LONG lUserID,LONG nChan, IntPtr render_wnd)
		{			
			
		}
ArrayList^ VideoFindDown_DH::VideoFindDown_DHer::DVR_AddTest(String ^ s_ip,String ^s_userName, String ^ s_userPassword)
		{
			ArrayList^ ret=gcnew ArrayList;
			return ret;
		}

//bool VideoFindDown_HK::VideoFindDown_HKer::cmdIP(String ^strip)
//		{
//			Process ^p = gcnew Process();		//建立外部调用线程
//			p->StartInfo->FileName = "cmd.exe";	 //要调用外部程序
//			p->StartInfo->UseShellExecute = false; //不使用操作系统外壳程序启动线程
//		    p->StartInfo->RedirectStandardInput = true;
//			p->StartInfo->RedirectStandardOutput = true;
//			p->StartInfo->RedirectStandardError = true;	 //把外部程序错误输出写到StandardError流中
//			p->StartInfo->CreateNoWindow = true;		 //创建进程窗口
//			
//			p->Start();
//			p->StandardInput->WriteLine("ping -n 1 " + strip);
//			p->StandardInput->WriteLine("exit");
//			String ^stre = p->StandardOutput->ReadToEnd();
//
//			if (stre->IndexOf("(0% 丢失)") != -1)
//			{
//					 //pingstr = "连接";
//					 p->Close();
//					 return true;
//			}
//			else
//			{
//				 //pingstr ="连接失败！";
//				 p->Close();
//				 return false;
//			}
//		} 


//int VideoFindDown_DH::saveRecordFile(int userId,char * srcfile,char * destfile)
//{
//	return 0;
//}
//
//
//int VideoFindDown_DH::findRecordFile()
//{
//	 return 0;
//}

int VideoFindDown_DH::VideoFindDown_DHer::startRealPlay(char * ip,char * userName, char * userPassword,IntPtr render_wnd,WORD wDvrPort,int nChannel)
{

	//----------------------------------
	// 获取控制台窗口句柄
	//HWND hWnd = GetConsoleWindow();

	//----------------------------------
	//初始化SDK
	VideoFindDown_DH::CLIENT_Init(DisConnectFunc, 0);
	//设置连接参数
	VideoFindDown_DH::CLIENT_SetConnectTime(3000, 3);
	//设置自动重连回调函数
	VideoFindDown_DH::CLIENT_SetAutoReconnect(HaveReConnectFunc, 0);
	//设置子连接自动重连回调函数
	VideoFindDown_DH::CLIENT_SetSubconnCallBack(SubHaveReConnectFunc, 0);

	NET_DEVICEINFO stDevInfo = {0};
	int nError = 0;
	LLONG lLoginHandle = 0;
	LLONG lRealHandle = {0};
	//char szIP[16] = "192.168.0.8";
	//int nPort = 37777;
	//int nChannelID = 0; // 预览通道号

	//注册用户到设备
	lLoginHandle = VideoFindDown_DH::CLIENT_Login(ip, wDvrPort, userName, userPassword, &stDevInfo, &nError);
	if (lLoginHandle != 0)
	{
		//开启实时监视
		lRealHandle = VideoFindDown_DH::CLIENT_RealPlayEx(lLoginHandle, nChannel, render_wnd);
		if (lRealHandle == 0)//failed
		{
			//printf("CLIENT_RealPlay: failed! Error code: %x.\n", CLIENT_GetLastError());
			String^ s="播放实时流失败,请检查相关配置！\n 错误码："+ (CLIENT_GetLastError()).ToString();
			MessageBox::Show(s);
			return -1;//
		}
		else
		{
			m_bIsRealPlay=true;
			m_lLoginHandle=lLoginHandle;
			m_lRealHandle=lRealHandle;
			return 0;//success
		}
	}

	return -1;
}
int VideoFindDown_DH::VideoFindDown_DHer::stopRealPlay()
{
	if (false==m_bIsRealPlay)
	{
		m_lLoginHandle=0;
		m_lRealHandle=0;
		return 0;//表示成功
	}
	//关闭预览
	VideoFindDown_DH::CLIENT_StopRealPlayEx(m_lRealHandle);

	//注销用户
	VideoFindDown_DH::CLIENT_Logout(m_lLoginHandle);

	//释放SDK资源
	VideoFindDown_DH::CLIENT_Cleanup();

	m_bIsRealPlay=false;
	m_lLoginHandle=0;
	m_lRealHandle=0;

	return  0;//表示成功
}

int VideoFindDown_DH::VideoFindDown_DHer::startRealPlay(String ^ s_ip,String ^s_userName, String ^ s_userPassword,IntPtr render_wnd,WORD wDvrPort,String^ sChannel)
{
	char * ip = ManagedString2UnmanagedCharPtr(s_ip);
	char * userName = ManagedString2UnmanagedCharPtr(s_userName);
	char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);
	int channelNumber=0;
	int a;
	bool bRet= a.TryParse(sChannel,channelNumber);
	if (bRet==false)
	{
		MessageBox::Show("通道号错误！");
		return false;
	}
	return startRealPlay(ip, userName, userPassword, render_wnd,wDvrPort,channelNumber);
}

int VideoFindDown_DH::VideoFindDown_DHer::searchRecordList(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,/*std::vector<NET_DVR_FINDDATA_V40*>&*/ ArrayList^&  recordList)
{
	VideoFindDown_DH::CLIENT_Init(DisConnectFunc, 0);
	//cout<<"Initialize client SDK done;"<<endl;

	/*char szDevIp[64] = {0};*/
	NET_DEVICEINFO stDevInfo = {0};
	int nError = 0;
	LLONG lLoginHandle = 0;
	//int nPort = 37777;
	//int nChannelID = 0; // 预览通道号
	LLONG hPlayBack = 0;

	/*cout<<endl<<"IP:";
	cin>>szDevIp;
	cout<<endl<<"Port:";
	cin>>nPort;*/

	NET_RECORDFILE_INFO netFileInfo[30] = {0};

	lLoginHandle = VideoFindDown_DH::CLIENT_Login(ip, wDvrPort, userName, userPassword, &stDevInfo, &nError);
	if (lLoginHandle != 0)
	{

		NET_TIME StartTime = {0};
		NET_TIME StopTime = {0};
		StartTime.dwYear =recordStartTime->Year;
		StartTime.dwMonth = recordStartTime->Month;
		StartTime.dwDay = recordStartTime->Day;
		StartTime.dwHour = recordStartTime->Hour;
		StartTime.dwMinute = recordStartTime->Minute;
		StartTime.dwSecond=recordStartTime->Second;
		StopTime.dwYear = recordStopTime->Year;
		StopTime.dwMonth = recordStopTime->Month;
		StopTime.dwDay = recordStopTime->Day;
		StopTime.dwHour = recordStopTime->Hour;
		StopTime.dwMinute=recordStopTime->Minute;
		StopTime.dwSecond=recordStopTime->Second;

		
		//第二种查询方法(分次取时间段内的所有录像文件)
		//---------------------------------------
		//查找录像文件
		int lFindHandle = VideoFindDown_DH::CLIENT_FindFile(lLoginHandle, lChannel, 0, NULL, &StartTime, &StopTime, FALSE, 5000);
		if(lFindHandle == 0)
		{
			//printf("find file fail,last error %d\n",CLIENT_GetLastError());
			//MessageBox::Show("find file fail,last error "+(CLIENT_GetLastError()).ToString()+"\n");
			//goto_out;
			return -1;//返回查找文件失败
		}

		while(true)
		{
			NET_RECORDFILE_INFO struFileData;
			int result = VideoFindDown_DH::CLIENT_FindNextFile(lFindHandle, &struFileData);
			if(result == 1)//取回一条录像文件信息
			{
				//nFileIndex++;
				//存储录像文件
				NET_RECORDFILE_INFO_Managed^ struFileData_managed=gcnew NET_RECORDFILE_INFO_Managed;
				struFileData_managed->ch=struFileData.ch;
				struFileData_managed->filename=UnmanagedStringA2ManagedString(struFileData.filename);
				struFileData_managed->framenum=struFileData.framenum;
				struFileData_managed->size=struFileData.size;
				struFileData_managed->starttime=gcnew System::DateTime(struFileData.starttime.dwYear,struFileData.starttime.dwMonth,struFileData.starttime.dwDay,struFileData.starttime.dwHour,struFileData.starttime.dwMinute,struFileData.starttime.dwSecond);
				struFileData_managed->endtime  =gcnew System::DateTime(struFileData.endtime.dwYear,struFileData.endtime.dwMonth,struFileData.endtime.dwDay,struFileData.endtime.dwHour,struFileData.endtime.dwMinute,struFileData.endtime.dwSecond);
				struFileData_managed->driveno=struFileData.driveno;
				struFileData_managed->startcluster=struFileData.startcluster;
				struFileData_managed->nRecordFileType=struFileData.nRecordFileType;
				struFileData_managed->bImportantRecID=struFileData.bImportantRecID;
				struFileData_managed->bHint=struFileData.bHint;
				struFileData_managed->bRecType=struFileData.bRecType;
				recordList->Add(struFileData_managed);

			}
			else if(result == 0)//录像文件信息数据取完
			{	
				break;
			}
			else//参数出错
			{
				//printf("find file fail for illegal param\n");
				//MessageBox::Show("find file fail for illegal param!\n");
				break;
			}
		}

		//停止查找
		if(lFindHandle != 0)
		{
			VideoFindDown_DH::CLIENT_FindClose(lFindHandle);
		}
		/*memcpy(&netFileInfo[0], (void *)&pstruFileData[0],sizeof(NET_RECORDFILE_INFO));
		delete []pstruFileData;*/

		/*FILE *file = fopen("PlayBack.dav", "wb");
		hPlayBack = CLIENT_PlayBackByRecordFileEx(lLoginHandle, &netFileInfo[0], NULL, DownLoadPosCallBackFunc, 0, DataCallBackFunc, (DWORD)file);*/
	}
	else
	{
		//printf("CLIENT_LoginEx: %s failed! error = %d\n", szDevIp, nError);
		String^ s="CLIENT_LoginEx: "+UnmanagedStringA2ManagedString(ip)+ "FAILED! error = "+nError.ToString()+"\n";
		//MessageBox::Show(s);
	}

	////停止回放
	//if (hPlayBack != 0)
	//{
	//	VideoFindDown_DH::CLIENT_StopPlayBack(hPlayBack);
	//}
	//注销用户
	VideoFindDown_DH::CLIENT_Logout(lLoginHandle);

	//释放SDK资源
	VideoFindDown_DH::CLIENT_Cleanup();//如果本行崩溃，就注释掉本行，参照海康的做法

	return  0;
}


int VideoFindDown_DH::VideoFindDown_DHer::searchRecordList(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,ArrayList/*<NET_DVR_FINDDATA_V40^>*/^&  recordList)
{
	char * ip = ManagedString2UnmanagedCharPtr(s_ip);
	char * userName = ManagedString2UnmanagedCharPtr(s_userName);
	char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);
	return searchRecordList(ip, userName, userPassword, wDvrPort,lChannel,recordStartTime,recordStopTime,recordList);
}

int VideoFindDown_DH::VideoFindDown_DHer::downRecordFile(char * ip,char * userName, char * userPassword,WORD wDvrPort,NET_RECORDFILE_INFO_Managed^ recordInfo,char * destfile,int* sDownText)
{
	VideoFindDown_DH::CLIENT_Init(DisConnectFunc, 0);
	//cout<<"Initialize client SDK done;"<<endl;

	/*char szDevIp[64] = {0};*/
	NET_DEVICEINFO stDevInfo = {0};
	int nError = 0;
	LLONG lLoginHandle = 0;
	//int nPort = 37777;
	//int nChannelID = 0; // 预览通道号
	LLONG hPlayBack = 0;

	/*cout<<endl<<"IP:";
	cin>>szDevIp;
	cout<<endl<<"Port:";
	cin>>nPort;*/

	NET_RECORDFILE_INFO netFileInfo[30] = {0};

	lLoginHandle = VideoFindDown_DH::CLIENT_Login(ip, wDvrPort, userName, userPassword, &stDevInfo, &nError);
	if (lLoginHandle != 0)
	{
		/*NET_TIME StartTime = {0};
		NET_TIME StopTime = {0};
		StartTime.dwYear =recordStartTime->Year;
		StartTime.dwMonth = recordStartTime->Month;
		StartTime.dwDay = recordStartTime->Day;
		StartTime.dwHour = recordStartTime->Hour;
		StartTime.dwMinute = recordStartTime->Minute;
		StartTime.dwSecond=recordStartTime->Second;
		StopTime.dwYear = recordStopTime->Year;
		StopTime.dwMonth = recordStopTime->Month;
		StopTime.dwDay = recordStopTime->Day;
		StopTime.dwHour = recordStopTime->Hour;
		StopTime.dwMinute=recordStopTime->Minute;
		StopTime.dwSecond=recordStopTime->Second;*/

		//第一种查询方法(一次取完时间段内的所有录像文件)
		//int nFileCount = 0;
		////查询
		//BOOL bSuccess = CLIENT_QueryRecordFile(lLoginHandle, lChannel, 0, &StartTime, &StopTime, NULL, &netFileInfo[0], 30*sizeof(NET_RECORDFILE_INFO), &nFileCount,5000, FALSE);
		//if (bSuccess)
		//{
		//	printf("CLIENT_QueryRecordFile succeed!!!!\n");
		//}
		//else
		//{
		//	printf("CLIENT_QueryRecordFile failed!!!!\n");
		//}

		//下载录像文件
		LDWORD dwUserData=(LDWORD)(sDownText);
		//fDownLoadPosCallBack cbDownLoadPos=nullptr;
		LPNET_RECORDFILE_INFO lpRecordFile=new NET_RECORDFILE_INFO;
		lpRecordFile->bHint=recordInfo->bHint;
		lpRecordFile->bImportantRecID=recordInfo->bImportantRecID;
		lpRecordFile->bRecType=recordInfo->bRecType;
		lpRecordFile->ch=recordInfo->ch;
		lpRecordFile->driveno=recordInfo->driveno;
		lpRecordFile->endtime.dwYear=recordInfo->endtime->Year;
		lpRecordFile->endtime.dwMonth=recordInfo->endtime->Month;
		lpRecordFile->endtime.dwDay=recordInfo->endtime->Day;
		lpRecordFile->endtime.dwHour=recordInfo->endtime->Hour;
		lpRecordFile->endtime.dwMinute=recordInfo->endtime->Minute;
		lpRecordFile->endtime.dwSecond=recordInfo->endtime->Second;
		char* pcFilename=ManagedString2UnmanagedCharPtr(recordInfo->filename);//名称不能超过123个字符！！！！
		//lpRecordFile->filename=(recordInfo->filename)->ToCharArray(0,124);//名称不能超过123个字符！！！！
		memcpy(lpRecordFile->filename,pcFilename,124);
		lpRecordFile->framenum=recordInfo->framenum;
		lpRecordFile->nRecordFileType=recordInfo->nRecordFileType;
		lpRecordFile->size=recordInfo->size;
		lpRecordFile->startcluster=recordInfo->startcluster;
		//lpRecordFile->starttime=recordInfo->starttime;
		lpRecordFile->starttime.dwYear=recordInfo->starttime->Year;
		lpRecordFile->starttime.dwMonth=recordInfo->starttime->Month;
		lpRecordFile->starttime.dwDay=recordInfo->starttime->Day;
		lpRecordFile->starttime.dwHour=recordInfo->starttime->Hour;
		lpRecordFile->starttime.dwMinute=recordInfo->starttime->Minute;
		lpRecordFile->starttime.dwSecond=recordInfo->starttime->Second;
		LLONG Ret=VideoFindDown_DH::CLIENT_DownloadByRecordFile(lLoginHandle,lpRecordFile, destfile, cbDownLoadPos,  dwUserData);
		if (0!=Ret)
		{
			MessageBox::Show("下载失败，错误码："+(VideoFindDown_DH::CLIENT_GetLastError()).ToString());
			return -1;//下载失败
		}


	}
	//注销用户
	VideoFindDown_DH::CLIENT_Logout(lLoginHandle);

	//释放SDK资源
	VideoFindDown_DH::CLIENT_Cleanup();//如果本行崩溃，就注释掉本行，参照海康的做法

	return  0;
}
bool VideoFindDown_DH::VideoFindDown_DHer::isRealPlay()
{
	return m_bIsRealPlay;
}

int VideoFindDown_DH::VideoFindDown_DHer::downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,NET_RECORDFILE_INFO_Managed^ recordInfo,String ^ s_destfile,int* sDownText)
{
	char * ip = ManagedString2UnmanagedCharPtr(s_ip);
	char * userName = ManagedString2UnmanagedCharPtr(s_userName);
	char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);
	//char * srcfile = ManagedString2UnmanagedCharPtr(s_srcfile);
	char * destfile = ManagedString2UnmanagedCharPtr(s_destfile);
	return downRecordFile(ip, userName, userPassword, wDvrPort,recordInfo,destfile,sDownText);
}
LLONG VideoFindDown_DH::VideoFindDown_DHer::getUserID()
{
	return m_lLoginHandle;
}
LLONG VideoFindDown_DH::VideoFindDown_DHer::getRealPlayHandle()
{
	return m_lRealHandle;
}


//////////////////////////////////////////////////////////////////////////
//大华的断线重连回调函数
//断线回调函数
void __stdcall VideoFindDown_DH::DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	//printf("Device disconnect, IP=%s, Port=%d\n", pchDVRIP, nDVRPort);
	String^ s="Device disconnect, IP="+UnmanagedStringA2ManagedString(pchDVRIP)+"Port="+ nDVRPort.ToString()+"\n";
	MessageBox::Show(s);
}

//自动重连回调函数
void __stdcall VideoFindDown_DH::HaveReConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	//printf("Device reconnect, IP=%s, Port=%d\n", pchDVRIP);
	String^ s="Device reconnect, IP="+UnmanagedStringA2ManagedString(pchDVRIP)+"Port="+ nDVRPort.ToString()+"\n";
	MessageBox::Show(s);
}

//子连接自动重连回调函数
void __stdcall VideoFindDown_DH::SubHaveReConnectFunc(EM_INTERFACE_TYPE emInterfaceType, BOOL bOnline, LLONG lOperateHandle, LLONG lLoginID, LDWORD dwUser)
{
	//printf("Device reconnecte.");
	String^ s="Device reconnecte.";
	MessageBox::Show(s);
}


//////////////////////////////////////////////////////////////////////////
//以下为录像查询时用的回调函数
//void __stdcall DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
//{
//	printf("Device disconn, IP=%s+++++++++++++++++++++++\n", pchDVRIP);
//}

void __stdcall VideoFindDown_DH::DownLoadPosCallBackFunc(LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser)
{ 
	if (dwDownLoadSize == -1)
	{
		//printf("PlayBackPosCallBack: 100\n");
		MessageBox::Show("PlayBackPosCallBack: 100\n");
	}
	else if (dwTotalSize != 0)
	{
		//	printf("PlayBackPosCallBack: %d%\n", dwDownLoadSize*100/dwTotalSize);
		MessageBox::Show("PlayBackPosCallBack: "+(dwDownLoadSize*100/dwTotalSize).ToString());
	}
}

int __stdcall VideoFindDown_DH::DataCallBackFunc(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser)
{
	/*FILE *file =(FILE *)dwUser;
	if (file)
	{
		fwrite(pBuffer, 1, dwBufSize, file);
	}*/

	//printf("PlayBack: data++++, len=%d+++++++++++++++\n", dwBufSize);
	String^ s="PlayBack: data++++, len="+dwBufSize.ToString()+"++++++++++++++++\n";
	MessageBox::Show(s);

	return 1;
}

//int VideoFindDown_DH::VideoFindDown_DHer::downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,String ^ s_srcfile,String ^ s_destfile)
//{
//	char * ip = ManagedString2UnmanagedCharPtr(s_ip);
//	char * userName = ManagedString2UnmanagedCharPtr(s_userName);
//	char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);
//	char * srcfile = ManagedString2UnmanagedCharPtr(s_srcfile);
//	char * destfile = ManagedString2UnmanagedCharPtr(s_destfile);
//	return downRecordFile(ip, userName, userPassword, wDvrPort,lChannel,srcfile,destfile);
//}

//如果只有文件名（内部再执行一遍查找），执行下行
//int VideoFindDown_DH::VideoFindDown_DHer::downRecordFile(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,char* srcfile,char * destfile)
//{
//	VideoFindDown_DH::CLIENT_Init(DisConnectFunc, 0);
//	//cout<<"Initialize client SDK done;"<<endl;
//
//	/*char szDevIp[64] = {0};*/
//	NET_DEVICEINFO stDevInfo = {0};
//	int nError = 0;
//	LLONG lLoginHandle = 0;
//	//int nPort = 37777;
//	//int nChannelID = 0; // 预览通道号
//	LLONG hPlayBack = 0;
//
//	/*cout<<endl<<"IP:";
//	cin>>szDevIp;
//	cout<<endl<<"Port:";
//	cin>>nPort;*/
//
//	NET_RECORDFILE_INFO netFileInfo[30] = {0};
//
//	lLoginHandle = VideoFindDown_DH::CLIENT_Login(ip, wDvrPort, userName, userPassword, &stDevInfo, &nError);
//	if (lLoginHandle != 0)
//	{
//
//		NET_TIME StartTime = {0};
//		NET_TIME StopTime = {0};
//		StartTime.dwYear =recordStartTime->Year;
//		StartTime.dwMonth = recordStartTime->Month;
//		StartTime.dwDay = recordStartTime->Day;
//		StartTime.dwHour = recordStartTime->Hour;
//		StartTime.dwMinute = recordStartTime->Minute;
//		StartTime.dwSecond=recordStartTime->Second;
//		StopTime.dwYear = recordStopTime->Year;
//		StopTime.dwMonth = recordStopTime->Month;
//		StopTime.dwDay = recordStopTime->Day;
//		StopTime.dwHour = recordStopTime->Hour;
//		StopTime.dwMinute=recordStopTime->Minute;
//		StopTime.dwSecond=recordStopTime->Second;
//
//		
//		//第二种查询方法(分次取时间段内的所有录像文件)
//		//---------------------------------------
//		//查找录像文件
//		int lFindHandle = VideoFindDown_DH::CLIENT_FindFile(lLoginHandle, lChannel, 0, NULL, &StartTime, &StopTime, FALSE, 5000);
//		if(lFindHandle == 0)
//		{
//			//printf("find file fail,last error %d\n",CLIENT_GetLastError());
//			MessageBox::Show("find file fail,last error "+(CLIENT_GetLastError()).ToString()+"\n");
//			//goto_out;
//			return -1;//返回查找文件失败
//		}
//
//		while(true)
//		{
//			NET_RECORDFILE_INFO struFileData;
//			int result = VideoFindDown_DH::CLIENT_FindNextFile(lFindHandle, &struFileData);
//			if(result == 1)//取回一条录像文件信息
//			{
//				//匹配看看是否是自己需要的录像文件
//				NET_RECORDFILE_INFO_Managed^ struFileData_managed=gcnew NET_RECORDFILE_INFO_Managed;
//				struFileData_managed->ch=struFileData.ch;
//				struFileData_managed->filename=UnmanagedStringA2ManagedString(struFileData.filename);
//				struFileData_managed->framenum=struFileData.framenum;
//				struFileData_managed->size=struFileData.size;
//				struFileData_managed->starttime=gcnew System::DateTime(struFileData.starttime.dwYear,struFileData.starttime.dwMonth,struFileData.starttime.dwDay,struFileData.starttime.dwHour,struFileData.starttime.dwMinute,struFileData.starttime.dwSecond);
//				struFileData_managed->endtime  =gcnew System::DateTime(struFileData.endtime.dwYear,struFileData.endtime.dwMonth,struFileData.endtime.dwDay,struFileData.endtime.dwHour,struFileData.endtime.dwMinute,struFileData.endtime.dwSecond);
//				struFileData_managed->driveno=struFileData.driveno;
//				struFileData_managed->startcluster=struFileData.startcluster;
//				struFileData_managed->nRecordFileType=struFileData.nRecordFileType;
//				struFileData_managed->bImportantRecID=struFileData.bImportantRecID;
//				struFileData_managed->bHint=struFileData.bHint;
//				struFileData_managed->bRecType=struFileData.bRecType;
//				recordList->Add(struFileData_managed);
//
//			}
//			else if(result == 0)//录像文件信息数据取完
//			{	
//				break;
//			}
//			else//参数出错
//			{
//				//printf("find file fail for illegal param\n");
//				MessageBox::Show("find file fail for illegal param!\n");
//				break;
//			}
//		}
//
//		//停止查找
//		if(lFindHandle != 0)
//		{
//			VideoFindDown_DH::CLIENT_FindClose(lFindHandle);
//		}
//		/*memcpy(&netFileInfo[0], (void *)&pstruFileData[0],sizeof(NET_RECORDFILE_INFO));
//		delete []pstruFileData;*/
//
//		/*FILE *file = fopen("PlayBack.dav", "wb");
//		hPlayBack = CLIENT_PlayBackByRecordFileEx(lLoginHandle, &netFileInfo[0], NULL, DownLoadPosCallBackFunc, 0, DataCallBackFunc, (DWORD)file);*/
//	}
//	else
//	{
//		//printf("CLIENT_LoginEx: %s failed! error = %d\n", szDevIp, nError);
//		String^ s="CLIENT_LoginEx: "+UnmanagedStringA2ManagedString(ip)+ "FAILED! error = "+nError.ToString()+"\n";
//		MessageBox::Show(s);
//	}
//
//	////停止回放
//	//if (hPlayBack != 0)
//	//{
//	//	VideoFindDown_DH::CLIENT_StopPlayBack(hPlayBack);
//	//}
//	//注销用户
//	VideoFindDown_DH::CLIENT_Logout(lLoginHandle);
//
//	//释放SDK资源
//	VideoFindDown_DH::CLIENT_Cleanup();//如果本行崩溃，就注释掉本行，参照海康的做法
//
//	return  0;
//
//}

BOOL VideoFindDown_DH::VideoFindDown_DHer::capturePicInRealPlay(String^ sFilename)
{
	const char* pchPicFileName=ManagedString2UnmanagedCharPtr(sFilename);
	return CLIENT_CapturePicture(m_lRealHandle,pchPicFileName);
}
BOOL VideoFindDown_DH::VideoFindDown_DHer::startSaverRealData(String^ sFilename)
{
	const char* pchFileName=ManagedString2UnmanagedCharPtr(sFilename);
	BOOL bRet=CLIENT_SaveRealData(m_lRealHandle,pchFileName);
	if (TRUE==bRet)
	{
		m_bIsSaveRealData=TRUE;
	}
	return bRet;
}
BOOL VideoFindDown_DH::VideoFindDown_DHer::stopSaveRealData()
{
	if (false==m_bIsSaveRealData)
	{
		return TRUE;
	}
	BOOL bRet= CLIENT_StopSaveRealData(m_lRealHandle);
	if (TRUE==bRet)
	{
		m_bIsSaveRealData=false;
	}
	return bRet;

}
bool VideoFindDown_DH::VideoFindDown_DHer::IsSaveRealData()
{
	return m_bIsSaveRealData;
}

ArrayList^ VideoFindDown_DH::VideoFindDown_DHer::DVR_AddTestEx(String ^ s_ip, WORD w_port, String ^ & s_userName, String ^ s_userPassword, int & AnChanNum,int &  IPChanNum,int &  AnChanStartNum,int &  IPChanStartNum,int& nError)
{
	char * ip = ManagedString2UnmanagedCharPtr(s_ip);
	char * userName = ManagedString2UnmanagedCharPtr(s_userName);
	char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);

	ArrayList^ ChanName = gcnew ArrayList();

	CLIENT_Init(DisConnectFunc, 0);
	//cout<<"Initialize client SDK done;"<<endl;

	//char szDevIp[64] = {0};
	char* szDevIp=ip;
	NET_DEVICEINFO stDevInfo = {0};
	//int nError = 0;
	LLONG lLoginHandle = 0;
	int nPort = w_port;
	int nChannelID = 0xFFFFFFFF; // 通道号-1表示获取全部通道（非三代设备可用，三代设备必须逐个获取通道名称）noted by  dongjiquan 20150317-10:32
	LLONG hdownloadfile = 0;


	/*cout<<endl<<"IP:";
	cin>>szDevIp;
	cout<<endl<<"Port:";
	cin>>nPort;*/

	BOOL b3GProtocol = FALSE;

	lLoginHandle = VideoFindDown_DH::CLIENT_Login(szDevIp, nPort, userName, userName, &stDevInfo, &nError);
	if (lLoginHandle != 0)
	{

		//判断是否为三代协议设备
		int len = 0;
		DH_DEV_ENABLE_INFO stDevEn = {0};
		BOOL bRet = VideoFindDown_DH::CLIENT_QuerySystemInfo(lLoginHandle, ABILITY_DEVALL_INFO, (char*)&stDevEn, sizeof(DH_DEV_ENABLE_INFO), &len);
		if (bRet&&len == sizeof(DH_DEV_ENABLE_INFO))
		{
			if (stDevEn.IsFucEnable[EN_JSON_CONFIG]!=0 || stDevInfo.byChanNum> 32)
			{
				b3GProtocol = TRUE;
			}
			else
			{
				b3GProtocol = FALSE;
			}
		}
		if (b3GProtocol == TRUE)
		{
			int nChannelCount = stDevInfo.byChanNum;
			char *szOutBuffer = new char[32*1024];
			if (szOutBuffer == NULL)
			{
				//goto_out;	
				return ChanName;
			}
			memset(szOutBuffer, 0, 32*1024);

			int nerror = 0;
			CFG_ENCODE_INFO stuEncodeInfo = {0};
			int nrestart = 0;

			//给的示例代码第三参数为0，可是说明书里说的是通道号为1-最大通道号，而且这条命令只有返回一个通道信息的能力
			//因而把下面的代码改成循环逐个获取通道名称
			//注释  from dongjiaquan 20150317-10:36
			//BOOL bSuccess = CLIENT_GetNewDevConfig(lLoginHandle, CFG_CMD_ENCODE, 0, szOutBuffer, 32*1024, &nerror, 5000);	
			for (int i=0;i<nChannelCount;i++)
			{
				BOOL bSuccess = CLIENT_GetNewDevConfig(lLoginHandle, CFG_CMD_ENCODE, 0, szOutBuffer, 32*1024, &nerror, 5000);	
				if (bSuccess)
				{
					int nRetLen = 0;
					//解析
					BOOL bRet = VideoFindDown_DH::CLIENT_ParseData(CFG_CMD_ENCODE, (char *)szOutBuffer, &stuEncodeInfo, sizeof(CFG_ENCODE_INFO), &nRetLen);
					if (bRet == FALSE)
					{
						//printf("CLIENT_ParseData: CFG_CMD_ENCODE failed!\n");
						MessageBox::Show("CLIENT_ParseData: CFG_CMD_ENCODE failed!\n");
					}
					else  //成功解析数据，将通道名称加入返回值中
					{
								
							ChanName->Add(UnmanagedStringA2ManagedString(stuEncodeInfo.szChnName));
					}
				}
				else
				{
					//printf("CLIENT_GetNewDevConfig: CFG_CMD_ENCODE failed!\n");
					MessageBox::Show("CLIENT_GetNewDevConfig: CFG_CMD_ENCODE failed!\n");

				}
			}
			AnChanNum=nChannelCount;//全部看作模拟通道
			AnChanStartNum=1;
			IPChanNum=0;
			IPChanStartNum=0;
					



			//不需要如下修改配置的代码
			//stuEncodeInfo.stuMainStream[0].stuVideoFormat.nFrameRate = 20;//修改帧率
			//memset(szOutBuffer, 0, 32*1024);

			//bSuccess = CLIENT_PacketData(CFG_CMD_ENCODE, (char *)&stuEncodeInfo, sizeof(CFG_ENCODE_INFO), szOutBuffer, 32*1024);
			//if (bSuccess == FALSE)
			//{
			//	printf("CLIENT_PacketData: CFG_CMD_ENCODE failed!\n");
			//}
			//else
			//{
			//	bSuccess = CLIENT_SetNewDevConfig(lLoginHandle, CFG_CMD_ENCODE, 0, szOutBuffer, 32*1024, &nerror, &nrestart, 3000);		
			//	if (bSuccess)
			//	{
			//		if (nrestart == 1)
			//		{
			//			printf("Save config info successfully!devide need restart!\n");
			//		}
			//	}
			//	else
			//	{
			//		printf("CLIENT_SetNewDevConfig CFG_CMD_ENCODE failed!\n");
			//	}		
			//}

			delete []szOutBuffer;

		}
		else
		{
			int nChannelCount = stDevInfo.byChanNum;
			DWORD dwRetLen = 0;
			DHDEV_CHANNEL_CFG *pChannelInfo = new DHDEV_CHANNEL_CFG[nChannelCount];
			if (pChannelInfo == NULL)
			{
				//goto_out;
				return ChanName;
			}
			memset(pChannelInfo, 0, nChannelCount*sizeof(DHDEV_CHANNEL_CFG));
			BOOL bSuccess = VideoFindDown_DH::CLIENT_GetDevConfig(lLoginHandle, DH_DEV_CHANNELCFG, -1, pChannelInfo, nChannelCount * sizeof(DHDEV_CHANNEL_CFG), &dwRetLen);
			//第三个值填-1 或者0xffffffffff代表获取全部通道数据
			if (!(bSuccess&&dwRetLen == nChannelCount * sizeof(DHDEV_CHANNEL_CFG)))
			{
				//printf("CLIENT_GetDevConfig: DH_DEV_CHANNELCFG failed!\n");
				MessageBox::Show("CLIENT_GetDevConfig: DH_DEV_CHANNELCFG failed!\n");
			}
			//通道数据加入返回的arraylist中
			for (int i=0;i<nChannelCount;i++)
			{
				String^ sChanName=UnmanagedStringA2ManagedString(pChannelInfo[i].szChannelName);
				ChanName->Add(sChanName);
			}
			AnChanNum=nChannelCount;//全部看作模拟通道
			AnChanStartNum=1;
			IPChanNum=0;
			IPChanStartNum=0;



			//不需要如下修改配置的代码
			//pChannelInfo[0].stMainVideoEncOpt[0].byFramesPerSec = 20;//修改帧率

			//bSuccess = CLIENT_SetDevConfig(lLoginHandle, DH_DEV_CHANNELCFG, -1, pChannelInfo, nChannelCount * sizeof(DHDEV_CHANNEL_CFG));
			//if (bSuccess == FALSE)
			//{
			//	printf("CLIENT_SetDevConfig: DH_DEV_CHANNELCFG failed!\n");
			//}

			delete []pChannelInfo;

		}
	}
	else
	{
		//printf("CLIENT_LoginEx: %s failed! error = %d\n", szDevIp, nError);
		//MessageBox::Show("CLIENT_LoginEx: "+UnmanagedStringA2ManagedString(szDevIp)+" failed! error = "+nError.ToString+"\n");
	}

	//注销用户
	CLIENT_Logout(lLoginHandle);

	//释放SDK资源
	CLIENT_Cleanup();//如果崩溃在此处，则注释此句 noted by dongjiaquan 20150317 09:17

	return ChanName;
}


//下载进度回调函数
void __stdcall VideoFindDown_DH::cbDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
{
	int local_nPos=((double)dwDownLoadSize)*100.0/dwTotalSize;
	int* nPos=(int*)dwUser;
	*nPos=local_nPos;

}