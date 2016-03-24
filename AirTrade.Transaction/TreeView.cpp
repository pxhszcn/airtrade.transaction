#include "TreeView.h"

RemoteTreeViewItem::RemoteTreeViewItem()
{
	m_hProcess = nullptr;
	m_hTreeView = nullptr;
	m_hTreeItem = nullptr;
    m_text = TEXT("");
}

RemoteTreeViewItem::RemoteTreeViewItem(HANDLE hProcess, HWND hTreeView, HTREEITEM hTreeItem)
{
	m_hProcess = hProcess;
	m_hTreeView = hTreeView;
	m_hTreeItem = hTreeItem;
    m_text = TEXT("");
}

RemoteTreeViewItem::RemoteTreeViewItem(const RemoteTreeViewItem& item)
{
	m_hProcess = item.m_hProcess;
	m_hTreeView = item.m_hTreeView;
	m_hTreeItem = item.m_hTreeItem;
    m_text = item.m_text;
}

RemoteTreeViewItem& RemoteTreeViewItem::operator=(const RemoteTreeViewItem& item)
{
	m_hProcess = item.m_hProcess;
	m_hTreeView = item.m_hTreeView;
	m_hTreeItem = item.m_hTreeItem;
    m_text = item.m_text;
	return *this;
}

RemoteTreeViewItem::~RemoteTreeViewItem()
{
}

RemoteTreeViewItem RemoteTreeViewItem::GetParent()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetParent(m_hTreeView, m_hTreeItem));
}

RemoteTreeViewItem RemoteTreeViewItem::GetFirstChild()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetChild(m_hTreeView, m_hTreeItem));
}

RemoteTreeViewItem RemoteTreeViewItem::GetPrevious()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetPrevSibling(m_hTreeView, m_hTreeItem));
}

RemoteTreeViewItem RemoteTreeViewItem::GetNext()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetNextSibling(m_hTreeView, m_hTreeItem));
}

RemoteTreeViewItem RemoteTreeViewItem::GetNextVisible()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetNextVisible(m_hTreeView, m_hTreeItem));
}

RemoteTreeViewItem RemoteTreeViewItem::GetPrevVisible()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetPrevVisible(m_hTreeView, m_hTreeItem));
}

RemoteTreeViewItem RemoteTreeViewItem::GetNextSelected()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetNextSelected(m_hTreeView, m_hTreeItem));
}

bool RemoteTreeViewItem::GetExpandedState()
{
	return (TreeView_GetItemState(m_hTreeView, m_hTreeItem, TVIS_EXPANDED) != 0);
}

bool RemoteTreeViewItem::GetSelectedState()
{
	return (TreeView_GetItemState(m_hTreeView, m_hTreeItem, TVIS_SELECTED) != 0);;
}

bool RemoteTreeViewItem::GetExistedState()
{
	return (m_hTreeItem != nullptr);
}

void RemoteTreeViewItem::Expand()
{
	TreeView_Expand(m_hTreeView, m_hTreeItem, TVE_EXPAND);
}

void RemoteTreeViewItem::Collapse()
{
	TreeView_Expand(m_hTreeView, m_hTreeItem, TVE_COLLAPSE);
}

void RemoteTreeViewItem::Select()
{
	TreeView_SelectItem(m_hTreeView, m_hTreeItem);
}

void RemoteTreeViewItem::Click()
{
	this->Select();

    // below seems useless...
	//// reserve memory in remote process
	//RECT* pRect = (RECT*)::VirtualAllocEx(m_hProcess, nullptr, sizeof(RECT), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//if (pRect == nullptr) return;

	//// read the rect
	//SIZE_T dwRead = 0;
	//RECT rect = { 0 };
	//*(HTREEITEM*)&rect = m_hTreeItem;
	//::WriteProcessMemory(m_hProcess, pRect, &rect, sizeof(RECT), &dwRead);
	//::SendMessage(m_hTreeView, TVM_GETITEMRECT, (WPARAM)FALSE, (LPARAM)pRect);
	//::ReadProcessMemory(m_hProcess, pRect, &rect, sizeof(RECT), &dwRead);
	//
	//// do the click
	//POINT point = { 0 };
	//point.x = rect.left + 50;
	//point.y = rect.bottom - 10;
	//::SendMessage(m_hTreeView, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(point.x, point.y));
	//::SendMessage(m_hTreeView, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(point.x, point.y));
	//
	//// release memory
	//::VirtualFreeEx(m_hProcess, pRect, 0, MEM_RELEASE);
}

