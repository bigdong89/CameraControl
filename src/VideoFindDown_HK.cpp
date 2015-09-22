#include "StdAfx.h"
#include "VideoFindDown_HK.h"
using namespace System::Windows::Forms;

VideoFindDown_HK::VideoFindDown_HKer::VideoFindDown_HKer(void)
{
	m_bIsRealPlay=false;
	m_bIsSaveRealData=false;
	m_lRealPlayHandle=-1;
	m_lUserID=-1;

}


/************************************************************************/
/*        托管string^  转换成非托管const  char*                         */    
/************************************************************************/
char* VideoFindDown_HK::ManagedString2UnmanagedCharPtr(String^ strIn)
{
	System::IntPtr ip = Marshal::StringToHGlobalAnsi(strIn);
	const char* pTemp = static_cast<const char*>(ip.ToPointer());
	char *pOut = new char[strlen(pTemp)+1];
	strcpy(pOut, pTemp);
	Marshal::FreeHGlobal(ip);
	return pOut;
} 
/************************************************************************/
		 /*       海康DVR:播放视频，供dragdrop掉用                   */
		 /************************************************************************/
void VideoFindDown_HK::VideoFindDown_HKer::HKNet_DVR_Play(LONG lUserID,LONG nChan, IntPtr render_wnd)
		{			
			NET_DVR_CLIENTINFO2 ClientInfo = {0}; //数组初始化时，定义第一个元素为零后，其它就都为零了。  

			//赋给播放窗口句柄
			ClientInfo.hPlayWnd = render_wnd; //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空 
			ClientInfo.lChannel = nChan;    //预览通道号 
			ClientInfo.lLinkMode = 0;   /*最高位(31)为0表示主码流，为1表示子码流。0～30位表示连接方式：
									     0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式; 4- RTP over RTSP，5- RTSP over HTTP */ 
			ClientInfo.sMultiCastIP = nullptr; //多播地址，需要多播预览时配置
			
			bool bPreviewBlock = false; //请求码流过程是否阻塞，0：否，1：是

			LONG lRealPlayHandle; 
			lRealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, nullptr, nullptr, 0); 
			if (lRealPlayHandle < 0) 
			{ 
				//printf("NET_DVR_RealPlay_V30 error\n"); 
				NET_DVR_Logout(lUserID); 
				NET_DVR_Cleanup(); 
				return;
			} 
		}
ArrayList^ VideoFindDown_HK::VideoFindDown_HKer::HKDVR_AddTest(String ^ s_ip,String ^s_userName, String ^ s_userPassword)
		{
			char * ip = ManagedString2UnmanagedCharPtr(s_ip);
			char * userName = ManagedString2UnmanagedCharPtr(s_userName);
			char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);

			ArrayList^ ChanName = gcnew ArrayList();

			// 初始化
			NET_DVR_Init();

			// 注册设备 
			LONG lUserID;
			NET_DVR_DEVICEINFO_V30 struDeviceInfo;
			lUserID = VideoFindDown_HK::NET_DVR_Login_V30(ip, 8000, userName , userPassword, &struDeviceInfo);

			if (lUserID < 0)
			{ 
				int nError = NET_DVR_GetLastError();
				/*System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
				box->Show("DVR添加出错！");*/
				NET_DVR_Cleanup(); 
				return nullptr; 
			}

			//获得通道号
			int nChanNum = struDeviceInfo.byChanNum;
			DWORD dwReturned = 0;

			NET_DVR_PICCFG_V30	m_struPicCfg[MAX_CHANNUM_V30];

			for (int i=1;i<=nChanNum;i++)
			{
				//获取通道状态
				if (VideoFindDown_HK::NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_PICCFG_V30, i, &m_struPicCfg[i], sizeof(NET_DVR_PICCFG_V30), &dwReturned))
				{		
					char cTemp[100] = {0};		
					memcpy(cTemp,m_struPicCfg[i].sChanName,NAME_LEN);		
					String ^ str_ChanName = gcnew String(cTemp);

					ChanName->Add(str_ChanName);
				}
				else
				{
					/*System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
					box->Show("DVR通道名称获取出错，添加失败！");*/
					return nullptr;
				}												   
			}
			//注销用户
			NET_DVR_Logout(lUserID); 
			//释放SDK资源
			NET_DVR_Cleanup();
			return ChanName;
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


