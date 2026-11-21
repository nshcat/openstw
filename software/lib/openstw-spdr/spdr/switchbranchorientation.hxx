#pragma once

#include <QObject>

namespace Openstw::SpDr
{
    namespace SwitchBranchOrientationNS
    {
        Q_NAMESPACE;

        // Enumeration describing the orientation of the branch of
        // switch feature. In combination with a general
        // feature orientation described by the `FeatureOrientation`
        // enumeration, this fully describes the direction the switch
        // fork is pointing to, and where the branch is leading to.
        enum class SwitchBranchOrientation
        {
            // Branch leaves the tile at the bottom.
            Down,

            // Branch leaves the tile at the top.
            Up
        };

        Q_ENUM_NS(SwitchBranchOrientation);
    }

    using namespace SwitchBranchOrientationNS;
}