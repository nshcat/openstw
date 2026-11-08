#pragma once

#include "inputprocessor.hxx"
#include "stelltisch.hxx"
#include <QDomElement>
#include <QObject>
#include <interlocking/system.hxx>

namespace Openstw::SpDr
{
    // Simulation of a SpDr Stellwerk.
    class Stellwerk : public QObject
    {
        Q_OBJECT

    public:
        explicit Stellwerk(Interlocking::System* interlocking, Stelltisch* stelltisch, QObject* parent = nullptr);

    public:
        // Load Stellwerk from given XML element.
        static Stellwerk* createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                        QObject* parent = nullptr);

    public:
        // Stelltisch associated with this stellwerk.
        const Stelltisch* stelltisch() const;

        // Stelltisch associated with this stellwerk.
        Stelltisch* stelltisch();

        // The interlocking system associated with this stelltisch
        const Interlocking::System* interlocking() const;

        // The interlocking system associated with this stelltisch
        Interlocking::System* interlocking();

    signals:

    protected:
        // Reference to the interlocking system the SpDr simulation
        // is sitting on top of.
        // It implements all of the safety-critical interlocking logic.
        Interlocking::System* m_interlocking;

        // Stelltisch of the SpDr stellwerk, containing a grid of
        // tiles that the Fdl can interact with.
        Stelltisch* m_stelltisch;

        // The input processor takes care of registering individual
        // SpDr button state changes and turning them into interlocking
        // requests that get passed to the interlocking system.
        InputProcessor* m_input;
    };

}
