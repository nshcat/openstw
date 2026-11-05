#pragma once

#include <QMetaObject>

namespace Openstw::Interlocking
{
    namespace MovementAuthorityTypeNS
    {
        Q_NAMESPACE

        // The different types of movement authorities the interlocking
        // system can give out to a train.
        enum class MovementAuthorityType
        {
            // Train is allowed to proceed with maximum track speed.
            FullSpeed = 0,

            // Train is allowed to proceed, with a specific maximum speed.
            ReducedSpeed = 1,

            // Train is allowed to proceed, but is required to drive on sight,
            // ensuring that it can brake in time for any kind of obstacle encountered
            // on the tracks.
            OnSight = 2,

            // A shunting operation is allowed to proceed.
            // This is a stop for trains!
            Shunting = 3
        };

        Q_ENUM_NS(MovementAuthorityType)
    }

    using namespace MovementAuthorityTypeNS;
}