#include "simulation.hxx"

namespace Openstw::SpDr
{
    Simulation::Simulation(Interlocking::Interlocking* interlocking, Stelltisch* stelltisch, QObject* parent)
        : QObject{parent}, m_interlocking{interlocking}, m_stelltisch{stelltisch}
    {
        // Parent the stelltisch to this object for
        // lifetime management purposes
        this->m_stelltisch->setParent(this);

        // == Create input processor, and hook up button signals emitted
        // from the stelltisch to its state change processing slot.
        this->m_input = new InputProcessor(this);

        // clang-format off
        connect(this->m_stelltisch, &Stelltisch::buttonStateChanged,
                this->m_input, &InputProcessor::onButtonStateChanged);
        // clang-format on
        // ==
    }

    const Stelltisch* Simulation::stelltisch() const
    {
        return this->m_stelltisch;
    }

    Stelltisch* Simulation::stelltisch()
    {
        return this->m_stelltisch;
    }

    const Interlocking::Interlocking* Simulation::interlocking() const
    {
        return this->m_interlocking;
    }

    Interlocking::Interlocking* Simulation::interlocking()
    {
        return this->m_interlocking;
    }
}
