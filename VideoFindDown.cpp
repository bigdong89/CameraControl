#include "StdAfx.h"
#include "VideoFindDown.h"
using namespace System::Windows::Forms;
using namespace System::IO;

//////////////////////////////////////////////////////////////////////////
//命名空间中的局部全局方法


//////////////////////////////////////////////////////////////////////////
///类成员函数
VideoFindDown::VideoFindDowner::VideoFindDowner(void)
{
	m_HK=gcnew VideoFindDown_HK::VideoFindDown_HKer;
	m_DH=gcnew VideoFindDown_DH::VideoFindDown_DHer;
	m_sDevSupplier=gcnew String("");//先给个默认值，这样安全些
}
/************************************************************************/
/*        托管string^  转换成非托管const  char*                         */    
/************************************************************************/
char* VideoFindDown::ManagedString2UnmanagedCharPtr(String^ strIn)
{
	System::IntPtr ip = Marshal::StringToHGlobalAnsi(strIn);
	const char* pTemp = static_cast<const char*>(ip.ToPointer());
	char *pOut = new char[strlen(pTemp)+1];
	strcpy(pOut, pTemp);
	Marshal::FreeHGlobal(ip);
	return pOut;
} 
String^ VideoFindDown::UnmanagedStringA2ManagedString(char *pIn)
{
	String^ strOut = Marshal::PtrToStringAnsi(static_cast<IntPtr>(pIn));
	return strOut;
}


/************************************************************************/
/*       类接口:               */
/************************************************************************/
//added by dongjiaquan
int VideoFindDown::VideoFindDowner::searchRecordList(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG lChannel,DateTime^ recordStartTime,DateTime^ recordStopTime,ArrayList/*<NET_DVR_FINDDATA_V40^>*/^&  recordList,String^ devSupplier,int indexofDev)
{
	if (devSupplier->Equals("海康"))
	{
		ArrayList^ temp=gcnew ArrayList;
		int nRet= m_HK->searchRecordList(s_ip,s_userName, s_userPassword, wDvrPort, lChannel, recordStartTime, recordStopTime, temp);
		for(int i=0;i<temp->Count;i++)
		{
			VideoFindDown_HK::NET_DVR_FINDDATA_V40_managed^ info=(VideoFindDown_HK::NET_DVR_FINDDATA_V40_managed^)(temp[i]);
			COMMON_RECORDINFO^ comInfo=gcnew COMMON_RECORDINFO;
			comInfo->sFileName=info->sFileName;
			comInfo->struStartTime=info->struStartTime;
			comInfo->struStopTime=info->struStopTime;
			comInfo->dwFileSize=info->dwFileSize;
			comInfo->indexofDev=indexofDev;
			recordList->Add(comInfo);
		}
		return nRet;
	}
	else if (devSupplier->Equals("大华"))
	{
		ArrayList^ temp=gcnew ArrayList;
		int nRet=m_DH->searchRecordList(s_ip,s_userName, s_userPassword, wDvrPort, lChannel, recordStartTime, recordStopTime, temp);
		for(int i=0;i<temp->Count;i++)
		{
			VideoFindDown_DH::NET_RECORDFILE_INFO_Managed^ info=(VideoFindDown_DH::NET_RECORDFILE_INFO_Managed^)(temp[i]);
			COMMON_RECORDINFO^ comInfo=gcnew COMMON_RECORDINFO;
			comInfo->sFileName=info->filename;
			comInfo->struStartTime=info->starttime;
			comInfo->struStopTime=info->endtime;
			comInfo->dwFileSize=info->size;
			//大华特有的属性的翻译工作
			comInfo->bHint=info->bHint;
			comInfo->bImportantRecID=info->bImportantRecID;
			comInfo->bRecType=info->bRecType;
			comInfo->ch=info->ch;
			comInfo->driveno=info->driveno;
			comInfo->framenum=info->framenum;
			comInfo->nRecordFileType=info->nRecordFileType;
			comInfo->startcluster=info->startcluster;
			comInfo->indexofDev=indexofDev;
			recordList->Add(comInfo);
		}
		return nRet;
	}
	else
	{
		return -1;
	}

}
int VideoFindDown::VideoFindDowner::startRealPlay(String ^ s_ip,String ^s_userName, String ^ s_userPassword,IntPtr render_wnd,WORD wDvrPort,String^ sChannel,String^ devSupplier)
{
	m_ip=s_ip;
	m_userName=s_userName;
	m_userPassword=s_userPassword;
	m_render_wnd=render_wnd;
	m_wDvrPort=wDvrPort;
	m_sChannel=sChannel;
	m_sDevSupplier=devSupplier;
	if (devSupplier->Equals("海康"))
	{
		return m_HK->startRealPlay_HK( s_ip, s_userName, s_userPassword, render_wnd, wDvrPort, sChannel);
	}
	else if (devSupplier->Equals("大华"))
	{
		return m_DH->startRealPlay(s_ip, s_userName, s_userPassword, render_wnd, wDvrPort, sChannel);
	}
	else
	{
		MessageBox::Show("不支持的厂家类型！");
		return -1;
	}
}
int VideoFindDown::VideoFindDowner::stopRealPlay(void)
{
	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->stopRealPlay_HK();
	}
	else if (m_sDevSupplier->Equals("大华"))
	{
		return m_DH->stopRealPlay();
	}
	else
	{
		return -1;
	}
}