int VideoFindDown_HK::saveRecordFile(int userId,char * srcfile,char * destfile)
{
	try
	{
		int bRes = 1;
		int hPlayback = 0;
		if( (hPlayback = NET_DVR_GetFileByName(userId, srcfile, destfile)) < 0 )
		{
			//printf( "GetFileByName failed. error[%d]\n", NET_DVR_GetLastError());
			System::Console::WriteLine("GetFileByName failed. error[%d]\n", NET_DVR_GetLastError());
			bRes= -1;
			return bRes;
		}

		if(!NET_DVR_PlayBackControl_V40(hPlayback, NET_DVR_PLAYSTART, NULL,0,NULL,NULL))
		{
			//printf("play back control failed [%d]\n",NET_DVR_GetLastError());
			System::Console::WriteLine("NET_DVR_PlayBackControl_V40 failed. error[%d]\n", NET_DVR_GetLastError());
			bRes=-1;
			return bRes;
		}

		int nPos = 0;
		for(nPos = 0;  nPos < 100&&nPos>=0; nPos = NET_DVR_GetDownloadPos(hPlayback))
		{
			//printf("Be downloading...%d %%\n", nPos);
			System::Console::WriteLine("Be downloading...%d %%\n", nPos);
			Sleep(5000);  //millisecond
		}
		//printf("have got %d\n", nPos);
		System::Console::WriteLine("have got %d\n", nPos);

		if(!NET_DVR_StopGetFile(hPlayback))
		{
			//printf("failed to stop get file [%d]\n",NET_DVR_GetLastError());
			System::Console::WriteLine("failed to stop get file [%d]\n",NET_DVR_GetLastError());
			bRes = -1;
			return bRes;
		}
		//printf("%s\n",srcfile);
		//System::Console::WriteLine("%s\n",srcfile);// wrong...

		if(nPos<0||nPos>100)
		{
			//printf("download err [%d]\n",NET_DVR_GetLastError());
			System::Console::WriteLine("download err [%d]\n",NET_DVR_GetLastError());
			bRes=-1;
			return bRes;
		}
		else
		{
			return 0;
		}
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message);
		return -1;
	}	 
}


int VideoFindDown_HK::findRecordFile()
{
		// 初始化
		NET_DVR_Init();
		//设置连接时间与重连时间
		NET_DVR_SetConnectTime(2000, 1);
		NET_DVR_SetReconnect(10000, true);

		//---------------------------------------
		// 注册设备
		LONG lUserID;
		NET_DVR_DEVICEINFO_V30 struDeviceInfo;
		lUserID = VideoFindDown_HK::NET_DVR_Login_V30("192.0.0.64", 8000, "admin", "12345", &struDeviceInfo);
		if (lUserID < 0)
		{
			//printf("Login error, %d\n", NET_DVR_GetLastError());
			System::Console::WriteLine("Login error, %d\n", NET_DVR_GetLastError());
			NET_DVR_Cleanup(); 
			return  -1;
		}
        NET_DVR_FILECOND_V40 struFileCond={0};
		struFileCond.dwFileType = 0xFF;
		struFileCond.lChannel = 1;
		struFileCond.dwIsLocked = 0xFF;
		struFileCond.dwUseCardNo = 0;
		struFileCond.struStartTime.dwYear   = 2011;
		struFileCond.struStartTime.dwMonth  = 3;
		struFileCond.struStartTime.dwDay    = 1;
		struFileCond.struStartTime.dwHour   = 10;
		struFileCond.struStartTime.dwMinute = 6;
		struFileCond.struStartTime.dwSecond =50;
		struFileCond.struStopTime.dwYear    = 2011;
		struFileCond.struStopTime.dwMonth   = 3;
		struFileCond.struStopTime.dwDay     = 1;
		struFileCond.struStopTime.dwHour    = 11;
		struFileCond.struStopTime.dwMinute  = 7;
		struFileCond.struStopTime.dwSecond  = 0;

		//---------------------------------------
		//查找录像文件
		int lFindHandle = VideoFindDown_HK::NET_DVR_FindFile_V40(lUserID, &struFileCond);
		if(lFindHandle < 0)
		{
			//printf("find file fail,last error %d\n",NET_DVR_GetLastError());
			System::Console::WriteLine("find file fail,last error %d\n",NET_DVR_GetLastError());
			return -1;
		}
		NET_DVR_FINDDATA_V40 struFileData;
		while(true)
		{
			int result = VideoFindDown_HK::NET_DVR_FindNextFile_V40(lFindHandle, &struFileData);
			if(result == NET_DVR_ISFINDING)
			{
				continue;
			}
			else if(result == NET_DVR_FILE_SUCCESS)
			{
				char strFileName[256] = {0};
				//sprintf(strFileName, "./%s", struFileData.sFileName);
				//System::Console::WriteLine(strFileName, "./%s", struFileData.sFileName);
				saveRecordFile(lUserID, struFileData.sFileName,  strFileName);
				break;
			}
			else if(result == NET_DVR_FILE_NOFIND || result == NET_DVR_NOMOREFILE)
			{	
				break;
			}
			else
			{
				//printf("find file fail for illegal get file state");
				System::Console::WriteLine("download err [%d]\n",NET_DVR_GetLastError());
				break;
			}
		}

		//停止查找
		if(lFindHandle >= 0)
		{
			NET_DVR_FindClose_V30(lFindHandle);
		}	

		//注销用户
		NET_DVR_Logout(lUserID);
		//释放SDK资源
		NET_DVR_Cleanup();
		return  0;


}

