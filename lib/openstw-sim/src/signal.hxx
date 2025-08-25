#pragma once

#include <string>

#include "enums.hxx"

namespace Openstw::Simulation
{
    /**
     * @brief Encapsulates a single signal on a tile.
     *
     * The signal can have Hauptsignal, Vorsignal and Rangiersignal components,
     * and it can be a block signal. Kennlichter are also supported.
     */
    class Signal
    {
    public:
        Signal();

    public: // == Property accessors
        bool isBlockSignal() const;
        BlockSignalType blockSignalType() const;
        bool hasHauptSignal() const;
        HauptSignalBild hauptSignalBild() const;
        bool hasKennLicht() const;
        KennLichtState kennLichtState() const;
        bool hasConnector() const;
        bool hasVorSignal() const;
        VorSignalBild vorSignalBild() const;
        bool hasRangierSignal() const;
        RangierSignalBild rangierSignalBild() const;
        const std::string& name() const;

    protected:
        std::string m_signalName{""};

        bool m_isBlockSignal{false};
        BlockSignalType m_blockSignalType{BlockSignalType::SelbstBlock};

        bool m_hasHauptSignal{false};
        HauptSignalBild m_hauptSignalBild{HauptSignalBild::Off};

        bool m_hasVorSignal{false};
        VorSignalBild m_vorSignalBild{VorSignalBild::Off};

        bool m_hasRangierSignal{false};
        RangierSignalBild m_rangierSignalBild{RangierSignalBild::Off};

        bool m_hasKennLicht{false};
        KennLichtState m_kennLichtState{KennLichtState::Off};

        bool m_hasConnector{false}; //< Used on Vorsignale that are connected to a Hauptsignal on a neighbouring tile
    };
}
