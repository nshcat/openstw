#pragma once

#include <QObject>
#include <optional>

#include "directionarrows.hxx"
#include "erlaubnisfeld.hxx"
#include "feststellmelder.hxx"
#include "signal.hxx"
#include "switch.hxx"
#include "tracksegment.hxx"
#include "utility.hxx"
#include "zugnummernanzeige.hxx"

namespace Openstw::Simulation
{
    class TilePanel;

    class Tile : public QObject
    {
        Q_OBJECT

        friend class TilePanel;

    public:
        Tile(QObject* parent);
        Tile(const Tile&) = delete;
        Tile& operator=(const Tile&) = delete;

        Tile(Tile&&) = delete;
        Tile& operator=(Tile&&) = delete;

    public:
        static Tile* CreateFrom(QObject* parent, const pugi::xml_node&);

    protected:
        static ArrowAlignment ParseArrowAlignment(const pugi::xml_node&);

    public: // == Public interface
        /**
         * @brief Marks this tile to be dirty. This doesnt cause an invalidation signal
         * to be emitted right away though - that is done when `invalidateIfDirty` is
         * eventually called.
         */
        void setDirty();

        /**
         * @brief Notify dependants of changes if this tile is dirty.
         */
        void invalidateIfDirty();

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
        bool hasZugStrassenTaste() const;
        TileElementDirection zugStrassenTasteDirection() const;
        bool hasFeststellMelder() const;
        FeststellMelder& feststellMelder();
        bool hasSwitch() const;
        Switch* getSwitch();

    signals:
        void invalidated();

    protected:
        void setPosition(const GridPosition& newPosition);

    protected:
        bool m_isDirty{false};
        GridPosition m_position{};
        std::optional<Signal> m_forwardSignal{};  //< Signal for traffic going left to right
        std::optional<Signal> m_backwardSignal{}; //< Signal for traffic from right to left
        std::optional<TrackSegment> m_track{};
        std::optional<DirectionArrows> m_arrows{};
        std::optional<ErlaubnisFeld> m_erlaubnisFeld{};
        std::optional<ZugnummernAnzeige> m_zugNummernAnzeige{};
        std::optional<TileElementDirection> m_zugStrassenTaste{};
        std::optional<FeststellMelder> m_feststellMelder{};
        std::optional<std::unique_ptr<Switch>> m_switch{};
        QString m_tileLabel{};
    };
}
