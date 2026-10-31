#include "tracksection.hxx"
#include "logging.hxx"

namespace Openstw::Interlocking
{

    TrackSection::TrackSection(QStringView identifier, QSpan<AxleCounterLink*> axleCounters, QObject* parent)
        : Element{ElementType::TrackSection, identifier, parent},
          m_axleCounterLinks{axleCounters.begin(), axleCounters.end()}
    {
    }

    bool TrackSection::occupied() const
    {
        return this->m_isOccupied;
    }

    bool TrackSection::locked() const
    {
        return this->m_isLocked;
    }

    int TrackSection::counterValue() const
    {
        return this->m_counterValue;
    }

    QSpan<const AxleCounterLink* const> TrackSection::links() const
    {
        return {this->m_axleCounterLinks};
    }

    void TrackSection::lock()
    {
        this->m_isLocked = true;

        emit this->lockedChanged(true);
    }

    void TrackSection::unlock()
    {
        this->m_isLocked = false;

        emit this->lockedChanged(false);
    }

    void TrackSection::onLinkTriggered(AxleCounterOperation operation)
    {
        bool counterChanged{false};

        if (operation == AxleCounterOperation::CountDown)
        {
            // Sanity check
            if (this->m_counterValue == 0)
            {
                // clang-format off
                qCWarning(interlocking)
                    << "Track section '"
                    << this->identifier()
                    << "' encountered count down on counter value 0"
                    << Qt::endl;
                // clang-format on
            }
            else
            {
                this->m_counterValue--;
                counterChanged = true;
            }
        }
        else
        {
            this->m_counterValue++;
            counterChanged = true;
        }

        if (counterChanged)
        {
            // Determine whether we need to update the occupancy state.
            if (this->m_counterValue == 0 && this->occupied())
            {
                this->m_isOccupied = false;
                emit this->occupancyChanged(false);
            }
            else if (this->m_counterValue > 0 && !this->occupied())
            {
                this->m_isOccupied = true;
                emit this->occupancyChanged(true);
            }

            emit this->counterValueChanged(this->m_counterValue);
        }
    }
}
