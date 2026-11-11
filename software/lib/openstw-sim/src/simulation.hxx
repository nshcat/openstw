#pragma once

#include <QDomElement>
#include <QObject>
#include <interlocking/system.hxx>
#include <spdr/stellwerk.hxx>
#include <string>
#include <topo/topology.hxx>

namespace Openstw::Simulation
{
    // The simulation class orchestrates the whole simulation
    // of a railroad network including its Stellwerke, and train
    // traffic within it.
    // It owns instances of a topology graph, an interlocking system
    // and a SpDr Stellwerk simulation.
    class Simulation : public QObject
    {
        Q_OBJECT

    public:
        Simulation(Topology::Topology* topology, Interlocking::System* interlocking, SpDr::Stellwerk* stellwerk,
                   QObject* parent = nullptr);

    public:
        // Create simulation instance based on given Xml element.
        // Topology, interlocking system and the Stellwerk simulation
        // will be created based on the appropiate sub elements.
        static Simulation* createFromXml(QDomElement xmlElement, QObject* parent = nullptr);

    public:
        Topology::Topology* topology() const;
        Interlocking::System* interlocking() const;
        SpDr::Stellwerk* stellwerk() const;

    public:
        void update(const float deltaTime);

    protected:
        // Load simulation data from given Xml element.
        void populateSimulation(QDomElement xmlElement);

    protected:
        // References to simulation subsystems
        Topology::Topology* m_topology;
        Interlocking::System* m_interlocking;
        SpDr::Stellwerk* m_stellwerk;

        float m_simTime{0.0f};
    };
}
