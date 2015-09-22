#pragma once
#include <windows.h>
#include "VideoFindDown_DH.h"
#include "VideoFindDown_HK.h"

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


namespace VideoFindDown {

	ref struct COMMON_RECORDINFO
	{
		String^ sFileName;//�ļ���
		System::DateTime^ struStartTime;//�ļ��Ŀ�ʼʱ��
		System::DateTime^ struStopTime;//�ļ��Ľ���ʱ��
		DWORD dwFileSize;//�ļ��Ĵ�С

		//�����е�����
		unsigned int        ch;                         // ͨ����
		unsigned int        framenum;                   // �ļ���֡��
		unsigned int        driveno;                    // ���̺�(��������¼��ͱ���¼������ͣ�0��127��ʾ����¼��,����64��ʾ����1��128��ʾ����¼��)
		unsigned int        startcluster;               // ��ʼ�غ�
		BYTE                nRecordFileType;            // ¼���ļ�����  0����ͨ¼��1������¼��2���ƶ���⣻3������¼��4��ͼƬ, 5: ����¼��
		BYTE                bImportantRecID;            // 0:��ͨ¼�� 1:��Ҫ¼��
		BYTE                bHint;                      // �ļ���λ����
		BYTE                bRecType;                   // 0-������¼�� 1-����1��¼�� 2-������2 3-������3¼��

		//�������ӵ�����
		//String^ sCamID;//����Ψһȷ�������
		int indexofDev;//���豸���е�������
		
	};


	//////////////////////////////////////////////////////////////////////////
	//added function,copied from HK example
	//�йܷ��й�ת������
	//copied from fengyuan realtimeplayer
	char* ManagedString2UnmanagedCharPtr(String^ strIn);
	//�����йܵ�ANSI�ַ���ת�����й��ַ� �� 
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
	int restartRealPlay();//ִ�й�startRealPlay���������ǰ����Ϣ��������
	//����в��ҵ���¼�������Ϣ��ִ������
	int downRecordFile(String ^ s_ip,String ^s_userName, String ^ s_userPassword,WORD wDvrPort,LONG nChannel,COMMON_RECORDINFO^ recordInfo,String ^ destfile,String^ devSupplier,int* nPos);
	//���һ������Ϊ��������ذٷֱ�
	bool isRealPlay();
	LLONG getUserID();
	LLONG getRealPlayHandle();
public:
	BOOL capturePicInRealPlay(String^ sFilename);//��֧��bmpģʽ����׺ֻ��Ϊbmp��������������ļ�������,ʵ������bmp)
	//���ڲ�������Ч������ͬ��
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

