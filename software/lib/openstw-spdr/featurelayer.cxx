#include "featurelayer.hxx"
#include "exception.hxx"

namespace Openstw::SpDr
{
    FeatureLayer::FeatureLayer(FeatureType type, QObject* parent)
        : QObject{parent}, m_type{type}
    {
    }

    FeatureType FeatureLayer::type() const
    {
        return this->m_type;
    }

    QSpan<const DisplayElement* const> FeatureLayer::displayElements() const
    {
        return {this->m_displayElements};
    }

    void FeatureLayer::addElement(DisplayElement* element)
    {
        // Make sure we dont already have this element.
        if (this->m_displayElements.contains(element))
        {
            throw Core::ArgumentError("Feature layer '{}' already contains display element '{}'",
                                      featureTypeToString(this->type()), element->identifier());
        }

        // Parent it under ourselves, such that its lifetime is
        // associated with the lifetime of this feature layer.
        element->setParent(this);

        // Add it to our collection of managed display elements.
        this->m_displayElements.append(element);

        // Connect to state changed signal, and emit our element state changed
        // signal in response.
        connect(element, &DisplayElement::stateChanged, this,
                [this, element](ElementState newState) { this->notifyElementStateChanged(element, newState); });
    }

    void FeatureLayer::notifyElementStateChanged(DisplayElement* element, const ElementState& newState)
    {
        emit this->elementStateChanged(element, newState);
    }
}
