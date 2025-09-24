#include "simpleswitch.hxx"

namespace Openstw::Simulation
{
    SimpleSwitch::SimpleSwitch() : Switch{SwitchType::Simple}
    {
    }

    std::unique_ptr<Switch> SimpleSwitch::CreateFrom(const pugi::xml_node&)
    {
        return std::make_unique<SimpleSwitch>();
    }

    SimpleSwitchDirection SimpleSwitch::currentDirection() const
    {
        return this->m_currentDirection;
    }

    TrackState SimpleSwitch::partState(const SimpleSwitchPart part) const
    {
        switch (part)
        {
        case SimpleSwitchPart::Stem:
            return this->m_stemState;
        case SimpleSwitchPart::Branch:
            return this->m_branchState;
        default:
            return this->m_straightState;
        }
    }
}
