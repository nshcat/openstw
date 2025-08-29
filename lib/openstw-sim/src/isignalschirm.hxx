#pragma once

#include "enums.hxx"
#include "utility.hxx"
#include <memory>
#include <pugixml.hpp>

namespace Openstw::Simulation
{
    /**
     * @brief Enum that describes all the subclasses of ISignalSchirm
     */
    enum SignalSchirmType
    {
        HauptSignal = 0,
        VorSignal = 1
    };

    /**
     * @brief Base interface for a single Signalschirm on a Signal
     */
    class ISignalSchirm
    {
    public:
        ISignalSchirm();
        virtual ~ISignalSchirm();

    public:
        static std::unique_ptr<ISignalSchirm> CreateFrom(const pugi::xml_node&);

    public:
        virtual SignalSchirmType type() const = 0;

        virtual FlagField<SignalBildType> supportedSignalBilder() const = 0;

        virtual HauptSignalBild hauptSignalBild() const = 0;
        virtual VorSignalBild vorSignalBild() const = 0;
        virtual RangierSignalBild rangierSignalBild() const = 0;
        virtual ZugDeckungsSignalBild zugDeckungsSignalBild() const = 0;

        virtual bool hasKennLicht() const = 0;
        virtual KennLichtState kennLichtState() const = 0;
    };
}
