#include "tile.hxx"

namespace Openstw
{
    namespace SpDr
    {

        Tile::Tile(const TileLocation& location, QObject* parent)
            : QObject{parent}, m_location{location}
        {
        }

        const TileLocation& Tile::location() const
        {
            return this->m_location;
        }

        QSpan<const FeatureLayer* const> Tile::layers() const
        {
            return {this->m_featureLayers};
        }
    }
}
