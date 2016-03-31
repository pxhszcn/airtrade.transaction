#pragma once

#include <objptr.h>
#include <list>
#include <string>

using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif


typedef char AirTradeFtdcInstrumentIDType[31]; // ��Լ�������

/////////////////////////////////////////////////////////////////////////
///TFtdcUstpDirectionType��һ��������������
/////////////////////////////////////////////////////////////////////////
///��
#define USTP_FTDC_D_Buy '0'
///��
#define USTP_FTDC_D_Sell '1'
typedef char TUstpFtdcDirectionType;

typedef char AirTradeFtdcLimitPriceType; // �۸�
typedef int AirTradeFtdcVolumeType; // ����
typedef char AirTradeFtdcBrokerIDType[11]; // ���͹�˾���
typedef char AirTradeFtdcUserIDType[16]; // �û�����
typedef char AirTradeFtdcExchangeIDType[11]; // ����������
typedef char AirTradeFtdcInvestorIDType[19]; // Ͷ���߱��
typedef int AirTradeFtdcErrorIDType; // �������
typedef char AirTradeFtdcErrorMsgType[81]; // ������Ϣ
typedef char AirTradeFtdcAccountIDType[13]; // �ʽ��ʺ�
typedef double AirTradeFtdcMoneyType; // �ʽ�


// ���뱨���ṹ
struct AirTradeFtdcInputOrderField
{
	///��Լ����
	AirTradeFtdcInstrumentIDType InstrumentID;
	///��������
	AirTradeFtdcDirectionType Direction;
	///�۸�
	AirTradeFtdcLimitPriceType LimitPrice;
	///����
	AirTradeFtdcVolumeType Volume;
};

// Ͷ���ֲֲ߳�ѯ�ṹ
struct AirTradeFtdcQryInvestorPositionField
{
	///���͹�˾���
	AirTradeFtdcBrokerIDType BrokerID;
	///�û�����
	AirTradeFtdcUserIDType UserID;
	///����������
	AirTradeFtdcExchangeIDType ExchangeID;
	///Ͷ���߱��
	AirTradeFtdcInvestorIDType InvestorID;
	///��Լ����
	AirTradeFtdcInstrumentIDType InstrumentID;
};

// Ͷ�����˻���ѯ�ṹ��
struct AirTradeFtdcQryInvestorAccountField
{
	///���͹�˾���
	AirTradeFtdcBrokerIDType BrokerID;
	///�û�����
	AirTradeFtdcUserIDType UserID;
	///Ͷ���߱��
	AirTradeFtdcInvestorIDType InvestorID;
};

// Ͷ�����˻��ṹ
struct AirTradeFtdcRspInvestorAccountField
{
	///���͹�˾���
	AirTradeFtdcBrokerIDType BrokerID;
	///Ͷ���߱��
	AirTradeFtdcInvestorIDType InvestorID;
	///�ʽ��ʺ�
	AirTradeFtdcAccountIDType AccountID;
	///�ϴν���׼����
	AirTradeFtdcMoneyType PreBalance;
	///�����
	AirTradeFtdcMoneyType Deposit;
	///������
	AirTradeFtdcMoneyType Withdraw;
	///����ı�֤��
	AirTradeFtdcMoneyType FrozenMargin;
	///����������
	AirTradeFtdcMoneyType FrozenFee;
	///����Ȩ����
	AirTradeFtdcMoneyType FrozenPremium;
	///������
	AirTradeFtdcMoneyType Fee;
	///ƽ��ӯ��
	AirTradeFtdcMoneyType CloseProfit;
	///�ֲ�ӯ��
	AirTradeFtdcMoneyType PositionProfit;
	///�����ʽ�
	AirTradeFtdcMoneyType Available;
	///��ͷ����ı�֤��
	AirTradeFtdcMoneyType LongFrozenMargin;
	///��ͷ����ı�֤��
	AirTradeFtdcMoneyType ShortFrozenMargin;
	///��ͷռ�ñ�֤��
	AirTradeFtdcMoneyType LongMargin;
	///��ͷռ�ñ�֤��
	AirTradeFtdcMoneyType ShortMargin;
	///�����ͷű�֤��
	AirTradeFtdcMoneyType ReleaseMargin;
	///��̬Ȩ��
	AirTradeFtdcMoneyType DynamicRights;
	///���ճ����
	AirTradeFtdcMoneyType TodayInOut;
	///ռ�ñ�֤��
	AirTradeFtdcMoneyType Margin;
	///��ȨȨ������֧
	AirTradeFtdcMoneyType Premium;
	///���ն�
	AirTradeFtdcMoneyType risk;
};
// ��Ӧ��Ϣ�ṹ
struct CUstpFtdcRspInfoField
{
	///�������
	AirTradeFtdcErrorIDType ErrorID;
	///������Ϣ
	AirTradeFtdcErrorMsgType ErrorMsg;
};

class ITransaction : public x3::IObject
{
public:
    //X3DEFINE_IID(ITransaction);
	virtual int ReqOrderInsert(AirTradeFtdcInputOrderField* pInputOrder, int nRequestID) = 0;
	virtual int ReqQryInvestorPosition(AirTradeFtdcQryInvestorPositionField* pQryUserInvestorPosition, int nRequestID) = 0;
	virtual int ReqQryInvestorAccount(AirTradeFtdcQryInvestorAccountField* pQryInvestorAccount, int nRequestID) = 0;
	virtual void OnRspQryInvestorAccount(AirTradeFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
};