int VideoFindDown_HK::VideoFindDown_HKer::startRealPlay_HK(char * ip,char * userName, char * userPassword,IntPtr render_wnd,WORD wDvrPort,int nChannel)
{
	        // 初始化																	  
			NET_DVR_Init();

			// 注册设备 
			//LONG lUserID;
			NET_DVR_DEVICEINFO_V30 struDeviceInfo;
			m_lUserID = VideoFindDown_HK::NET_DVR_Login_V30(ip, wDvrPort, userName , userPassword, &struDeviceInfo);

			if (m_lUserID < 0)
			{ 
				int nError = NET_DVR_GetLastError();
				MessageBox::Show("DVR注册出错！");
				//NET_DVR_Cleanup(); 
				return -1; 
			}
											   
			//--------------------------------------- 
			//设置异常消息回调函数 
			//NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
			
			//---------------------------------------			
			//启动预览并设置回调数据流 

			NET_DVR_CLIENTINFO2 ClientInfo = {0}; //数组初始化时，定义第一个元素为零后，其它就都为零了。  

			//赋给播放窗口句柄
			ClientInfo.hPlayWnd = render_wnd; //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空 
			ClientInfo.lChannel = nChannel;    //预览通道号 
			ClientInfo.lLinkMode = 0;   /*最高位(31)为0表示主码流，为1表示子码流。0～30位表示连接方式：
									     0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式; 4- RTP over RTSP，5- RTSP over HTTP */ 
			ClientInfo.sMultiCastIP = NULL; //多播地址，需要多播预览时配置
			
			BOOL bPreviewBlock = false; //请求码流过程是否阻塞，0：否，1：是

			//LONG lRealPlayHandle; 
			m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID, &ClientInfo, NULL, NULL, 0); 
			if (m_lRealPlayHandle < 0) 
			{ 
				//printf("NET_DVR_RealPlay_V30 error\n"); 
				NET_DVR_Logout(m_lUserID); 
				NET_DVR_Cleanup(); 
				return -1;
			} 
			m_bIsRealPlay=true;

			return 0;

}
int VideoFindDown_HK::VideoFindDown_HKer::stopRealPlay_HK()
{
	//---------------------------------------
	//关闭预览 
	if(m_bIsRealPlay==false)//若是已经不在播放放弃执行logout
		return 0;
	NET_DVR_StopRealPlay(m_lRealPlayHandle); 
	DWORD dError=NET_DVR_GetLastError();
	//注销用户
	NET_DVR_Logout(m_lUserID); 
	dError=NET_DVR_GetLastError();
	//释放SDK资源
	//NET_DVR_Cleanup();
	m_bIsRealPlay=false;
	m_lRealPlayHandle=-1;
	m_lUserID=-1;

	return  0;

}

