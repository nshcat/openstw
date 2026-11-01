#include "elementtype.hxx"

namespace Openstw::Interlocking
{
    bool isSignal(ElementType type)
    {
        switch (type)
        {
        case ElementType::SignalMast:
            [[fallthrough]];
        case ElementType::RouteDestination:
            return true;

        default:
            return false;
        }
    }

    bool isSwitch(ElementType type)
    {
        switch (type)
        {
        case ElementType::Switch:
            return true;

        default:
            return false;
        }
    }
}