#include <QString>

#include "simpleswitch.hxx"
#include "switch.hxx"
#include "utility.hxx"

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

        std::unique_ptr<Switch> sw;

        if (typeStr == "simple")
            sw = SimpleSwitch::CreateFrom(node);

        // Parse general attributes
        if (sw)
        {
            sw->m_direction = Xml::parseDirection(node.attribute("direction"));
            sw->m_label = node.attribute("label").as_string("");

            return std::move(sw);
        }

        throw std::runtime_error("Invalid switch type");
    }

    TileElementDirection Switch::direction() const
    {
        return this->m_direction;
    }

    bool Switch::hasLabel() const
    {
        return !this->m_label.isEmpty();
    }

    const QString& Switch::label() const
    {
        return this->m_label;
    }
}
