#pragma once

#include <string>

#include "base.hxx"
#include "tilepanel.hxx"

namespace Openstw::Simulation
{
    class Simulation: public ISimulationObject
    {
    protected:
        Simulation(TilePanel&& tilePanel);

    public:
        ~Simulation() = default;

        Simulation(const Simulation&) = delete;
        Simulation& operator=(const Simulation&) = delete;

        Simulation(Simulation&&) = default;
        Simulation& operator=(Simulation&&) = default;

    public:
        static Simulation FromTemplateFile(const std::string& filePath);

    public:
        TilePanel* tilePanel();

    public: // == ISimulationObject interface implementation
        virtual void update(const float deltaTime);

    protected:
        TilePanel m_tilePanel;
    };
}
