#pragma once

#include <module/plugininc.h>
#include <module/pluginimpl.h>
#include <module/modulemacro.h>
#include <tchar.h>
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

int Transaction::ReqOrderInsert(AirTradeFtdcInputOrderField* pInputOrder, int nRequestID)
{
	// 尝试获取资金信息，发送Ctrl+C
	m_stockTreeView.ClickItem(TEXT("中签")); // 需要先切换到不常用的界面然后再点需要的界面才工作正常
	m_stockTreeView.ClickItem(TEXT("资金股票"));
	RefreshDialogsAndControls();
	m_sendKeys.Send("^C", m_controls[CONTROL_CHAXUN_ZJGP]);

    // 加载买入股票界面
	m_stockTreeView.ClickItem(TEXT("中签")); 
	m_stockTreeView.ClickItem(TEXT("买入[F1]"));
    RefreshDialogsAndControls();

    // 填写买入信息
	::PostMessage(m_controls[CONTROL_MAIRU_DAIMA], EM_SETSEL, 0, -1);
	m_sendKeys.Send("000793", m_controls[CONTROL_MAIRU_DAIMA]);
	::PostMessage(m_controls[CONTROL_MAIRU_JIAGE], EM_SETSEL, 0, -1);
	m_sendKeys.Send("9.99", m_controls[CONTROL_MAIRU_JIAGE]);
	::PostMessage(m_controls[CONTROL_MAIRU_SHULIANG], EM_SETSEL, 0, -1);
	m_sendKeys.Send("20000", m_controls[CONTROL_MAIRU_SHULIANG]);

    // 执行买入操作
	m_sendKeys.Send("B", m_controls[CONTROL_MAIRU_DAIMA]);

    // 确认买入操作
    ConfirmDialog(true);

    // 获取提示信息
    GetHint();

    return 0;
}

int Transaction::ReqQryInvestorPosition(AirTradeFtdcQryInvestorPositionField* pQryUserInvestorPosition, int nRequestID)
{
	return 0;
}

int Transaction::ReqQryInvestorAccount(AirTradeFtdcQryInvestorAccountField* pQryInvestorAccount, int nRequestID)
{
	return 0;
}

bool Transaction::Load()
{
    // 获取同花顺下单程序的主窗口句柄
    m_hMainWindow = ::FindWindow(NULL, TEXT("网上股票交易系统5.0"));
    if (m_hMainWindow == NULL) return false;

    // 获取主窗口中导航栏控件的句柄
    // 股票TreeView控件句柄
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

    // 刷新功能界面句柄及其中的控件句柄
    RefreshDialogsAndControls();

    // 获取同花顺下单程序的进程句柄
    DWORD dwProcessId;
    ::GetWindowThreadProcessId(m_hMainWindow, &dwProcessId);
    m_hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId);
    if (m_hProcess == NULL) return false;

	// 遍历股票TreeView控件
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

bool Transaction::RefreshDialogsAndControls()
{
    HWND hDialog = NULL;
    HWND hDialogParent = ::FindWindowEx(m_hMainWindow, NULL, TEXT("AfxMDIFrame42s"), NULL);
    if (hDialogParent == NULL) return false;

	// 等待界面稳定
	::Sleep(1000);

	m_controls.clear();
    while (true)
    {
        hDialog = ::FindWindowEx(hDialogParent, hDialog, TEXT("#32770"), NULL);
        if (hDialog == NULL) break;
        
        RefreshControls(hDialog);
    }

    return true;
}

bool Transaction::RefreshControls(HWND hDialog)
{
	HWND hControl = NULL;

	// 买入股票界面
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

	// 卖出股票界面
	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("卖出股票")) != NULL)
	{
		return true;
	}

	// 撤单股票界面
	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("在委托记录上用鼠标双击或回车即可撤单")) != NULL)
	{
		return true;
	}

	// 查询资金股票界面
	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("查询资金股票")) != NULL)
	{
		hControl = ::FindWindowEx(hDialog, NULL, NULL, TEXT("HexinScrollWnd"));
		if (hControl != NULL)
		{
			hControl = ::FindWindowEx(hControl, NULL, NULL, TEXT("HexinScrollWnd2"));
			if (hControl != NULL)
			{
				hControl = ::FindWindowEx(hControl, NULL, TEXT("CVirtualGridCtrl"), TEXT("Custom1"));
				m_controls[CONTROL_CHAXUN_ZJGP] = hControl;
			}
		}
		return true;
	}

    return false;
}

bool Transaction::ConfirmDialog(bool bConfirmed)
{
    HWND hDialog = NULL;

	// 等待窗口弹出
	::Sleep(500);

    // 寻找窗口并执行操作
    while (true)
    {
        hDialog = ::FindWindowEx(NULL, hDialog, TEXT("#32770"), NULL);
        if (hDialog == NULL) break;

        // 委托确认窗口
        HWND hControl = ::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("委托确认"));
        if (hControl != NULL)
        {
			m_sendKeys.Send(bConfirmed ? "Y": "N", hControl);
            break;
        }
    }

    return true;
}

bool Transaction::GetHint()
{
    HWND hDialog = NULL;
    TCHAR szStaticBuf[STATIC_CONTROL_MAX_LENGTH];

    // 等待窗口弹出
    ::Sleep(500);

    // 寻找窗口并执行操作
    while (true)
    {
        hDialog = ::FindWindowEx(NULL, hDialog, TEXT("#32770"), NULL);
        if (hDialog == NULL) break;

        // 提示窗口
        HWND hControl = ::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("提示"));
        if (hControl != NULL)
        {
            HWND hHintControl = ::GetWindow(hControl, GW_HWNDPREV);
            if (hHintControl != NULL)
            {
				// 提取提示内容
				::SendMessage(hHintControl, WM_GETTEXT, (WPARAM)STATIC_CONTROL_MAX_LENGTH, (LPARAM)szStaticBuf); // GetWindowText不工作 - -？
				SYSTEMTIME now = { 0 };
				::GetLocalTime(&now);
				TCHAR resultBuf[STATIC_CONTROL_MAX_LENGTH + 20];
				_stprintf_s(resultBuf, TEXT("[%04d-%02d-%02d %02d:%02d:%02d] - %s"), now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond, szStaticBuf);
				m_hints.push_back(resultBuf);
				OutputDebugString(resultBuf);

				// 点击确定按钮
				HWND hOKButton = ::FindWindowEx(hDialog, NULL, TEXT("Button"), TEXT("确定"));
				if (hOKButton != NULL)
				{
					::SendMessage(hOKButton, WM_LBUTTONDOWN, (WPARAM)MK_LBUTTON, (LPARAM)0);
					::SendMessage(hOKButton, WM_LBUTTONUP, (WPARAM)MK_LBUTTON, (LPARAM)0);
				}
            }
            break;
        }
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