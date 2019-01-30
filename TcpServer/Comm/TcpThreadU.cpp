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
	IdUDPServer = new TIdUDPServer();
	IdUDPServer->OnUDPRead = IdUDPServerUDPRead;

	Priority 		= tpNormal;
	FreeOnTerminate = true;                            //	??
	Suspended		= false;                           //	??
}
//---------------------------------------------------------------------------
__fastcall TTcpThread::~TTcpThread()
{
	delete m_pAppInfo;
	delete IdUDPServer;
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
//					case WM_ACP_OPEN:                    //	�޼����� WM_ACP_OPEN�̸�
//					fnAcpOpen();
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
void __fastcall TTcpThread::IdUDPServerUDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
		  TIdSocketHandle *ABinding)
{

}
//---------------------------------------------------------------------------
