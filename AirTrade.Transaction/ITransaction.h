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


typedef char AirTradeFtdcInstrumentIDType[31]; // 合约编号类型

/////////////////////////////////////////////////////////////////////////
///TFtdcUstpDirectionType是一个买卖方向类型
/////////////////////////////////////////////////////////////////////////
///买
#define USTP_FTDC_D_Buy '0'
///卖
#define USTP_FTDC_D_Sell '1'
typedef char TUstpFtdcDirectionType;

typedef char AirTradeFtdcLimitPriceType; // 价格
typedef int AirTradeFtdcVolumeType; // 数量
typedef char AirTradeFtdcBrokerIDType[11]; // 经纪公司编号
typedef char AirTradeFtdcUserIDType[16]; // 用户代码
typedef char AirTradeFtdcExchangeIDType[11]; // 交易所代码
typedef char AirTradeFtdcInvestorIDType[19]; // 投资者编号
typedef int AirTradeFtdcErrorIDType; // 错误代码
typedef char AirTradeFtdcErrorMsgType[81]; // 错误信息
typedef char AirTradeFtdcAccountIDType[13]; // 资金帐号
typedef double AirTradeFtdcMoneyType; // 资金


// 输入报单结构
struct AirTradeFtdcInputOrderField
{
	///合约代码
	AirTradeFtdcInstrumentIDType InstrumentID;
	///买卖方向
	AirTradeFtdcDirectionType Direction;
	///价格
	AirTradeFtdcLimitPriceType LimitPrice;
	///数量
	AirTradeFtdcVolumeType Volume;
};

// 投资者持仓查询结构
struct AirTradeFtdcQryInvestorPositionField
{
	///经纪公司编号
	AirTradeFtdcBrokerIDType BrokerID;
	///用户代码
	AirTradeFtdcUserIDType UserID;
	///交易所代码
	AirTradeFtdcExchangeIDType ExchangeID;
	///投资者编号
	AirTradeFtdcInvestorIDType InvestorID;
	///合约代码
	AirTradeFtdcInstrumentIDType InstrumentID;
};

// 投资者账户查询结构：
struct AirTradeFtdcQryInvestorAccountField
{
	///经纪公司编号
	AirTradeFtdcBrokerIDType BrokerID;
	///用户代码
	AirTradeFtdcUserIDType UserID;
	///投资者编号
	AirTradeFtdcInvestorIDType InvestorID;
};

// 投资者账户结构
struct AirTradeFtdcRspInvestorAccountField
{
	///经纪公司编号
	AirTradeFtdcBrokerIDType BrokerID;
	///投资者编号
	AirTradeFtdcInvestorIDType InvestorID;
	///资金帐号
	AirTradeFtdcAccountIDType AccountID;
	///上次结算准备金
	AirTradeFtdcMoneyType PreBalance;
	///入金金额
	AirTradeFtdcMoneyType Deposit;
	///出金金额
	AirTradeFtdcMoneyType Withdraw;
	///冻结的保证金
	AirTradeFtdcMoneyType FrozenMargin;
	///冻结手续费
	AirTradeFtdcMoneyType FrozenFee;
	///冻结权利金
	AirTradeFtdcMoneyType FrozenPremium;
	///手续费
	AirTradeFtdcMoneyType Fee;
	///平仓盈亏
	AirTradeFtdcMoneyType CloseProfit;
	///持仓盈亏
	AirTradeFtdcMoneyType PositionProfit;
	///可用资金
	AirTradeFtdcMoneyType Available;
	///多头冻结的保证金
	AirTradeFtdcMoneyType LongFrozenMargin;
	///空头冻结的保证金
	AirTradeFtdcMoneyType ShortFrozenMargin;
	///多头占用保证金
	AirTradeFtdcMoneyType LongMargin;
	///空头占用保证金
	AirTradeFtdcMoneyType ShortMargin;
	///当日释放保证金
	AirTradeFtdcMoneyType ReleaseMargin;
	///动态权益
	AirTradeFtdcMoneyType DynamicRights;
	///今日出入金
	AirTradeFtdcMoneyType TodayInOut;
	///占用保证金
	AirTradeFtdcMoneyType Margin;
	///期权权利金收支
	AirTradeFtdcMoneyType Premium;
	///风险度
	AirTradeFtdcMoneyType risk;
};
// 响应信息结构
struct CUstpFtdcRspInfoField
{
	///错误代码
	AirTradeFtdcErrorIDType ErrorID;
	///错误信息
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