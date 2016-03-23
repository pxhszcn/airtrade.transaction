#pragma once

#include <module/plugininc.h>
#include <module/pluginimpl.h>
#include <module/modulemacro.h>
#include "Transaction.h"

Transaction::Transaction()
{
    m_hMainWindow = nullptr;
    m_hStockTreeView = nullptr;
    m_hProcess = nullptr;
    Load();
}

Transaction::~Transaction()
{
    UnLoad();
}

bool Transaction::GetTreeViewStock(list<tstring>& treeViewContent)
{
    
    return true;
}

bool Transaction::Load()
{
    // get the window handle of "xiadan.exe"
    m_hMainWindow = ::FindWindow(NULL, TEXT("网上股票交易系统5.0"));
    if (m_hMainWindow == nullptr) return false;

    // get the window handle of controls inside the main window
    // "Stock Tree View"
    m_hStockTreeView = ::FindWindowEx(m_hMainWindow, nullptr, TEXT("AfxMDIFrame42s"), nullptr);
    if (m_hStockTreeView != nullptr)
    {
        m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, nullptr, TEXT("AfxWnd42s"), nullptr);
        if (m_hStockTreeView != nullptr)
        {
            m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, nullptr, nullptr, TEXT("HexinScrollWnd"));
            if (m_hStockTreeView != nullptr)
            {
                m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, nullptr, nullptr, TEXT("HexinScrollWnd2"));
                if (m_hStockTreeView != nullptr)
                {
                    m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, nullptr, TEXT("SysTreeView32"), nullptr);
                }
            }
        }
    }
    if (m_hMainWindow == nullptr) return false;

    // get the process handle of "xiadan.exe"
    DWORD dwProcessId;
    ::GetWindowThreadProcessId(m_hMainWindow, &dwProcessId);
    m_hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId);
    if (m_hProcess == nullptr) return false;

	// load the "Stock Tree View"
	m_stockTreeView = RemoteTreeView(m_hProcess, m_hStockTreeView);
	m_stockTreeView.TraverseTreeView();

    return true;
}

bool Transaction::UnLoad()
{
    if (m_hProcess != nullptr)
    {
        ::CloseHandle(m_hProcess);
        m_hProcess = nullptr;
    }

    return true;
}

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY(Transaction)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}