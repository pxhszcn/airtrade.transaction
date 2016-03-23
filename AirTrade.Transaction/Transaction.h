#pragma once

#include <module/classmacro.h>
#include "ITransaction.h"
#include <CommCtrl.h>

class Transaction : public ITransaction
{
    X3BEGIN_CLASS_DECLARE(Transaction, "AirTrade.Transaction")
        X3DEFINE_INTERFACE_ENTRY(ITransaction)
    X3END_CLASS_DECLARE()

protected:
    Transaction();
    virtual ~Transaction();

private:
    virtual bool GetTreeViewStock(list<tstring>& treeViewContent);
    HTREEITEM TreeNodeGetNext(HWND hwnd, HTREEITEM hTreeItem);
    bool GetTreeViewItem(HWND hwnd, list<tstring>& treeViewContent);
    bool Load();
    bool UnLoad();
private:
    HWND m_hMainWindow;
    HWND m_hStockTreeView;
    HANDLE m_hProcess;
};