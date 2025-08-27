#include "simulation.hxx"
#include <pugixml.hpp>
#include <stdexcept>

namespace Openstw::Simulation
{
    Simulation::Simulation(TilePanel&& tilePanel) : m_tilePanel{std::move(tilePanel)}
    {
    }

    Simulation Simulation::FromTemplateFile(const std::string& filePath)
    {
        pugi::xml_document templateDoc{};
        const auto parseResult = templateDoc.load_file(filePath.c_str());

        if (!parseResult)
            throw std::runtime_error(parseResult.description());

        const auto simulationNode = templateDoc.root().child("Simulation");
        if (!simulationNode)
            throw std::runtime_error("Simulation template has no Simulation entry");

        // == Tile panel
        const auto tilePanelNode = simulationNode.child("TilePanel");
        if (!tilePanelNode)
            throw std::runtime_error("Simulation template has no TilePanel entry");

        auto tilePanel = TilePanel::CreateFrom(tilePanelNode);
        // ==

        return Simulation(std::move(tilePanel));
    }

    TilePanel* Simulation::tilePanel()
    {
        return &this->m_tilePanel;
    }

    void Simulation::update(const float deltaTime)
    {
    }
}
