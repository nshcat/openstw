#pragma once

#include <QtCore>

namespace Rendering
{
    struct TileRenderingConstants
    {
        constexpr static qreal tileWidth = 160.0;
        constexpr static qreal tileHeight = 100.0;

    private:
        TileRenderingConstants() = delete;
    };
}
