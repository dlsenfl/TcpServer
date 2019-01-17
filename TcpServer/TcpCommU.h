//---------------------------------------------------------------------------

#ifndef TcpCommUH
#define TcpCommUH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>			//�߰��� ���
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
const int MAX_TCP_BUFFER = 1024;
//---------------------------------------------------------------------------
class TTcpBase
{
private:
protected:
	int __fastcall fnDefaultEncoding(BYTE *pBuffer, int &iIndex, void *pData, int iDataSize, int iTail);				// ����Ǿ��ִ� ����ü �����͸� ���� ���ۿ� �״� �Լ�
	int __fastcall fnDefaultDecoding(BYTE *pBuffer, int &iIndex, int iSize, void *pData, int iDataSize);	// ���� ���ۿ��� ����ü�� ������ �����͸� �����ϴ� �Լ�
public:
	__fastcall 	TTcpBase();
	__fastcall ~TTcpBase();
};
//---------------------------------------------------------------------------
struct TstHeader	//9����Ʈ
{
	BYTE bySTX1;
	BYTE bySTX2;
	WORD wVMSID;
	BYTE byOpCode;
	BYTE bySFNo;
	BYTE byAFNo;
	WORD wDataLen;
};
//---------------------------------------------------------------------------
struct TstTail		//4����Ʈ
{
	WORD wCRC_16;
	BYTE byETX1;
	BYTE byETX2;
};
//---------------------------------------------------------------------------
class TProtocol	: public TTcpBase
{
private:
	TstHeader	m_stHeader;
	TstTail	  	m_stTail;
	void	   *m_pBody;
	int			m_iDataLen;
	int		    m_iSendPackSize;
	int		  	m_iRecvPackSize;
	BYTE	  	m_bySendPacket[MAX_TCP_BUFFER];
	BYTE	  	m_byRecvPacket[MAX_TCP_BUFFER];
private:
	void __fastcall fnSetDataLen(int a_iValue);
	int  __fastcall fnGetDataLen();
public:
	__fastcall  TProtocol();
	__fastcall ~TProtocol();
public:
	int  __fastcall fnEncoding();
	int  __fastcall fnDecoding(BYTE *a_pBuffer, int a_iSize);
	void __fastcall fnDeleteBody();
//	void __fastcall fnSetOpCode(int a_op);
public:
	__property	BYTE	Code    = {read=m_stHeader.byOpCode		,	write=m_stHeader.byOpCode	};
	__property	int		DataLen	= {read=fnGetDataLen			,	write=fnSetDataLen			};
};
//---------------------------------------------------------------------------
struct TstCode05
{
	BYTE byCtrlCode;
	BYTE byCtrlData01;
	BYTE byCtrlData02[2];
	char byCtrlData08[8];
	char byCtrlData14[14];
};
//---------------------------------------------------------------------------
class TTcpData05   	: public TTcpBase
{
private:
	TstCode05 m_stData;
	int __fastcall fnGetDataLen();
public:
	int __fastcall fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iTail);
	int __fastcall fnDecodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iSize);
public:
	__fastcall  TTcpData05();
	__fastcall ~TTcpData05();
public:
	__property int DataLen	=	{read=fnGetDataLen	};
};
//---------------------------------------------------------------------------
struct TstCode06							// State     //12����Ʈ
{
	BYTE byDoor;							//	Open: 0x00, Close: 0x01, Unkown: 0x09
	BYTE byPower;							//  On: 0x00, 	Off: 0x01
	BYTE byFan;								//  On: 0x00, 	Off: 0x01,   Unkown: 0x09
	BYTE byHeater;                         	//  On: 0x00, 	Off: 0x01,   Unkown: 0x09
	BYTE byOuterLight;                 	   	//  On: 0x00, 	Off: 0x01
	BYTE byFoamKind;						//  CFoam: 0x00, 	DFoam: 0x01
	BYTE byReplayCheck;                	    //  Normal: 0x00, 	RePlay: 0x01
	BYTE byPowerOdd;						//	Eorror: 0x00,	Normal:	0x01,	0x02: Unknown
	BYTE byModulOdd;						//  Eorror: 0x00,	Normal:	0x01,	0x02: Unknown
	BYTE byTemperature;						//  Range:0x00 ~ 0xfe(-127~127),	0xff: Unknown
	BYTE byDisplayBright;             	    //  ����� ǥ��
	BYTE byEtc2;							// 	��Ÿ2
};
//---------------------------------------------------------------------------
class TTcpData06	: public TTcpBase
{
private:
	TstCode06 m_stData;
	int __fastcall fnGetDataLen();
public:
	int __fastcall fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iTail);
	int __fastcall fnDecodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iSize);
public:
	__fastcall  TTcpData06();
	__fastcall ~TTcpData06();
public:
	__property int DataLen	=	{read=fnGetDataLen	};
};
//---------------------------------------------------------------------------
struct TstCode07                      		// LocalState
{
	BYTE byPowerMode;					   	//	Off: 0x00,  On: 0x01, Auto: 0x02
	BYTE byFan;								//  Off: 0x00, 	On: 0x01, Auto: 0x02,  Unkown: 0x09
	BYTE byFanTemper;						//	Range:0x00 ~ 0x3f
	BYTE byEtc1;							// 	��Ÿ1
	BYTE byHeater;                        	//  Off: 0x00, 	On: 0x01, Auto: 0x02,  Unkown: 0x09
	BYTE byHeaterTemper;                  	//	Range:0x00 ~ 0x3f
	BYTE byDisplayBright;                 	//  Manual: 0x00,	Auto: 0x01
	BYTE byFlashCycle;						//	Range: 0x00 ~ 0x1e(0.1 ~ 3.0)
	BYTE byOuterLightOperating;				//	Off: 0x00,	On: 0x01,	Auto: 0x02
	BYTE byOuterLightOnBright;            	//	����� ǥ��
	BYTE byDefaultScenarioWait;           	//	Range: 0x00 ~ 0x1e(��)
	BYTE byEtc2;							// 	��Ÿ2

	BYTE byLocalTime[6];                    //	����Ͻú���
};
//---------------------------------------------------------------------------
class TTcpData07	: public TTcpBase
{
private:
	TstCode07 m_stData;
	int __fastcall fnGetDataLen();
public:
	int __fastcall fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iTail);
	int __fastcall fnDecodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iSize);
public:
	__fastcall  TTcpData07();
	__fastcall ~TTcpData07();
public:
	__property int DataLen	=	{read=fnGetDataLen	};
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
