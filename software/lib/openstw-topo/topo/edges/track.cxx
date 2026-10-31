#include "track.hxx"

namespace Openstw
{
    namespace Topology
    {
        Track::Track(const QString& identifier, double length, double maxSpeed, QObject* parent)
            : Edge{EdgeType::Track, identifier, parent}, m_length{length}, m_maxSpeed{maxSpeed}
        {
        }

        double Track::length() const
        {
            return this->m_length;
        }

        double Track::maximumSpeed() const
        {
            return this->m_maxSpeed;
        }

        double Track::weight() const
        {
            return this->length();
        }
    }
}
