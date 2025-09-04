#include "directionarrows.hxx"

namespace Openstw::Simulation
{
    DirectionArrows::DirectionArrows()
    {
    }

    DirectionArrows DirectionArrows::CreateFrom(const pugi::xml_node& node)
    {
        DirectionArrows arrow{};

        // Read alignment attribute
        const std::string arrowAlignmentTxt = node.attribute("alignment").as_string("left");
        ArrowAlignment arrowAlignment{ArrowAlignment::Left};
        if (arrowAlignmentTxt == "right")
            arrowAlignment = ArrowAlignment::Right;
        arrow.m_alignment = arrowAlignment;

        // Arrow directions
        const auto leftDirectionNode = node.child("Left");
        if (leftDirectionNode)
        {
            arrow.m_directions.set(ArrowDirection::Left);
        }

        const auto rightDirectionNode = node.child("Right");
        if (rightDirectionNode)
        {
            arrow.m_directions.set(ArrowDirection::Right);
        }

        return arrow;
    }

    FlagField<ArrowDirection> DirectionArrows::directions() const
    {
        return this->m_directions;
    }

    ArrowAlignment DirectionArrows::alignment() const
    {
        return this->m_alignment;
    }
}
