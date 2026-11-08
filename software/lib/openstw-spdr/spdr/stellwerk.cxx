#include "stellwerk.hxx"

namespace Openstw::SpDr
{
    Stellwerk::Stellwerk(Interlocking::System* interlocking, Stelltisch* stelltisch, QObject* parent)
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

    const Stelltisch* Stellwerk::stelltisch() const
    {
        return this->m_stelltisch;
    }

    Stelltisch* Stellwerk::stelltisch()
    {
        return this->m_stelltisch;
    }

    const Interlocking::System* Stellwerk::interlocking() const
    {
        return this->m_interlocking;
    }

    Interlocking::System* Stellwerk::interlocking()
    {
        return this->m_interlocking;
    }
}
