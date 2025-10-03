#pragma once

#include "enums.hxx"
#include "switch.hxx"

namespace Openstw::Simulation
{
    enum class SimpleSwitchDirection
    {
        Straight = 0,
        Branched = 1
    };

    enum class SimpleSwitchPart
    {
        Stem = 0,
        Straight = 1,
        Branch = 2
    };

    class SimpleSwitch : public Switch
    {
    public:
        SimpleSwitch();

    public:
        static std::unique_ptr<Switch> CreateFrom(const pugi::xml_node&);

    public:
        SimpleSwitchDirection currentDirection() const;
        TrackState partState(const SimpleSwitchPart) const;
        SwitchBranchDirection branchDirection() const;

    protected:
        SimpleSwitchDirection m_currentDirection{SimpleSwitchDirection::Straight};
        TrackState m_stemState{TrackState::Inactive};
        TrackState m_straightState{TrackState::Inactive};
        TrackState m_branchState{TrackState::Inactive};
        SwitchBranchDirection m_branchDirection{SwitchBranchDirection::Up};
    };
}
