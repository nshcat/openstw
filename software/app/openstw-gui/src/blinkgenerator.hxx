#pragma once

#include "blinkstate.hxx"
#include <QObject>
#include <QTimer>

// Class that generates the global blink impulse used all over
// the Stelltisch. This ensures that blinking tile elements all
// blink in unison.
class BlinkGenerator : public QObject
{
    Q_OBJECT

public:
    explicit BlinkGenerator(QObject* parent = nullptr);

public:
    // The current blinking state.
    BlinkState state() const;

    // Whether the blink generator indicates that blinking
    // lamps should be on.
    bool lampsOn() const;

    // Whether the blink generator indicates that blinking
    // lamps should be off.
    bool lampsOff() const;

public:
    // Stop the blink generator.
    void stop();

    // Start or resume the blink generator.
    void start();

signals:
    // Signal emitted whenever the generator changes blink state.
    void blink(BlinkState state);

protected slots:
    // Called whenever the QTimer associated with this class ticks.
    void onTimerElapsed();

protected:
    // The current blinking state.
    BlinkState m_currentState{BlinkState::LampOff};

    // The QTimer we use to generate the blinking impulses.
    QTimer* m_blinkTimer;
};
