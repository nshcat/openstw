#include "system.hxx"
#include "element.hxx"
#include "elements/tracksection.hxx"
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    System::System(Topology::Topology* topology, QObject* parent)
        : QObject{parent}, m_topology{topology}
    {
    } 

    Topology::Topology* System::topology() const
    {
        return this->m_topology;
    }

    void System::handleRequest(Request* request)
    {
        // XXXTODO
        return;
    }

    void System::populateFromXml(QDomElement xmlElement)
    {
        // First, validate that the Interlocking element isnt empty.
        if (!xmlElement.hasChildNodes())
        {
            throw Core::RuntimeError("Interlocking XML: Encountered empty Interlocking element");
        }

        // == Then, start with parsing track sections.
        // Some other types of elements (such as signals) can have references
        // to those!
        auto sectionsElement = xmlElement.namedItem("TrackSections");

        if (sectionsElement.isNull())
        {
            throw Core::RuntimeError("Interlocking XML: Expected <TrackSections> element");
        }

        if (!sectionsElement.isElement())
        {
            throw Core::RuntimeError("Interlocking XML: Expected <TrackSections> element");
        }

        // Actually read and construct track sections.
        this->populateSections(sectionsElement.toElement());
        // ==

        // == Next, parse switches.
        auto switchesElement = xmlElement.namedItem("Switches");

        if (switchesElement.isNull())
        {
            throw Core::RuntimeError("Interlocking XML: Expected <Switches> element");
        }

        if (!switchesElement.isElement())
        {
            throw Core::RuntimeError("Interlocking XML: Expected <Switches> element");
        }

        // Actually read and construct switches.
        this->populateSwitches(switchesElement.toElement());
        //==
    }

    void System::populateSections(QDomElement xmlElement)
    {
        // Iterate over all available <TrackSection> child elements.
        auto sectionElement = xmlElement.firstChildElement("TrackSection");
        while (!sectionElement.isNull())
        {
            // Read section name
            const auto sectionName = sectionElement.attribute("Name");
            if (sectionName.isEmpty())
            {
                throw Core::RuntimeError("Interlocking XML: Line {}: Track section element missing name attribute",
                                         xmlElement.lineNumber());
            }

            // Check whether the identifier is unique.
            if (this->m_elements.contains(sectionName))
            {
                throw Core::RuntimeError(
                    "Interlocking XML: Line {}: Track section element has non-unique identifier '{}'",
                    xmlElement.lineNumber(), sectionName);
            }

            // Parse the axle counter link elements within the track section
            // element.
            QList<AxleCounterLink*> links{};
            auto linkElement = sectionElement.firstChildElement("AxleCounterLink");
            while (!linkElement.isNull())
            {
                // == Retrieve axle counter topology node refered to by the link.
                // Parse axle counter name.
                const auto axleCounterName = linkElement.attribute("AxleCounter");
                if (axleCounterName.isEmpty())
                {
                    throw Core::RuntimeError(
                        "Interlocking XML: Line {}: Axle counter link element missing axle counter attribute",
                        xmlElement.lineNumber());
                }

                // Try to find the axle counter within the topology graph.
                auto axleCounterNode = this->topology()->findNode(axleCounterName);
                if (!axleCounterNode.has_value())
                {
                    throw Core::RuntimeError("Interlocking XML: Line {}: Axle counter link element refers to "
                                             "non-existing axle counter node '{}'",
                                             xmlElement.lineNumber(), axleCounterName);
                }

                // Check that is, in fact, an axle counter
                const auto nodeType = axleCounterNode.value()->type();
                if (nodeType != Topology::NodeType::AxleCounter)
                {
                    throw Core::RuntimeError("Interlocking XML: Line {}: Axle counter link element refers to "
                                             "topology node '{}' with wrong node type",
                                             xmlElement.lineNumber(), axleCounterName);
                }

                // Cast it.
                auto* axleCounter = dynamic_cast<Topology::AxleCounter*>(axleCounterNode.value());
                // ==

                // == Parse the link facing direction.
                const auto axleCounterFacing = linkElement.attribute("Facing");
                if (axleCounterFacing.isEmpty())
                {
                    throw Core::RuntimeError(
                        "Interlocking XML: Line {}: Axle counter link element missing facing attribute",
                        xmlElement.lineNumber());
                }

                AxleCounterFacing linkFacing{AxleCounterFacing::Inward};

                if (axleCounterFacing == "Inward")
                {
                    linkFacing = AxleCounterFacing::Inward;
                }
                else if (axleCounterFacing == "Outward")
                {
                    linkFacing = AxleCounterFacing::Outward;
                }
                else
                {
                    throw Core::RuntimeError(
                        "Interlocking XML: Line {}: Axle counter link element specifies invalid facing value '{}'",
                        xmlElement.lineNumber(), axleCounterFacing);
                }
                // ==

                auto* link = new AxleCounterLink(axleCounter, linkFacing);
                links.append(link);

                linkElement = linkElement.nextSiblingElement("AxleCounterLink");
            }

            // Construct track section element, and associate the previously
            // parsed axle counter links with it.
            auto* section = new TrackSection(sectionName, links, this);

            // Store it within the interlocking system.
            this->m_elements.insert(sectionName, section);

            // Continue with the next <TrackSection> element, if it exists.
            sectionElement = sectionElement.nextSiblingElement("TrackSection");
        }

        return;
    }

    void System::populateSwitches(QDomElement xmlElement)
    {
        // Iterate over all available <Switch> child elements.
        auto switchElement = xmlElement.firstChildElement("Switch");
        while (!switchElement.isNull())
        {
            // Read switch name
            const auto switchName = switchElement.attribute("Name");
            if (switchName.isEmpty())
            {
                throw Core::RuntimeError("Interlocking XML: Line {}: Switch element missing name attribute",
                                         xmlElement.lineNumber());
            }

            // Check whether the identifier is unique.
            if (this->m_elements.contains(switchName))
            {
                throw Core::RuntimeError("Interlocking XML: Line {}: Switch element has non-unique identifier '{}'",
                                         xmlElement.lineNumber(), switchName);
            }

            // == Parse and retrieve the switch topology node for the switch
            // Read reference attribute
            const auto refName = switchElement.attribute("Reference");
            if (refName.isEmpty())
            {
                throw Core::RuntimeError("Interlocking XML: Line {}: Switch element missing reference attribute",
                                         xmlElement.lineNumber());
            }

            // Try to find the corresponding node within the topology graph
            auto switchNodeBase = this->topology()->findNode(refName);
            if (!switchNodeBase.has_value())
            {
                throw Core::RuntimeError("Interlocking XML: Line {}: Switch element refers to "
                                         "non-existing switch node '{}'",
                                         xmlElement.lineNumber(), refName);
            }

            // Check that is, in fact, a switch
            const auto nodeType = switchNodeBase.value()->type();
            if (nodeType != Topology::NodeType::SimpleSwitch)
            {
                throw Core::RuntimeError("Interlocking XML: Line {}: Switch element refers to "
                                         "topology node '{}' with wrong node type",
                                         xmlElement.lineNumber(), refName);
            }

            // Cast it.
            auto* switchNode = dynamic_cast<Topology::SimpleSwitch*>(switchNodeBase.value());
            // ==

            // Finally, construct switch interlocking element.
            auto* switchObj = new Switch(switchName, switchNode, this);

            // Add it to the interlocking system.
            this->m_elements.insert(switchName, switchObj);

            // Continue with the next <Switch> element, if it exists.
            switchElement = switchElement.nextSiblingElement("Switch");
        }

        return;
    }

    System* System::createFromXml(Topology::Topology* topology, QDomElement xmlElement, QObject* parent)
    {
        auto* system = new System{topology, parent};

        system->populateFromXml(xmlElement);

        return system;
    }
}