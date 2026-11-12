#pragma once

namespace Rendering
{
    /**
     * @brief Interface that can be used on any element on the Stelltisch
     * to notify the scene view whether it can provide its own context menu
     * (and thus needs the view to pass on the context menu event)
     */
    class IContextMenuProvider
    {
    public:
        IContextMenuProvider() = default;
        virtual ~IContextMenuProvider() = default;

    public:
        virtual bool providesContextMenu() const = 0;
    };
}
