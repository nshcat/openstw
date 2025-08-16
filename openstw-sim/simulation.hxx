#pragma once

#include "base.hxx"

namespace Openstw::Simulation
{
    class Simulation: public ISimulationObject
    {
    public:
        Simulation();

    public:
        virtual void update(const float deltaTime);
    };
}
