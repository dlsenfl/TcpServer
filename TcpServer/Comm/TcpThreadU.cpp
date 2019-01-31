//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TcpThreadU.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TcpThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
__fastcall TTcpThread::TTcpThread(TApplicationInfo *a_pAppInfo)
	: TThread(true)		// false �̸� ������ ���Ŀ� �ٷ� ����.
{
	m_pAppInfo = a_pAppInfo;
	IdUDPServerThr = new TIdUDPServer();
	IdUDPServerThr->OnUDPRead = IdUDPServerThrUDPRead;

	Priority 		= tpNormal;
	FreeOnTerminate = true;                            //	�����尡 Terminate�Ҷ� �ڵ����� �ı������� �����ϴ� property
	Suspended		= false;                           //	������ �������� �����ϴ� property
}
//---------------------------------------------------------------------------
__fastcall TTcpThread::~TTcpThread()
{
	delete m_pAppInfo;
	delete IdUDPServerThr;
}
//---------------------------------------------------------------------------
void __fastcall TTcpThread::Execute()
{
	MSG stMsg;                                           // ���������� ���� �޼����� ���� �׸�
	while(!Terminated){                                  //	Terminated�� �Ӽ����μ� ���� �����尡 �۾��������� ���θ� ����Ű�� boolean�� �Ӽ�
		try{
			if(GetMessage(&stMsg, NULL, 0, 0) == 0){
				Terminate();                             //	�޼����� ���� 0�̸�(�޼����� ������)
			}else{
				switch(stMsg.message){
					case WM_TCP_OPEN:                    //	�޼����� WM_TCP_OPEN�̸�
					fnTcpOpen();
					break;
				}
			}
		}
		catch(Exception& E){
			OutputDebugString(E.Message.c_str());
		}
	}


}
//---------------------------------------------------------------------------
void __fastcall TTcpThread::fnTcpOpen()
{
	IdUDPServerThr->DefaultPort = m_pAppInfo->Port;
	IdUDPServerThr->Active		= true;
}
//---------------------------------------------------------------------------
void __fastcall TTcpThread::IdUDPServerThrUDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
		  TIdSocketHandle *ABinding)
{
	int iResult = 0;
	TProtocol *pRecvPack;
	BYTE byBuf[MAX_TCP_BUFFER];
	ZeroMemory(byBuf, sizeof(byBuf));
	BytesToRaw(AData, byBuf, sizeof(byBuf));

	pRecvPack = new TProtocol();
	if((iResult=pRecvPack->fnDecoding(byBuf, AData.Length)) == 0){
		switch (pRecvPack->Code) {
			case 0x05 : fnRecvThrData05(pRecvPack); break;
//			case 0x06 : fnSendData06(pRecvPack); break;
//			case 0x07 : fnSendData07(pRecvPack); break;
			default	  : break;
		}
	}else{
		OutputDebugStringW(L"Decoding Error");
	}
	delete pRecvPack;
}
//---------------------------------------------------------------------------
void __fastcall TTcpThread::fnRecvThrData05(TProtocol *a_pRecvPack)
{
	TTcpData05 *pData;
	pData = (TTcpData05*)a_pRecvPack->Body;
	switch (pData->CtrlCode) {
		case 0x30 : m_pAppInfo->Status->TcpData05->ModulPower	   = pData->CtrlData01; break;
		case 0x31 :	m_pAppInfo->Status->TcpData05->ResetTime	   = pData->CtrlData01; break;
		case 0x32 :	m_pAppInfo->Status->TcpData05->TryCount		   = pData->CtrlData01; break;
		case 0x33 :	m_pAppInfo->Status->TcpData05->ControlTime	   = pData->CtrlData14; break;
		case 0x34 :	m_pAppInfo->Status->TcpData05->RunTime		   = pData->CtrlData08; break;
		case 0x35 :	m_pAppInfo->Status->TcpData05->BlinkCycle	   = pData->CtrlData01; break;
		case 0x36 :	m_pAppInfo->Status->TcpData05->ScinarioRunTime = pData->CtrlData01; break;
		case 0x37 :	m_pAppInfo->Status->TcpData05->BrightControl1  = pData->CtrlData02; break;
		case 0x38 :	m_pAppInfo->Status->TcpData05->FanControl	   = pData->CtrlData01; break;
		case 0x39 :	m_pAppInfo->Status->TcpData05->HeaterControl   = pData->CtrlData01; break;
		case 0x40 :	m_pAppInfo->Status->TcpData05->LampControl	   = pData->CtrlData01; break;
		case 0x41 :	m_pAppInfo->Status->TcpData05->LedControl	   = pData->CtrlData01; break;
		default	  : break;
	}
	// Save To File
	fnSaveData05(pData);
	fnSendThrData();
	delete pData;
}
//---------------------------------------------------------------------------
void __fastcall TTcpThread::fnSaveData05(TTcpData05 *a_pRecvData)
{
	AnsiString sPath;
	sPath 			= ExtractFilePath(Application->ExeName) + "TcpServer_Project.ini";
	TIniFile *pIni  = new TIniFile(sPath);
	switch (a_pRecvData->CtrlCode) {
		case 0x30 : pIni->WriteInteger("Data05", "ModulPower"	   , m_pAppInfo->Status->TcpData05->ModulPower	   );break;
		case 0x31 :	pIni->WriteInteger("Data05", "ResetTime"	   , m_pAppInfo->Status->TcpData05->ResetTime	   );break;
		case 0x32 :	pIni->WriteInteger("Data05", "TryCount"		   , m_pAppInfo->Status->TcpData05->TryCount	   );break;
		case 0x33 :	pIni->WriteString ("Data05", "ControlTime"	   , m_pAppInfo->Status->TcpData05->ControlTime	   );break;
		case 0x34 :	pIni->WriteString ("Data05", "RunTime"	 	   , m_pAppInfo->Status->TcpData05->RunTime		   );break;
		case 0x35 :	pIni->WriteInteger("Data05", "BlinkCycle"	   , m_pAppInfo->Status->TcpData05->BlinkCycle	   );break;
		case 0x36 :	pIni->WriteInteger("Data05", "ScinarioRunTime" , m_pAppInfo->Status->TcpData05->ScinarioRunTime);break;
		case 0x37 :	pIni->WriteInteger("Data05", "BrightControl"   , m_pAppInfo->Status->TcpData05->BrightControl1 );break;
		case 0x38 :	pIni->WriteInteger("Data05", "FanControl"	   , m_pAppInfo->Status->TcpData05->FanControl	   );break;
		case 0x39 :	pIni->WriteInteger("Data05", "HeaterControl"   , m_pAppInfo->Status->TcpData05->HeaterControl  );break;
		case 0x40 :	pIni->WriteInteger("Data05", "LampControl" 	   , m_pAppInfo->Status->TcpData05->LampControl	   );break;
		case 0x41 :	pIni->WriteInteger("Data05", "LedControl"	   , m_pAppInfo->Status->TcpData05->LedControl	   );break;
		default	  : break;
	}
	delete pIni;
}
//---------------------------------------------------------------------------
void __fastcall TTcpThread::fnSendThrData()
{
	PostMessage(Application->MainFormHandle, WM_TCP_RECV, (WPARAM)5, 1);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


