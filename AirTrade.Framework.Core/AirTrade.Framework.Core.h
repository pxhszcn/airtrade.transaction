#pragma once

#include <module/classmacro.h>
#include <unordered_map>

#include "AirTrade.Framework.Core.IComponentService.h"

class ComponentService : public IComponentService
{
    X3BEGIN_CLASS_DECLARE(ComponentService, "AirTrade.Framework.Core.ComponentService")
        X3DEFINE_INTERFACE_ENTRY(IComponentService)
    X3END_CLASS_DECLARE()

protected:
    ComponentService();
    virtual ~ComponentService();

private:
    virtual void* LoadComponent(const char* componentName);

private:
    std::unordered_map<const char*, x3::AnyObject> m_components;
};