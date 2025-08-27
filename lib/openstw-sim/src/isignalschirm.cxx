#include "isignalschirm.hxx"
#include "hauptsignalschirm.hxx"
#include "vorsignalschirm.hxx"
#include <stdexcept>

namespace Openstw::Simulation
{
    ISignalSchirm::ISignalSchirm()
    {
    }

    ISignalSchirm::~ISignalSchirm()
    {
    }

    std::unique_ptr<ISignalSchirm> ISignalSchirm::CreateFrom(const pugi::xml_node& root)
    {
        const auto signalSchirmType = std::string{root.attribute("type").as_string()};

        if (signalSchirmType == std::string{"HauptSignalSchirm"})
        {
            return HauptSignalSchirm::CreateFrom(root);
        }
        else if (signalSchirmType == std::string{"VorSignalSchirm"})
        {
            return VorSignalSchirm::CreateFrom(root);
        }
        else
        {
            throw std::runtime_error("Unknown Signalschirm type encountered in Signal XML node");
        }
    }
}
