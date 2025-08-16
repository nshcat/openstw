#pragma once

#include "enums.hxx"

namespace Openstw::Simulation
{
    class Signal
    {
    public:
        Signal();

    public: // == Property accessors
        bool isBlockSignal() const;
        BlockSignalType blockSignalType() const;
        bool hasHauptSignal() const;
        HauptSignalBild hauptSignalBild() const;

    protected:
        bool m_isBlockSignal{false};
        BlockSignalType m_blockSignalType{BlockSignalType::SelbstBlock};

        bool m_hasHauptSignal{false};
        HauptSignalBild m_hauptSignalBild{HauptSignalBild::Off};
    };
}
