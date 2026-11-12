#include "orientedfeaturelayer.hxx"

namespace Openstw::SpDr
{
    OrientedFeatureLayer::OrientedFeatureLayer(FeatureType type, FeatureOrientation orientation, QObject* parent)
        : FeatureLayer{type, parent}, m_orientation{orientation}
    {
    }

    FeatureOrientation OrientedFeatureLayer::orientation() const
    {
        return this->m_orientation;
    }
}
