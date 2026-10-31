#include "axlecounterlink.hxx"
#include "logging.hxx"
#include <qlogging.h>
#include <qloggingcategory.h>

namespace Openstw::Interlocking
{
    AxleCounterLink::AxleCounterLink(Topology::AxleCounter* axleCounter, AxleCounterFacing facing, QObject* parent)
        : QObject{parent}, m_axleCounter{axleCounter}, m_facing{facing}
    {
        // Listen to traversal signals emitted by the axle counter node.
        // We translate those to abstract axle counter operations
        // based on the facing of the axle counter node in relation to
        // the track section we are a part of.

        // clang-format off
        connect(this->m_axleCounter, &Topology::AxleCounter::traversed,
                this, &AxleCounterLink::onTraversed);
        // clang-format on
    }

    void AxleCounterLink::onTraversed(Topology::Port entryPort, Topology::Port exitPort)
    {
        std::optional<AxleCounterOperation> operation{};

        // Which direction did we come from?
        if (entryPort == Topology::Port::Back && exitPort == Topology::Port::Front)
        {
            // Back -> Front. If the axle counter is facing inwards
            // into the section, we increment. Otherwise, decrement.
            operation = (this->m_facing == AxleCounterFacing::Inward) ? AxleCounterOperation::CountUp
                                                                      : AxleCounterOperation::CountDown;
        }
        else if (entryPort == Topology::Port::Front && exitPort == Topology::Port::Back)
        {
            // Front -> Back. If the axle counter is facing inwards
            // into the section, we decrement. Otherwise, increment.
            operation = (this->m_facing == AxleCounterFacing::Inward) ? AxleCounterOperation::CountDown
                                                                      : AxleCounterOperation::CountUp;
        }

        // Check if we actually determined an operation.
        if (operation.has_value())
        {
            // If we do, emit our triggered signal with it.
            emit this->triggered(*operation);
        }
        else
        {
            // This should never happen.
            // clang-format off
            qCWarning(catInterlocking)
                << "Encountered invalid traversal in axle counter '"
                << this->m_axleCounter->identifier()
                << "': "
                << Topology::portToString(entryPort)
                << " -> "
                << Topology::portToString(exitPort)
                << Qt::endl;
            // clang-format on
        }
    }
}
