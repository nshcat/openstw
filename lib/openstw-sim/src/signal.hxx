#pragma once

#include <memory>
#include <optional>
#include <pugixml.hpp>
#include <string>

#include "enums.hxx"
#include "isignalschirm.hxx"
#include "vorsignalschirm.hxx"

namespace Openstw::Simulation
{
    /**
     * @brief Encapsulates a single Signal on a tile, which can contain up to two
     * Signalschirme, represented by ISignalSchirm.
     */
    class Signal
    {
    protected:
        Signal();

    public:
        ~Signal() = default;

        // No copy allowed
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;

        Signal(Signal&&) = default;
        Signal& operator=(Signal&&) = default;

    public:
        static Signal CreateFrom(const pugi::xml_node&);

    public:
        TileElementDirection direction() const;
        bool isBlockSignal() const;
        BlockSignalType blockSignalType() const;
        bool hasConnector() const;
        const std::string& name() const;

        ISignalSchirm* primarySignalSchirm();
        const ISignalSchirm* primarySignalSchirm() const;
        bool hasSecondarySignalSchirm() const;
        VorSignalSchirm* secondarySignalSchirm();
        const VorSignalSchirm* secondarySignalSchirm() const;

    protected:
        TileElementDirection m_direction{TileElementDirection::Forward};
        std::string m_signalName{""};

        std::unique_ptr<ISignalSchirm> m_primarySchirm{};
        std::optional<std::unique_ptr<VorSignalSchirm>> m_secondarySchirm{}; //< Always is a Vorsignal

        bool m_isBlockSignal{false};
        BlockSignalType m_blockSignalType{BlockSignalType::SelbstBlock};

        bool m_hasConnector{false}; //< Used on both signals in a signal that spans multiple tiles
    };
}
