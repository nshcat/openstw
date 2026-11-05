#pragma once

#include "movementauthoritytype.hxx"
#include <optional>

namespace Openstw::Interlocking
{
    // A movement authority is a permission given to a train
    // that allows it to move within the rail network.
    // These are most often given out by a signal (but might
    // also be given out by LZB, for example)
    // Objects of this class are immutable after construction.
    class MovementAuthority
    {
    public:
        // Construct movement authority that allows proceeding at
        // maximum speed.
        static MovementAuthority FullSpeed();

        // Construct movement authority that allows proceeding at
        // given reduced speed.
        static MovementAuthority ReducedSpeed(double maxSpeed);

        // Construct movement authority that allows proceeding only
        // when driving on sight.
        static MovementAuthority OnSight();

        // Construct movement authority that allows shunting operations
        // to pass.
        static MovementAuthority Shunting();

    protected:
        // Internal constructor, called by the static factory methods.
        MovementAuthority(MovementAuthorityType type, std::optional<double> maxSpeed);

    public:
        // Specific type of this movement authority.
        MovementAuthorityType type() const;

        // Whether this movement authority applies to Zugfahrten
        bool forTrain() const;

        // Whether this movement authority applies to shunting operations
        bool forShunting() const;

        // Whether the proceed speed is restricted by this movement
        // authority.
        bool speedRestricted() const;

        // Which maximum speed the train can proceed at, if
        // type is ReducedSpeed.
        double speedRestriction() const;

    protected:
        // Type of movement authority. Determines what kind of entities
        // are allowed to proceed, and at which speed.
        MovementAuthorityType m_type;

        // If the movement authority allows proceeding at reduced speed,
        // that speed.
        std::optional<double> m_maxSpeed{};
    };

}
