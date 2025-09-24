#include <QString>

#include "simpleswitch.hxx"
#include "switch.hxx"

namespace Openstw::Simulation
{
    Switch::Switch(const SwitchType type) : m_type{type}
    {
    }

    SwitchType Switch::type() const
    {
        return this->m_type;
    }

    std::unique_ptr<Switch> Switch::CreateFrom(const pugi::xml_node& node)
    {
        const QString typeStr = node.attribute("type").as_string("");
        if (typeStr.isEmpty())
            throw std::runtime_error("Invalid switch type");

        if (typeStr == "simple")
            return SimpleSwitch::CreateFrom(node);

        throw std::runtime_error("Invalid switch type");
    }
}
