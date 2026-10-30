#include "featurelayer.hxx"
#include "exception.hxx"
#include <algorithm>
#include <ranges>

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

    QSpan<const Aperture* const> FeatureLayer::apertures() const
    {
        return {this->m_apertures};
    }

    std::optional<Aperture*> FeatureLayer::aperture(QStringView identifier)
    {
        // Try to find aperture with that name.
        // clang-format off
        const auto iter = std::ranges::find_if(
            this->m_apertures,
            [&identifier](const auto* ap) -> bool
            {
                return ap->identifier() == identifier;
            }
        );
        // clang-format on

        if (iter == this->m_apertures.end())
            return std::nullopt;

        return {*iter};
    }

    std::optional<const Aperture*> FeatureLayer::aperture(QStringView identifier) const
    {
        // Try to find aperture with that name.
        // clang-format off
        const auto iter = std::ranges::find_if(
            this->m_apertures,
            [&identifier](const auto* ap) -> bool
            {
                return ap->identifier() == identifier;
            }
        );
        // clang-format on

        if (iter == this->m_apertures.end())
            return std::nullopt;

        return {*iter};
    }

    std::optional<ApertureState> FeatureLayer::apertureState(QStringView identifier) const
    {
        // First, retrieve the aperture itself.
        const auto aperture = this->aperture(identifier);

        // clang-format off
        return aperture.and_then(
            [](const auto* aperture) -> std::optional<ApertureState>
            {
                return {aperture->state()};
            }
        );
        // clang-format on
    }

    QSpan<const Button* const> FeatureLayer::buttons() const
    {
        return {this->m_buttons};
    }

    std::optional<const Button*> FeatureLayer::button(QStringView identifier) const
    {
        // Try to find button with that name.
        // clang-format off
        const auto iter = std::ranges::find_if(
            this->m_buttons,
            [&identifier](const auto* btt) -> bool
            {
                return btt->identifier() == identifier;
            }
        );
        // clang-format on

        if (iter == this->m_buttons.end())
            return std::nullopt;

        return {*iter};
    }

    std::optional<Button*> FeatureLayer::button(QStringView identifier)
    {
        // Try to find button with that name.
        // clang-format off
        const auto iter = std::ranges::find_if(
            this->m_buttons,
            [&identifier](const auto* btt) -> bool
            {
                return btt->identifier() == identifier;
            }
        );
        // clang-format on

        if (iter == this->m_buttons.end())
            return std::nullopt;

        return {*iter};
    }

    std::optional<ButtonState> FeatureLayer::buttonState(QStringView identifier) const
    {
        // First, retrieve the button itself.
        const auto button = this->button(identifier);

        // clang-format off
        return button.and_then(
            [](const auto* btt) -> std::optional<ButtonState>
            {
                return {btt->state()};
            }
        );
        // clang-format on   
    }

    void FeatureLayer::addAperture(Aperture* element)
    {
        // Make sure we dont already have this element.
        if (this->m_apertures.contains(element))
        {
            throw Core::ArgumentError("Feature layer '{}' already contains display element '{}'",
                                      featureTypeToString(this->type()), element->identifier());
        }

        // Parent it under ourselves, such that its lifetime is
        // associated with the lifetime of this feature layer.
        element->setParent(this);

        // Add it to our collection of managed display elements.
        this->m_apertures.append(element);

        // Connect to state changed signal, and emit our element state changed
        // signal in response.
        // clang-format off
        connect(element, &Aperture::stateChanged, this,
            [this, element](ApertureState newState)
            {
                emit this->apertureStateChanged(element, newState);
            }
        );
        // clang-format on
    }

    void FeatureLayer::addButton(Button* button)
    {
        // Make sure we dont already have this button.
        if (this->m_buttons.contains(button))
        {
            throw Core::ArgumentError("Feature layer '{}' already contains button '{}'",
                                      featureTypeToString(this->type()), button->identifier());
        }

        // Parent it under ourselves, such that its lifetime is
        // associated with the lifetime of this feature layer.
        button->setParent(this);

        // Add it to our collection of managed display elements.
        this->m_buttons.append(button);

        // Connect to state changed signal, and emit our button state changed
        // signal in response.
        // clang-format off
        connect(button, &Button::stateChanged, this,
            [this, button](ButtonState newState)
            {
                emit this->buttonStateChanged(button, newState);
            }
        );
        // clang-format on
    }
}
