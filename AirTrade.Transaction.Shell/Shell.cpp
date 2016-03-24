#include <portability/x3port.h>
#include <nonplugin/scanplugins.h>
#include "../AirTrade.Framework.Core/AirTrade.Framework.Core.IComponentService.h"
#include "../AirTrade.Transaction/ITransaction.h"

int main()
{
    LOAD_COMPONENT_SERVICE(componentService);

    if (componentService)
    {
        ITransaction* transaction = (ITransaction*)componentService->LoadComponent("AirTrade.Transaction");
        if (transaction)
        {
            transaction->PlaceOrder();
        }
        else
        {
            // Transaction component load failed.
        }
    }

    UNLOAD_COMPONENT_SERVICE;

    return 0;
}