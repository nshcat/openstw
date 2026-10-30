#pragma once

#include "inputprocessor.hxx"
#include "stelltisch.hxx"
#include <QObject>
#include <QXmlStreamReader>
#include <interlocking.hxx>

namespace Openstw::SpDr
{
    // Simulation of a SpDr Stellwerk.
    class Simulation : public QObject
    {
        Q_OBJECT

    public:
        explicit Simulation(Interlocking::Interlocking* interlocking, Stelltisch* stelltisch,
                            QObject* parent = nullptr);

    public:
        // Create SpDr Stellwerk simulation from given XML data file.
        // XXXTODO we should pass a xmldocument node from pugixml here i think..
        static Simulation* createFromFile(Interlocking::Interlocking* interlocking, const QString& filePath,
                                          QObject* parent);

    public:
        // Stelltisch associated with this stellwerk.
        const Stelltisch* stelltisch() const;

        // Stelltisch associated with this stellwerk.
        Stelltisch* stelltisch();

        // The interlocking system associated with this stelltisch
        const Interlocking::Interlocking* interlocking() const;

        // The interlocking system associated with this stelltisch
        Interlocking::Interlocking* interlocking();

    signals:

    protected:
        // Reference to the interlocking system the SpDr simulation
        // is sitting on top of.
        // It implements all of the safety-critical interlocking logic.
        Interlocking::Interlocking* m_interlocking;

        // Stelltisch of the SpDr stellwerk, containing a grid of
        // tiles that the Fdl can interact with.
        Stelltisch* m_stelltisch;

        // The input processor takes care of registering individual
        // SpDr button state changes and turning them into interlocking
        // requests that get passed to the interlocking system.
        InputProcessor* m_input;
    };

}
