#include <QString>

#include "simpleswitch.hxx"

namespace Openstw::Simulation
{
    SimpleSwitch::SimpleSwitch() : Switch{SwitchType::Simple}
    {
    }

    std::unique_ptr<Switch> SimpleSwitch::CreateFrom(const pugi::xml_node& node)
    {
        auto sw = std::make_unique<SimpleSwitch>();

        SwitchBranchDirection branchDirection{SwitchBranchDirection::Up};
        const QString branchDirectionStr{node.attribute("branchDirection").as_string("up")};
        if (branchDirectionStr == "down")
            branchDirection = SwitchBranchDirection::Down;
        sw->m_branchDirection = branchDirection;

        return std::move(sw);
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