int VideoFindDown_HK::VideoFindDown_HKer::startRealPlay_HK(String ^ s_ip,String ^s_userName, String ^ s_userPassword,IntPtr render_wnd,WORD wDvrPort,String^ sChannel)
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
	return startRealPlay_HK(ip, userName, userPassword, render_wnd,wDvrPort,channelNumber);
}

int VideoFindDown_HK::VideoFindDown_HKer::searchRecordList(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,/*std::vector<NET_DVR_FINDDATA_V40*>&*/ ArrayList^&  recordList)
{
	//if (recordList!=nullptr)
	//{
	//	//
	//	//recordlist=nullptr;
	//}
	// 初始化																	  
	NET_DVR_Init();

	// 注册设备 
	//LONG lUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	m_lUserID = VideoFindDown_HK::NET_DVR_Login_V30(ip, wDvrPort, userName , userPassword, &struDeviceInfo);

	if (m_lUserID < 0)
	{ 
		int nError = NET_DVR_GetLastError();
		//MessageBox::Show("DVR注册出错！");
		//NET_DVR_Cleanup(); 
		return -1; 
	}

	NET_DVR_FILECOND_V40 struFileCond={0};
	struFileCond.dwFileType = 0xFF;
	struFileCond.lChannel =lChannel;// 1;
	struFileCond.dwIsLocked = 0xFF;
	struFileCond.dwUseCardNo = 0;
	struFileCond.struStartTime.dwYear   = recordStartTime->Year;
	struFileCond.struStartTime.dwMonth  = recordStartTime->Month;
	struFileCond.struStartTime.dwDay    = recordStartTime->Day;
	struFileCond.struStartTime.dwHour   = recordStartTime->Hour;
	struFileCond.struStartTime.dwMinute = recordStartTime->Minute;
	struFileCond.struStartTime.dwSecond =recordStartTime->Second;
	struFileCond.struStopTime.dwYear    = recordStopTime->Year;
	struFileCond.struStopTime.dwMonth   = recordStopTime->Month;
	struFileCond.struStopTime.dwDay     = recordStopTime->Day;
	struFileCond.struStopTime.dwHour    = recordStopTime->Hour;
	struFileCond.struStopTime.dwMinute  = recordStopTime->Minute;
	struFileCond.struStopTime.dwSecond  = recordStopTime->Second;

	//---------------------------------------
	//查找录像文件
	int lFindHandle = VideoFindDown_HK::NET_DVR_FindFile_V40(m_lUserID, &struFileCond);
	if(lFindHandle < 0)
	{
		//printf("find file fail,last error %d\n",NET_DVR_GetLastError());
		DWORD dError=NET_DVR_GetLastError();
		System::Console::WriteLine("find file fail,last error %d\n",dError);
		LONG lError=(LONG)dError;
		char* cErrorMessage=NET_DVR_GetErrorMsg(&lError);
		String^ sErrorMessage=UnmanagedStringA2ManagedString(cErrorMessage);
		//MessageBox::Show(sErrorMessage);
		return -1;
	}
	
	while(true)
	{
		VideoFindDown_HK::NET_DVR_FINDDATA_V40 struFileData;
		int result = VideoFindDown_HK::NET_DVR_FindNextFile_V40(lFindHandle,&struFileData);
		if(result == NET_DVR_ISFINDING)
		{
			continue;
		}
		else if(result == NET_DVR_FILE_SUCCESS)
		{
			//char strFileName[256] = {0};
			////sprintf(strFileName, "./%s", struFileData.sFileName);
			////System::Console::WriteLine(strFileName, "./%s", struFileData.sFileName);
			//saveRecordFile(lUserID, struFileData.sFileName,  strFileName);
			//break;
			NET_DVR_FINDDATA_V40_managed^ struFileData_managed=gcnew NET_DVR_FINDDATA_V40_managed;
			struFileData_managed->sFileName=UnmanagedStringA2ManagedString(struFileData.sFileName);
			struFileData_managed->dwFileSize=struFileData.dwFileSize;
			struFileData_managed->struStartTime=gcnew System::DateTime(
				struFileData.struStartTime.dwYear,
				struFileData.struStartTime.dwMonth,
				struFileData.struStartTime.dwDay,
				struFileData.struStartTime.dwHour,
				struFileData.struStartTime.dwMinute,
				struFileData.struStartTime.dwSecond,
				0/*,
				calender*/
				);
			struFileData_managed->struStopTime=gcnew System::DateTime(
				struFileData.struStopTime.dwYear,
				struFileData.struStopTime.dwMonth,
				struFileData.struStopTime.dwDay,
				struFileData.struStopTime.dwHour,
				struFileData.struStopTime.dwMinute,
				struFileData.struStopTime.dwSecond,
				0//,
				//calendar  
				);
				
			recordList->Add(struFileData_managed);
		}
		else if(result == NET_DVR_FILE_NOFIND || result == NET_DVR_NOMOREFILE)
		{	
			break;
		}
		else
		{
			//printf("find file fail for illegal get file state");
			System::Console::WriteLine("download err [%d]\n",NET_DVR_GetLastError());
			break;
		}
	}

	//停止查找
	if(lFindHandle >= 0)
	{
		NET_DVR_FindClose_V30(lFindHandle);
	}	

	//注销用户
	NET_DVR_Logout(m_lUserID);
	//释放SDK资源
	//NET_DVR_Cleanup();
	return  0;
}

