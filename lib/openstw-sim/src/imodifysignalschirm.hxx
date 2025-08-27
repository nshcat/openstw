#pragma once

#include "enums.hxx"

namespace Openstw::Simulation
{
    /**
     * @brief Interface used to change the signal bilder
     * a Signalschirm shows.
     *
     * @todo Maybe change the name of this interface - maybe IChangeSignalBild?
     * This would be more specific, and separated from the Signalschirm.
     */
    class IModifySignalSchirm
    {
    public:
        IModifySignalSchirm();
        virtual ~IModifySignalSchirm();

    public:
        virtual void setHauptSignalBild(const HauptSignalBild bild) = 0;
        virtual void setVorSignalBild(const VorSignalBild bild) = 0;
        virtual void setRangierSignalBild(const RangierSignalBild bild) = 0;
        virtual void setZugDeckungsSignalBild(const ZugDeckungsSignalBild bild) = 0;
        virtual void setKennLichtState(const KennLichtState state) = 0;
    };
}
