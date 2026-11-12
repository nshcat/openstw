#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "tilerenderingconstants.hxx"
#include "trackindicatorfeatureobject.hxx"

namespace Rendering
{

    TrackIndicatorFeatureObject::TrackIndicatorFeatureObject(const Openstw::SpDr::TrackIndicatorFeature* feature,
                                                             TileGraphicsObject* parent)
        : TileFeatureGraphicsObject{Openstw::SpDr::FeatureType::TrackIndicator, parent}, m_feature{feature}
    {
        // Retrieve the aperture representing the occupancy indicator, and listen for
        // state changes
        this->m_indicatorAperture = feature->occupancyIndicator();

        // clang-format off
        connect(this->m_indicatorAperture, &Openstw::SpDr::Aperture::stateChanged,
                this, &TrackIndicatorFeatureObject::onIndicatorStateChanged);
        // clang-format on
    }

    QRectF TrackIndicatorFeatureObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void TrackIndicatorFeatureObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void TrackIndicatorFeatureObject::drawBesetztMelder(QPainter* painter, const QRectF& trackRect,
                                                        const Openstw::SpDr::ApertureState& state) const
    {
        painter->save();

        const auto trackIndicatorRect =
            QRectF{centerWithin(TrackIndicatorFeatureObject::trackIndicatorWidth, trackRect.width(), trackRect.left()),
                   centerWithin(TrackIndicatorFeatureObject::trackIndicatorHeight, trackRect.height(), trackRect.top()),
                   TrackIndicatorFeatureObject::trackIndicatorWidth, TrackIndicatorFeatureObject::trackIndicatorHeight};

        const auto trackIndicatorColor = convertApertureColor(state.color());
        painter->setBrush(trackIndicatorColor);
        painter->setPen(rectanglePen(trackIndicatorColor, 1.0f));
        painter->drawRect(adjustRectForBorder(trackIndicatorRect, 1.0f));

        painter->restore();
    }

    void TrackIndicatorFeatureObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        // Base class method call sets up clipping region
        TileFeatureGraphicsObject::paint(painter, option, widget);

        painter->save();

        const auto boundingRect = this->boundingRect();

        const auto trackRect =
            QRectF{boundingRect.left(),
                   centerWithin(TrackIndicatorFeatureObject::trackThickness, boundingRect.height(), boundingRect.top()),
                   boundingRect.width(), TrackIndicatorFeatureObject::trackThickness};

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(trackRect, 1.0f));

        this->drawBesetztMelder(painter, trackRect, this->m_indicatorAperture->state());

        painter->restore();
    }

    void TrackIndicatorFeatureObject::onIndicatorStateChanged(Openstw::SpDr::ApertureState newState)
    {
        // Trigger a redraw.
        this->update();
    }
}
