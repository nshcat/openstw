#include "hauptsignalschirmpainter.hxx"

namespace Rendering
{
    HauptSignalSchirmPainter::HauptSignalSchirmPainter(Aperture* redLamp, Aperture* greenLamp, BlinkGenerator* blink,
                                                       QObject* parent)
        : SignalPainter{blink, parent}, m_redLamp{redLamp}, m_greenLamp{greenLamp}
    {
    }

    const Aperture* HauptSignalSchirmPainter::greenLamp() const
    {
        return m_greenLamp;
    }

    const Aperture* HauptSignalSchirmPainter::redLamp() const
    {
        return m_redLamp;
    }

    std::optional<const Aperture*> HauptSignalSchirmPainter::shuntingLamp() const
    {
        return m_shuntingLamp;
    }

    bool HauptSignalSchirmPainter::hasShuntingLamp() const
    {
        return this->m_shuntingLamp.has_value();
    }

    void HauptSignalSchirmPainter::setShuntingLamp(const Aperture* newShuntingLamp)
    {
        m_shuntingLamp = newShuntingLamp;
    }

    std::optional<const Aperture*> HauptSignalSchirmPainter::kennlichtLamp() const
    {
        return m_kennlichtLamp;
    }

    bool HauptSignalSchirmPainter::hasKennlicht() const
    {
        return this->m_kennlichtLamp.has_value();
    }

    void HauptSignalSchirmPainter::setKennlichtLamp(const Aperture* newKennlichtLamp)
    {
        m_kennlichtLamp = newKennlichtLamp;
    }

    KennlichtStyle HauptSignalSchirmPainter::kennlichtStyle() const
    {
        return m_kennlichtStyle;
    }

    void HauptSignalSchirmPainter::setKennlichtStyle(KennlichtStyle newKennlichtStyle)
    {
        m_kennlichtStyle = newKennlichtStyle;
    }

    QSizeF HauptSignalSchirmPainter::measure()
    {
        qreal signalWidth = (signalLampPadding * 2) + (2 * hpSpaceForLamp) + (hpPaddingBetweenLamps);

        // If a possible kennlicht or Rangiersignal do not share their space
        // with the other lamps, we need to adjust our measurement to account
        // for them.
        if (this->kennlichtStyle() == KennlichtStyle::Individual)
        {
            // We always leave the space for the kennlicht if we have a rangier lamp, even if its not installed
            if (this->hasKennlicht() || this->hasShuntingLamp())
            {
                signalWidth += hpSpaceForLamp + hpPaddingBetweenLamps;
            }

            if (this->hasShuntingLamp())
            {
                signalWidth += hpSpaceForLamp + hpPaddingBetweenLamps;
            }
        }

        return QSizeF{signalWidth, signalHeight};
    }

    void HauptSignalSchirmPainter::paint(QPainter* painter, const QRectF& location)
    {
        painter->save();

        painter->setBrush(Qt::black);
        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->drawRoundedRect(location, 10, 10);

        // Draw signal lamps
        const qreal lampYPos = centerWithin(hpSpaceForLamp, location.height(), location.top());

        qreal currentXPos = location.right() - signalLampPadding - hpSpaceForLamp;

        // Green lamp
        this->drawSignalLamp(painter, QRectF{currentXPos, lampYPos, hpSpaceForLamp, hpSpaceForLamp}, this->m_greenLamp,
                             activeLampDiameter, inactiveLampDiameter);

        // Red lamp
        currentXPos -= hpPaddingBetweenLamps + hpSpaceForLamp;

        // If we have a kennlicht and its supposed to share the space with
        // the red lamp, we need to check if its active. And if so, draw the Kennlicht instead
        // (which uses a smaller lamp diameter)
        if (this->hasKennlicht() && this->kennlichtStyle() == KennlichtStyle::Integrated &&
            !(*this->m_kennlichtLamp)->isOff())
        {
            // Draw the Kennlicht.
            this->drawSignalLamp(painter, QRectF{currentXPos, lampYPos, hpSpaceForLamp, hpSpaceForLamp},
                                 *(this->m_kennlichtLamp), activeKennLampDiameter, inactiveKennLampDiameter);
        }
        else
        {
            // Otherwise, just draw the red lamp.
            this->drawSignalLamp(painter, QRectF{currentXPos, lampYPos, hpSpaceForLamp, hpSpaceForLamp},
                                 this->m_redLamp, activeLampDiameter, inactiveLampDiameter);
        }

        // If we have a Kennlicht and its not integrated with the red lamp, draw it now.
        if (this->hasKennlicht() && this->kennlichtStyle() == KennlichtStyle::Individual)
        {
            currentXPos -= hpPaddingBetweenLamps + hpSpaceForLamp;

            this->drawSignalLamp(painter, QRectF{currentXPos, lampYPos, hpSpaceForLamp, hpSpaceForLamp},
                                 *this->m_kennlichtLamp, activeKennLampDiameter, inactiveKennLampDiameter);
        }

        // Draw the diagonal shunting signal, if we have one
        if (this->hasShuntingLamp())
        {
            currentXPos -= hpPaddingBetweenLamps + hpSpaceForLamp;

            // If we did not render a kennlicht, we leave that space free.
            if (!this->hasKennlicht())
            {
                currentXPos -= hpPaddingBetweenLamps + hpSpaceForLamp;
            }

            const auto rangierLampOn = !((*this->m_shuntingLamp)->isOff());
            const auto rangierLampWidth = rangierLampOn ? 5.0f : 2.0f;
            const auto rangierLampColor = this->resolveColor(*(this->m_shuntingLamp));

            QRectF rangierLampRect{currentXPos + 2.0f, lampYPos, hpSpaceForLamp, hpSpaceForLamp};

            if (!rangierLampOn)
            {
                rangierLampRect = rangierLampRect.marginsRemoved(QMarginsF{1.0f, 1.0f, 1.0f, 1.0f});
            }

            painter->save();

            painter->setPen(QPen{rangierLampColor, rangierLampWidth, Qt::SolidLine, Qt::FlatCap});
            painter->drawLine(rangierLampRect.topLeft(), rangierLampRect.bottomRight());

            painter->restore();
        }

        painter->restore();
    }
}
