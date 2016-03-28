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

class ITransaction : public x3::IObject
{
public:
    //X3DEFINE_IID(ITransaction);
	virtual int ReqOrderInsert(AirTradeFtdcInputOrderField* pInputOrder, int nRequestID) = 0;
};