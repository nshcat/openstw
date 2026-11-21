#pragma once

#include <QObject>

#include <QDomElement>

#include "featurelayer.hxx"
#include "featureorientation.hxx"
#include "featuretype.hxx"

namespace Openstw::SpDr
{
    // Base class for feature layers that can have multiple orientations,
    // as described by the `FeatureOrientation` enumeration.
    class OrientedFeatureLayer : public FeatureLayer
    {
        Q_OBJECT

    public:
        explicit OrientedFeatureLayer(FeatureType type, FeatureOrientation orientation, QObject* parent = nullptr);

    public:
        // The orientation of the feature.
        FeatureOrientation orientation() const;

    protected:
        // Parse the feature orientation from given XML element.
        static FeatureOrientation parseOrientation(QDomElement xmlElement);

    protected:
        // The orientation of the feature.
        FeatureOrientation m_orientation;
    };

}
