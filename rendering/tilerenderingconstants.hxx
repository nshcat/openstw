#pragma once

#include <QtCore>

namespace Rendering
{
    struct TileRenderingConstants
    {
        constexpr static qreal tileWidth = 160.0;
        constexpr static qreal tileHeight = 100.0;

        constexpr static qreal halfTileWidth = tileWidth / 2.0f;
        constexpr static qreal halfTileHeight = tileHeight / 2.0f;

        constexpr static qreal tileDiagonalAngle = 65.0f / 2.0f; //< Angle between diagonal and horizontal center line

    private:
        TileRenderingConstants() = delete;
    };
}
