#pragma once

namespace Openstw::SpDr
{
    // Enumeration describing the orientation of feature layer.
    // This is used for the most common, simple situations where
    // features are either in or against the left-to-right direction
    // of the Stelltisch.
    // This is pretty crude, but it is more than enough for most
    // features such as signals, level crossings, etc!
    enum class FeatureOrientation
    {
        Forward = 0,

        Backward = 1
    };
}