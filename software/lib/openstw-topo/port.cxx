#include "port.hxx"
#include <QString>

namespace Openstw
{
    namespace Topology
    {
        std::optional<Port> parsePort(QStringView portName)
        {
            if (portName == "Center")
            {
                return Port::Center;
            }
            else if (portName == "Front")
            {
                return Port::Front;
            }
            else if (portName == "Back")
            {
                return Port::Back;
            }
            else if (portName == "Wurzel")
            {
                return Port::Wurzel;
            }
            else if (portName == "Abzweig")
            {
                return Port::Abzweig;
            }
            else if (portName == "Gerade")
            {
                return Port::Gerade;
            }

            return std::nullopt;
        }

        QString portToString(Port port)
        {
            switch (port)
            {
            case Port::Center:
                return "Center";
            case Port::Back:
                return "Back";
            case Port::Front:
                return "Front";
            case Port::Wurzel:
                return "Wurzel";
            case Port::Gerade:
                return "Gerade";
            case Port::Abzweig:
                return "Abzweig";
            default:
                return "<unknown>";
            }
        }
    }
}