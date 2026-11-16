#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "hpsignalfeatureobject.hxx"
#include "renderinghelpers.hxx"
#include "tilerenderingconstants.hxx"

namespace Rendering
{
    HpSignalFeatureObject::HpSignalFeatureObject(const Openstw::SpDr::HpSignalFeature* feature,
                                                 TileGraphicsObject* parent)
        : TileFeatureGraphicsObject{Openstw::SpDr::FeatureType::HpSignal, parent, true}, m_feature{feature}
    {
        // Retrieve the signal lamp apertures.
        this->m_redLamp = this->m_feature->redLamp();
        this->m_greenLamp = this->m_feature->greenLamp();

        // Listen to their state changing.
        // clang-format off
        connect(this->m_redLamp, &Openstw::SpDr::Aperture::stateChanged,
                this, &HpSignalFeatureObject::onApertureStateChanged);

        connect(this->m_greenLamp, &Openstw::SpDr::Aperture::stateChanged,
                this, &HpSignalFeatureObject::onApertureStateChanged);
        // clang-format on
    }

    QRectF HpSignalFeatureObject::boundingRect() const
    {
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void HpSignalFeatureObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    QRectF HpSignalFeatureObject::calculateAreaRect(VerticalDirection location) const
    {
        return this->tileGraphicsObject()->horizontalDrawingAreaRect(location);
    }

    QRectF HpSignalFeatureObject::calculateSignalArea() const
    {
        if (this->m_feature->orientation() == Openstw::SpDr::FeatureOrientation::Backward)
        {
            return this->calculateAreaRect(VerticalDirection::Top);
        }
        else
        {
            return this->calculateAreaRect(VerticalDirection::Bottom);
        }
    }

    void HpSignalFeatureObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        painter->save();

        // Base class method call sets up clipping region
        TileFeatureGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();

        // == Draw area containing the signal graphic
        const auto signalAreaRect = this->calculateSignalArea();
        const auto drawRotated = (this->m_feature->orientation() == Openstw::SpDr::FeatureOrientation::Backward);
        this->drawSignalArea(painter, signalAreaRect, drawRotated);

        // == Draw area containing the various melder and the signal label
        // XXXTODO

        painter->restore();
    }

    void HpSignalFeatureObject::drawSignalArea(QPainter* painter, const QRectF& location, const bool rotated) const
    {
        painter->save();

        // We need to rotate our drawing by 180 degrees if we are rendering a backwards signal
        if (rotated)
        {
            // We want to rotate around the center of the bounding rectangle.
            const auto locationCenter = location.center();

            painter->translate(locationCenter);
            painter->rotate(180.0f);
            painter->translate(-locationCenter);
        }

        // Measure out how big the Signalschirm would be if rendered.
        // The returned rect always has position 0,0.
        const auto hauptSchirmSize = this->measureHauptSignal();

        // Lay it out. The top end of the topmost Signalschirm is always fixed to be the same distance
        // to the tile border.
        QRectF hauptSchirmRect{location.right() - hauptSchirmSize.width() -
                                   HpSignalFeatureObject::signalToBorderPadding,
                               centerWithin(hauptSchirmSize.height(), location.height(), location.top()),
                               hauptSchirmSize.width(), hauptSchirmSize.height()};

        // Render it
        this->drawHauptSignal(painter, hauptSchirmRect);

        // Render mast
        const auto mastBaseRect =
            QRectF{hauptSchirmRect.left() - HpSignalFeatureObject::mastBaseWidth,
                   centerWithin(HpSignalFeatureObject::mastBaseHeight, hauptSchirmRect.height(), hauptSchirmRect.top()),
                   HpSignalFeatureObject::mastBaseWidth, HpSignalFeatureObject::mastBaseHeight};

        this->drawMastBase(painter, mastBaseRect);

        painter->restore();
    }

    void HpSignalFeatureObject::drawMastBase(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        const QRectF mastBaseRect{location.left(), location.top(), HpSignalFeatureObject::mastThickness,
                                  location.height()};

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(mastBaseRect, 1.0f));

        this->drawMastSegment(painter, location);

        painter->restore();
    }

    void HpSignalFeatureObject::drawMastSegment(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        const QRectF mastRect{location.left(),
                              centerWithin(HpSignalFeatureObject::mastThickness, location.height(), location.top()),
                              location.width(), HpSignalFeatureObject::mastThickness};

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(mastRect, 1.0f));
    }

    void HpSignalFeatureObject::drawSignalLamp(QPainter* painter, const QRectF& location,
                                               const Openstw::SpDr::Aperture* aperture, const qreal onDiameter,
                                               const qreal offDiameter) const
    {
        painter->save();

        // First, determine the colour to use.
        const auto lampColor = this->apertureRenderColor(aperture);

        // Also determine which diameter to use, based on whether the aperture is on
        // or not.
        const auto diameter = this->apertureIsOn(aperture) ? onDiameter : offDiameter;

        // Finally, draw the signal lamp.
        QRectF lampRect{centerWithin(diameter, location.width(), location.left()),
                        centerWithin(diameter, location.height(), location.top()), diameter, diameter};

        painter->setBrush(lampColor);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(lampRect);
        painter->restore();
    }

    QSizeF HpSignalFeatureObject::measureHauptSignal() const
    {
        constexpr qreal signalHeight = 18.0f;

        // Base case - only two lamps. No rangiersignal, no kennlicht.
        qreal signalWidth = (HpSignalFeatureObject::signalLampPadding * 2) +
                            (2 * HpSignalFeatureObject::hpSpaceForLamp) +
                            (HpSignalFeatureObject::hpPaddingBetweenLamps);

        return QSizeF{signalWidth, signalHeight};
    }

    void HpSignalFeatureObject::drawHauptSignal(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRoundedRect(location, 10, 10);

        // Draw signal lamps
        const qreal lampYPos = centerWithin(HpSignalFeatureObject::hpSpaceForLamp, location.height(), location.top());

        qreal currentXPos =
            location.right() - HpSignalFeatureObject::signalLampPadding - HpSignalFeatureObject::hpSpaceForLamp;

        // Green lamp
        this->drawSignalLamp(
            painter,
            QRectF{currentXPos, lampYPos, HpSignalFeatureObject::hpSpaceForLamp, HpSignalFeatureObject::hpSpaceForLamp},
            this->m_greenLamp, HpSignalFeatureObject::activeLampDiameter, HpSignalFeatureObject::inactiveLampDiameter);

        // Red lamp
        currentXPos -= HpSignalFeatureObject::hpPaddingBetweenLamps + HpSignalFeatureObject::hpSpaceForLamp;

        this->drawSignalLamp(
            painter,
            QRectF{currentXPos, lampYPos, HpSignalFeatureObject::hpSpaceForLamp, HpSignalFeatureObject::hpSpaceForLamp},
            this->m_redLamp, HpSignalFeatureObject::activeLampDiameter, HpSignalFeatureObject::inactiveLampDiameter);

        painter->restore();
    }

    void HpSignalFeatureObject::onApertureStateChanged(Openstw::SpDr::ApertureState newState)
    {
        // Invalid feature layer object and redraw
        this->update();
    }
}
