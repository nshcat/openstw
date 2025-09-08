#include "tile.hxx"

namespace Openstw::Simulation
{
    Tile::Tile(QObject* parent) : QObject(parent)
    {
    }

    Tile* Tile::CreateFrom(QObject* parent, const pugi::xml_node& root)
    {
        Tile* tile = new Tile(parent);

        // == Coordinates
        const auto x = root.attribute("x").as_int();
        const auto y = root.attribute("y").as_int();

        if (x < 0 || y < 0)
            throw std::runtime_error("Missing or invalid coordinates in tile XML node");

        tile->m_position = GridPosition{static_cast<std::size_t>(x), static_cast<std::size_t>(y)};
        // ==

        // == Tile label
        const auto labelNode = root.child("LabelBox");
        if (labelNode)
        {
            tile->m_tileLabel = QString{labelNode.attribute("label").as_string("")};
        }
        // ==

        // == Zugstrassentaste
        const auto zstNode = root.child("ZugStrassenTaste");
        if (zstNode)
        {
            const QString directionTxt = zstNode.attribute("direction").as_string("forward");
            TileElementDirection direction{TileElementDirection::Forward};
            if (directionTxt == "backward")
                direction = TileElementDirection::Backward;

            tile->m_zugStrassenTaste = direction;
        }
        // ==

        // == Zugnummernanzeige
        const auto znaNode = root.child("ZugnummernAnzeige");
        if (znaNode)
        {
            tile->m_zugNummernAnzeige = ZugnummernAnzeige::CreateFrom(znaNode);
        }
        // ==

        // == Arrows
        const auto arrowsNode = root.child("DirectionArrows");
        if (arrowsNode)
        {
            tile->m_arrows = DirectionArrows::CreateFrom(arrowsNode);
        }
        // ==

        // == Erlaubnisfeld
        const auto erlaubnisFeldNode = root.child("ErlaubnisFeld");
        if (erlaubnisFeldNode)
        {
            tile->m_erlaubnisFeld = ErlaubnisFeld::CreateFrom(erlaubnisFeldNode);
        }
        // ==

        // == Track
        const auto trackNode = root.child("Track");
        if (trackNode)
        {
            tile->m_track = TrackSegment::CreateFrom(trackNode);
        }
        // ==

        // == Signals in forward and backwards direction
        const auto fwdSignalNode = root.child("ForwardSignal");
        if (fwdSignalNode)
        {
            tile->m_forwardSignal = Signal::CreateFrom(fwdSignalNode);
        }

        const auto bwdSignalNode = root.child("BackwardSignal");
        if (bwdSignalNode)
        {
            tile->m_backwardSignal = Signal::CreateFrom(bwdSignalNode);
        }
        // ==

        return tile;
    }

    void Tile::setDirty()
    {
        this->m_isDirty = true;
    }

    void Tile::invalidateIfDirty()
    {
        if (this->m_isDirty)
        {
            emit this->invalidated();
            this->m_isDirty = false;
        }
    }

    bool Tile::hasSignal(const TileElementDirection direction) const
    {
        if (direction == TileElementDirection::Forward)
            return this->m_forwardSignal.has_value();
        else
            return this->m_backwardSignal.has_value();
    }

    Signal& Tile::signal(const TileElementDirection direction)
    {
        auto& signal = (direction == TileElementDirection::Forward) ? this->m_forwardSignal : this->m_backwardSignal;

        if (!signal.has_value())
            throw std::runtime_error("Tile doesnt contain signal in given direction");

        return signal.value();
    }

    bool Tile::hasTrack() const
    {
        return this->m_track.has_value();
    }

    TrackSegment& Tile::track()
    {
        if (!this->m_track.has_value())
            throw std::runtime_error("Tile doesnt contain track segment");

        return this->m_track.value();
    }

    DirectionArrows& Tile::directionArrows()
    {
        if (!this->hasDirectionArrows())
            throw std::runtime_error("Tile doesnt contain direction arrows");

        return this->m_arrows.value();
    }

    bool Tile::hasErlaubnisFeld() const
    {
        return this->m_erlaubnisFeld.has_value();
    }

    ErlaubnisFeld& Tile::erlaubnisFeld()
    {
        if (!this->hasErlaubnisFeld())
            throw std::runtime_error("Tile doesnt contain Erlaubnisfeld");

        return this->m_erlaubnisFeld.value();
    }

    bool Tile::hasZugnummernAnzeige() const
    {
        return this->m_zugNummernAnzeige.has_value();
    }

    ZugnummernAnzeige& Tile::zugnummernAnzeige()
    {
        if (!this->hasZugnummernAnzeige())
            throw std::runtime_error("Tile doesnt contain Zugnummernanzeige");

        return this->m_zugNummernAnzeige.value();
    }

    bool Tile::hasTileLabel() const
    {
        return !this->m_tileLabel.isEmpty();
    }

    const QString& Tile::tileLabel() const
    {
        return this->m_tileLabel;
    }

    bool Tile::hasZugStrassenTaste() const
    {
        return this->m_zugStrassenTaste.has_value();
    }

    TileElementDirection Tile::zugStrassenTasteDirection() const
    {
        if (!this->hasZugStrassenTaste())
            throw std::runtime_error("Tile has no Zugstrassentaste");

        return this->m_zugStrassenTaste.value();
    }

    bool Tile::hasDirectionArrows() const
    {
        return this->m_arrows.has_value();
    }

    void Tile::setPosition(const GridPosition& newPosition)
    {
        this->m_position = newPosition;
    }

    const GridPosition& Tile::position() const
    {
        return this->m_position;
    }
}
