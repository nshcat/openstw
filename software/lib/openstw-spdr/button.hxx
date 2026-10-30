#pragma once

#include "buttonrole.hxx"
#include "buttonstate.hxx"
#include <QObject>
#include <QString>
#include <QStringView>
#include <optional>

namespace Openstw::SpDr
{
    // Class representing a single button on a SpDr tile.
    // Some buttons reference elements within the interlocking
    // system (such as a Signaltaste referencing a Interlocking::Signal),
    // while others do not (i.e. a Gruppentaste). The role of the button
    // decides this.
    class Button : public QObject
    {
        Q_OBJECT

    public:
        Button(QStringView identifier, ButtonRole role, std::optional<QStringView> reference,
               QObject* parent = nullptr);

    public:
        // Identifier of the button, unique within the tile its found on.
        QStringView identifier() const;

        // The specific role of this button.
        ButtonRole role() const;

        // Identifier of the interlocking element this button references,
        // if applicable (depends on the buttons specific role).
        std::optional<QStringView> reference() const;

        // The current state of the button.
        ButtonState state() const;

        // Set the current state of the button. This will cause the
        // `stateChanged` signal to be emitted.
        void setState(ButtonState newState);

    signals:
        // Emitted when the state of this button changes.
        void stateChanged(Openstw::SpDr::ButtonState newState);

    protected:
        // Identifier for the button, unique within the tile it belongs to.
        QString m_identifier;

        // Role of this button. Determines what kind of interlocking object
        // it references, if any.
        ButtonRole m_role;

        // Identifier of the interlocking element this button refers to, if any.
        // Whether this is used or not depends on the role of the button
        std::optional<QString> m_reference;

        // The current state of the button.
        ButtonState m_state{ButtonState::Released};
    };
}
