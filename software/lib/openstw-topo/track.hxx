#pragma once

#include "edge.hxx"

namespace Openstw
{
    namespace Topology
    {
        // Class representing a track edge within the topology graph.
        class Track : public Edge
        {
        public:
            Track(const QString& identifier, double length, double maxSpeed, QObject* parent = nullptr);

        public:
            double length() const;
            double maximumSpeed() const;

        protected:
            double m_length;
            double m_maxSpeed;
        };
    }
}