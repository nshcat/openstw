#include "port.hxx"
#include "exception.hxx"
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
            else if (portName == "None")
            {
                return Port::None;
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
            case Port::None:
                return "None";
            default:
                return "<unknown>";
            }
        }

        std::ostream& operator<<(std::ostream& os, const Port& port)
        {
            const auto portName = portToString(port);

            os << portName.toStdString();

            return os;
        }

        std::istream& operator>>(std::istream& is, Port& port)
        {
            std::string portName{};

            is >> portName;

            const auto maybePort = parsePort(QString::fromStdString(portName));

            if (!maybePort.has_value())
            {
                throw RuntimeError("Encountered invalid port name '{}'", portName);
            }

            port = *maybePort;

            return is;
        }
    }
}