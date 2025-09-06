#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "tilerenderingconstants.hxx"
#include "trackgraphicsobject.hxx"

namespace Rendering
{

    TrackGraphicsObject::TrackGraphicsObject(TileGraphicsObject* parent) : TileComponentGraphicsObject(parent)
    {
    }

    QRectF TrackGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void TrackGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    void TrackGraphicsObject::drawLabel(QPainter* painter, const QString& label, const bool withAlternativeMelder,
                                        const Openstw::Simulation::TrackState trackState) const
    {
        painter->save();

        const auto boundingRect = this->boundingRect();

        const qreal labelWidth = (boundingRect.width() - 2.0f * TrackGraphicsObject::labelBoxHorzPadding);
        const qreal labelHeight = (boundingRect.height() - 2.0f * TrackGraphicsObject::labelBoxVertPadding);

        const QRectF labelRect{centerWithin(labelWidth, boundingRect.width(), boundingRect.left()),
                               centerWithin(labelHeight, boundingRect.height(), boundingRect.top()), labelWidth,
                               labelHeight};

        drawTextBox(painter, labelRect, label, Qt::white, Qt::transparent, 0.0f, Qt::black);

        if (withAlternativeMelder)
        {
            const QRectF trackRect{labelRect.left(), boundingRect.bottom() - TrackGraphicsObject::trackThickness,
                                   labelRect.width(), TrackGraphicsObject::trackThickness};

            painter->setBrush(Qt::black);
            painter->setPen(rectanglePen(Qt::black, 1.0f));
            painter->drawRect(adjustRectForBorder(trackRect, 1.0f));

            this->drawBesetztMelder(painter, trackRect, trackState);
        }

        painter->restore();
    }

    void TrackGraphicsObject::drawBesetztMelder(QPainter* painter, const QRectF& trackRect,
                                                const Openstw::Simulation::TrackState trackState) const
    {
        painter->save();

        const auto trackIndicatorRect =
            QRectF{centerWithin(TrackGraphicsObject::trackIndicatorWidth, trackRect.width(), trackRect.left()),
                   centerWithin(TrackGraphicsObject::trackIndicatorHeight, trackRect.height(), trackRect.top()),
                   TrackGraphicsObject::trackIndicatorWidth, TrackGraphicsObject::trackIndicatorHeight};

        const auto trackIndicatorColor = TrackGraphicsObject::colorForTrackState(trackState);
        painter->setBrush(trackIndicatorColor);
        painter->setPen(rectanglePen(trackIndicatorColor, 1.0f));
        painter->drawRect(adjustRectForBorder(trackIndicatorRect, 1.0f));

        painter->restore();
    }

    QColor TrackGraphicsObject::colorForTrackState(const Openstw::Simulation::TrackState trackState)
    {
        switch (trackState)
        {
        case Openstw::Simulation::TrackState::Inactive:
            return QColor{0x4b, 0x4b, 0x4b};
            break;
        case Openstw::Simulation::TrackState::FahrstrasseSet:
            return QColor{0xed, 0xff, 0x9f};
            break;
        case Openstw::Simulation::TrackState::Occupied:
            return QColor{0xff, 0x00, 0x00};
            break;
        default:
            return QColor{0x4b, 0x4b, 0x4b};
            break;
        }
    }

    void TrackGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasTrack())
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto& track = this->m_tile->track();

        const auto boundingRect = this->boundingRect();

        const auto trackRect =
            QRectF{boundingRect.left(),
                   centerWithin(TrackGraphicsObject::trackThickness, boundingRect.height(), boundingRect.top()),
                   boundingRect.width(), TrackGraphicsObject::trackThickness};

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(trackRect, 1.0f));

        if (track.hasLeuchtMelder())
        {
            this->drawBesetztMelder(painter, trackRect, track.state());
        }

        if (track.hasLabel())
        {
            this->drawLabel(painter, track.label(), (track.hasLeuchtMelder() && track.showAlternativeLeuchtMelder()),
                            track.state());
        }
    }
}
