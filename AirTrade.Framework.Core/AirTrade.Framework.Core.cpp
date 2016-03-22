#include <module/plugininc.h>
#include <module/pluginimpl.h>
#include <module/modulemacro.h>
#include <nonplugin/useplugins.h>
#include "AirTrade.Framework.Core.h"

ComponentService::ComponentService()
{
}

ComponentService::~ComponentService()
{
}

void* ComponentService::LoadComponent(const char *componentName)
{
    if (m_components.find(componentName) == m_components.end())
    {
        x3::AnyObject component(componentName);
        m_components[componentName] = component;
    }

    return m_components[componentName].p();
}

/// Module definition
XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY(ComponentService)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}