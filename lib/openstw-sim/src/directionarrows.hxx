#pragma once

#include <pugixml.hpp>

#include <qstring.h>

#include "enums.hxx"
#include "utility.hxx"

namespace Openstw::Simulation
{
    /**
     * @brief Represents a set of Richtungspfeile on a single tile, used on tracks leading
     * to a neighbouring Stellwerk
     */
    class DirectionArrows
    {
    protected:
        DirectionArrows();

    public:
        // No copy allowed
        DirectionArrows(const DirectionArrows&) = delete;
        DirectionArrows& operator=(const DirectionArrows&) = delete;

        DirectionArrows(DirectionArrows&&) = default;
        DirectionArrows& operator=(DirectionArrows&&) = default;

    public:
        static DirectionArrows CreateFrom(const pugi::xml_node&);

    public:
        FlagField<ArrowDirection> directions() const;
        ArrowAlignment alignment() const;
        bool hasLabel() const;
        const QString& label() const;

    protected:
        FlagField<ArrowDirection> m_directions{ArrowDirection::None};
        ArrowAlignment m_alignment{ArrowAlignment::Left};
        QString m_label{};
    };
}
