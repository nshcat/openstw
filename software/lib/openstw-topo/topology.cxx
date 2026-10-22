#include "topology.hxx"
#include "edges/track.hxx"
#include "exception.hxx"
#include "nodes/axlecounter.hxx"
#include "nodes/boundary.hxx"
#include "nodes/bumper.hxx"
#include "nodes/connector.hxx"
#include "nodes/routedestination.hxx"
#include "nodes/signal.hxx"
#include "nodes/simpleswitch.hxx"
#include "utility.hxx"
#include <QFile>
#include <boost/graph/graphviz.hpp>
#include <format>
#include <sstream>
#include <stdexcept>

namespace Openstw::Topology
{
    Topology::Topology(QObject* parent)
        : QObject{parent}
    {
    }

    std::optional<Path> Topology::findPath(Node* from, Node* to) const
    {
        return std::nullopt;
    }

    QString Topology::dumpAsGraphVizDot()
    {
        boost::dynamic_properties dp;
        dp.property("node_id", get(&VertexProperties::identifier, this->m_topologyGraph));
        dp.property("weight", get(&EdgeProperties::length, this->m_topologyGraph));
        dp.property("taillabel", get(&EdgeProperties::fromPort, this->m_topologyGraph));
        dp.property("headlabel", get(&EdgeProperties::toPort, this->m_topologyGraph));

        std::ostringstream dotOutput{};

        write_graphviz_dp(dotOutput, this->m_topologyGraph, dp, "node_id");

        return QString::fromStdString(dotOutput.str());
    }

    std::optional<Topology::VertexDescriptor> Topology::lookupNode(Node* node) const
    {
        if (!node)
        {
            throw ArgumentError("Nullptr passed as node");
        }

        return this->lookupNode(node->identifier());
    }

    std::optional<Topology::VertexDescriptor> Topology::lookupNode(QStringView nodeIdentifier) const
    {
        // The node might not be part of the graph.
        const auto iter = this->m_nodes.find(nodeIdentifier);

        if (iter == this->m_nodes.end())
        {
            return std::nullopt;
        }

        return std::make_optional<Topology::VertexDescriptor>(iter->second);
    }

    std::optional<Topology::EdgeDescriptor> Topology::lookupEdge(Edge* edge) const
    {
        if (!edge)
        {
            throw ArgumentError("Nullptr passed as edge");
        }

        // The edge might not be part of the graph.
        const auto iter = this->m_edges.find(edge->identifier());

        if (iter == this->m_edges.end())
        {
            return std::nullopt;
        }

        return std::make_optional<Topology::EdgeDescriptor>(iter->second);
    }

    Topology* Topology::readFromFile(const QString& filePath, QObject* parent)
    {
        auto* topology = new Topology(parent);

        QFile xmlFile{filePath};

        if (!xmlFile.open(QFile::ReadOnly | QFile::Text))
        {
            throw std::invalid_argument("Failed to open topology XML file");
        }

        QXmlStreamReader xmlReader{&xmlFile};

        topology->populateFromXml(&xmlReader);

        return topology;
    }

    std::optional<Node*> Topology::findNode(const QString& identifier) const
    {
        const auto it = this->m_elements.find(identifier);
        if (it == this->m_elements.end())
        {
            return std::nullopt;
        }

        if (!it.value()->isNode())
        {
            return std::nullopt;
        }

        return std::make_optional(dynamic_cast<Node*>(it.value()));
    }

    std::optional<Element*> Topology::findElement(const QString& identifier) const
    {
        const auto it = this->m_elements.find(identifier);
        if (it == this->m_elements.end())
        {
            return std::nullopt;
        }

        return std::make_optional<Element*>(*it);
    }

    void Topology::populateFromXml(QXmlStreamReader* xmlReader)
    {
        if (xmlReader->readNextStartElement() && xmlReader->name() == "Topology")
        {
            while (xmlReader->readNextStartElement())
            {
                // Determine which collection of elements this is.
                const auto& elementName = xmlReader->name();

                if (elementName == "Nodes")
                {
                    this->populateNodes(xmlReader);
                }
                else if (elementName == "Edges")
                {
                    this->populateEdges(xmlReader);
                }
                else
                {
                    throw RuntimeError("Topology XML: Encountered unexpected top level element '{}'", elementName);
                }
            }
        }
        else
        {
            throw RuntimeError("Topology XML: Expected 'Topology' root element");
        }
    }