//如果有查找到的录像相关信息，执行下行
int VideoFindDown::VideoFindDowner::downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG nChannel,COMMON_RECORDINFO^ recordInfo,String ^ destfile,String^ devSupplier,int* sDownText)
{
	m_sDevSupplier=devSupplier;
	if(devSupplier->Equals("海康"))
	{
		return m_HK->downRecordFile(s_ip,s_userName, s_userPassword, wDvrPort,nChannel,recordInfo->sFileName ,destfile,sDownText);
	}
	else if (devSupplier->Equals("大华"))
	{
		VideoFindDown_DH::NET_RECORDFILE_INFO_Managed^ newRecordInfo=gcnew VideoFindDown_DH::NET_RECORDFILE_INFO_Managed;
		newRecordInfo->filename=recordInfo->sFileName;
		newRecordInfo->starttime=recordInfo->struStartTime;
		newRecordInfo->endtime=recordInfo->struStopTime;
		newRecordInfo->size=recordInfo->dwFileSize;
		//大华特有的属性的翻译工作
		newRecordInfo->bHint=recordInfo->bHint;
		newRecordInfo->bImportantRecID=recordInfo->bImportantRecID;
		newRecordInfo->bRecType=recordInfo->bRecType;
		newRecordInfo->ch=recordInfo->ch;
		newRecordInfo->driveno=recordInfo->driveno;
		newRecordInfo->framenum=recordInfo->framenum;
		newRecordInfo->nRecordFileType=recordInfo->nRecordFileType;
		newRecordInfo->startcluster=recordInfo->startcluster;
		return m_DH->downRecordFile(s_ip,s_userName, s_userPassword, wDvrPort, newRecordInfo,destfile,sDownText);
	}
	else
	{
		return -1;
	}

}

bool VideoFindDown::VideoFindDowner::isRealPlay()
{
	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->isRealPlay();
	}
	else if (m_sDevSupplier->Equals("大华"))
	{
		return m_DH->isRealPlay();
	}
	else
	{
		return -1;
	}

}
LLONG VideoFindDown::VideoFindDowner::getUserID()
{

	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->getUserID();
	}
	else if(m_sDevSupplier->Equals("大华"))
	{
		return m_DH->getUserID();
	}
	else
	{
		return -1;
	}
}
LLONG VideoFindDown::VideoFindDowner::getRealPlayHandle()
{
	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->getRealPlayHandle();
	}
	else if(m_sDevSupplier->Equals("大华"))
	{
		return m_DH->getRealPlayHandle();
	}
	else
	{
		return -1;
	}
}
int VideoFindDown::VideoFindDowner::restartRealPlay()
{
	//安全检查
	if (m_ip==nullptr)
	{
		return -1;
	}
	if (m_userName==nullptr)
	{
		return -1;
	}
	if (m_userPassword==nullptr)
	{
		return -1;
	}
	if (m_wDvrPort<0)
	{
		return -1;
	}
	if (m_sChannel==nullptr)
	{
		return -1;
	}
	/*if (m_render_wnd>=0)
	{
		return -1;
	}*/
	return startRealPlay(m_ip,m_userName,m_userPassword,m_render_wnd,m_wDvrPort,m_sChannel,m_sDevSupplier);
}

//仅在播放中有效
//仅支持bmp模式（后缀只能为bmp，，填错则安填错的文件名命名,实质仍是bmp)
BOOL VideoFindDown::VideoFindDowner::capturePicInRealPlay(String^ sFilename)
{
	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->capturePicInRealPlay(sFilename);
	}
	else if (m_sDevSupplier->Equals("大华"))
	{
		return m_DH->capturePicInRealPlay(sFilename);
	}
	else
		return FALSE;
}
BOOL VideoFindDown::VideoFindDowner::startSaverRealData(String^ sFilename)
{
	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->startSaveRealData(sFilename);
	}
	else if (m_sDevSupplier->Equals("大华"))
	{
		return m_DH->startSaverRealData(sFilename);
	}
	else
		return FALSE;
}
BOOL VideoFindDown::VideoFindDowner::stopSaveRealData()
{
	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->stopSaveRealData();
	}
	else if (m_sDevSupplier->Equals("大华"))
	{
		return m_DH->stopSaveRealData();
	}
	else
		return FALSE;
}
bool VideoFindDown::VideoFindDowner::IsSaveRealData()
{
	if (m_sDevSupplier->Equals("海康"))
	{
		return m_HK->IsSaveRealData();
	}
	else if (m_sDevSupplier->Equals("大华"))
	{
		return m_DH->IsSaveRealData();
	}
	else
		return false;
}

ArrayList^ VideoFindDown::VideoFindDowner::DVR_AddTestEx(String ^ s_ip, WORD w_port, String ^ & s_userName, String ^ s_userPassword, int & AnChanNum,int &  IPChanNum, int &  AnChanStartNum,int &  IPChanStartNum,int& nError,String^ sDevSupplier)
{
	if (sDevSupplier->Equals("海康"))
	{
		return m_HK->HKDVR_AddTestEx(s_ip,w_port,s_userName,s_userPassword,AnChanNum,AnChanStartNum,IPChanNum,IPChanStartNum,nError);
	}
	else if (sDevSupplier->Equals("大华"))
	{
		return m_DH->DVR_AddTestEx(s_ip,w_port,s_userName,s_userPassword,AnChanNum,AnChanStartNum,IPChanNum,IPChanStartNum,nError);
	}
	else
	{
		return gcnew ArrayList;
	}
}