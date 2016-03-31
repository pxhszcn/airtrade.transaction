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
	// ���Ի�ȡ�ʽ���Ϣ������Ctrl+C
	m_stockTreeView.ClickItem(TEXT("��ǩ")); // ��Ҫ���л��������õĽ���Ȼ���ٵ���Ҫ�Ľ���Ź�������
	m_stockTreeView.ClickItem(TEXT("�ʽ��Ʊ"));
	RefreshDialogsAndControls();
	m_sendKeys.Send("^C", m_controls[CONTROL_CHAXUN_ZJGP]);

    // ���������Ʊ����
	m_stockTreeView.ClickItem(TEXT("��ǩ")); 
	m_stockTreeView.ClickItem(TEXT("����[F1]"));
    RefreshDialogsAndControls();

    // ��д������Ϣ
	::PostMessage(m_controls[CONTROL_MAIRU_DAIMA], EM_SETSEL, 0, -1);
	m_sendKeys.Send("000793", m_controls[CONTROL_MAIRU_DAIMA]);
	::PostMessage(m_controls[CONTROL_MAIRU_JIAGE], EM_SETSEL, 0, -1);
	m_sendKeys.Send("9.99", m_controls[CONTROL_MAIRU_JIAGE]);
	::PostMessage(m_controls[CONTROL_MAIRU_SHULIANG], EM_SETSEL, 0, -1);
	m_sendKeys.Send("20000", m_controls[CONTROL_MAIRU_SHULIANG]);

    // ִ���������
	m_sendKeys.Send("B", m_controls[CONTROL_MAIRU_DAIMA]);

    // ȷ���������
    ConfirmDialog(true);

    // ��ȡ��ʾ��Ϣ
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
    // ��ȡͬ��˳�µ�����������ھ��
    m_hMainWindow = ::FindWindow(NULL, TEXT("���Ϲ�Ʊ����ϵͳ5.0"));
    if (m_hMainWindow == NULL) return false;

    // ��ȡ�������е������ؼ��ľ��
    // ��ƱTreeView�ؼ����
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

    // ˢ�¹��ܽ����������еĿؼ����
    RefreshDialogsAndControls();

    // ��ȡͬ��˳�µ�����Ľ��̾��
    DWORD dwProcessId;
    ::GetWindowThreadProcessId(m_hMainWindow, &dwProcessId);
    m_hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId);
    if (m_hProcess == NULL) return false;

	// ������ƱTreeView�ؼ�
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

	// �ȴ������ȶ�
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

	// �����Ʊ����
	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("�����Ʊ")) != NULL)
	{
		// ֤ȯ����
		hControl = ::FindWindowEx(hDialog, NULL, TEXT("Edit"), NULL);
		m_controls[CONTROL_MAIRU_DAIMA] = hControl;

		// ����۸�
		hControl = ::FindWindowEx(hDialog, hControl, TEXT("Edit"), NULL);
		m_controls[CONTROL_MAIRU_JIAGE] = hControl;

		// ��������
		hControl = ::FindWindowEx(hDialog, hControl, TEXT("Edit"), NULL);
		m_controls[CONTROL_MAIRU_SHULIANG] = hControl;

		return true;
	}

	// ������Ʊ����
	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("������Ʊ")) != NULL)
	{
		return true;
	}

	// ������Ʊ����
	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("��ί�м�¼�������˫����س����ɳ���")) != NULL)
	{
		return true;
	}

	// ��ѯ�ʽ��Ʊ����
	if (::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("��ѯ�ʽ��Ʊ")) != NULL)
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

	// �ȴ����ڵ���
	::Sleep(500);

    // Ѱ�Ҵ��ڲ�ִ�в���
    while (true)
    {
        hDialog = ::FindWindowEx(NULL, hDialog, TEXT("#32770"), NULL);
        if (hDialog == NULL) break;

        // ί��ȷ�ϴ���
        HWND hControl = ::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("ί��ȷ��"));
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

    // �ȴ����ڵ���
    ::Sleep(500);

    // Ѱ�Ҵ��ڲ�ִ�в���
    while (true)
    {
        hDialog = ::FindWindowEx(NULL, hDialog, TEXT("#32770"), NULL);
        if (hDialog == NULL) break;

        // ��ʾ����
        HWND hControl = ::FindWindowEx(hDialog, NULL, TEXT("Static"), TEXT("��ʾ"));
        if (hControl != NULL)
        {
            HWND hHintControl = ::GetWindow(hControl, GW_HWNDPREV);
            if (hHintControl != NULL)
            {
				// ��ȡ��ʾ����
				::SendMessage(hHintControl, WM_GETTEXT, (WPARAM)STATIC_CONTROL_MAX_LENGTH, (LPARAM)szStaticBuf); // GetWindowText������ - -��
				SYSTEMTIME now = { 0 };
				::GetLocalTime(&now);
				TCHAR resultBuf[STATIC_CONTROL_MAX_LENGTH + 20];
				_stprintf_s(resultBuf, TEXT("[%04d-%02d-%02d %02d:%02d:%02d] - %s"), now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond, szStaticBuf);
				m_hints.push_back(resultBuf);
				OutputDebugString(resultBuf);

				// ���ȷ����ť
				HWND hOKButton = ::FindWindowEx(hDialog, NULL, TEXT("Button"), TEXT("ȷ��"));
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