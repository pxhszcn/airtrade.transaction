#pragma once

#include <module/classmacro.h>
#include "ITransaction.h"
#include <CommCtrl.h>
#include "TreeView.h"
#include "sendkeys.h"
#include <unordered_map>

#define STATIC_CONTROL_MAX_LENGTH 512

enum ControlType
{
    CONTROL_UNKNOWN = 0,
    CONTROL_MAIRU_DAIMA,
    CONTROL_MAIRU_JIAGE,
    CONTROL_MAIRU_SHULIANG,
	CONTROL_CHAXUN_ZJGP,
};

class Transaction : public ITransaction
{
    X3BEGIN_CLASS_DECLARE(Transaction, "AirTrade.Transaction")
        X3DEFINE_INTERFACE_ENTRY(ITransaction)
    X3END_CLASS_DECLARE()

protected:
    Transaction();
    virtual ~Transaction();

private:
	virtual int ReqOrderInsert(AirTradeFtdcInputOrderField* pInputOrder, int nRequestID);
	virtual int ReqQryInvestorPosition(AirTradeFtdcQryInvestorPositionField* pQryUserInvestorPosition, int nRequestID);
	virtual int ReqQryInvestorAccount(AirTradeFtdcQryInvestorAccountField* pQryInvestorAccount, int nRequestID);
    bool Load();
    bool UnLoad();
    bool RefreshDialogsAndControls();
    bool RefreshControls(HWND hDialog);
    bool ConfirmDialog(bool bConfirmed);
    bool GetHint();
private:
	HWND m_hMainWindow;
	HWND m_hStockTreeView;
	HANDLE m_hProcess;
	RemoteTreeView m_stockTreeView;
	HS_SendKeys m_sendKeys;
    unordered_map<ControlType, HWND> m_controls;
	vector<tstring> m_hints;
};