#pragma once

#include <module/classmacro.h>
#include "ITransaction.h"
#include <CommCtrl.h>
#include "TreeView.h"
#include <unordered_map>

enum DialogType
{
    DIALOG_UNKNOWN = 0,
    DIALOG_MAIRU,
    DIALOG_MAICHU,
    DIALOG_CHEDAN
};

enum ControlType
{
    CONTROL_UNKNOWN = 0,
    CONTROL_MAIRU_DAIMA,
    CONTROL_MAIRU_JIAGE,
    CONTROL_MAIRU_SHULIANG
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
    virtual bool PlaceOrder();
    bool Load();
    bool UnLoad();
    bool RefreshDialogs();
    bool RefreshControls(HWND hDialog);
    void SimKeyDown(HWND hwnd, UINT key);
    void SimKeyUp(HWND hwnd, UINT key);
    bool IsVKExtended(UINT key);
private:
	HWND m_hMainWindow;
	HWND m_hStockTreeView;
	HANDLE m_hProcess;
	RemoteTreeView m_stockTreeView;
    unordered_map<ControlType, HWND> m_controls;
};