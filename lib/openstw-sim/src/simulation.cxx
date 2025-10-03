#include "simulation.hxx"
#include <QtLogging>
#include <cstdio>
#include <pugixml.hpp>
#include <stdexcept>

#include "simpleswitch.hxx"

namespace Openstw::Simulation
{
    Simulation::Simulation(QObject* parent) : QObject{parent}
    {
    }

    Simulation* Simulation::FromTemplateFile(QObject* parent, const std::string& filePath)
    {
        pugi::xml_document templateDoc{};
        const auto parseResult = templateDoc.load_file(filePath.c_str());

        Simulation* sim = new Simulation(parent);

        if (!parseResult)
            throw std::runtime_error(parseResult.description());

        const auto simulationNode = templateDoc.root().child("Simulation");
        if (!simulationNode)
            throw std::runtime_error("Simulation template has no Simulation entry");

        // == Tile panel
        const auto tilePanelNode = simulationNode.child("TilePanel");
        if (!tilePanelNode)
            throw std::runtime_error("Simulation template has no TilePanel entry");

        sim->m_tilePanel = TilePanel::CreateFrom(sim, tilePanelNode);
        // ==

        return sim;
    }

    TilePanel* Simulation::tilePanel()
    {
        return this->m_tilePanel;
    }

    void Simulation::update(const float deltaTime)
    {
        this->m_simTime += deltaTime;

        {
            Tile* tile = this->tilePanel()->tileAt(6, 1);
            tile->track().setState(TrackState::FahrstrasseSet);
            tile->setDirty();

            tile = this->tilePanel()->tileAt(7, 1);
            tile->getSwitch()->as<SimpleSwitch>()->setPartState(SimpleSwitchPart::Stem, TrackState::FahrstrasseSet);
            tile->getSwitch()->as<SimpleSwitch>()->setPartState(SimpleSwitchPart::Branch, TrackState::FahrstrasseSet);
            tile->setDirty();

            tile = this->tilePanel()->tileAt(7, 0);
            tile->getSwitch()->as<SimpleSwitch>()->setPartState(SimpleSwitchPart::Stem, TrackState::FahrstrasseSet);
            tile->getSwitch()->as<SimpleSwitch>()->setPartState(SimpleSwitchPart::Branch, TrackState::FahrstrasseSet);
            tile->setDirty();

            tile = this->tilePanel()->tileAt(8, 0);
            tile->getSwitch()->as<SimpleSwitch>()->setPartState(SimpleSwitchPart::Stem, TrackState::FahrstrasseSet);
            tile->getSwitch()->as<SimpleSwitch>()->setPartState(SimpleSwitchPart::Straight, TrackState::FahrstrasseSet);
            tile->setDirty();

            tile = this->tilePanel()->tileAt(9, 0);
            tile->track().setState(TrackState::FahrstrasseSet);
            tile->setDirty();
        }

        this->m_tilePanel->invalidateIfDirty();
    }
}