    void Topology::populateNodes(QXmlStreamReader* xmlReader)
    {
        while (xmlReader->readNextStartElement())
        {
            // This always needs to be a node element.
            if (xmlReader->name() != "Node")
            {
                throw RuntimeError("Topology XML: Line {}: Encountered unexpected element in node collection: '{}'",
                                   xmlReader->lineNumber(), xmlReader->name());
            }

            // We got a node. Find out what type it is, and what its name is.
            const auto nodeAttributes = xmlReader->attributes();
            const auto nodeTypeName = nodeAttributes.value("Type");
            if (nodeTypeName.isNull())
            {
                throw RuntimeError("Topology XML: Line {}: Node element missing type attribute",
                                   xmlReader->lineNumber());
            }

            const auto nodeName = nodeAttributes.value("Name");
            if (nodeName.isNull())
            {
                throw RuntimeError("Topology XML: Line {}: Node element missing name attribute",
                                   xmlReader->lineNumber());
            }

            // Check whether the identifier is unique.
            if (this->m_elements.contains(nodeName))
            {
                throw RuntimeError("Topology XML: Line {}: Node element has non-unique identifier '{}'",
                                   xmlReader->lineNumber(), nodeName);
            }

            // Construct node object instance of the correct type,
            // reading more information as required by each specific node type.
            Node* newNode{nullptr};

            if (nodeTypeName == "Signal")
            {
                newNode = new Signal(nodeName.toString(), this);
            }
            else if (nodeTypeName == "Connector")
            {
                newNode = new Connector(nodeName.toString(), this);
            }
            else if (nodeTypeName == "Boundary")
            {
                newNode = new Boundary(nodeName.toString(), this);
            }
            else if (nodeTypeName == "AxleCounter")
            {
                newNode = new AxleCounter(nodeName.toString(), this);
            }
            else if (nodeTypeName == "Bumper")
            {
                newNode = new Bumper(nodeName.toString(), this);
            }
            else if (nodeTypeName == "RouteDestination")
            {
                newNode = new RouteDestination(nodeName.toString(), this);
            }
            else if (nodeTypeName == "SimpleSwitch")
            {
                newNode = new SimpleSwitch(nodeName.toString(), this);
            }
            else
            {
                throw RuntimeError("Topology XML: Line {}: Encountered unknown node type '{}'", xmlReader->lineNumber(),
                                   nodeTypeName);
            }

            // Have we actually constructed a new node?
            // If so, add it to our collections, and to the graph.
            if (newNode != nullptr)
            {
                this->m_elements.insert(newNode->identifier(), newNode);

                const auto nodeDescriptor = boost::add_vertex({newNode, newNode->identifier()}, this->m_topologyGraph);

                this->m_nodes.insert(newNode->identifier(), qMakePair(newNode, nodeDescriptor));
            }

            xmlReader->skipCurrentElement();
        }
    }

