#include "signal.hxx"
#include "utility.hxx"
#include <boost/smart_ptr.hpp>

namespace Openstw::Simulation
{
    Signal::Signal()
    {
    }

    Signal Signal::CreateFrom(const pugi::xml_node& root)
    {
        Signal signal{};

        // == Direction
        TileElementDirection signalDirection{TileElementDirection::Forward};
        if (root.name() == std::string{"BackwardSignal"})
            signalDirection = TileElementDirection::Backward;

        signal.m_direction = signalDirection;
        // ==

        // == Attributes
        const std::string signalName{root.attribute("name").as_string()};
        signal.m_signalName = signalName;
        // ==

        // == Primary Signalschirm
        const auto primarySignalSchirmNode = root.child("PrimarySignalSchirm");
        if (!primarySignalSchirmNode)
            throw std::runtime_error("Signal XML node contains not primary Signalschirm child node");

        signal.m_primarySchirm = ISignalSchirm::CreateFrom(primarySignalSchirmNode);
        // ==

        // == Secondary Signalschirm
        const auto secondarySignalSchirmNode = root.child("SecondarySignalSchirm");
        if (secondarySignalSchirmNode)
        {
            if (secondarySignalSchirmNode.attribute("type").as_string() != std::string{"VorSignalSchirm"})
                throw std::runtime_error("Secondary Signalschirm in Signal XML node was not a VorSignalSchirm");

            auto secondarySignalSchirm =
                dynamic_pointer_cast<VorSignalSchirm>(ISignalSchirm::CreateFrom(secondarySignalSchirmNode));
            if (!secondarySignalSchirm)
                throw std::runtime_error("Secondary Signalschirm in Signal XML node was not a VorSignalSchirm");

            signal.m_secondarySchirm = std::move(secondarySignalSchirm);
        }
        // ==

        return signal;
    }

    TileElementDirection Signal::direction() const
    {
        return this->m_direction;
    }

    bool Signal::isBlockSignal() const
    {
        return m_isBlockSignal;
    }

    BlockSignalType Signal::blockSignalType() const
    {
        return m_blockSignalType;
    }

    bool Signal::hasConnector() const
    {
        return m_hasConnector;
    }

    const std::string& Signal::name() const
    {
        return m_signalName;
    }

    ISignalSchirm* Signal::primarySignalSchirm()
    {
        return this->m_primarySchirm.get();
    }

    const ISignalSchirm* Signal::primarySignalSchirm() const
    {
        return this->m_primarySchirm.get();
    }

    bool Signal::hasSecondarySignalSchirm() const
    {
        return this->m_secondarySchirm.has_value();
    }

    VorSignalSchirm* Signal::secondarySignalSchirm()
    {
        if (!this->hasSecondarySignalSchirm())
            throw std::runtime_error("Signal has no secondary Signalschirm");

        return this->m_secondarySchirm.value().get();
    }

    const VorSignalSchirm* Signal::secondarySignalSchirm() const
    {
        if (!this->hasSecondarySignalSchirm())
            throw std::runtime_error("Signal has no secondary Signalschirm");

        return this->m_secondarySchirm.value().get();
    }
}
