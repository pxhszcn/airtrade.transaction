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
	//尝试获取资金信息，发送Ctrl+C
	//m_stockTreeView.ClickItem(TEXT("资金股票"));
	//RefreshDialogsAndControls();
	//LPARAM lparam;
	//BYTE keybdState[256];
	//::GetKeyboardState((LPBYTE)&keybdState);
	//keybdState[VK_CONTROL] |= 0x80;
	//::SetKeyboardState((LPBYTE)&keybdState);
	//lparam = 0x00000001 | (LPARAM)(::MapVirtualKey(VK_CONTROL, 0) << 16);
	//PostMessage(m_controls[CONTROL_CHAXUN_ZJGP_CXZJGP], WM_KEYDOWN, VK_CONTROL, lparam);

	//SimKeystroke(m_controls[CONTROL_CHAXUN_ZJGP_CXZJGP], 'c');

	//::GetKeyboardState((LPBYTE)&keybdState);
	//lparam = 0xC0000001 | (LPARAM)(::MapVirtualKey(VK_CONTROL, 0) << 16);
	//keybdState[VK_CONTROL] ^= 0x80;
	//::SetKeyboardState((LPBYTE)&keybdState);

    // 加载买入股票界面
	m_stockTreeView.ClickItem(TEXT("资金股票")); // 一定要切换到资金股票界面一次才不会出错 - - ？
	m_stockTreeView.ClickItem(TEXT("买入[F1]"));
    RefreshDialogsAndControls();

    // 填写买入信息
    SimClearAndSend(m_controls[CONTROL_MAIRU_DAIMA], "000793");
    SimClearAndSend(m_controls[CONTROL_MAIRU_JIAGE], "9.99");
    SimClearAndSend(m_controls[CONTROL_MAIRU_SHULIANG], "20000");

    // 执行买入操作
    SimKeystroke(m_controls[CONTROL_MAIRU_DAIMA], 'B');

    // 确认买入操作
    ConfirmDialog(true);

    // 获取提示信息
    GetHint();

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

	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("查询资金股票")) != NULL)
	{
		// 查询资金股票
		hControl = ::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("查询资金股票"));
		m_controls[CONTROL_CHAXUN_ZJGP_CXZJGP] = hControl;
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
            SimKeystroke(hControl, bConfirmed ? 'Y' : 'N');
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

void Transaction::SimClearAndSend(HWND hwnd, const char* pszSendChars)
{
    char ch;
    int nPos = 0;
    int nLen = strlen(pszSendChars);

    // 全选Edit控件内容
    ::PostMessage(hwnd, EM_SETSEL, 0, -1);

    // 模拟键盘输入
    while (nLen > nPos && (ch = pszSendChars[nPos]) != '\0')
    {
        nPos++;
        SimSendChar(hwnd, ch);
    }
}

void Transaction::SimSendChar(HWND hwnd, char ch)
{
    UINT keyRes = ::VkKeyScan(ch);

    SimKeystroke(hwnd, (BYTE)(keyRes & 0xFF));
}

void Transaction::SimKeystroke(HWND hwnd, UINT key)
{
    SimKeyDown(hwnd, key);
    //::DoKeyDelay(1); // 按下后延迟
    //SimKeyUp(hwnd, key);
    //::DoKeyDelay(5); // 弹起后延迟
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

void Transaction::DoKeyDelay(int nTimeOutMS)
{
	if (nTimeOutMS < 0)
	{
		return;
	}
	else if (nTimeOutMS == 0)
	{
		::Sleep(0);
		return;
	}

	__int64 start, current, frequence;
	double diff;
	DWORD dwMin;
	DWORD dwTimeOut = (DWORD)nTimeOutMS;

	// 设置适用Performance Counter定时器的最小的精度
	dwMin = 10;

	// 如果dwTimeOut >= dwMin，或者Performance Counter不能用，则使用原生的Sleep函数
	if (dwTimeOut >= dwMin || !::QueryPerformanceCounter((LARGE_INTEGER*)&start))
	{
		::Sleep(dwTimeOut);
		return;
	}

	// 获取频率
	::QueryPerformanceFrequency((LARGE_INTEGER*)&frequence);

	do
	{
		::Sleep(0);
		::QueryPerformanceCounter((LARGE_INTEGER*)&current);
		diff = ((double)(current - start) / (double)frequence) * 1000.0;
	} while ((DWORD)diff < dwTimeOut);
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