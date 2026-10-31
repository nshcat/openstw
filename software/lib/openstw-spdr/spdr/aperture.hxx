#pragma once

#include "aperturestate.hxx"
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
    // Each aperture has a unique id within each tile.
    class Aperture : public QObject
    {
        Q_OBJECT

    public:
        explicit Aperture(const QString& identifier, QObject* parent = nullptr);

    public:
        // Current display state of this aperture.
        const ApertureState& state() const;

        // Whether this aperture is currently not lighting up.
        bool isOff() const;

        // Current colour of this aperture.
        ApertureColor color() const;

        // Current blink mode of this aperture.
        BlinkMode blinkMode() const;

        // Identifier of this aperture.
        QStringView identifier() const;

    public:
        // Update the display state of this aperture.
        // This causes the `stateChanged` signal to be fired
        // if the state is indeed different.
        void updateState(const ApertureState& newState);

    signals:
        // Emitted whenever the state of this aperture changes.
        void stateChanged(Openstw::SpDr::ApertureState newState);

    protected:
        // Identifier of this aperture.
        QString m_identifier;

        // Current display state of this aperture
        ApertureState m_currentState{ApertureState::off()};
    };
}