String^ VideoFindDown_HK::UnmanagedStringA2ManagedString(char *pIn)
{
	String^ strOut = Marshal::PtrToStringAnsi(static_cast<IntPtr>(pIn));
	return strOut;
}


int VideoFindDown_HK::VideoFindDown_HKer::searchRecordList(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,ArrayList/*<NET_DVR_FINDDATA_V40^>*/^&  recordList)
{
	char * ip = ManagedString2UnmanagedCharPtr(s_ip);
	char * userName = ManagedString2UnmanagedCharPtr(s_userName);
	char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);
	return searchRecordList(ip, userName, userPassword, wDvrPort,lChannel,recordStartTime,recordStopTime,recordList);
}

int VideoFindDown_HK::VideoFindDown_HKer::downRecordFile(char * ip,char * userName, char * userPassword,WORD wDvrPort,LONG lChannel,char * srcfile,char * destfile,int* sDownText)
{
	//if (recordList!=nullptr)
	//{
	//	//
	//	//recordlist=nullptr;
	//}
	// 初始化																	  
	NET_DVR_Init();

	// 注册设备 
	//LONG lUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	m_lUserID = VideoFindDown_HK::NET_DVR_Login_V30(ip, wDvrPort, userName , userPassword, &struDeviceInfo);

	if (m_lUserID < 0)
	{ 
		int nError = NET_DVR_GetLastError();
		MessageBox::Show("DVR注册出错！");
		//NET_DVR_Cleanup(); 
		return -1; 
	}

	//---------------------------------------
	//下载录像文件
	try
	{
		int bRes = 1;
		int hPlayback = 0;
		if( (hPlayback = NET_DVR_GetFileByName(m_lUserID, srcfile, destfile)) < 0 )
		{
			//printf( "GetFileByName failed. error[%d]\n", NET_DVR_GetLastError());
			System::Console::WriteLine("GetFileByName failed. error[%d]\n", NET_DVR_GetLastError());
			bRes= -1;
			return bRes;
		}

		if(!NET_DVR_PlayBackControl_V40(hPlayback, NET_DVR_PLAYSTART, NULL,0,NULL,NULL))
		{
			//printf("play back control failed [%d]\n",NET_DVR_GetLastError());
			System::Console::WriteLine("NET_DVR_PlayBackControl_V40 failed. error[%d]\n", NET_DVR_GetLastError());
			bRes=-1;
			return bRes;
		}

		//int nPos = 0;
		int nPos;
		for(nPos = 0;  nPos < 100&&nPos>=0; nPos = NET_DVR_GetDownloadPos(hPlayback))
		{
			//printf("Be downloading...%d %%\n", nPos);
			//sDownText=nPos.ToString();
			//DevComponents::DotNetBar::SuperGrid::GridCell^ cell=(DevComponents::DotNetBar::SuperGrid::GridCell^)sDownText;//
			//cell->Value=nPos.ToString();
			*sDownText=nPos;
			System::Console::WriteLine("Be downloading...%d %%\n", nPos);
			Sleep(1000);  //millisecond
		}
		//printf("have got %d\n", nPos);
		System::Console::WriteLine("have got %d\n", nPos);

		if(!NET_DVR_StopGetFile(hPlayback))
		{
			//printf("failed to stop get file [%d]\n",NET_DVR_GetLastError());
			System::Console::WriteLine("failed to stop get file [%d]\n",NET_DVR_GetLastError());
			MessageBox::Show("failed to stop get file !\n ErrorCode:"+(NET_DVR_GetLastError()).ToString());
			bRes = -1;
			return bRes;
		}
		//printf("%s\n",srcfile);
		//System::Console::WriteLine("%s\n",srcfile);// wrong...

		if(nPos<0||nPos>100)
		{
			//printf("download err [%d]\n",NET_DVR_GetLastError());
			System::Console::WriteLine("download err [%d]\n",NET_DVR_GetLastError());
			MessageBox::Show("download error!\n ErrorCode:"+(NET_DVR_GetLastError()).ToString());
			bRes=-1;
			return bRes;
		}
		/*else
		{
			return 0;
		}*/
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message);
		return -1;
	}	 
	

	//注销用户
	NET_DVR_Logout(m_lUserID);
	//释放SDK资源
	//NET_DVR_Cleanup();//这句话在快速调用时时长崩溃，不再使用
	return  0;
}
bool VideoFindDown_HK::VideoFindDown_HKer::isRealPlay()
{
	return m_bIsRealPlay;
}

