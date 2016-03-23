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
    m_hMainWindow = ::FindWindow(NULL, TEXT("网上股票交易系统5.0"));
    if (m_hMainWindow != nullptr)
    {
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
                        if (m_hStockTreeView != nullptr)
                        {
                            GetTreeViewItem(m_hStockTreeView, treeViewContent);
                        }
                    }
                }
            }
        }
    }

    return true;
}

HTREEITEM Transaction::TreeNodeGetNext(HWND hwnd, HTREEITEM hTreeItem)
{
    if (hwnd == nullptr || hTreeItem == nullptr) return nullptr;

    HTREEITEM result = TreeView_GetChild(hwnd, hTreeItem);
    if (result == nullptr)
    {
        result = TreeView_GetNextSibling(hwnd, hTreeItem);
    }

    HANDLE hParentID = hTreeItem;
    while (result == nullptr && hParentID != nullptr)
    {
        hParentID = TreeView_GetParent(hwnd, hParentID);
        result = TreeView_GetNextSibling(hwnd, hParentID);
    }

    return result;
}

bool Transaction::GetTreeViewItem(HWND hwnd, list<tstring>& treeViewContent)
{
    if (hwnd == nullptr) return false;

    // Create memory for passing data.
    LPVOID pMemory = VirtualAllocEx(hProcess, nullptr, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (pMemory == nullptr)
    {
        ::CloseHandle(hProcess);
        return false;
    }

    // Traverse the tree view control.
    int nItemCount = ::SendMessage(hwnd, TVM_GETCOUNT, (WPARAM)nullptr, (LPARAM)nullptr);
    HTREEITEM hTreeItem = (HTREEITEM)::SendMessage(hwnd, TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)nullptr);
    for (int i = 0; i < nItemCount; ++i)
    {
        TCHAR buf[1024] = { 0 };
        TVITEM tvItem = { 0 };
        tvItem.mask = TVIF_TEXT;
        tvItem.hItem = hTreeItem;
        tvItem.pszText = (LPWSTR)((int)pMemory + sizeof(TVITEM));
        tvItem.cchTextMax = sizeof(buf) / sizeof(TCHAR);

        DWORD dwRead = 0;
        ::WriteProcessMemory(hProcess, pMemory, &tvItem, sizeof(TVITEM), &dwRead);
        ::SendMessage(hwnd, TVM_GETITEM, (WPARAM)NULL, (LPARAM)(TVITEM*)(pMemory));
        ::ReadProcessMemory(hProcess, (LPCVOID)((int)pMemory + sizeof(TVITEM)), buf, sizeof(buf), &dwRead);

        treeViewContent.push_back(buf);
        hTreeItem = TreeNodeGetNext(hwnd, hTreeItem);

        ::SendMessage(hwnd, TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)(HTREEITEM)(hTreeItem));
        RECT rect = { 0 };
        RECT* pRect = nullptr;
        pRect = (RECT*)::VirtualAllocEx(hProcess, nullptr, sizeof(RECT), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (pRect == nullptr)
        {
            ::CloseHandle(hProcess);
            return false;
        }
        *(HTREEITEM*)&rect = hTreeItem;
        ::WriteProcessMemory(hProcess, pRect, &rect, sizeof(RECT), &dwRead);
        ::SendMessage(hwnd, TVM_GETITEMRECT, (WPARAM)FALSE, (LPARAM)pRect);
        ::ReadProcessMemory(hProcess, pRect, &rect, sizeof(RECT), &dwRead);
        POINT point = { 0 };
        point.x = rect.left + 50;
        point.y = rect.bottom + 10;
        //::SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(point.x, point.y));
        //::SendMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(point.x, point.y));
        ::VirtualFreeEx(hProcess, pRect, 0, MEM_RELEASE);
    }

    // Release memory.
    ::VirtualFreeEx(hProcess, pMemory, 0, MEM_RELEASE);

    // Terminate process.
    ::CloseHandle(hProcess);
    return true;
}

bool Transaction::Load()
{
    // Get the window handle of "xiadan.exe".
    m_hMainWindow = ::FindWindow(NULL, TEXT("网上股票交易系统5.0"));
    if (m_hMainWindow == nullptr) return false;

    // Get the window handle of controls inside the main window.
    // Stock Tree View
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

    // Get the process handle of "xiadan.exe".
    DWORD dwProcessId;
    ::GetWindowThreadProcessId(m_hMainWindow, &dwProcessId);
    m_hProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId);
    if (m_hProcess == nullptr) return false;

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