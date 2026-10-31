#include "interlocking.hxx"

namespace Openstw::Interlocking
{

    Interlocking::Interlocking(Topology::Topology* topology, QObject* parent)
        : QObject{parent}, m_topology{topology}
    {
    }

    Topology::Topology* Interlocking::topology() const
    {
        return this->m_topology;
    }
}