#include "tile.hxx"

namespace Openstw::Simulation
{
    Tile::Tile()
    {
    }

    Tile Tile::CreateFrom(const pugi::xml_node& root)
    {
        Tile tile{};

        // == Coordinates
        const auto x = root.attribute("x").as_int();
        const auto y = root.attribute("y").as_int();

        if (x < 0 || y < 0)
            throw std::runtime_error("Missing or invalid coordinates in tile XML node");

        tile.m_position = GridPosition{static_cast<std::size_t>(x), static_cast<std::size_t>(y)};
        // ==

        // == Arrows
        const auto leftArrowNode = root.child("LeftArrow");
        if (leftArrowNode)
        {
            tile.m_arrowAlignment = Tile::ParseArrowAlignment(leftArrowNode);
            tile.m_arrows.set(ArrowDirection::Left);
        }

        const auto rightArrowNode = root.child("RightArrow");
        if (rightArrowNode)
        {
            tile.m_arrowAlignment = Tile::ParseArrowAlignment(rightArrowNode);
            tile.m_arrows.set(ArrowDirection::Right);
        }
        // ==

        // == Track
        const auto trackNode = root.child("Track");
        if (trackNode)
        {
            tile.m_track = TrackSegment::CreateFrom(trackNode);
        }
        // ==

        // == Signals in forward and backwards direction
        const auto fwdSignalNode = root.child("ForwardSignal");
        if (fwdSignalNode)
        {
            tile.m_forwardSignal = Signal::CreateFrom(fwdSignalNode);
        }

        const auto bwdSignalNode = root.child("BackwardSignal");
        if (bwdSignalNode)
        {
            tile.m_backwardSignal = Signal::CreateFrom(bwdSignalNode);
        }
        // ==

        return std::move(tile);
    }

    ArrowAlignment Tile::ParseArrowAlignment(const pugi::xml_node& node)
    {
        const std::string arrowAlignmentTxt = node.attribute("alignment").as_string("left");
        ArrowAlignment arrowAlignment{ArrowAlignment::Left};
        if (arrowAlignmentTxt == "right")
            arrowAlignment = ArrowAlignment::Right;

        return arrowAlignment;
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

    bool Tile::hasArrow() const
    {
        return this->m_arrows.hasAny();
    }

    FlagField<ArrowDirection> Tile::arrows() const
    {
        return this->m_arrows;
    }

    ArrowAlignment Tile::arrowAlignment() const
    {
        return this->m_arrowAlignment;
    }

    void Tile::setPosition(const GridPosition& newPosition)
    {
        this->m_position = newPosition;
    }

    const GridPosition& Tile::position() const
    {
        return this->m_position;
    }

    Tile::changed_event_t& Tile::on_changed()
    {
        return this->m_evtChanged;
    }
}
