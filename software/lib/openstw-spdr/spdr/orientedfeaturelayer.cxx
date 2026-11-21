#include <core/exception.hxx>
#include <core/xmlutility.hxx>

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

    FeatureOrientation OrientedFeatureLayer::parseOrientation(QDomElement xmlElement)
    {
        const auto orientation = Core::Xml::enumAttribute<FeatureOrientation>(xmlElement, u"Orientation");
        if (!orientation.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Feature element missing orientation attribute",
                                     xmlElement.lineNumber());
        }

        return orientation.value();
    }
}
