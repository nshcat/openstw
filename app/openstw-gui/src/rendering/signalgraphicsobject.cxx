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

    void SignalGraphicsObject::drawSignalLamp(QPainter* painter, const QRectF& location, const QColor color,
                                              const qreal diameter) const
    {
        painter->save();

        QRectF lampRect{centerWithin(diameter, location.width(), location.left()),
                        centerWithin(diameter, location.height(), location.top()), diameter, diameter};

        painter->setBrush(color);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(lampRect);
        painter->restore();
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
            const auto hasRangierLamp =
                hauptSignalSchirm->supportedSignalBilder().has(Openstw::Simulation::SignalBildType::RangierSignal);

            // We always leave the space for the kennlicht if we have a rangier lamp, even if its not installed
            if (hauptSignalSchirm->hasKennLicht() || hasRangierLamp)
            {
                signalWidth += SignalGraphicsObject::activeLampDiameter + (0.66f * activeLampDiameter);
            }

            if (hasRangierLamp)
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

        // ==== Draw signal lamps
        constexpr qreal spaceForLamp = SignalGraphicsObject::activeLampDiameter;
        constexpr qreal paddingBetweenLamps = 0.66f * spaceForLamp;
        const qreal lampYPos = centerWithin(spaceForLamp, location.height(), location.top());

        qreal currentXPos = location.right() - signalLampPadding - spaceForLamp;

        // == Green lamp
        const auto greenLampOn = (hauptSignalBild == Openstw::Simulation::HauptSignalBild::Hp1);
        const auto greenLampDiameter =
            (greenLampOn ? SignalGraphicsObject::activeLampDiameter : SignalGraphicsObject::inactiveLampDiameter);
        const auto greenLampColor = greenLampOn ? QColor{Qt::green} : SignalGraphicsObject::inactiveLampColor;

        this->drawSignalLamp(painter, QRectF{currentXPos, lampYPos, spaceForLamp, spaceForLamp}, greenLampColor,
                             greenLampDiameter);

        // == Red lamp
        currentXPos -= paddingBetweenLamps + spaceForLamp;

        const auto redLampOn =
            (hauptSignalBild == Openstw::Simulation::HauptSignalBild::Hp0 || (isCompact && kennLichtOn));
        auto redLampColor = SignalGraphicsObject::inactiveLampColor;
        auto redLampDiameter = SignalGraphicsObject::inactiveLampDiameter;
        if (isCompact && kennLichtOn)
        {
            redLampColor = SignalGraphicsObject::kennLichtColor;
            redLampDiameter = SignalGraphicsObject::activeLampDiameter;
        }
        else if (redLampOn)
        {
            redLampColor = Qt::red;
            redLampDiameter = SignalGraphicsObject::activeLampDiameter;
        }

        this->drawSignalLamp(painter, QRectF{currentXPos, lampYPos, spaceForLamp, spaceForLamp}, redLampColor,
                             redLampDiameter);

        // == Kennlicht lamp
        if (!isCompact && hasKennLicht)
        {
            currentXPos -= paddingBetweenLamps + spaceForLamp;

            const auto kennLichtDiameter = (kennLichtOn ? SignalGraphicsObject::activeKennLampDiameter
                                                        : SignalGraphicsObject::inactiveKennLampDiameter);
            const auto kennLichtColor =
                kennLichtOn ? SignalGraphicsObject::kennLichtColor : SignalGraphicsObject::inactiveLampColor;

            this->drawSignalLamp(painter, QRectF{currentXPos, lampYPos, spaceForLamp, spaceForLamp}, kennLichtColor,
                                 kennLichtDiameter);
        }

        // == Rangiersignal diagonal lamp
        const auto hasRangierLamp =
            hauptSignalSchirm->supportedSignalBilder().has(Openstw::Simulation::SignalBildType::RangierSignal);
        if (!isCompact && hasRangierLamp)
        {
            currentXPos -= paddingBetweenLamps + spaceForLamp;

            // If we did not render a kennlicht, we leave that space free.
            if (!hasKennLicht)
                currentXPos -= paddingBetweenLamps + spaceForLamp;

            const auto rangierLampOn =
                (hauptSignalSchirm->rangierSignalBild() == Openstw::Simulation::RangierSignalBild::Sh1);
            const auto rangierLampColor =
                rangierLampOn ? SignalGraphicsObject::kennLichtColor : SignalGraphicsObject::inactiveLampColor;
            const auto rangierLampWidth = rangierLampOn ? 5.0f : 2.0f;

            QRectF rangierLampRect{currentXPos + 2.0f, lampYPos, spaceForLamp, spaceForLamp};

            if (!rangierLampOn)
            {
                rangierLampRect = rangierLampRect.marginsRemoved(QMarginsF{1.0f, 1.0f, 1.0f, 1.0f});
            }

            painter->save();

            painter->setPen(QPen{rangierLampColor, rangierLampWidth, Qt::SolidLine, Qt::FlatCap});
            painter->drawLine(rangierLampRect.topLeft(), rangierLampRect.bottomRight());

            painter->restore();
        }
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
