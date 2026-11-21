#pragma once

#include <QDomElement>
#include <QObject>
#include <QSpan>
#include <QVector>
#include <optional>
#include <type_traits>

#include <core/exception.hxx>
#include <core/xmlutility.hxx>
#include <interlocking/element.hxx>
#include <interlocking/system.hxx>

#include "aperture.hxx"
#include "button.hxx"
#include "featuretype.hxx"

namespace Openstw::SpDr
{
    // A single, self-contained feature found on a SpDr tile.
    // Features take care of listening to their corresponding
    // interlocking objects and deriving output aperture state
    // from the state of those objects.
    class FeatureLayer : public QObject
    {
        Q_OBJECT

    public:
        explicit FeatureLayer(FeatureType type, QObject* parent = nullptr);

        // Pure virtual destructor to force the class to be abstract.
        virtual ~FeatureLayer() = 0;

    public:
        // Specific type of feature this layer represents.
        FeatureType type() const;

        // All apertures this feature layer manages.
        QSpan<Aperture const* const> apertures() const;

        // Retrieve aperture managed by this layer via its identifier.
        std::optional<Aperture*> aperture(QStringView identifier);

        // Retrieve aperture managed by this layer via its identifier.
        std::optional<const Aperture*> aperture(QStringView identifier) const;

        // Retrieve current state of aperture managed by this layer, via its identifier.
        std::optional<ApertureState> apertureState(QStringView identifier) const;

        // All buttons this feature layer manages.
        QSpan<Button const* const> buttons() const;

        // Retrieve button managed by this later via its identifier.
        std::optional<const Button*> button(QStringView identifier) const;

        // Retrieve button managed by this later via its identifier.
        std::optional<Button*> button(QStringView identifier);

        // Retrieve current state of button managed by this layer, via its identifier.
        std::optional<ButtonState> buttonState(QStringView identifier) const;

    protected:
        // Parse interlocking element the feature layer references from given
        // XML element, and retrieve it from the interlocking system, casting it
        // to the correct type.
        template <typename T>
        static T* parseInterlockingElement(QDomElement xmlElement, QStringView attributeName,
                                           Interlocking::System* interlocking);

    protected:
        // Add given aperture to the collection of apertures
        // this feature layer manages. This should be called while
        // constructing a specific feature layer sub class to populate
        // the layer with apertures.
        // It takes care of listening to the apertures state change
        // signals, and routing them to the feature layers `apertureStateChanged`
        // signal.
        void addAperture(Aperture* aperture);

        // Add given button to the collection of buttons this feature layer manages.
        // Signals and lifetime will be set up as needed.
        void addButton(Button* button);

        // Convenience method that creates a new aperture object with given identifier,
        // adds it to the feature layer via a call to `addAperture`, and returns a pointer
        // to it such that the calling sub class can store it within its member attributes.
        Aperture* createAperture(QStringView identifier);

        // Convenience method that creates a new button object with given identifier,
        // given interlocking system element reference and given role, adds it to the
        // feature layer via a call to `addButton`, and returns a pointer to it such
        // that the calling sub class can store it within its member attributes.
        Button* createButton(QStringView identifier, ButtonRole role,
                             std::optional<Interlocking::Element*> referencedElement = std::nullopt);

    signals:
        // Emitted whenever the state of an aperture managed by this
        // feature layer changes.
        void apertureStateChanged(Openstw::SpDr::Aperture* aperture, Openstw::SpDr::ApertureState newState);

        // Emitted whenever the state of a button managed by this
        // feature layer changes.
        void buttonStateChanged(Openstw::SpDr::Button* button, Openstw::SpDr::ButtonState newState);

    protected:
        // Type of feature this layer represents.
        FeatureType m_type;

        // All apertures managed by this layer.
        // They are owned by this class, and are thus children of it!
        QVector<Aperture*> m_apertures{};

        // All buttons managed by this layer.
        // They are owned by this class.
        QVector<Button*> m_buttons{};
    };

    template <typename T>
    T* FeatureLayer::parseInterlockingElement(QDomElement xmlElement, QStringView attributeName,
                                              Interlocking::System* interlocking)
    {
        // Ensure that T actually derives from Interlocking::Element.
        static_assert(std::is_base_of_v<Interlocking::Element, std::decay_t<T>>,
                      "T needs to be a sub class of Interlocking::Element");

        // Retrieve the name of the element that the feature references.
        const auto elementName = Core::Xml::stringAttribute(xmlElement, attributeName);
        if (!elementName.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Feature element missing {} attribute", xmlElement.lineNumber(),
                                     attributeName);
        }

        // Try to find the element within the interlocking system.
        auto element = interlocking->element(elementName.value());
        if (!element.has_value())
        {
            throw Core::RuntimeError(
                "SpDr XML: Line {}: Feature element references non-existent interlocking element '{}'",
                xmlElement.lineNumber(), elementName.value());
        }

        // Try to downcast the interlocking element.
        auto* castedElement = dynamic_cast<T*>(element.value());

        // Validate the cast.
        if (!castedElement)
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Feature element references interlocking "
                                     "element '{}', which unexpectedly is not of the expected type",
                                     xmlElement.lineNumber(), elementName.value());
        }

        return castedElement;
    }
}