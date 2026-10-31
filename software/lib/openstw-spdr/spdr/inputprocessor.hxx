#pragma once

#include "button.hxx"
#include "tile.hxx"
#include <QObject>

namespace Openstw::SpDr
{
    // Class implementing a state machine that processes SpDr
    // button state changes, and if valid, translates them into
    // interlocking actions that can be passed to the interlocking system.
    // The button state changed signal of the stelltisch needs to connected
    // with this classes slot for processing button states externally.
    class InputProcessor : public QObject
    {
        /* IDEA: `Openstw::Interlocking` exposes `Request`, this class
         * emits those using a signal, and the interlocking system can receive them.
         * During processing, the request could be granted or denied (and in the latter
         * case, contain an explanation as to why it was rejected).
         * The Ui could then show the reason in a status bar or something!
         * Request errors could be emitted by the interlocking system itself.
         * This class shouldnt need to concern itself with those - and the
         * requests itself would be handled asynchroniosly anyways
         * (this class shouldnt block when it hands the requests to the interlocking
         * system)
         */

        Q_OBJECT

    public:
        explicit InputProcessor(QObject* parent = nullptr);

    signals:

    public slots:
        // Process a button state change signal.
        void onButtonStateChanged(const Openstw::SpDr::Tile* tile, const Openstw::SpDr::Button* button,
                                  Openstw::SpDr::ButtonState newState);

    protected:
        // XXX State machine..
    };
}
