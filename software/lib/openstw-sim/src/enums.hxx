#pragma once

#include <cstdint>

namespace Openstw::Simulation
{
    enum class Zs1MelderLocation
    {
        Free = 0,
        OnMast = 1
    };

    enum class Digit : std::int8_t
    {
        None = -1,
        D0 = 0,
        D1 = 1,
        D2 = 2,
        D3 = 3,
        D4 = 4,
        D5 = 5,
        D6 = 6,
        D7 = 7,
        D8 = 8,
        D9 = 9
    };

    /**
     * @brief Enum describing the possible states of a lamp that can either
     * be on or off.
     */
    enum class StaticLampState
    {
        Off = 0,
        On = 1
    };

    /**
     * @brief Enum describing the three possible states of a lamp that can
     * blink.
     */
    enum class BlinkLampState
    {
        Off = 0,
        On = 1,
        Blinking = 2
    };

    /**
     * @brief Somewhat arbitrary labeling of the direction a tile
     * element is facing. We assign elements that are meant for traffic going
     * from left to right the label 'forward', and ones that go in the other
     * direction the label 'backward'.
     */
    enum class TileElementDirection
    {
        Forward = 0,      //< Left to right
        Backward = 1,     //< Right to left
        Bidirectional = 2 //< Either way (used for e.g. bidirectional ZST)
    };

    enum class AusfahrErlaubnisState
    {
        ThisStellwerk = 0,
        OtherStellwerk = 1
    };

    enum class ArrowDirection
    {
        None = 0,
        Left = 1 << 0,
        Right = 1 << 1
    };

    enum class ArrowAlignment
    {
        Left = 1,
        Right = 2
    };

    enum class SperrMelderState
    {
        Off = 0,
        On = 1
    };

    enum class SignalBildType
    {
        None = 0,
        HauptSignal = 1 << 0,
        VorSignal = 1 << 1,
        RangierSignal = 1 << 2,
        ZugDeckungsSignal = 1 << 3
    };

    enum class ZugDeckungsSignalBild
    {
        Off = 0,
        Hp0 = 1
    };

    enum class HauptSignalBild
    {
        Off = 0,
        Hp0 = 1,
        Hp1 = 2,
        // Hp2 = 3
    };

    enum class VorSignalBild
    {
        Off = 0,
        Vr0 = 1,
        Vr1 = 2,
        // Vr2 = 3
    };

    enum class RangierSignalBild
    {
        Off = 0,
        Sh0 = 1,
        Sh1 = 2
    };

    enum class KennLichtState
    {
        Off = 0,
        On = 1
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

    enum class TrackState
    {
        Inactive = 0,
        FahrstrasseSet = 1,
        Occupied = 2
    };
}
