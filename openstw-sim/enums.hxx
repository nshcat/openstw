#pragma once

namespace Openstw::Simulation
{
    enum class HauptSignalBild
    {
        Off = 0,
        Hp0 = 1,
        Hp1 = 2,
        Hp2 = 3
    };

    enum class VorSignalBild
    {
        Off = 0,
        Vr0 = 1,
        Vr1 = 2,
        Vr2 = 3
    };

    enum class RangierSignalBild
    {
        Off = 0,
        Sh0 = 1,
        Sh1 = 2
    };

    enum class BlockSignalType
    {
        SelbstBlock = 0,
        ZentralBlock = 1
    };

    enum class Platform
    {
        None = 0,
        Top = 1 << 0,
        Bottom = 1 << 1
    };
}
