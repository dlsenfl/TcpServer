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
int __fastcall TTcpBase::fnDefaultEncoding(BYTE *pBuffer, int &iIndex, void *pData, int iDataSize, int iTail)
{
	if(iIndex + iDataSize + iTail > MAX_TCP_BUFFER) return 11;
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
//*************???? AutoFree ????*************
void __fastcall TProtocol::fnDeleteBody()
{
	if(m_pBody == NULL) return;

	switch (Code) {
		case 0x05:
			delete (TTcpData05*)m_pBody;
			break;
		case 0x06:
			delete (TTcpData06*)m_pBody;
			break;
		case 0x07:
			delete (TTcpData07*)m_pBody;
			break;
		default:
			break;
	}
		m_pBody = NULL;
}
//---------------------------------------------------------------------------
//====== TProtocol::fnEncoding => TTcpData??::fnEncodingBody => TTcpBase::fnDefaultEncoding ȣ�� ============
int __fastcall TProtocol::fnEncoding()
{
	int iResult = 0;
	int iTailSize = 0;
	m_iSendPackSize = 0;
	ZeroMemory(m_bySendPacket, sizeof(m_bySendPacket));                      //	���� ��Ŷ���� �ʱ�ȭ
	m_iSendPackSize += sizeof(m_stHeader);                                   //	���� ��Ŷ����� ���ũ�� �߰�
	iTailSize += sizeof(m_stTail);
	DataLen = 0;                                                             //	(m_stHeader.byDataLen �κ� �ʱ�ȭ)

	switch (Code) {
		case 0x05:	{
			TTcpData05 *pData = (TTcpData05*)m_pBody;
			DataLen = pData->DataLen;											 //	Data05�� ������ ������ ��ȯ
			iResult = pData->fnEncodingBody(m_bySendPacket, m_iSendPackSize, iTailSize);
			m_iSendPackSize += DataLen;
			break;
			}
		case 0x06:	{
			TTcpData06 *pData = (TTcpData06*)m_pBody;
			DataLen = pData->DataLen;											 //	Data06�� ������ ������ ��ȯ
			iResult = pData->fnEncodingBody(m_bySendPacket, m_iSendPackSize, iTailSize);
			m_iSendPackSize += DataLen;
			break;
			}
		case 0x07:	{
			TTcpData07 *pData = (TTcpData07*)m_pBody;
			DataLen = pData->DataLen;											 //	Data07�� ������ ������ ��ȯ
			iResult = pData->fnEncodingBody(m_bySendPacket, m_iSendPackSize, iTailSize);
			m_iSendPackSize += DataLen;
			break;
			}
		default:	{
			break;
			}
	}
	if (iResult == 0) {
		CopyMemory(m_bySendPacket, &m_stHeader, sizeof(m_stHeader));
		CopyMemory(m_bySendPacket + m_iSendPackSize, &m_stTail, sizeof(m_stTail));
		m_iSendPackSize += iTailSize;										   	 //	(��������� + �����ͻ����� + ���ϻ�����)
	}
	return iResult;                                                              //	�����̸� 0��ȯ.
}
//---------------------------------------------------------------------------
//====== TProtocol::fnDecoding => TTcpData??::fnDecodingBody => TTcpBase::fnDefaultDecoding ȣ�� ============
int __fastcall TProtocol::fnDecoding(BYTE *a_pBuffer, int a_iSize)
{
	int iResult = 0;
	int iTailSize = 0;
	fnDeleteBody();
	if(a_iSize > MAX_TCP_BUFFER)	return 1;
	ZeroMemory(m_byRecvPacket, sizeof(m_byRecvPacket));
	m_iRecvPackSize = 0;
	CopyMemory(m_byRecvPacket, a_pBuffer, a_iSize);
	CopyMemory(&m_stHeader, m_byRecvPacket, sizeof(m_stHeader));       	//	����ü�ȿ� ������ ����
	m_iRecvPackSize += sizeof(m_stHeader);                              //	��������ŭ �ε��� ����

	switch (Code) {
		case 0x05:	{
			TTcpData05 *pData = new TTcpData05();
			iResult = pData->fnDecodingBody(a_pBuffer, m_iRecvPackSize, a_iSize);      // m_iRecvPackSize�� a_pBuffer�� �޸���ġ �ε���
			if (iResult == 0) {
				m_pBody = (void*)pData;
			}else	delete pData;
			break;
		}
		case 0x06:
		case 0x07:
		default:
			break;
	}

	CopyMemory(&m_stTail, (m_byRecvPacket + m_iRecvPackSize), sizeof(m_stTail));
	m_iRecvPackSize += iTailSize;                          				//	��������� + �����ͻ����� + ���ϻ�����
	if (iResult == 0) {
		if (m_iRecvPackSize != a_iSize) {
			iResult = 3;
		}
	}
	return iResult;			// �����̸� 0��ȯ, Error�̸� 3��ȯ.
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
int __fastcall TTcpData05::fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iTail)
{
	return fnDefaultEncoding(a_pBuffer, a_iIndex, &m_stData, sizeof(m_stData), a_iTail);
}
//---------------------------------------------------------------------------
int __fastcall TTcpData05::fnDecodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iSize)
{
	return fnDefaultDecoding(a_pBuffer, a_iIndex, a_iSize, &m_stData, sizeof(m_stData));
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
int __fastcall TTcpData06::fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iTail)
{
	return fnDefaultEncoding(a_pBuffer, a_iIndex, &m_stData, sizeof(m_stData), a_iTail);
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
int __fastcall TTcpData07::fnEncodingBody(BYTE *a_pBuffer, int &a_iIndex, int a_iTail)
{
	return fnDefaultEncoding(a_pBuffer, a_iIndex, &m_stData, sizeof(m_stData), a_iTail);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//void __fastcall TProtocol::fnSetOpCode(int a_op)
//{
//	m_stHeader.byOpCode = a_op;
//}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
