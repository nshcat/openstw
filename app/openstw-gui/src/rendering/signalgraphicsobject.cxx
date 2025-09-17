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
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void SignalGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    QRectF SignalGraphicsObject::calculateAreaRect(VerticalDirection location) const
    {
        return this->tileGraphicsObject()->horizontalDrawingAreaRect(location);
    }

    QRectF SignalGraphicsObject::calculateSignalArea() const
    {
        if (this->m_direction == Openstw::Simulation::TileElementDirection::Backward)
        {
            return this->calculateAreaRect(VerticalDirection::Top);
        }
        else
        {
            return this->calculateAreaRect(VerticalDirection::Bottom);
        }
    }

    QRectF SignalGraphicsObject::calculateMelderArea() const
    {
        if (this->m_direction == Openstw::Simulation::TileElementDirection::Backward)
        {
            return this->calculateAreaRect(VerticalDirection::Bottom);
        }
        else
        {
            return this->calculateAreaRect(VerticalDirection::Top);
        }
    }

    void SignalGraphicsObject::drawSignalArea(QPainter* painter, const QRectF& location, const bool rotated) const
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

        const auto& signal = this->m_tile->signal(this->m_direction);
        const auto* primarySchirm = signal.primarySignalSchirm();

        switch (primarySchirm->type())
        {
        case Openstw::Simulation::SignalSchirmType::ZugDeckungsSignal:
            {
                // Cast down pointer
                const auto* zugDeckungsSignalSchirm =
                    dynamic_cast<const Openstw::Simulation::ZugDeckungsSignalSchirm*>(primarySchirm);

                if (!zugDeckungsSignalSchirm)
                    throw std::runtime_error("ISignalSchirm was unexpectedly not ZugDeckungsSignalSchirm");

                const auto zugDeckungsSignalSchirmSize = this->measureZugDeckungsSignal(zugDeckungsSignalSchirm);

                const QRectF zugDeckungsSignalSchirmRect{
                    location.right() - zugDeckungsSignalSchirmSize.width() -
                        SignalGraphicsObject::signalToBorderPadding,
                    centerWithin(zugDeckungsSignalSchirmSize.height(), location.height(), location.top()),
                    zugDeckungsSignalSchirmSize.width(), zugDeckungsSignalSchirmSize.height()};

                this->drawZugDeckungsSignal(painter, zugDeckungsSignalSchirm, zugDeckungsSignalSchirmRect);

                break;
            }
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
                QRectF hauptSchirmRect{location.right() - hauptSchirmSize.width() -
                                           SignalGraphicsObject::signalToBorderPadding,
                                       centerWithin(hauptSchirmSize.height(), location.height(), location.top()),
                                       hauptSchirmSize.width(), hauptSchirmSize.height()};

                // Render it
                this->drawHauptSignal(painter, renderStyle, hauptSignalSchirm, hauptSchirmRect);

                // We need this later to draw the block signal connection.
                QRectF mastBaseRect;

                // Also render the vorsignal attached to it if needed
                if (signal.hasSecondarySignalSchirm())
                {
                    const auto* vorSignalSchirm = signal.secondarySignalSchirm();

                    // Measure out Vorsignalschirm
                    const auto vorSignalSchirmSize = this->measureVorSignal(vorSignalSchirm);

                    // Lay it out. Its right edge is close to the left edge of the Hauptsignalschirm
                    QRectF vorSignalSchirmRect{
                        hauptSchirmRect.left() - vorSignalSchirmSize.width() - SignalGraphicsObject::hpVrPadding,
                        centerWithin(vorSignalSchirmSize.height(), location.height(), location.top()),
                        vorSignalSchirmSize.width(), vorSignalSchirmSize.height()};

                    // Draw mast segment between Vorsignal and Hauptsignal
                    const auto mastSegmentRect =
                        QRectF{vorSignalSchirmRect.right() - SignalGraphicsObject::mastBaseWidth,
                               centerWithin(SignalGraphicsObject::mastBaseHeight, vorSignalSchirmRect.height(),
                                            vorSignalSchirmRect.top()),
                               SignalGraphicsObject::mastBaseWidth *
                                   2.0, //< In order to extend under the diagonal of the Vorsignalschirm
                               SignalGraphicsObject::mastBaseHeight};

                    this->drawMastSegment(painter, mastSegmentRect);

                    // Draw mast base. This depends on whether we have a Sperrmelder or not.
                    if (signal.hasSperrMelder())
                    {
                        const auto mastBaseXPos = location.left() + SignalGraphicsObject::sperrMelderDiameter +
                                                  SignalGraphicsObject::sperrMelderToBorderPadding +
                                                  SignalGraphicsObject::sperrMelderMastBasePadding;

                        mastBaseRect = QRectF{mastBaseXPos,
                                              centerWithin(SignalGraphicsObject::mastBaseHeight,
                                                           vorSignalSchirmRect.height(), vorSignalSchirmRect.top()),
                                              (vorSignalSchirmRect.left() - mastBaseXPos) + 15.0f,
                                              SignalGraphicsObject::mastBaseHeight};
                    }
                    else
                    {
                        mastBaseRect = QRectF{vorSignalSchirmRect.left() - SignalGraphicsObject::mastBaseWidth,
                                              centerWithin(SignalGraphicsObject::mastBaseHeight,
                                                           vorSignalSchirmRect.height(), vorSignalSchirmRect.top()),
                                              SignalGraphicsObject::mastBaseWidth *
                                                  2.0, //< In order to extend under the diagonal of the Vorsignalschirm
                                              SignalGraphicsObject::mastBaseHeight};
                    }

                    // Render the mast base
                    this->drawMastBase(painter, mastBaseRect);

                    // Render the Vorsignalschirm
                    this->drawVorSignal(painter, vorSignalSchirm, vorSignalSchirmRect);
                }
                else
                {
                    // Signal either has a mast base or a connector to the next tile - if it is part
                    // of a multi-tile signal
                    if (signal.hasConnector())
                    {
                        const QRectF mastSegmentRect{location.left(), hauptSchirmRect.top(),
                                                     (hauptSchirmRect.left() - location.left()),
                                                     hauptSchirmRect.height()};

                        this->drawMastSegment(painter, mastSegmentRect);
                    }
                    else
                    {
                        if (signal.hasSperrMelder())
                        {
                            const auto mastBaseXPos = location.left() + SignalGraphicsObject::sperrMelderDiameter +
                                                      SignalGraphicsObject::sperrMelderToBorderPadding +
                                                      SignalGraphicsObject::sperrMelderMastBasePadding;

                            mastBaseRect =
                                QRectF{mastBaseXPos,
                                       centerWithin(SignalGraphicsObject::mastBaseHeight, hauptSchirmRect.height(),
                                                    hauptSchirmRect.top()),
                                       (hauptSchirmRect.left() - mastBaseXPos), SignalGraphicsObject::mastBaseHeight};
                        }
                        else
                        {
                            mastBaseRect =
                                QRectF{hauptSchirmRect.left() - SignalGraphicsObject::mastBaseWidth,
                                       centerWithin(SignalGraphicsObject::mastBaseHeight, hauptSchirmRect.height(),
                                                    hauptSchirmRect.top()),
                                       SignalGraphicsObject::mastBaseWidth, SignalGraphicsObject::mastBaseHeight};
                        }

                        this->drawMastBase(painter, mastBaseRect);
                    }
                }

                if (signal.hasSperrMelder())
                {
                    const auto sperrMelderState = signal.sperrMelderState();

                    const QRectF sperrMelderRect{location.left() + SignalGraphicsObject::sperrMelderToBorderPadding,
                                                 centerWithin(SignalGraphicsObject::sperrMelderDiameter,
                                                              hauptSchirmRect.height(), hauptSchirmRect.top()),
                                                 SignalGraphicsObject::sperrMelderDiameter,
                                                 SignalGraphicsObject::sperrMelderDiameter};

                    this->drawSperrMelder(painter, sperrMelderRect, sperrMelderState);
                }

                // Blocksignale have a line connecting them to the tracks.
                if (signal.isBlockSignal() && !signal.hasConnector())
                {
                    painter->save();

                    const QRectF blockSignalConnectorRect{mastBaseRect.left(), location.top(),
                                                          SignalGraphicsObject::mastThickness,
                                                          (mastBaseRect.bottom() - location.top())};

                    painter->setPen(rectanglePen(Qt::black, 1.0f));
                    painter->setBrush(Qt::black);
                    painter->drawRect(adjustRectForBorder(blockSignalConnectorRect, 1.0f));

                    painter->restore();
                }

                break;
            }

        case Openstw::Simulation::SignalSchirmType::VorSignal:
            {
                // Cast down pointer
                const auto* vorSignalSchirm = dynamic_cast<const Openstw::Simulation::VorSignalSchirm*>(primarySchirm);

                if (!vorSignalSchirm)
                    throw std::runtime_error("ISignalSchirm was unexpectedly not VorSignalSchirm");

                const auto vorSignalSchirmSize = this->measureVorSignal(vorSignalSchirm);

                // We have two possible cases here - if we are part of a multi-tile signal, we want to render
                // at the same distance to tile border as if we were a Hauptsignalschirm.
                // If we are just an individual vorsignal, center us more within the tile.
                if (signal.hasConnector())
                {
                    // Lay it out. The top end of the topmost Signalschirm is always fixed to be the same distance
                    // to the tile border.
                    const QRectF vorSignalSchirmRect{
                        location.right() - vorSignalSchirmSize.width() - SignalGraphicsObject::signalToBorderPadding,
                        centerWithin(vorSignalSchirmSize.height(), location.height(), location.top()),
                        vorSignalSchirmSize.width(), vorSignalSchirmSize.height()};

                    // We have to render the mast base first
                    const QRectF mastBaseRect{vorSignalSchirmRect.left() - SignalGraphicsObject::mastBaseWidth,
                                              centerWithin(SignalGraphicsObject::mastBaseHeight,
                                                           vorSignalSchirmRect.height(), vorSignalSchirmRect.top()),
                                              SignalGraphicsObject::mastBaseWidth *
                                                  2.0, //< In order to extend under the diagonal of the Vorsignalschirm
                                              SignalGraphicsObject::mastBaseHeight};

                    this->drawMastBase(painter, mastBaseRect);

                    // And then the connector
                    const qreal mastSegmentStartX = vorSignalSchirmRect.right() - SignalGraphicsObject::mastBaseWidth;

                    const QRectF mastSegmentRect{mastSegmentStartX, vorSignalSchirmRect.top(),
                                                 (location.right() - mastSegmentStartX), vorSignalSchirmRect.height()};

                    this->drawMastSegment(painter, mastSegmentRect);

                    // Finally, draw the Vorsignalschirm
                    this->drawVorSignal(painter, vorSignalSchirm, vorSignalSchirmRect);
                }
                else
                {
                    // Lay it out so its centered horizontally in the tile
                    const QRectF vorSignalSchirmRect{
                        centerWithin(vorSignalSchirmSize.width(), location.width(), location.left()),
                        centerWithin(vorSignalSchirmSize.height(), location.height(), location.top()),
                        vorSignalSchirmSize.width(), vorSignalSchirmSize.height()};

                    // We have to render the mast base first
                    const QRectF mastBaseRect{vorSignalSchirmRect.left() - SignalGraphicsObject::mastBaseWidth,
                                              centerWithin(SignalGraphicsObject::mastBaseHeight,
                                                           vorSignalSchirmRect.height(), vorSignalSchirmRect.top()),
                                              SignalGraphicsObject::mastBaseWidth *
                                                  2.0, //< In order to extend under the diagonal of the Vorsignalschirm
                                              SignalGraphicsObject::mastBaseHeight};

                    this->drawMastBase(painter, mastBaseRect);

                    // Finally, render the Vorsignalschirm
                    this->drawVorSignal(painter, vorSignalSchirm, vorSignalSchirmRect);
                }

                break;
            }

        default:
            break;
        }

        painter->restore();
    }

    void SignalGraphicsObject::drawMelderArea(QPainter* painter, const QRectF& location, const bool rotated) const
    {
        painter->save();

        const auto& signal = this->m_tile->signal(this->m_direction);

        bool hasOnlyLabel = !this->m_tile->hasFeststellMelder();

        if (signal.hasDWegMelder())
        {
            hasOnlyLabel = false;

            const QRectF dwegMelderRect{
                location.left() + SignalGraphicsObject::dwegMelderDiameter,
                centerWithin(SignalGraphicsObject::dwegMelderDiameter, location.height(), location.top()),
                SignalGraphicsObject::dwegMelderDiameter, SignalGraphicsObject::dwegMelderDiameter};

            this->drawDWegMelder(painter, dwegMelderRect, signal.dWegMelderState());
        }

        if (signal.hasName())
        {
            QRectF labelRect{};

            if (hasOnlyLabel)
            {
                const qreal xPos = centerWithin(SignalGraphicsObject::labelWidth, location.width(), location.left());

                labelRect =
                    QRectF{xPos, centerWithin(SignalGraphicsObject::labelHeight, location.height(), location.top()),
                           SignalGraphicsObject::labelWidth, SignalGraphicsObject::labelHeight};
            }
            else
            {
                // The location of the label depends on where the Feststellmelder is
                bool fsmIsCentered = false;
                if (this->m_tile->hasFeststellMelder() && this->m_tile->feststellMelder().direction() ==
                                                              Openstw::Simulation::TileElementDirection::Bidirectional)
                {
                    fsmIsCentered = true;
                }

                const qreal xPadding = fsmIsCentered ? SignalGraphicsObject::labelPaddingCenteredFSM
                                                     : SignalGraphicsObject::labelPaddingAlignedFSM;
                const qreal xPos = rotated ? location.left() + xPadding
                                           : location.right() - xPadding - SignalGraphicsObject::labelWidth;

                labelRect =
                    QRectF{xPos, centerWithin(SignalGraphicsObject::labelHeight, location.height(), location.top()),
                           SignalGraphicsObject::labelWidth, SignalGraphicsObject::labelHeight};
            }

            this->drawLabel(painter, labelRect, signal.name());
        }

        painter->restore();
    }

    void SignalGraphicsObject::drawSperrMelder(QPainter* painter, const QRectF& location,
                                               const Openstw::Simulation::SperrMelderState state) const
    {
        painter->save();

        const auto sperrMelderInnerColor = (state == Openstw::Simulation::SperrMelderState::Off)
                                               ? SignalGraphicsObject::sperrMelderInactiveColor
                                               : SignalGraphicsObject::sperrMelderActiveColor;

        painter->setPen(QPen{Qt::black, 0.5f});
        painter->setBrush(sperrMelderInnerColor);

        const QRectF sperrMelderRect{
            centerWithin(SignalGraphicsObject::sperrMelderDiameter, location.width(), location.left()),
            centerWithin(SignalGraphicsObject::sperrMelderDiameter, location.height(), location.top()),
            SignalGraphicsObject::sperrMelderDiameter, SignalGraphicsObject::sperrMelderDiameter};

        painter->drawEllipse(sperrMelderRect);

        painter->restore();
    }

    void SignalGraphicsObject::drawDWegMelder(QPainter* painter, const QRectF& location,
                                              const Openstw::Simulation::StaticLampState state) const
    {
        painter->save();

        const auto dwegMelderInnerColor = (state == Openstw::Simulation::StaticLampState::Off)
                                              ? SignalGraphicsObject::dwegMelderInactiveColor
                                              : SignalGraphicsObject::dwegMelderActiveColor;

        painter->setPen(QPen{Qt::black, 0.5f});
        painter->setBrush(dwegMelderInnerColor);

        const QRectF dwegMelderRect{
            centerWithin(SignalGraphicsObject::dwegMelderDiameter, location.width(), location.left()),
            centerWithin(SignalGraphicsObject::dwegMelderDiameter, location.height(), location.top()),
            SignalGraphicsObject::dwegMelderDiameter, SignalGraphicsObject::dwegMelderDiameter};

        painter->drawEllipse(dwegMelderRect);

        painter->restore();
    }

    void SignalGraphicsObject::drawLabel(QPainter* painter, const QRectF& location, const QString& label) const
    {
        painter->save();

        drawTextBox(painter, location, label, Qt::white, Qt::transparent, 0.0f, Qt::black);

        painter->restore();
    }

    void SignalGraphicsObject::drawMastBase(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        const QRectF mastBaseRect{location.left(), location.top(), SignalGraphicsObject::mastThickness,
                                  location.height()};

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(mastBaseRect, 1.0f));

        this->drawMastSegment(painter, location);

        painter->restore();
    }

    void SignalGraphicsObject::drawMastSegment(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        const QRectF mastRect{location.left(),
                              centerWithin(SignalGraphicsObject::mastThickness, location.height(), location.top()),
                              location.width(), SignalGraphicsObject::mastThickness};

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRect(adjustRectForBorder(mastRect, 1.0f));

        painter->restore();
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

    QSizeF SignalGraphicsObject::measureHauptSignal(
        HauptSignalRenderingStyle style, const Openstw::Simulation::HauptSignalSchirm* hauptSignalSchirm) const
    {
        constexpr qreal signalHeight = 18.0f;

        // Base case - only two lamps. No rangiersignal, no kennlicht.
        qreal signalWidth = (SignalGraphicsObject::signalLampPadding * 2) + (2 * SignalGraphicsObject::hpSpaceForLamp) +
                            (SignalGraphicsObject::hpPaddingBetweenLamps);

        // If in compact style, thats it - we will never render more than two lamps
        // (Rangiersignalbild is not supported here XXX does it need to be?)
        if (style == HauptSignalRenderingStyle::Normal)
        {
            const auto hasRangierLamp =
                hauptSignalSchirm->supportedSignalBilder().has(Openstw::Simulation::SignalBildType::RangierSignal);

            // We always leave the space for the kennlicht if we have a rangier lamp, even if its not installed
            if (hauptSignalSchirm->hasKennLicht() || hasRangierLamp)
            {
                signalWidth += SignalGraphicsObject::hpSpaceForLamp + SignalGraphicsObject::hpPaddingBetweenLamps;
            }

            if (hasRangierLamp)
            {
                signalWidth += SignalGraphicsObject::hpSpaceForLamp + SignalGraphicsObject::hpPaddingBetweenLamps;
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
        const qreal lampYPos = centerWithin(SignalGraphicsObject::hpSpaceForLamp, location.height(), location.top());

        qreal currentXPos =
            location.right() - SignalGraphicsObject::signalLampPadding - SignalGraphicsObject::hpSpaceForLamp;

        // == Green lamp
        const auto greenLampOn = (hauptSignalBild == Openstw::Simulation::HauptSignalBild::Hp1);
        const auto greenLampDiameter =
            (greenLampOn ? SignalGraphicsObject::activeLampDiameter : SignalGraphicsObject::inactiveLampDiameter);
        const auto greenLampColor = greenLampOn ? QColor{Qt::green} : SignalGraphicsObject::inactiveLampColor;

        this->drawSignalLamp(
            painter,
            QRectF{currentXPos, lampYPos, SignalGraphicsObject::hpSpaceForLamp, SignalGraphicsObject::hpSpaceForLamp},
            greenLampColor, greenLampDiameter);

        // == Red lamp
        currentXPos -= SignalGraphicsObject::hpPaddingBetweenLamps + SignalGraphicsObject::hpSpaceForLamp;

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

        this->drawSignalLamp(
            painter,
            QRectF{currentXPos, lampYPos, SignalGraphicsObject::hpSpaceForLamp, SignalGraphicsObject::hpSpaceForLamp},
            redLampColor, redLampDiameter);

        // == Kennlicht lamp
        if (!isCompact && hasKennLicht)
        {
            currentXPos -= SignalGraphicsObject::hpPaddingBetweenLamps + SignalGraphicsObject::hpSpaceForLamp;

            const auto kennLichtDiameter = (kennLichtOn ? SignalGraphicsObject::activeKennLampDiameter
                                                        : SignalGraphicsObject::inactiveKennLampDiameter);
            const auto kennLichtColor =
                kennLichtOn ? SignalGraphicsObject::kennLichtColor : SignalGraphicsObject::inactiveLampColor;

            this->drawSignalLamp(painter,
                                 QRectF{currentXPos, lampYPos, SignalGraphicsObject::hpSpaceForLamp,
                                        SignalGraphicsObject::hpSpaceForLamp},
                                 kennLichtColor, kennLichtDiameter);
        }

        // == Rangiersignal diagonal lamp
        const auto hasRangierLamp =
            hauptSignalSchirm->supportedSignalBilder().has(Openstw::Simulation::SignalBildType::RangierSignal);
        if (!isCompact && hasRangierLamp)
        {
            currentXPos -= SignalGraphicsObject::hpPaddingBetweenLamps + SignalGraphicsObject::hpSpaceForLamp;

            // If we did not render a kennlicht, we leave that space free.
            if (!hasKennLicht)
                currentXPos -= SignalGraphicsObject::hpPaddingBetweenLamps + SignalGraphicsObject::hpSpaceForLamp;

            const auto rangierLampOn =
                (hauptSignalSchirm->rangierSignalBild() == Openstw::Simulation::RangierSignalBild::Sh1);
            const auto rangierLampColor =
                rangierLampOn ? SignalGraphicsObject::kennLichtColor : SignalGraphicsObject::inactiveLampColor;
            const auto rangierLampWidth = rangierLampOn ? 5.0f : 2.0f;

            QRectF rangierLampRect{currentXPos + 2.0f, lampYPos, SignalGraphicsObject::hpSpaceForLamp,
                                   SignalGraphicsObject::hpSpaceForLamp};

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

    QSizeF SignalGraphicsObject::measureVorSignal(const Openstw::Simulation::VorSignalSchirm* vorSignalSchirm) const
    {
        constexpr qreal vorSignalHeight = 18.0;

        qreal vorSignalWidth = 48.0f;

        if (vorSignalSchirm->hasKennLicht())
        {
            vorSignalWidth += SignalGraphicsObject::vrExtraSpaceForKennLicht;
        }

        return QSizeF{vorSignalWidth, vorSignalHeight};
    }

    void SignalGraphicsObject::drawVorSignal(QPainter* painter,
                                             const Openstw::Simulation::VorSignalSchirm* vorSignalSchirm,
                                             const QRectF& location) const
    {
        painter->save();

        // == Schirm
        const qreal vorSignalShiftX =
            (vorSignalSchirm->hasKennLicht() ? location.width() - SignalGraphicsObject::vrExtraSpaceForKennLicht
                                             : location.width()) *
            0.25f;
        const qreal vorSignalShiftY = location.height() * 0.33f;

        QPolygonF vorSignalPolygon{};
        vorSignalPolygon << QPointF{location.left(), location.top()}
                         << QPointF{location.left() + location.width() - vorSignalShiftX, location.top()}
                         << QPointF{location.left() + location.width(),
                                    location.top() + location.height() - vorSignalShiftY}
                         << QPointF{location.left() + location.width(), location.top() + location.height()}
                         << QPointF{location.left() + vorSignalShiftX, location.top() + location.height()}
                         << QPointF{location.left(), location.top() + vorSignalShiftY}
                         << QPointF{location.left(), location.top()};

        painter->setPen(Qt::black);
        painter->setBrush(Qt::black);
        painter->drawPolygon(vorSignalPolygon);

        // == Main lamps
        const auto vorSignalBild = vorSignalSchirm->vorSignalBild();

        QColor lowerLampColor = SignalGraphicsObject::inactiveLampColor;
        qreal lowerLampDiameter = SignalGraphicsObject::vrInactiveLampDiameter;
        QColor upperLampColor = SignalGraphicsObject::inactiveLampColor;
        qreal upperLampDiameter = SignalGraphicsObject::vrInactiveLampDiameter;

        if (vorSignalBild == Openstw::Simulation::VorSignalBild::Vr0)
        {
            lowerLampColor = Qt::yellow;
            lowerLampDiameter = SignalGraphicsObject::vrActiveLampDiameter;
        }
        else if (vorSignalBild == Openstw::Simulation::VorSignalBild::Vr1)
        {
            upperLampColor = Qt::green;
            upperLampDiameter = SignalGraphicsObject::vrActiveLampDiameter;
        }

        // = Lower lamps
        const qreal lowerLampsXPos =
            (vorSignalSchirm->hasKennLicht() ? location.left() + SignalGraphicsObject::vrExtraSpaceForKennLicht
                                             : location.left()) +
            SignalGraphicsObject::vrLampPaddingX;

        this->drawSignalLamp(painter,
                             QRectF{lowerLampsXPos, location.top() + SignalGraphicsObject::vrLampPaddingY,
                                    SignalGraphicsObject::vrSpaceForLamp, SignalGraphicsObject::vrSpaceForLamp},
                             lowerLampColor, lowerLampDiameter);

        this->drawSignalLamp(
            painter,
            QRectF{lowerLampsXPos + vorSignalShiftX - 2.5,
                   location.bottom() - SignalGraphicsObject::vrLampPaddingY - SignalGraphicsObject::vrSpaceForLamp,
                   SignalGraphicsObject::vrSpaceForLamp, SignalGraphicsObject::vrSpaceForLamp},
            lowerLampColor, lowerLampDiameter);

        // = Upper lamps
        const qreal upperLampsXPos =
            location.right() - SignalGraphicsObject::vrLampPaddingX - SignalGraphicsObject::vrSpaceForLamp;

        this->drawSignalLamp(painter,
                             QRectF{upperLampsXPos - vorSignalShiftX + 2.5,
                                    location.top() + SignalGraphicsObject::vrLampPaddingY,
                                    SignalGraphicsObject::vrSpaceForLamp, SignalGraphicsObject::vrSpaceForLamp},
                             upperLampColor, upperLampDiameter);

        this->drawSignalLamp(
            painter,
            QRectF{upperLampsXPos,
                   location.bottom() - SignalGraphicsObject::vrLampPaddingY - SignalGraphicsObject::vrSpaceForLamp,
                   SignalGraphicsObject::vrSpaceForLamp, SignalGraphicsObject::vrSpaceForLamp},
            upperLampColor, upperLampDiameter);

        // Kennlicht lamp
        if (vorSignalSchirm->hasKennLicht())
        {
            QColor kennLampColor = SignalGraphicsObject::inactiveLampColor;
            qreal kennLampDiameter = SignalGraphicsObject::inactiveKennLampDiameter;

            if (vorSignalSchirm->kennLichtState() == Openstw::Simulation::KennLichtState::On)
            {
                kennLampColor = SignalGraphicsObject::kennLichtColor;
                kennLampDiameter = SignalGraphicsObject::activeKennLampDiameter;
            }

            const QRectF extraKennLichtSpace{location.left(), location.top(),
                                             SignalGraphicsObject::vrExtraSpaceForKennLicht + 6, location.height()};

            this->drawSignalLamp(painter, extraKennLichtSpace, kennLampColor, kennLampDiameter);
        }

        painter->restore();
    }

    QSizeF SignalGraphicsObject::measureZugDeckungsSignal(
        const Openstw::Simulation::ZugDeckungsSignalSchirm* zugDeckungsSignalSchirm) const
    {
        constexpr qreal signalHeight = 18.0f;

        qreal signalWidth = (SignalGraphicsObject::signalLampPadding * 2) + (2 * SignalGraphicsObject::hpSpaceForLamp) +
                            (SignalGraphicsObject::hpPaddingBetweenLamps);

        return QSizeF{signalWidth, signalHeight};
    }

    void SignalGraphicsObject::drawZugDeckungsSignal(
        QPainter* painter, const Openstw::Simulation::ZugDeckungsSignalSchirm* zugDeckungsSignalSchirm,
        const QRectF& location) const
    {
        painter->save();

        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->setBrush(Qt::black);
        painter->drawRect(adjustRectForBorder(location, 1.0f));

        const auto signalBild = zugDeckungsSignalSchirm->zugDeckungsSignalBild();
        const auto kennLichtState = zugDeckungsSignalSchirm->kennLichtState();

        const qreal lampYPos = centerWithin(SignalGraphicsObject::hpSpaceForLamp, location.height(), location.top());
        qreal currentXPos =
            location.right() - SignalGraphicsObject::signalLampPadding - SignalGraphicsObject::hpSpaceForLamp;

        // == Red lamp
        const auto redLampOn = (signalBild == Openstw::Simulation::ZugDeckungsSignalBild::Hp0);
        const auto redLampDiameter =
            (redLampOn ? SignalGraphicsObject::activeLampDiameter : SignalGraphicsObject::inactiveLampDiameter);
        const auto redLampColor = redLampOn ? QColor{Qt::red} : SignalGraphicsObject::inactiveLampColor;

        this->drawSignalLamp(
            painter,
            QRectF{currentXPos, lampYPos, SignalGraphicsObject::hpSpaceForLamp, SignalGraphicsObject::hpSpaceForLamp},
            redLampColor, redLampDiameter);

        // == White lamp
        currentXPos -= SignalGraphicsObject::hpPaddingBetweenLamps + SignalGraphicsObject::hpSpaceForLamp;

        const auto kennLampOn = (kennLichtState == Openstw::Simulation::KennLichtState::On);
        const auto kennLampDiameter =
            (kennLampOn ? SignalGraphicsObject::activeLampDiameter : SignalGraphicsObject::inactiveLampDiameter);
        const auto kennLampColor =
            kennLampOn ? SignalGraphicsObject::kennLichtColor : SignalGraphicsObject::inactiveLampColor;

        this->drawSignalLamp(
            painter,
            QRectF{currentXPos, lampYPos, SignalGraphicsObject::hpSpaceForLamp, SignalGraphicsObject::hpSpaceForLamp},
            kennLampColor, kennLampDiameter);

        painter->restore();
    }

    void SignalGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasSignal(this->m_direction))
            return;  

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        const auto boundingRect = this->boundingRect();

        // == Draw area containing the signal graphic
        const auto signalAreaRect = this->calculateSignalArea();
        const auto drawRotated = (this->m_direction == Openstw::Simulation::TileElementDirection::Backward);
        this->drawSignalArea(painter, signalAreaRect, drawRotated);

        // == Draw area containing the various melder and the signal label
        const auto melderAreaRect = this->calculateMelderArea();
        this->drawMelderArea(painter, melderAreaRect, drawRotated);
    }
}
