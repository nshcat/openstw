#pragma once

#include <QString>
#include <memory>
#include <optional>
#include <pugixml.hpp>
#include <string>

#include "enums.hxx"
#include "isignalschirm.hxx"
#include "tileelement.hxx"
#include "vorsignalschirm.hxx"

namespace Openstw::Simulation
{
    /**
     * @brief Encapsulates a single Signal on a tile, which can contain up to two
     * Signalschirme, represented by ISignalSchirm.
     */
    class Signal : public TileElement
    {
    protected:
        Signal(Tile* parent);

    public:
        ~Signal() = default;

        // No copy allowed
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;

        Signal(Signal&&) = default;
        Signal& operator=(Signal&&) = default;

    public:
        static Signal CreateFrom(Tile*, const pugi::xml_node&);

    public:
        TileElementDirection direction() const;
        bool isBlockSignal() const;
        BlockSignalType blockSignalType() const;
        bool hasConnector() const;
        const QString& name() const;
        bool hasName() const;
        bool hasSperrMelder() const;
        SperrMelderState sperrMelderState() const;

        ISignalSchirm* primarySignalSchirm();
        const ISignalSchirm* primarySignalSchirm() const;
        bool hasSecondarySignalSchirm() const;
        VorSignalSchirm* secondarySignalSchirm();
        const VorSignalSchirm* secondarySignalSchirm() const;

        bool hasDWegMelder() const;
        StaticLampState dWegMelderState() const;
        bool hasZs1Melder() const;
        StaticLampState zs1MelderState() const;
        Zs1MelderLocation zs1MelderLocation() const;

    protected:
        TileElementDirection m_direction{TileElementDirection::Forward};
        QString m_signalName{""};

        std::unique_ptr<ISignalSchirm> m_primarySchirm{};
        std::optional<std::unique_ptr<VorSignalSchirm>> m_secondarySchirm{}; //< Always is a Vorsignal

        bool m_isBlockSignal{false};
        BlockSignalType m_blockSignalType{BlockSignalType::SelbstBlock};

        bool m_hasConnector{false}; //< Used on both signals in a signal that spans multiple tiles
        std::optional<SperrMelderState> m_sperrMelder{};

        std::optional<StaticLampState> m_dWegMelderState{};
        std::optional<StaticLampState> m_zs1MelderState{};
        Zs1MelderLocation m_zs1MelderLocation{Zs1MelderLocation::Free};
    };
}
