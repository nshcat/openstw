#pragma once

namespace Openstw::Simulation
{
    /**
     * @brief Base interface for all classes that are part of the simulation, and need
     * to be updated with each time step.
     */
    class ISimulationObject
    {
    public:
        virtual void update(const float deltaTime) = 0;
    };
}
