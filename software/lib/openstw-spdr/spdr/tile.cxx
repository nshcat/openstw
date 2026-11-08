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

        void Tile::addLayer(FeatureLayer* layer)
        {
            this->m_featureLayers.append(layer);

            layer->setParent(this);

            // XXX Add its apertures and such to our registry?
        }
    }
}