int VideoFindDown_HK::VideoFindDown_HKer::downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,String ^ s_srcfile,String ^ s_destfile,int* sDownText)
{
	char * ip = ManagedString2UnmanagedCharPtr(s_ip);
	char * userName = ManagedString2UnmanagedCharPtr(s_userName);
	char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);
	char * srcfile = ManagedString2UnmanagedCharPtr(s_srcfile);
	char * destfile = ManagedString2UnmanagedCharPtr(s_destfile);
	return downRecordFile(ip, userName, userPassword, wDvrPort,lChannel,srcfile,destfile,sDownText);
}
LONG VideoFindDown_HK::VideoFindDown_HKer::getUserID()
{
	return m_lUserID;
}
LONG VideoFindDown_HK::VideoFindDown_HKer::getRealPlayHandle()
{
	return m_lRealPlayHandle;
}

//在实时播放中截图功能，filename为全路径名（后缀只能为jpg或bmp)
BOOL VideoFindDown_HK::VideoFindDown_HKer::capturePicInRealPlay(String^ filename)
{
	char* pcFilename=ManagedString2UnmanagedCharPtr(filename);
	//文件类型
	String^ sType=filename->Substring(filename->LastIndexOf(".")+1);
	DWORD    dwCaptureMode;
	if (sType->Equals("jpg")||sType->Equals("JPG"))
	{
		dwCaptureMode=JPEG_MODE;
	}
	else if(sType->Equals("BMP")||sType->Equals("bmp"))
	{
		dwCaptureMode=BMP_MODE;
	}
	else
	{
		//dwCaptureMode=BMP_MODE;
		return false;
	}

	BOOL bRet=NET_DVR_SetCapturePictureMode(dwCaptureMode);
	if (FALSE==bRet)
	{
		return false;
	}
	BOOL bRetCap=NET_DVR_CapturePicture(m_lRealPlayHandle,pcFilename);
	return bRetCap;
}

