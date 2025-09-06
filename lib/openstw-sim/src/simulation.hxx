#pragma once

#include <QObject>
#include <string>

#include "base.hxx"
#include "tilepanel.hxx"

namespace Openstw::Simulation
{
    class Simulation : public QObject, public ISimulationObject
    {
        Q_OBJECT

    protected:
        Simulation(QObject* parent);

    public:
        ~Simulation() = default;

        Simulation(const Simulation&) = delete;
        Simulation& operator=(const Simulation&) = delete;

        Simulation(Simulation&&) = delete;
        Simulation& operator=(Simulation&&) = delete;

    public:
        static Simulation* FromTemplateFile(QObject* parent, const std::string& filePath);

    public:
        TilePanel* tilePanel();

    public: // == ISimulationObject interface implementation
        virtual void update(const float deltaTime);

    protected:
        TilePanel* m_tilePanel;
    };
}
