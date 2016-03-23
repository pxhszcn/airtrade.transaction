#pragma once

#include <list>
#include <string>
#include <vector>

using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

struct TreeViewNode
{
    tstring label;
    bool isExpanded;
    bool isSelected;
    vector<TreeViewNode> children;
};

class TreeView
{
public:
    TreeView();
    virtual ~TreeView();

private:
    
};