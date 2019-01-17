//---------------------------------------------------------------------------

#pragma hdrstop

#include "TcpCommU.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TTcpBase::TTcpBase()
{
}
//---------------------------------------------------------------------------
__fastcall TTcpBase::~TTcpBase()
{
}
//---------------------------------------------------------------------------
int __fastcall TTcpBase::fnDefaultEncoding(BYTE *pBuffer, int &iIndex, void *pData, int iDataSize)
{
	if(iIndex + iDataSize > MAX_TCP_BUFFER) return 11;
	CopyMemory(pBuffer+iIndex, pData, iDataSize);             //	(���ۿ� ��������ŭ �̵�����ġ�� pData�� iDataSize��ŭ ����)
	iIndex += iDataSize;                                      // 	(iDataSize��ŭ �ε��� ����)
	return 0;
}
//---------------------------------------------------------------------------
int __fastcall TTcpBase::fnDefaultDecoding(BYTE *pBuffer, int &iIndex, int iSize, void *pData, int iDataSize)
{
	if(iIndex + iDataSize > iSize) return 11;
	CopyMemory(pData, pBuffer+iIndex, iDataSize);
	iIndex += iDataSize;
	return 0;
}
//---------------------------------------------------------------------------
__fastcall TProtocol::TProtocol()
{
	ZeroMemory(&m_stHeader, sizeof(m_stHeader));
	ZeroMemory(&m_stTail, 	sizeof(m_stTail));

	m_stHeader.bySTX1 = 0x10;
	m_stHeader.bySTX2 = 0x02;

	m_stTail.byETX1	  = 0x10;
	m_stTail.byETX1	  = 0x03;
}
//---------------------------------------------------------------------------
__fastcall TProtocol::~TProtocol()
{
}
//---------------------------------------------------------------------------
void __fastcall TProtocol::fnSetDataLen(int a_iValue)
{
	m_iDataLen = a_iValue;
	m_stHeader.wDataLen	= a_iValue;
}
//---------------------------------------------------------------------------
int __fastcall TProtocol::fnGetDataLen()
{
	return m_iDataLen;
}
//---------------------------------------------------------------------------
int __fastcall TProtocol::fnEncoding()
{
	int iResult = 0;
	m_iSendPackSize = 0;
	ZeroMemory(m_bySendPacket, sizeof(m_bySendPacket));                      //	���� ��Ŷ���� �ʱ�ȭ
	m_iSendPackSize += sizeof(m_stHeader);                                   //	���� ��Ŷ����� ���ũ�� �߰�
	DataLen = 0;                                                             //	(m_stHeader.byDataLen �κ� �ʱ�ȭ)

	switch (Code) {
		case 0x05:	{
			TTcpData05 *pData = (TTcpData05*)m_pBody;
			DataLen = pData->DataLen;											 //	Data05�� ������ ������ ��ȯ
			iResult = pData->fnEncodingBody(m_bySendPacket, m_iSendPackSize);
			break;
			}
		case 0x06:	{
			TTcpData06 *pData = (TTcpData06*)m_pBody;
			DataLen = pData->DataLen;											 //	Data06�� ������ ������ ��ȯ
			iResult = pData->fnEncodingBody(m_bySendPacket, m_iSendPackSize);
			break;
			}
		case 0x07:	{
			TTcpData07 *pData = (TTcpData07*)m_pBody;
			DataLen = pData->DataLen;											 //	Data07�� ������ ������ ��ȯ
			iResult = pData->fnEncodingBody(m_bySendPacket, m_iSendPackSize);
			break;
			}
		default:	{
			break;
			}
	}
	if (iResult == 0) {
		CopyMemory(m_bySendPacket, &m_stHeader, sizeof(m_stHeader));
	}
	return iResult;




}
//---------------------------------------------------------------------------

__fastcall TTcpData05::TTcpData05()
{
	ZeroMemory(&m_stData, sizeof(m_stData));
}
//---------------------------------------------------------------------------
__fastcall TTcpData05::~TTcpData05()
{
}
//---------------------------------------------------------------------------
int __fastcall TTcpData05::fnGetDataLen()
{
	return sizeof(m_stData);
}
//---------------------------------------------------------------------------
int __fastcall TTcpData05::fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex)
{
	return fnDefaultEncoding(a_pBuffer, a_iIndex, &m_stData, sizeof(m_stData));
}
//---------------------------------------------------------------------------
__fastcall TTcpData06::TTcpData06()
{
	ZeroMemory(&m_stData, sizeof(m_stData));
}
//---------------------------------------------------------------------------
__fastcall TTcpData06::~TTcpData06()
{
}
//---------------------------------------------------------------------------
int __fastcall TTcpData06::fnGetDataLen()
{
	return sizeof(m_stData);
}
//---------------------------------------------------------------------------
int __fastcall TTcpData06::fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex)
{
	return fnDefaultEncoding(a_pBuffer, a_iIndex, &m_stData, sizeof(m_stData));
}
//---------------------------------------------------------------------------
__fastcall TTcpData07::TTcpData07()
{
	ZeroMemory(&m_stData, sizeof(m_stData));
}
//---------------------------------------------------------------------------
__fastcall TTcpData07::~TTcpData07()
{
}
//---------------------------------------------------------------------------
int __fastcall TTcpData07::fnGetDataLen()
{
	return sizeof(m_stData);
}
//---------------------------------------------------------------------------
int __fastcall TTcpData07::fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex)
{
	return fnDefaultEncoding(a_pBuffer, a_iIndex, &m_stData, sizeof(m_stData));
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//void __fastcall TProtocol::fnSetOpCode(int a_op)
//{
//	m_stHeader.byOpCode = a_op;
//}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
