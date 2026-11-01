#include "system.hxx"
#include "element.hxx"
#include "elements/tracksection.hxx"

namespace Openstw::Interlocking
{
    System::System(Topology::Topology* topology, QObject* parent)
        : QObject{parent}, m_topology{topology}
    {
    }

    Topology::Topology* System::topology() const
    {
        return this->m_topology;
    }
}