//在实时播放中开始保存实时数据，filename为全路径名
BOOL VideoFindDown_HK::VideoFindDown_HKer::startSaveRealData(String^ sFileName)
{
	char* pcFileName=ManagedString2UnmanagedCharPtr(sFileName);
	BOOL bRet=  NET_DVR_SaveRealData(m_lRealPlayHandle,pcFileName);
	if (TRUE==bRet)
	{
		m_bIsSaveRealData=TRUE;
	}
	return bRet;
}
//在实时播放中停止保存实时数据
BOOL VideoFindDown_HK::VideoFindDown_HKer::stopSaveRealData()
{
	if (false==m_bIsSaveRealData)
	{
		return TRUE;
	}
	BOOL bRet= NET_DVR_StopSaveRealData(m_lRealPlayHandle);
	if (TRUE==bRet)
	{
		m_bIsSaveRealData=false;
	}
	return bRet;
}
//在实时播放中截图功能，filename为全路径名（后缀只能为bmp，，填错则返回失败)，这个函数因为仅支持默认的bmp会快一些
BOOL VideoFindDown_HK::VideoFindDown_HKer::capturePicInRealPlay_onlyBmp(String^ filename)
{
	char* pcFilename=ManagedString2UnmanagedCharPtr(filename);
	BOOL bRetCap=NET_DVR_CapturePicture(m_lRealPlayHandle,pcFilename);
	return bRetCap;
}
bool VideoFindDown_HK::VideoFindDown_HKer::IsSaveRealData()
{
	return m_bIsSaveRealData;
}

ArrayList^ VideoFindDown_HK::VideoFindDown_HKer::HKDVR_AddTest(String ^ s_ip, WORD w_port, String ^s_userName, String ^ s_userPassword)
		{
			char * ip = ManagedString2UnmanagedCharPtr(s_ip);
			char * userName = ManagedString2UnmanagedCharPtr(s_userName);
			char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);

			ArrayList^ ChanName = gcnew ArrayList();

			// 初始化
			NET_DVR_Init();

			// 注册设备 
			LONG lUserID;
			NET_DVR_DEVICEINFO_V30 struDeviceInfo;
			lUserID = VideoFindDown_HK::NET_DVR_Login_V30(ip, w_port, userName , userPassword, &struDeviceInfo);

			if (lUserID < 0)
			{ 
				int nError = NET_DVR_GetLastError();
				/*System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
				box->Show("DVR添加出错！");*/
				NET_DVR_Cleanup(); 
				return nullptr; 
			}

			NET_DVR_PICCFG_V30	m_struPicCfg[MAX_CHANNUM_V30];

			//获得模拟通道号
			int nChanNum = struDeviceInfo.byChanNum;
			DWORD dwReturned = 0;
			for (int i=struDeviceInfo.byStartChan;i<=struDeviceInfo.byStartChan+nChanNum-1;i++)
			{
				//获取通道状态
				if (VideoFindDown_HK::NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_PICCFG_V30, i, &m_struPicCfg[i], sizeof(NET_DVR_PICCFG_V30), &dwReturned))
				{		
					char cTemp[100] = {0};		
					memcpy(cTemp,m_struPicCfg[i].sChanName,NAME_LEN);		
					String ^ str_ChanName = gcnew String(cTemp);

					ChanName->Add(str_ChanName);
				}
				else
				{
					/*System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
					box->Show("DVR通道名称获取出错，添加失败！");*/
					return nullptr;
				}												   
			}
			////获得数字通道号
			int nChanNum_d = struDeviceInfo.byIPChanNum;
			DWORD dwReturned_d = 0;
			for (int i=33/*struDeviceInfo.byStartDChan*/;i<=32+nChanNum_d;i++)
			{
				//int a=i;
				//ChanName->Add("test");
				//获取通道状态
				if (VideoFindDown_HK::NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_PICCFG_V30, i, &m_struPicCfg[i], sizeof(NET_DVR_PICCFG_V30), &dwReturned_d))
				{		
					char cTemp[100] = {0};		
					memcpy(cTemp,m_struPicCfg[i].sChanName,NAME_LEN);		
					String ^ str_ChanName = gcnew String(cTemp);

				ChanName->Add(str_ChanName);
				}
				else
				{
					/*System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
					box->Show("DVR通道名称获取出错，添加失败！");*/
					return nullptr;
				}												   
			}
			//注销用户
			NET_DVR_Logout(lUserID); 
			//释放SDK资源
			NET_DVR_Cleanup();
			return ChanName;
		}
