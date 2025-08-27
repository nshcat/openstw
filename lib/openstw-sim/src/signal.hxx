#pragma once

#include <memory>
#include <optional>
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
    public:
        Signal();
        ~Signal() = default;

        // No copy allowed
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;

        Signal(Signal&&) = default;
        Signal& operator=(Signal&&) = default;

    public:
        TileElementDirection direction() const;
        bool isBlockSignal() const;
        BlockSignalType blockSignalType() const;
        bool hasConnector() const;
        const std::string& name() const;

        ISignalSchirm* primarySignalSchirm();
        bool hasSecondarySignalSchirm() const;
        VorSignalSchirm* secondarySignalSchirm();

    protected:
        TileElementDirection m_direction{TileElementDirection::Forward};
        std::string m_signalName{""};

        std::unique_ptr<ISignalSchirm> m_primarySchirm{};
        std::optional<std::unique_ptr<VorSignalSchirm>> m_secondarySchirm{}; //< Always is a Vorsignal

        bool m_isBlockSignal{false};
        BlockSignalType m_blockSignalType{BlockSignalType::SelbstBlock};

        bool m_hasConnector{false}; //< Used on Vorsignale that are connected to a Hauptsignal on a neighbouring tile
    };
}