HTREEITEM RemoteTreeViewItem::GetHandle()
{
	return m_hTreeItem;
}

tstring RemoteTreeViewItem::GetText()
{
	if (m_hProcess == nullptr) return TEXT("");

    if (m_text != TEXT("")) return m_text;

	// reserve memory in remote process
	LPVOID pMemory = VirtualAllocEx(m_hProcess, nullptr, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (pMemory == nullptr) return TEXT("");

	TCHAR buffer[LISTVIEW_TEXT_BUFFER_SIZE] = { 0 };
	TVITEM tvItem = { 0 };
	tvItem.mask = TVIF_TEXT;
	tvItem.hItem = m_hTreeItem;
	tvItem.pszText = (LPWSTR)((int)pMemory + sizeof(TVITEM));
	tvItem.cchTextMax = sizeof(buffer) / sizeof(TCHAR);

	// read the text
	SIZE_T dwRead = 0;
	::WriteProcessMemory(m_hProcess, pMemory, &tvItem, sizeof(TVITEM), &dwRead);
	::SendMessage(m_hTreeView, TVM_GETITEM, (WPARAM)NULL, (LPARAM)(TVITEM*)(pMemory));
	::ReadProcessMemory(m_hProcess, (LPCVOID)((int)pMemory + sizeof(TVITEM)), buffer, sizeof(buffer), &dwRead);

	// release memory
	::VirtualFreeEx(m_hProcess, pMemory, 0, MEM_RELEASE);

    m_text = buffer;
	return m_text;
}

RemoteTreeViewItem RemoteTreeViewItem::TraverseGetNext()
{
	RemoteTreeViewItem item = this->GetFirstChild();
	if (!item.GetExistedState())
	{
		item = this->GetNext();
	}
	RemoteTreeViewItem parentItem = *this;
	while (!item.GetExistedState() && parentItem.GetExistedState())
	{
		parentItem = parentItem.GetParent();
		item = parentItem.GetNext();
	}

	return item;
}

RemoteTreeView::RemoteTreeView()
{
	m_hProcess = nullptr;
	m_hTreeView = nullptr;
}

RemoteTreeView::RemoteTreeView(HANDLE hProcess, HWND hTreeView)
{
	m_hProcess = hProcess;
	m_hTreeView = hTreeView;
}

RemoteTreeView::RemoteTreeView(const RemoteTreeView& treeView)
{
	m_hProcess = treeView.m_hProcess;
	m_hTreeView = treeView.m_hTreeView;
	m_treeViewItems = treeView.m_treeViewItems;
}

RemoteTreeView& RemoteTreeView::operator=(const RemoteTreeView& treeView)
{
	m_hProcess = treeView.m_hProcess;
	m_hTreeView = treeView.m_hTreeView;
	m_treeViewItems = treeView.m_treeViewItems;
	return *this;
}

RemoteTreeView::~RemoteTreeView()
{
}

int RemoteTreeView::GetCount()
{
	return TreeView_GetCount(m_hTreeView);
}

RemoteTreeViewItem RemoteTreeView::GetRoot()
{
	return RemoteTreeViewItem(m_hProcess, m_hTreeView, TreeView_GetRoot(m_hTreeView));
}

bool RemoteTreeView::TraverseTreeView()
{
	m_treeViewItems.clear();

	RemoteTreeViewItem item = this->GetRoot();
    item.GetText();
	m_treeViewItems.push_back(item);

	for (int i = 0; i < this->GetCount() - 1; ++i)
	{
		item = item.TraverseGetNext();
        item.GetText();
		m_treeViewItems.push_back(item);
	}

	return true;
}

bool RemoteTreeView::ClickItem(tstring text)
{
    for (auto iter = m_treeViewItems.begin(); iter != m_treeViewItems.end(); ++iter)
    {
        if (text == (*iter).GetText())
        {
            (*iter).Click();
            return true;
        }
    }

    return false;
}