ArrayList^ VideoFindDown_HK::VideoFindDown_HKer::HKDVR_AddTestEx(String ^ s_ip, WORD w_port, String ^ & s_userName, String ^ s_userPassword, int & AnChanNum,int &  IPChanNum,int &  AnChanStartNum,int &  IPChanStartNum,int& nError)
		{
			char * ip = ManagedString2UnmanagedCharPtr(s_ip);
			char * userName = ManagedString2UnmanagedCharPtr(s_userName);
			char * userPassword = ManagedString2UnmanagedCharPtr(s_userPassword);

			ArrayList^ ChanName = gcnew ArrayList();

			// 初始化
			NET_DVR_Init();

			// 注册设备 
			LONG lUserID;
			NET_DVR_DEVICEINFO_V30 struDeviceInfo;
			lUserID = VideoFindDown_HK::NET_DVR_Login_V30(ip, w_port, userName , userPassword, &struDeviceInfo);

			if (lUserID < 0)
			{ 
				nError = NET_DVR_GetLastError();
				//String^ s="DVR登陆失败！\n 错误码："+nError.ToString();
				////s+="hahgagaghahgag";
				//System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
				//box->Show(s);
				NET_DVR_Cleanup(); 
				return nullptr; 
			}

			NET_DVR_PICCFG_V30	m_struPicCfg[MAX_CHANNUM_V30];

			//获得模拟通道号
			int nChanNum = struDeviceInfo.byChanNum;
			AnChanNum=struDeviceInfo.byChanNum;
			AnChanStartNum=struDeviceInfo.byStartChan;
			DWORD dwReturned = 0;
			for (int i=struDeviceInfo.byStartChan;i<=struDeviceInfo.byStartChan+nChanNum-1;i++)
			{
				//获取通道状态
				if (VideoFindDown_HK::NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_PICCFG_V30, i, &m_struPicCfg[i], sizeof(NET_DVR_PICCFG_V30), &dwReturned))
				{		
					char cTemp[100] = {0};		
					memcpy(cTemp,m_struPicCfg[i].sChanName,NAME_LEN);		
					String ^ str_ChanName = gcnew String(cTemp);

					ChanName->Add(str_ChanName);
				}
				else
				{
					/*System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
					box->Show("DVR通道名称获取出错，添加失败！");*/
					nError=NET_DVR_GetLastError();
					return nullptr;
				}												   
			}
			////获得数字通道号
			int nChanNum_d = struDeviceInfo.byIPChanNum;
			IPChanNum=struDeviceInfo.byIPChanNum;
		    //IPChanStartNum=struDeviceInfo.byStartDChan;//v30里面尚未由此定义
			IPChanStartNum=33;
			DWORD dwReturned_d = 0;
			for (int i=33/*struDeviceInfo.byStartDChan*/;i<=32+nChanNum_d;i++)
			{
				//int a=i;
				//ChanName->Add("test");
				//获取通道状态
				if (VideoFindDown_HK::NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_PICCFG_V30, i, &m_struPicCfg[i], sizeof(NET_DVR_PICCFG_V30), &dwReturned_d))
				{		
					char cTemp[100] = {0};		
					memcpy(cTemp,m_struPicCfg[i].sChanName,NAME_LEN);		
					String ^ str_ChanName = gcnew String(cTemp);

				    ChanName->Add(str_ChanName);
				}
				else
				{
					/*System::Windows::Forms::MessageBox^ box = gcnew System::Windows::Forms::MessageBox();
					box->Show("DVR通道名称获取出错，添加失败！");*/
					nError=NET_DVR_GetLastError();
					return nullptr;
				}												   
			}
			//注销用户
			NET_DVR_Logout(lUserID); 
			//释放SDK资源
			//NET_DVR_Cleanup();
			return ChanName;
		}
