#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <list>
#include <string>
#include <vector>

using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#define LISTVIEW_TEXT_BUFFER_SIZE 1024

class RemoteTreeViewItem
{
public:
	RemoteTreeViewItem();
	RemoteTreeViewItem(HANDLE hProcess, HWND hTreeView, HTREEITEM hTreeItem);
	RemoteTreeViewItem(const RemoteTreeViewItem& item);
	RemoteTreeViewItem& operator=(const RemoteTreeViewItem& item);
	virtual ~RemoteTreeViewItem();

	RemoteTreeViewItem GetParent();
	RemoteTreeViewItem GetFirstChild();
	RemoteTreeViewItem GetPrevious();
	RemoteTreeViewItem GetNext();
	RemoteTreeViewItem GetNextVisible();
	RemoteTreeViewItem GetPrevVisible();
	RemoteTreeViewItem GetNextSelected();
	bool GetExpandedState();
	bool GetSelectedState();
	bool GetExistedState();
	void Expand();
	void Collapse();
	void Select();
	void Click();
	HTREEITEM GetHandle();

	tstring GetText();

	RemoteTreeViewItem TraverseGetNext();

private:
	HANDLE m_hProcess;
	HWND m_hTreeView;
	HTREEITEM m_hTreeItem;
    tstring m_text;
};

class RemoteTreeView
{
public:
	RemoteTreeView();
	RemoteTreeView(HANDLE hProcess, HWND hTreeView);
	RemoteTreeView(const RemoteTreeView& treeView);
	RemoteTreeView& operator=(const RemoteTreeView& treeView);
	~RemoteTreeView();

	int GetCount();
	RemoteTreeViewItem GetRoot();
	bool TraverseTreeView();
    bool ClickItem(tstring text);

private:
	HANDLE m_hProcess;
	HWND m_hTreeView;
	vector<RemoteTreeViewItem> m_treeViewItems;
};