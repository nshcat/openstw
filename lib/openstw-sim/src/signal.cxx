#include "signal.hxx"
#include "utility.hxx"
#include <QString>
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
        const QString signalName{root.attribute("name").as_string()};
        signal.m_signalName = signalName;

        const bool isConnected{root.attribute("connected").as_bool(false)};
        signal.m_hasConnector = isConnected;

        const bool isBlockSignal{root.attribute("isBlockSignal").as_bool(false)};
        signal.m_isBlockSignal = isBlockSignal;
        // ==

        // == Sperrmelder
        const auto hasSperrMelder = (bool)root.child("SperrMelder");
        if (hasSperrMelder)
            signal.m_sperrMelder = SperrMelderState::Off;
        // ==

        // == Feststellmelder
        const auto hasFeststellMelder = (bool)root.child("FeststellMelder");
        if (hasFeststellMelder)
            signal.m_feststellMelderState = StaticLampState::Off;
        // ==

        // == D-Weg Melder
        const auto hasDWegMelder = (bool)root.child("DWegMelder");
        if (hasDWegMelder)
            signal.m_dWegMelderState = StaticLampState::Off;
        // ==

        // == Zs1 Melder
        const auto zs1MelderNode = root.child("Zs1Melder");
        if (zs1MelderNode)
        {
            signal.m_zs1MelderState = StaticLampState::Off;

            const QString zs1LocationStr = zs1MelderNode.attribute("location").as_string("free");
            Zs1MelderLocation zs1Location{Zs1MelderLocation::Free};
            if (zs1LocationStr == "onMast")
                zs1Location = Zs1MelderLocation::OnMast;

            signal.m_zs1MelderLocation = zs1Location;
        }
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

    bool Signal::hasName() const
    {
        return !this->m_signalName.isEmpty();
    }

    const QString& Signal::name() const
    {
        return m_signalName;
    }

    bool Signal::hasSperrMelder() const
    {
        return this->m_sperrMelder.has_value();
    }

    SperrMelderState Signal::sperrMelderState() const
    {
        if (!this->hasSperrMelder())
            throw std::runtime_error("Signal has no Sperrmelder");

        return this->m_sperrMelder.value();
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

    bool Signal::hasDWegMelder() const
    {
        return this->m_dWegMelderState.has_value();
    }

    StaticLampState Signal::dWegMelderState() const
    {
        if (!this->hasDWegMelder())
            throw std::runtime_error("Tile has no D-Weg Melder");

        return this->m_dWegMelderState.value();
    }

    bool Signal::hasFeststellMelder() const
    {
        return this->m_feststellMelderState.has_value();
    }

    StaticLampState Signal::feststellMelderState() const
    {
        if (!this->hasFeststellMelder())
            throw std::runtime_error("Tile has no Feststellmelder");

        return this->m_feststellMelderState.value();
    }

    bool Signal::hasZs1Melder() const
    {
        return this->m_zs1MelderState.has_value();
    }

    StaticLampState Signal::zs1MelderState() const
    {
        if (!this->hasZs1Melder())
            throw std::runtime_error("Tile has no Zs1 Melder");

        return this->m_zs1MelderState.value();
    }

    Zs1MelderLocation Signal::zs1MelderLocation() const
    {
        return this->m_zs1MelderLocation;
    }
}
