#pragma once

#include <module/plugininc.h>
#include <module/pluginimpl.h>
#include <module/modulemacro.h>
#include "Transaction.h"

Transaction::Transaction()
{
    m_hMainWindow = NULL;
    m_hStockTreeView = NULL;
    m_hProcess = NULL;
    Load();
}

Transaction::~Transaction()
{
    UnLoad();
}

bool Transaction::PlaceOrder()
{
    m_stockTreeView.ClickItem(TEXT("资金股票"));
    RefreshDialogs();
    m_stockTreeView.ClickItem(TEXT("买入[F1]"));
    RefreshDialogs();
    SimKeyDown(m_controls[CONTROL_MAIRU_DAIMA], '0');
    SimKeyDown(m_controls[CONTROL_MAIRU_JIAGE], '1');
    SimKeyDown(m_controls[CONTROL_MAIRU_SHULIANG], '2');

    return true;
}

bool Transaction::Load()
{
    // get the window handle of "xiadan.exe"
    m_hMainWindow = ::FindWindow(NULL, TEXT("网上股票交易系统5.0"));
    if (m_hMainWindow == NULL) return false;

    // get the window handle of controls inside the main window
    // "Stock Tree View"
    m_hStockTreeView = ::FindWindowEx(m_hMainWindow, NULL, TEXT("AfxMDIFrame42s"), NULL);
    if (m_hStockTreeView != NULL)
    {
        m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, NULL, TEXT("AfxWnd42s"), NULL);
        if (m_hStockTreeView != NULL)
        {
            m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, NULL, NULL, TEXT("HexinScrollWnd"));
            if (m_hStockTreeView != NULL)
            {
                m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, NULL, NULL, TEXT("HexinScrollWnd2"));
                if (m_hStockTreeView != NULL)
                {
                    m_hStockTreeView = ::FindWindowEx(m_hStockTreeView, NULL, TEXT("SysTreeView32"), NULL);
                }
            }
        }
    }
    if (m_hStockTreeView == NULL) return false;

    // refresh current dialog handles
    RefreshDialogs();

    // get the process handle of "xiadan.exe"
    DWORD dwProcessId;
    ::GetWindowThreadProcessId(m_hMainWindow, &dwProcessId);
    m_hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId);
    if (m_hProcess == NULL) return false;

	// load the "Stock Tree View"
	m_stockTreeView = RemoteTreeView(m_hProcess, m_hStockTreeView);
	m_stockTreeView.TraverseTreeView();

    return true;
}

bool Transaction::UnLoad()
{
    if (m_hProcess != NULL)
    {
        ::CloseHandle(m_hProcess);
        m_hProcess = NULL;
    }

    return true;
}

bool Transaction::RefreshDialogs()
{
    HWND hDialog = NULL;
    HWND hDialogParent = ::FindWindowEx(m_hMainWindow, NULL, TEXT("AfxMDIFrame42s"), NULL);
    if (hDialogParent == NULL) return false;

    while (true)
    {
        hDialog = ::FindWindowEx(hDialogParent, hDialog, TEXT("#32770"), NULL);
        if (hDialog == NULL) break;
        
        RefreshControls(hDialog);
    }
}

bool Transaction::RefreshControls(HWND hDialog)
{
    HWND hControl = NULL;

    // Is it dialog for 买入股票?
    if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("买入股票")) != NULL)
    {
        // 证券代码
        hControl = ::FindWindowEx(hDialog, NULL, TEXT("Edit"), NULL);
        m_controls[CONTROL_MAIRU_DAIMA] = hControl;

        // 买入价格
        hControl = ::FindWindowEx(hDialog, hControl, TEXT("Edit"), NULL);
        m_controls[CONTROL_MAIRU_JIAGE] = hControl;

        // 买入数量
        hControl = ::FindWindowEx(hDialog, hControl, TEXT("Edit"), NULL);
        m_controls[CONTROL_MAIRU_SHULIANG] = hControl;

        return true;
    }

    // is it dialog for 卖出股票?
    if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("卖出股票")) != NULL)
    {
        return true;
    }

    // is it dialog for 撤单?
    if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("在委托记录上用鼠标双击或回车即可撤单")) != NULL)
    {
        return true;
    }

    return false;
}

void Transaction::SimKeyDown(HWND hwnd, UINT key)
{
    UINT scanCode;
    LPARAM lParam;

    scanCode = ::MapVirtualKey(key, 0);

    lParam = 0x00000001 | (LPARAM)(scanCode << 16);
    if (IsVKExtended(key))
    {
        lParam = lParam | 0x01000000;
    }
    ::PostMessage(hwnd, WM_KEYDOWN, (WPARAM)key, lParam);
}

void Transaction::SimKeyUp(HWND hwnd, UINT key)
{
    UINT scanCode;
    LPARAM lParam;

    scanCode = ::MapVirtualKey(key, 0);

    lParam = 0x00000001 | (LPARAM)(scanCode << 16);
    if (IsVKExtended(key))
    {
        lParam = lParam | 0x01000000;
    }
    ::PostMessage(hwnd, WM_KEYUP, (WPARAM)key, lParam);
}

bool Transaction::IsVKExtended(UINT key)
{
    if (key == VK_INSERT || key == VK_DELETE || key == VK_END || key == VK_DOWN ||
        key == VK_NEXT || key == VK_LEFT || key == VK_RIGHT || key == VK_HOME || key == VK_UP ||
        key == VK_PRIOR || key == VK_DIVIDE || key == VK_APPS || key == VK_LWIN || key == VK_RWIN ||
        key == VK_RMENU || key == VK_RCONTROL || key == VK_SLEEP || key == VK_BROWSER_BACK ||
        key == VK_BROWSER_FORWARD || key == VK_BROWSER_REFRESH || key == VK_BROWSER_STOP ||
        key == VK_BROWSER_SEARCH || key == VK_BROWSER_FAVORITES || key == VK_BROWSER_HOME ||
        key == VK_VOLUME_MUTE || key == VK_VOLUME_DOWN || key == VK_VOLUME_UP || key == VK_MEDIA_NEXT_TRACK ||
        key == VK_MEDIA_PREV_TRACK || key == VK_MEDIA_STOP || key == VK_MEDIA_PLAY_PAUSE ||
        key == VK_LAUNCH_MAIL || key == VK_LAUNCH_MEDIA_SELECT || key == VK_LAUNCH_APP1 || key == VK_LAUNCH_APP2)
    {
        return true;
    }
    else
    {
        return false;
    }
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