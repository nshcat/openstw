#pragma once

#include "hauptsignalbild.hxx"
#include <QObject>

namespace Openstw::Interlocking
{
    // A Signalschirm representing a Hauptsignal.
    class HauptSignalSchirm : public QObject
    {
        Q_OBJECT

    public:
        explicit HauptSignalSchirm(QObject* parent = nullptr);

    public:
        // The current Signalbild.
        HauptSignalBild signalBild() const;

        // Whether the current Signalbild allows passage or not.
        bool allowsPassage() const;

    public:
        // Change the current Signalbild to given value.
        void setSignalBild(HauptSignalBild signalBild);

    signals:
        // Emitted when the displayed Signalbild changes.
        void signalBildChanged(Openstw::Interlocking::HauptSignalBild signalBild);

    protected:
        // The current Signalbild.
        HauptSignalBild m_signalBild{HauptSignalBild::Halt};
    };
}