    void Topology::populateEdges(QXmlStreamReader* xmlReader)
    {
        while (xmlReader->readNextStartElement())
        {
            // This always needs to be a edge element.
            if (xmlReader->name() != "Edge")
            {
                throw RuntimeError("Topology XML: Line {}: Encountered unexpected element in edge collection: '{}'",
                                   xmlReader->lineNumber(), xmlReader->name());
            }

            // We got an edge. Find out what type it is, and what its name is.
            const auto edgeAttributes = xmlReader->attributes();
            const auto edgeTypeName = edgeAttributes.value("Type");
            if (edgeTypeName.isNull())
            {
                throw RuntimeError("Topology XML: Line {}: Edge element missing type attribute",
                                   xmlReader->lineNumber());
            }

            const auto edgeName = edgeAttributes.value("Name");
            if (edgeName.isNull())
            {
                throw RuntimeError("Topology XML: Line {}: Edge element missing name attribute",
                                   xmlReader->lineNumber());
            }

            // Check whether the identifier is unique.
            if (this->m_elements.contains(edgeName))
            {
                throw RuntimeError("Topology XML: Line {}: Edge element has non-unique identifier '{}'",
                                   xmlReader->lineNumber(), edgeName);
            }

            // Construct edge based on type given in attributes
            Edge* newEdge{nullptr};

            if (edgeTypeName == "Track")
            {
                bool conversionOkay{false};

                // We need to read the length and maximum speed for tracks
                const auto lengthAttribute = edgeAttributes.value("Length");
                if (lengthAttribute.isNull())
                {
                    throw RuntimeError("Topology XML: Line {}: Track element {} missing length attribute",
                                       xmlReader->lineNumber(), edgeName);
                }

                const auto length = lengthAttribute.toDouble(&conversionOkay);
                if (!conversionOkay)
                {
                    throw RuntimeError("Topology XML: Line {}: Track element {} has invalid length attribute value {}",
                                       xmlReader->lineNumber(), edgeName, lengthAttribute);
                }

                const auto maxSpeedAttribute = edgeAttributes.value("MaximumSpeed");
                if (maxSpeedAttribute.isNull())
                {
                    throw RuntimeError("Topology XML: Line {}: Track element {} missing maximum speed attribute",
                                       xmlReader->lineNumber(), edgeName);
                }

                const auto maxSpeed = maxSpeedAttribute.toDouble(&conversionOkay);
                if (!conversionOkay)
                {
                    throw RuntimeError(
                        "Topology XML: Line {}: Track element {} has invalid maximum speed attribute value {}",
                        xmlReader->lineNumber(), edgeName, maxSpeedAttribute);
                }

                // Finally, construct track object
                newEdge = new Track(edgeName.toString(), length, maxSpeed, this);
            }
            else
            {
                throw RuntimeError("Topology XML: Line {}: Encountered unknown edge type '{}'", xmlReader->lineNumber(),
                                   edgeTypeName);
            }

            // Have we actually created a new Edge?
            if (newEdge != nullptr)
            {
                // Every edge element contains sub elements describing
                // which elements and ports it connects.
                // Figure out which ports on which nodes the edge connects.
                std::optional<EdgeLink> fromLink, toLink;

                // Try to find the "From" and "To" elements
                while (xmlReader->readNextStartElement())
                {
                    const auto elementName = xmlReader->name();

                    if (elementName == "From" || elementName == "To")
                    {
                        // Found link element, parse it
                        const auto link = this->parseAndVerifyLink(xmlReader);

                        if (elementName == "From")
                        {
                            fromLink = link;
                        }
                        else
                        {
                            toLink = link;
                        }
                    }
                    else
                    {
                        throw RuntimeError(
                            "Topology XML: Line {}: Encountered invalid subelement '{}' within Edge element",
                            xmlReader->lineNumber(), elementName);
                    }
                }

                // Assert that we have found both a from and to link
                if (!fromLink.has_value() || !toLink.has_value())
                {
                    throw RuntimeError("Topology XML: Line {}: Edge '{}' requires both a From and To link element",
                                       xmlReader->lineNumber(), edgeName);
                }

                // Actually insert edge into graph
                const auto edgeDescriptor =
                    boost::add_edge(fromLink->TargetNodeDescriptor, toLink->TargetNodeDescriptor,
                                    EdgeProperties{newEdge, newEdge->identifier(), newEdge->weight(),
                                                   fromLink->NodePort, toLink->NodePort},
                                    this->m_topologyGraph)
                        .first;

                this->m_edges.insert(newEdge->identifier(), qMakePair(newEdge, edgeDescriptor));
                this->m_elements.insert(newEdge->identifier(), newEdge);
            }
        }     
    }

    Topology::EdgeLink Topology::parseAndVerifyLink(QXmlStreamReader* xmlReader)
    {
        // Create edge link object
        EdgeLink link;

        // Read the port attribute.
        const auto portName = xmlReader->attributes().value("Port");

        if (portName.isNull())
        {
            throw RuntimeError("Topology XML: Line {}: Edge link element has no port attribute",
                               xmlReader->lineNumber());
        }

        const auto port = parsePort(portName);

        if (!port.has_value())
        {
            throw RuntimeError("Topology XML: Line {}: Edge link element has invalid port attribute value '{}'",
                               xmlReader->lineNumber(), portName);
        }

        link.NodePort = port.value();

        // The referenced node is specified by its identifier, given as the element text.
        const auto nodeIdentifier = xmlReader->readElementText();

        // Try to find the node object pointer
        const auto node = this->findNode(nodeIdentifier);

        if (!node.has_value())
        {
            throw RuntimeError("Topology XML: Line {}: Link refers to nonexistant Node '{}'", xmlReader->lineNumber(),
                               nodeIdentifier);
        }

        link.TargetNode = node.value();

        // Look up its vertex descriptor
        const auto nodeDescriptor = this->lookupNode(link.TargetNode);

        if (!nodeDescriptor.has_value())
        {
            throw RuntimeError(
                "Topology XML: Line {}: Link refers to Node '{}' which is not part of the topology graph",
                xmlReader->lineNumber(), nodeIdentifier);
        }

        link.TargetNodeDescriptor = nodeDescriptor.value();

        // Now, perform validation.
        // First, check if the node actually has a matching port.
        if (!link.TargetNode->hasPort(link.NodePort))
        {
            throw RuntimeError("Topology XML: Line {}: Edge refers to nonexisting source port '{}' on node '{}'",
                               xmlReader->lineNumber(), portToString(link.NodePort), link.TargetNode->identifier());
        }

        // Then, check that the port of the node is not already used.
        // XXX use boost::out_edges

        return link;
    }
}
