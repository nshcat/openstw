#include <QCursor>
#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "tilefeaturegraphicsobject.hxx"

namespace Rendering
{
    TileFeatureGraphicsObject::TileFeatureGraphicsObject(Openstw::SpDr::FeatureType featureType,
                                                         TileGraphicsObject* parent, bool requiresBlinkImpulse,
                                                         bool providesContextMenu)
        : QGraphicsObject(parent),
          IContextMenuProvider(),
          m_tileGraphicsObj{parent},
          m_providesContextMenu{providesContextMenu},
          m_featureType{featureType},
          m_requiresBlinkImpulse{requiresBlinkImpulse}
    {
        // If the sub class indicated that it contains apertures that might blink,
        // we need to invalidate and redraw the feature graphics object whenever
        // the global blink timer fires.
        if (this->m_requiresBlinkImpulse)
        {
            // clang-format off
            connect(this->blinkGenerator(), &BlinkGenerator::blink,
                    this, &TileFeatureGraphicsObject::onBlinkImpulse);
            // clang-format on
        }
    }

    Openstw::SpDr::FeatureTypeNS::FeatureType TileFeatureGraphicsObject::featureType() const
    {
        return this->m_featureType;
    }

    void TileFeatureGraphicsObject::setup()
    {
        const auto mouseButtons = this->acceptsMouseInput();
        this->setAcceptedMouseButtons(mouseButtons.value_or(Qt::MouseButton::NoButton));
        if (mouseButtons.has_value())
            this->setCursor(Qt::PointingHandCursor);

        this->positionSelf();
        this->afterSetup();
    }

    TileGraphicsObject* TileFeatureGraphicsObject::tileGraphicsObject() const
    {
        return this->m_tileGraphicsObj;
    }

    bool TileFeatureGraphicsObject::providesContextMenu() const
    {
        return this->m_providesContextMenu;
    }

    std::optional<Qt::MouseButtons> TileFeatureGraphicsObject::acceptsMouseInput() const
    {
        return std::nullopt;
    }

    void TileFeatureGraphicsObject::afterSetup()
    {
        return;
    }

    void TileFeatureGraphicsObject::onBlinkImpulse(BlinkState state)
    {
        // Trigger a redraw of the feature graphics object.
        this->update();
    }

    void TileFeatureGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        // Always clip to the bounding rectangle to make painting diagonal items
        // easier (by being able to use rotated rectangles)
        painter->setClipRect(this->boundingRect());

        // Enable antialiasing
        painter->setRenderHint(QPainter::Antialiasing);
    }

    BlinkGenerator* TileFeatureGraphicsObject::blinkGenerator() const
    {
        return this->tileGraphicsObject()->blinkGenerator();
    }

    QColor TileFeatureGraphicsObject::apertureRenderColor(const Openstw::SpDr::Aperture* aperture) const
    {
        // Retrieve the apertures current state.
        const auto& currentState = aperture->state();

        // We need to do special handling if the aperture is blinking.
        if (currentState.blinkMode() == Openstw::SpDr::BlinkMode::Blinking)
        {
            // If the state of the global blink timer is "lamp on", translate
            // the aperture color to a rendering color.
            if (this->blinkGenerator()->lampsOn())
            {
                return convertApertureColor(currentState.color());
            }
            else
            {
                // Otherwise, return the Off color.
                return convertApertureColor(Openstw::SpDr::ApertureColor::Off);
            }
        }
        else
        {
            // Otherwise, just translate the aperture color to a rendering
            // color directly.
            return convertApertureColor(currentState.color());
        }
    }

    bool TileFeatureGraphicsObject::apertureIsOn(const Openstw::SpDr::Aperture* aperture) const
    {
        // Retrieve the apertures current state.
        const auto& currentState = aperture->state();

        // We need to do special handling if the aperture is blinking.
        if (currentState.blinkMode() == Openstw::SpDr::BlinkMode::Blinking)
        {
            return this->blinkGenerator()->lampsOn();
        }

        // Otherwise, check whether the aperture is off.
        return !currentState.isOff();
    }
}
