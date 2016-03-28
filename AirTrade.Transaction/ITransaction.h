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

typedef char AirTradeFtdcInstrumentIDType[31];
typedef char AirTradeFtdcDirectionType;
typedef char AirTradeFtdcLimitPriceType;
typedef int AirTradeFtdcVolumeType;

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

class ITransaction : public x3::IObject
{
public:
    //X3DEFINE_IID(ITransaction);
	virtual int ReqOrderInsert(AirTradeFtdcInputOrderField* pInputOrder, int nRequestID) = 0;
};