#pragma once

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

    public:
        bool isBlockSignal() const;
        BlockSignalType blockSignalType() const;
        bool hasConnector() const;
        const std::string& name() const;

        // XXX This needs to always have a primaty ISignalSchirm, and an optional secondary one

    protected:
        std::string m_signalName{""};

        bool m_isBlockSignal{false};
        BlockSignalType m_blockSignalType{BlockSignalType::SelbstBlock};

        bool m_hasConnector{false}; //< Used on Vorsignale that are connected to a Hauptsignal on a neighbouring tile
    };
}
