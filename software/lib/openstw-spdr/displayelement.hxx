#pragma once

#include "elementstate.hxx"
#include "lampposition.hxx"
#include <QObject>
#include <QSpan>
#include <QString>
#include <QStringView>
#include <QVector>

namespace Openstw::SpDr
{
    // Class describing a single element within a tile that
    // can output light and this indicate some information to the
    // Fdl. It has a state associated with it, composed of a color (or off)
    // and a blinking state.
    // Each display element has a unique id within each tile, and also knows
    // which lamps it corresponds to (in the 5x3 grid layout of a SpDr tile)
    class DisplayElement : public QObject
    {
        Q_OBJECT

    public:
        explicit DisplayElement(const QString& identifier, const QVector<LampPosition>& lamps,
                                QObject* parent = nullptr);

    public:
        // Current display state of this display element.
        const ElementState& state() const;

        // Whether this display element is currently not lighting up.
        bool isOff() const;

        // Current colour of this display element.
        ElementColor color() const;

        // Current blink mode of this display element.
        BlinkMode blinkMode() const;

        // Identifier of this display element.
        QStringView identifier() const;

        // Actual lamp positions within a tile that correspond to this
        // display element.
        QSpan<const LampPosition> lamps() const;

    public:
        // Update the display state of this element.
        // This causes the `stateChanged` signal to be fired
        // if the state is indeed different.
        void updateState(const ElementState& newState);

    signals:
        // Emitted whenever the state of this display element changes.
        void stateChanged(Openstw::SpDr::ElementState newState);

    protected:
        // Identifier of this element.
        QString m_identifier;

        // Current display state of this element
        ElementState m_currentState{ElementState::off()};

        // Actual lamp positions within the tile that this display element
        // corresponds to.
        QVector<LampPosition> m_lamps{};
    };
}