#pragma once

#include <objptr.h>
#include <list>
#include <string>

using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

enum PanelType
{
    PANEL_PUY,
    PANEL_SELL,
    PANEL_REVOKE,

};
class ITransaction : public x3::IObject
{
public:
    //X3DEFINE_IID(ITransaction);
    virtual bool PlaceOrder() = 0;
};