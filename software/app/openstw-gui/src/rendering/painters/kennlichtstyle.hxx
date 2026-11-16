#pragma once

namespace Rendering
{
    // The different ways a kennlicht can be rendered on a signal
    // feature.
    enum class KennlichtStyle
    {
        // The Kennlicht lamp has its own lamp position, even if off.
        Individual,

        // The Kennlicht lamp is integrated into the red lamp of a main
        // signal and shares one lamp position - if the Kennlicht aperture
        // is on, the application renders the Kennlicht, and if not, it renders
        // the red lamp in the exact same location (no matter whether it is
        // on or off)
        Integrated
    };
}