//#pragma once

#include <objptr.h>

#define LOAD_COMPONENT_SERVICE(obj) \
    x3::loadScanPlugins(""); \
    x3::Object<IComponentService> obj("AirTrade.Framework.Core.ComponentService");

#define UNLOAD_COMPONENT_SERVICE \
    x3::unloadScanPlugins();

class IComponentService : public x3::IObject
{
public:
    //X3DEFINE_IID(IComponentService);
    virtual void* LoadComponent(const char* componentName) = 0;
};