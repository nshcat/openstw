#include "signalgraphicsobject.hxx"
#include "../tilegraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "tilerenderingconstants.hxx"
#include "trackgraphicsobject.hxx"
#include <QPainter>
#include <hauptsignalschirm.hxx>
#include <vorsignalschirm.hxx>

namespace Rendering
{
    SignalGraphicsObject::SignalGraphicsObject(TileGraphicsObject* parent,
                                               const Openstw::Simulation::TileElementDirection direction)
        : TileComponentGraphicsObject(parent), m_direction{direction}
    {
    }

    QRectF SignalGraphicsObject::boundingRect() const
    {
        const auto tileBoundingRect = this->tileGraphicsObject()->innerBoundingRect();

        constexpr auto signalBoundingRectHeight =
            TileRenderingConstants::halfTileHeight - (TrackGraphicsObject::trackThickness / 2.0f);

        return QRectF{tileBoundingRect.left(), tileBoundingRect.top(), tileBoundingRect.width(),
                      signalBoundingRectHeight};
    }

    void SignalGraphicsObject::positionSelf()
    {
        const auto tileInnerRect = this->tileGraphicsObject()->innerBoundingRect();

        if (this->m_direction == Openstw::Simulation::TileElementDirection::Backward)
        {
            this->setPos(tileInnerRect.topLeft());
        }
        else
        {
            this->setPos(tileInnerRect.left(), tileInnerRect.bottom() - this->boundingRect().height());
        }
    }

    QSizeF SignalGraphicsObject::measureHauptSignal(HauptSignalRenderingStyle style,
                                                    const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm)
    {
        constexpr qreal signalHeight = 18.0f;

        // Base case - only two lamps. No rangiersignal, no kennlicht.
        qreal signalWidth = (SignalGraphicsObject::signalLampPadding * 2) +
                            (2 * SignalGraphicsObject::activeLampDiameter) + (0.66f * activeLampDiameter);

        // If in compact style, thats it - we will never render more than two lamps
        // (Rangiersignalbild is not supported here XXX does it need to be?)
        if (style == HauptSignalRenderingStyle::Normal)
        {
            // 20 pixels extra for each extra lamp..
            if (hauptSignalSchirm->hasKennLicht())
            {
                signalWidth += SignalGraphicsObject::activeLampDiameter + (0.66f * activeLampDiameter);
            }

            if (hauptSignalSchirm->supportedSignalBilder().has(Openstw::Simulation::SignalBildType::RangierSignal))
            {
                signalWidth += SignalGraphicsObject::activeLampDiameter + (0.66f * activeLampDiameter);
            }
        }

        return QSizeF{signalWidth, signalHeight};
    }

    void SignalGraphicsObject::drawHauptSignal(QPainter* painter, HauptSignalRenderingStyle style,
                                               const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm,
                                               const QRectF& location) const
    {
        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRoundedRect(location, 10, 10);

        // Retrieve currently shown Hauptsignalbild
        const auto isCompact = (style == HauptSignalRenderingStyle::Compact);
        const auto hauptSignalBild = hauptSignalSchirm->hauptSignalBild();
        const auto hasKennLicht = hauptSignalSchirm->hasKennLicht();
        const auto kennLichtOn =
            hasKennLicht && (hauptSignalSchirm->kennLichtState() == Openstw::Simulation::KennLichtState::On);

        // == Green lamp
        const auto greenLampOn = (hauptSignalBild == Openstw::Simulation::HauptSignalBild::Hp1);
        const auto greenLampDiameter =
            (greenLampOn ? SignalGraphicsObject::activeLampDiameter : SignalGraphicsObject::inactiveLampDiameter);

        painter->setBrush(greenLampOn ? Qt::green : SignalGraphicsObject::inactiveLampColor);
        painter->setPen(Qt::NoPen);

        qreal xPosRelGreen = SignalGraphicsObject::activeLampDiameter + signalLampPadding;
        if (!greenLampOn)
            xPosRelGreen -=
                (SignalGraphicsObject::activeLampDiameter - SignalGraphicsObject::inactiveLampDiameter) / 2.0f;

        painter->drawEllipse(QRectF{location.right() - xPosRelGreen,
                                    centerWithin(greenLampDiameter, location.height(), location.top()),
                                    greenLampDiameter, greenLampDiameter});

        // == Red lamp / Kennlicht (if in compact mode)
        const auto redLampOn =
            (hauptSignalBild == Openstw::Simulation::HauptSignalBild::Hp0 || (isCompact && kennLichtOn));

        auto redLampColor = SignalGraphicsObject::inactiveLampColor;
        auto redLampDiameter = SignalGraphicsObject::inactiveLampDiameter;
        auto redLampActive = false;
        if (isCompact && kennLichtOn)
        {
            redLampColor = SignalGraphicsObject::kennLichtColor;
            redLampDiameter = SignalGraphicsObject::activeLampDiameter;
            redLampActive = true;
        }
        else if (redLampOn)
        {
            redLampColor = Qt::red;
            redLampDiameter = SignalGraphicsObject::activeLampDiameter;
            redLampActive = true;
        }

        painter->setBrush(redLampColor);
        painter->setPen(Qt::NoPen);

        qreal xPosRelRed = SignalGraphicsObject::activeLampDiameter + signalLampPadding;
        if (!redLampActive)
            xPosRelRed -=
                (SignalGraphicsObject::activeLampDiameter - SignalGraphicsObject::inactiveLampDiameter) / 2.0f;

        painter->drawEllipse(QRectF{location.right() - (1.66f * SignalGraphicsObject::activeLampDiameter) - xPosRelRed,
                                    centerWithin(redLampDiameter, location.height(), location.top()), redLampDiameter,
                                    redLampDiameter});
    }

    void SignalGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasSignal(this->m_direction))
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();

        const auto& signal = this->m_tile->signal(this->m_direction);
        const auto* primarySchirm = signal.primarySignalSchirm();

        switch (primarySchirm->type())
        {
        case Openstw::Simulation::SignalSchirmType::HauptSignal:
            {
                // Cast down pointer
                const auto* hauptSignalSchirm =
                    dynamic_cast<const Openstw::Simulation::HauptSignalSchirm*>(primarySchirm);

                if (!hauptSignalSchirm)
                    throw std::runtime_error("ISignalSchirm was unexpectedly not HauptSignalSchirm");

                // If we need to display a secondary Signalschirm, we use the compact
                // rendering style for the Hauptsignal.
                const auto renderStyle = (signal.hasSecondarySignalSchirm() ? HauptSignalRenderingStyle::Compact
                                                                            : HauptSignalRenderingStyle::Normal);

                // Measure out how big the Signalschirm would be if rendered.
                // The returned rect always has position 0,0.
                const auto hauptSchirmSize = this->measureHauptSignal(renderStyle, hauptSignalSchirm);

                // Lay it out. The top end of the topmost Signalschirm is always fixed to be the same distance
                // to the tile border.
                QRectF hauptSchirmRect{
                    boundingRect.right() - hauptSchirmSize.width() - SignalGraphicsObject::signalToBorderPadding,
                    centerWithin(hauptSchirmSize.height(), boundingRect.height(), boundingRect.top()),
                    hauptSchirmSize.width(), hauptSchirmSize.height()};

                // Render it
                this->drawHauptSignal(painter, renderStyle, hauptSignalSchirm, hauptSchirmRect);

                break;
            }

        case Openstw::Simulation::SignalSchirmType::VorSignal:
            {
                break;
            }

        default:
            break;
        }

        /*painter->setBrush(Qt::transparent);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(boundingRect, 1.0f));*/
    }
}
