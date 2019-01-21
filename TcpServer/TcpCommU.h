//---------------------------------------------------------------------------

#ifndef TcpCommUH
#define TcpCommUH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>			//추가한 헤더
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
const int MAX_TCP_BUFFER = 1024;
//---------------------------------------------------------------------------
class TTcpBase
{
private:
protected:
	int __fastcall fnDefaultEncoding(BYTE *pBuffer, int &iIndex, void *pData, int iDataSize, int iTail);  // 저장되어있는 구조체 데이터를 보낼 버퍼에 쌓는 함수
	int __fastcall fnDefaultDecoding(BYTE *pBuffer, int &iIndex, int iSize, void *pData, int iDataSize);  // 받은 버퍼에서 구조체에 저장할 데이터를 추출하는 함수
public:
	__fastcall 	TTcpBase();
	__fastcall ~TTcpBase();
};
//---------------------------------------------------------------------------
struct TstHeader	//9바이트
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
struct TstTail		//4바이트
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
	__property 	void*	Body	= {read=m_pBody					,	write=m_pBody				};
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
	__property BYTE		   CtrlCode	    		=	{read=m_stData.byCtrlCode   	};
	__property BYTE		   CtrlData01	       	=	{read=m_stData.byCtrlData01		};
	__property BYTE		   CtrlData02	 		=	{read=m_stData.byCtrlData02[0]  };
	__property char		   CtrlData08  		 	=	{read=m_stData.byCtrlData08[0]  };
	__property char		   CtrlData14	    	=	{read=m_stData.byCtrlData14[0]  };
	__property int 		   DataLen				=	{read=fnGetDataLen			 	};
};
//---------------------------------------------------------------------------
struct TstCode06							// State     //12바이트
{
	BYTE byDoor;							//	Open: 0x00, Close: 0x01, Unkown: 0x09
	BYTE byPower;							//  On: 0x00, 	Off: 0x01
	BYTE byFan;								//  On: 0x00, 	Off: 0x01,   Unkown: 0x09
	BYTE byHeater;                         	//  On: 0x00, 	Off: 0x01,   Unkown: 0x09
	BYTE byOuterLight;                 	   	//  On: 0x00, 	Off: 0x01
	BYTE byFormKind;						//  CFoam: 0x00, 	DFoam: 0x01
	BYTE byReplayCheck;                	    //  Normal: 0x00, 	RePlay: 0x01
	BYTE byPowerOdd;						//	Eorror: 0x00,	Normal:	0x01,	0x02: Unknown
	BYTE byModulOdd;						//  Eorror: 0x00,	Normal:	0x01,	0x02: Unknown
	BYTE byTemperature;						//  Range:0x00 ~ 0xfe(-127~127),	0xff: Unknown
	BYTE byDisplayBright;             	    //  백분율 표기
	BYTE byEtc2;							// 	기타2
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
	__fastcall  TTcpData06(const TTcpData06 *data);     //	깊은복사 생성자
	__fastcall ~TTcpData06();
public:
	__property BYTE		   Door	    		=	{read=m_stData.byDoor,			write=m_stData.byDoor			};
	__property BYTE		   Power	    	=	{read=m_stData.byPower,			write=m_stData.byPower			};
	__property BYTE		   Fan	    		=	{read=m_stData.byFan,			write=m_stData.byFan		 	};
	__property BYTE		   Heater	    	=	{read=m_stData.byHeater,		write=m_stData.byHeater			};
	__property BYTE		   OuterLight	   	=	{read=m_stData.byOuterLight,	write=m_stData.byOuterLight		};
	__property BYTE		   FormKind	    	=	{read=m_stData.byFormKind,		write=m_stData.byFormKind		};
	__property BYTE		   ReplayCheck	   	=	{read=m_stData.byReplayCheck,	write=m_stData.byReplayCheck   	};
	__property BYTE		   PowerOdd	    	=	{read=m_stData.byPowerOdd,		write=m_stData.byPowerOdd		};
	__property BYTE		   ModulOdd	    	=	{read=m_stData.byModulOdd,		write=m_stData.byModulOdd		};
	__property BYTE		   Temperature	   	=	{read=m_stData.byTemperature,	write=m_stData.byTemperature   	};
	__property BYTE		   DisplayBright   	=	{read=m_stData.byDisplayBright,	write=m_stData.byDisplayBright	};
	__property BYTE		   Etc2	    		=	{read=m_stData.byEtc2,			write=m_stData.byEtc2			};

	__property int 		   DataLen			=	{read=fnGetDataLen					  							};
};
//---------------------------------------------------------------------------
struct TstCode07                      		// LocalState
{
	BYTE byPowerMode;					   	//	Off: 0x00,  On: 0x01, Auto: 0x02
	BYTE byFan;								//  Off: 0x00, 	On: 0x01, Auto: 0x02,  Unkown: 0x09
	BYTE byFanTemper;						//	Range:0x00 ~ 0x3f
	BYTE byEtc1;							// 	기타1
	BYTE byHeater;                        	//  Off: 0x00, 	On: 0x01, Auto: 0x02,  Unkown: 0x09
	BYTE byHeaterTemper;                  	//	Range:0x00 ~ 0x3f
	BYTE byDisplayBright;                 	//  Manual: 0x00,	Auto: 0x01
	BYTE byFlashCycle;						//	Range: 0x00 ~ 0x1e(0.1 ~ 3.0)
	BYTE byOuterLightOperating;				//	Off: 0x00,	On: 0x01,	Auto: 0x02
	BYTE byOuterLightOnBright;            	//	백분율 표기
	BYTE byDefaultScenarioWait;           	//	Range: 0x00 ~ 0x1e(분)
	BYTE byEtc2;							// 	기타2

	BYTE byLocalTime[6];                    //	년월일시분초
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
	__property BYTE		   PowerMode	    		=	{read=m_stData.byPowerMode,				write=m_stData.byPowerMode				};
	__property BYTE		   Fan	    				=	{read=m_stData.byFan,					write=m_stData.byFan		 			};
	__property BYTE		   FanTemper	    		=	{read=m_stData.byFanTemper,				write=m_stData.byFanTemper				};
	__property BYTE		   Etc1	  				 	=	{read=m_stData.byEtc1,					write=m_stData.byEtc1					};
	__property BYTE		   Heater	    			=	{read=m_stData.byHeater,				write=m_stData.byHeater					};
	__property BYTE		   HeaterTemper	   			=	{read=m_stData.byHeaterTemper,			write=m_stData.byHeaterTemper   		};
	__property BYTE		   DisplayBright	    	=	{read=m_stData.byDisplayBright,			write=m_stData.byDisplayBright			};
	__property BYTE		   FlashCycle	    		=	{read=m_stData.byFlashCycle,			write=m_stData.byFlashCycle				};
	__property BYTE		   OuterLightOperating	   	=	{read=m_stData.byOuterLightOperating,	write=m_stData.byOuterLightOperating   	};
	__property BYTE		   OuterLightOnBright   	=	{read=m_stData.byOuterLightOnBright,	write=m_stData.byOuterLightOnBright		};
	__property BYTE		   Etc2	    				=	{read=m_stData.byEtc2,					write=m_stData.byEtc2					};
	__property int DataLen	=	{read=fnGetDataLen	};
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
