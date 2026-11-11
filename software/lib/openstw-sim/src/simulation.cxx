#include "simulation.hxx"
#include <core/exception.hxx>
#include <core/xmlutility.hxx>

namespace Openstw::Simulation
{

    Simulation::Simulation(Topology::Topology* topology, Interlocking::System* interlocking, SpDr::Stellwerk* stellwerk,
                           QObject* parent)
        : QObject{parent}, m_topology{topology}, m_interlocking{interlocking}, m_stellwerk{stellwerk}
    {
        // Reparent all subsystem objects to the simulation object,
        // for lifetime management.
        this->m_topology->setParent(this);
        this->m_interlocking->setParent(this);
        this->m_stellwerk->setParent(this);
    }

    Simulation* Simulation::createFromXml(QDomElement xmlElement, QObject* parent)
    {
        // First, create simulation subsystem instances, since we need those to
        // construct the simulation object itself.

        // We begin with the topology object.
        auto topoElement = xmlElement.namedItem("Topology");

        if (topoElement.isNull() || !topoElement.isElement())
        {
            throw Core::RuntimeError("Simulation XML: Expected <Topology> element");
        }

        auto* topology = Topology::Topology::createFromXml(topoElement.toElement());

        // Then, create the interlocking system on top of it.
        auto interlockingElement = xmlElement.namedItem("Interlocking");

        if (interlockingElement.isNull() || !interlockingElement.isElement())
        {
            throw Core::RuntimeError("Simulation XML: Expected <Interlocking> element");
        }

        auto* interlocking = Interlocking::System::createFromXml(topology, interlockingElement.toElement());

        // Afterwards, create the SpDr Stellwerk object.
        auto spdrElement = xmlElement.namedItem("SpDr");

        if (spdrElement.isNull() || !spdrElement.isElement())
        {
            throw Core::RuntimeError("Simulation XML: Expected <SpDr> element");
        }

        auto* stellwerk = SpDr::Stellwerk::createFromXml(spdrElement.toElement(), interlocking);

        // Finally, create the simulation object, and populate its data as well.
        auto* simulation = new Simulation{topology, interlocking, stellwerk, parent};

        simulation->populateSimulation(xmlElement);

        return simulation;
    }

    Topology::Topology* Simulation::topology() const
    {
        return this->m_topology;
    }

    Interlocking::System* Simulation::interlocking() const
    {
        return this->m_interlocking;
    }

    SpDr::Stellwerk* Simulation::stellwerk() const
    {
        return this->m_stellwerk;
    }

    void Simulation::update(const float deltaTime)
    {
        return;
    }

    void Simulation::populateSimulation(QDomElement xmlElement)
    {
    }
}
