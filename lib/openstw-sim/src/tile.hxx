#pragma once

#include "directionarrows.hxx"
#include "erlaubnisfeld.hxx"
#include "signal.hxx"
#include "tracksegment.hxx"
#include "utility.hxx"
#include "zugnummernanzeige.hxx"
#include <boost/signals2.hpp>
#include <optional>

namespace Openstw::Simulation
{
    class TilePanel;

    class Tile
    {
        friend class TilePanel;

    public:
        using changed_event_t = boost::signals2::signal<void()>;

    public:
        Tile();
        Tile(const Tile&) = delete;
        Tile& operator=(const Tile&) = delete;

        Tile(Tile&&) = default;
        Tile& operator=(Tile&&) = default;

    public:
        static Tile CreateFrom(const pugi::xml_node&);

    protected:
        static ArrowAlignment ParseArrowAlignment(const pugi::xml_node&);

    public: // == Public interface
        const GridPosition& position() const;
        bool hasSignal(const TileElementDirection direction) const;
        Signal& signal(const TileElementDirection direction);
        bool hasTrack() const;
        TrackSegment& track();
        bool hasDirectionArrows() const;
        DirectionArrows& directionArrows();
        bool hasErlaubnisFeld() const;
        ErlaubnisFeld& erlaubnisFeld();
        bool hasZugnummernAnzeige() const;
        ZugnummernAnzeige& zugnummernAnzeige();
        bool hasTileLabel() const;
        const QString& tileLabel() const;

    public: // == Signals
        /**
         * Called whenever the tiles state was changed.
         *
         * Clients should invalidate and redraw their tile representation
         * upon receiving this signal.
         */
        changed_event_t& on_changed();

    protected:
        void setPosition(const GridPosition& newPosition);

    protected:
        changed_event_t m_evtChanged{};

    protected:
        GridPosition m_position{};
        std::optional<Signal> m_forwardSignal{};  //< Signal for traffic going left to right
        std::optional<Signal> m_backwardSignal{}; //< Signal for traffic from right to left
        std::optional<TrackSegment> m_track{};
        std::optional<DirectionArrows> m_arrows{};
        std::optional<ErlaubnisFeld> m_erlaubnisFeld{};
        std::optional<ZugnummernAnzeige> m_zugNummernAnzeige{};
        QString m_tileLabel{};
    };
}
