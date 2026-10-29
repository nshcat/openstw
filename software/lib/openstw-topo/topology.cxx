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
#include <expected>
#include <format>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>

namespace Openstw::Topology
{
    Topology::Topology(QObject* parent)
        : QObject{parent}
    {
    }

    std::optional<Path> Topology::findPath(const Node* from, const Node* to, Port fromExitPort, Port toEntryPort) const
    {
        // Validate arguments.
        if (!from || !to)
        {
            return std::nullopt;
        }

        // Then, try to find the vertex descriptors for the passed nodes.
        const auto startVertex = this->lookupNode(from);
        if (!startVertex.has_value())
        {
            return std::nullopt;
        }

        const auto destinationVertex = this->lookupNode(to);
        if (!destinationVertex.has_value())
        {
            return std::nullopt;
        }

        // Setup data structures used for path finding.
        std::priority_queue<PathFindingState, std::vector<PathFindingState>, std::greater<PathFindingState>>
            prioQueue{};

        std::map<PathFindingStateKey, double> minDistanceMap{};
        std::map<PathFindingStateKey, PathFindingPredecessorInfo> predecessorMap{};

        // Initialize path finding state.
        const PathFindingStateKey startKey{*startVertex, Port::None};
        prioQueue.push(PathFindingState{startKey, 0.0});
        minDistanceMap[startKey] = 0.0;

        const PathFindingStateKey destinationKey{*destinationVertex, toEntryPort};
        bool pathFound{false};

        // == Actually perform path search.
        while (!prioQueue.empty())
        {
            // Retrieve current search state
            const auto currentState = prioQueue.top();
            prioQueue.pop();

            const auto currentVertex = currentState.key.vertex();
            const auto currentEntryPort = currentState.key.entryPort();

            // Termination condition: Check if we have reached our destination!
            if (currentState.key == destinationKey)
            {
                pathFound = true;
                break;
            }

            // Skip entries that we already know cant be the shortest path.
            if (currentState.distance > minDistanceMap.at(currentState.key))
            {
                continue;
            }

            // Explore outgoing edges from the current vertex.
            // Here, we make sure to only explore edges connected to ports
            // that we can exit from, given the entry port associated with our
            // search state.
            const auto outgoingEdges = boost::out_edges(currentVertex, this->m_topologyGraph);

            for (auto edgeIterator = outgoingEdges.first; edgeIterator != outgoingEdges.second; ++edgeIterator)
            {
                const auto currentEdge = *edgeIterator;

                // Figure out what the next vertex is - we need to do this
                // since our graph is undirected, and the current vertex might be
                // the one the edge points to! But since we know the current vertex,
                // we can just make sure to pick the other one.
                VertexDescriptor nextVertex{};
                const auto currentEdgeTarget = boost::target(currentEdge, this->m_topologyGraph);
                if (currentEdgeTarget == currentVertex)
                {
                    // If the target of the edge is our current vertex, the next vertex
                    // has to be the source.
                    nextVertex = boost::source(currentEdge, this->m_topologyGraph);
                }
                else
                {
                    // Otherwise it has to be the target of the edge.
                    nextVertex = currentEdgeTarget;
                }

                // We need to figure out from which port we are leaving the current
                // vertex - it might not be allowed, i.e. if we got to the current vertex
                // by entering a switch from the Abzweig port, we can't traverse to a next
                // vertex via the Gerade port of the current vertex!
                const auto currentExitPort = *this->lookupLinkPort(currentEdge, currentVertex);

                // After retrieving the exit port, make sure taking the current edge
                // would not violate traveral rules.
                if (currentVertex == startVertex)
                {
                    // If we are currently dealing with the starting vertex,
                    // we need to make sure we are leaving it from the requested
                    // exit port!
                    if (currentExitPort != fromExitPort)
                    {
                        continue;
                    }
                }
                else
                {
                    // Otherwise, evaluate traversal rules for the node associated with
                    // the current vertex.
                    const auto* currentNode = this->m_topologyGraph[currentVertex].node;

                    if (!currentNode->canTraverse(currentEntryPort, currentExitPort))
                    {
                        continue;
                    }
                }

                // Find out at which port we are entering the next vertex.
                const Port nextEntryPort = *this->lookupLinkPort(currentEdge, nextVertex);

                // Construct state for next step and update search data structures if
                // we have found a new state, or found a better sub path.
                PathFindingStateKey nextKey{nextVertex, nextEntryPort};
                const auto newDistance = currentState.distance + this->m_topologyGraph[currentEdge].length;

                if (!minDistanceMap.contains(nextKey) || newDistance < minDistanceMap[nextKey])
                {
                    minDistanceMap[nextKey] = newDistance;
                    predecessorMap[nextKey] =
                        PathFindingPredecessorInfo{currentState.key, currentEdge, currentExitPort};
                    prioQueue.push(PathFindingState{nextKey, newDistance});
                }
            }
        }
        // ==

        // Did we actually find a path?
        if (!pathFound)
        {
            return std::nullopt;
        }

        // == Construct path sequence from destination to start
        Path path{};

        // Segment representing the destination node has its exit port set to None in the path
        auto nextExitPort = Port::None;
        auto currentKey = destinationKey;

        // Iterate until we have reached the starting node.
        while (currentKey != startKey)
        {
            // Retrieve the predecessor for the current node.
            const auto& currentPredecessor = predecessorMap[currentKey];

            // Create a path segment for the current node.
            auto* currentNode = this->m_topologyGraph[currentKey.vertex()].node;
            path.addNode(currentNode, currentKey.entryPort(), nextExitPort);

            // Also, create a path segment for the edge linking the current and predecessor
            // nodes.
            auto* currentEdge = this->m_topologyGraph[currentPredecessor.takenEdge].edge;
            // Note that even though we are building up the path in reverse order,
            // the from and to ports passed to the edge path segment are in correct order!
            // So, those attributes will not need to be reversed later :)
            path.addEdge(currentEdge, currentPredecessor.exitPort, currentKey.entryPort());

            // Move to next (more correctly, previous x)) segment
            nextExitPort = currentPredecessor.exitPort;
            currentKey = currentPredecessor.previousState;
        }

        // Then, add start segment.
        path.addNode(from, Port::None, fromExitPort);

        // Finally, reverse path.
        path.reverse();

        // ==
        return {path};
    }

    QString Topology::dumpGraph()
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

    std::optional<Topology::VertexDescriptor> Topology::lookupNode(const Node* node) const
    {
        if (!node)
        {
            throw Core::ArgumentError("Nullptr passed as node");
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

    std::optional<Topology::EdgeDescriptor> Topology::lookupEdge(const Edge* edge) const
    {
        if (!edge)
        {
            throw Core::ArgumentError("Nullptr passed as edge");
        }

        // The edge might not be part of the graph.
        const auto iter = this->m_edges.find(edge->identifier());

        if (iter == this->m_edges.end())
        {
            return std::nullopt;
        }

        return std::make_optional<Topology::EdgeDescriptor>(iter->second);
    }

    std::optional<Port> Topology::lookupLinkPort(EdgeDescriptor edge, VertexDescriptor vertex) const
    {
        const auto edgeProperties = this->m_topologyGraph[edge];

        if (boost::source(edge, this->m_topologyGraph) == vertex)
        {
            // If the vertex is the edges source vertex, return the
            // edges 'From' port
            return {edgeProperties.fromPort};
        }
        else if (boost::target(edge, this->m_topologyGraph) == vertex)
        {
            // Otherwise, if the vertex is the edges target vertex, return the
            // edges 'To' port.
            return {edgeProperties.toPort};
        }
        else
        {
            // Edge isn't even connected to the vertex :(
            return std::nullopt;
        }
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
                    throw Core::RuntimeError("Topology XML: Encountered unexpected top level element '{}'",
                                             elementName);
                }
            }
        }
        else
        {
            throw Core::RuntimeError("Topology XML: Expected 'Topology' root element");
        }
    }

    void Topology::populateNodes(QXmlStreamReader* xmlReader)
    {
        while (xmlReader->readNextStartElement())
        {
            // This always needs to be a node element.
            if (xmlReader->name() != "Node")
            {
                throw Core::RuntimeError(
                    "Topology XML: Line {}: Encountered unexpected element in node collection: '{}'",
                    xmlReader->lineNumber(), xmlReader->name());
            }

            // We got a node. Find out what type it is, and what its name is.
            const auto nodeAttributes = xmlReader->attributes();
            const auto nodeTypeName = nodeAttributes.value("Type");
            if (nodeTypeName.isNull())
            {
                throw Core::RuntimeError("Topology XML: Line {}: Node element missing type attribute",
                                         xmlReader->lineNumber());
            }

            const auto nodeName = nodeAttributes.value("Name");
            if (nodeName.isNull())
            {
                throw Core::RuntimeError("Topology XML: Line {}: Node element missing name attribute",
                                         xmlReader->lineNumber());
            }

            // Check whether the identifier is unique.
            if (this->m_elements.contains(nodeName))
            {
                throw Core::RuntimeError("Topology XML: Line {}: Node element has non-unique identifier '{}'",
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
                throw Core::RuntimeError("Topology XML: Line {}: Encountered unknown node type '{}'",
                                         xmlReader->lineNumber(), nodeTypeName);
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
                throw Core::RuntimeError(
                    "Topology XML: Line {}: Encountered unexpected element in edge collection: '{}'",
                    xmlReader->lineNumber(), xmlReader->name());
            }

            // We got an edge. Find out what type it is, and what its name is.
            const auto edgeAttributes = xmlReader->attributes();
            const auto edgeTypeName = edgeAttributes.value("Type");
            if (edgeTypeName.isNull())
            {
                throw Core::RuntimeError("Topology XML: Line {}: Edge element missing type attribute",
                                         xmlReader->lineNumber());
            }

            const auto edgeName = edgeAttributes.value("Name");
            if (edgeName.isNull())
            {
                throw Core::RuntimeError("Topology XML: Line {}: Edge element missing name attribute",
                                         xmlReader->lineNumber());
            }

            // Check whether the identifier is unique.
            if (this->m_elements.contains(edgeName))
            {
                throw Core::RuntimeError("Topology XML: Line {}: Edge element has non-unique identifier '{}'",
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
                    throw Core::RuntimeError("Topology XML: Line {}: Track element {} missing length attribute",
                                             xmlReader->lineNumber(), edgeName);
                }

                const auto length = lengthAttribute.toDouble(&conversionOkay);
                if (!conversionOkay)
                {
                    throw Core::RuntimeError(
                        "Topology XML: Line {}: Track element {} has invalid length attribute value {}",
                        xmlReader->lineNumber(), edgeName, lengthAttribute);
                }

                const auto maxSpeedAttribute = edgeAttributes.value("MaximumSpeed");
                if (maxSpeedAttribute.isNull())
                {
                    throw Core::RuntimeError("Topology XML: Line {}: Track element {} missing maximum speed attribute",
                                             xmlReader->lineNumber(), edgeName);
                }

                const auto maxSpeed = maxSpeedAttribute.toDouble(&conversionOkay);
                if (!conversionOkay)
                {
                    throw Core::RuntimeError(
                        "Topology XML: Line {}: Track element {} has invalid maximum speed attribute value {}",
                        xmlReader->lineNumber(), edgeName, maxSpeedAttribute);
                }

                // Finally, construct track object
                newEdge = new Track(edgeName.toString(), length, maxSpeed, this);
            }
            else
            {
                throw Core::RuntimeError("Topology XML: Line {}: Encountered unknown edge type '{}'",
                                         xmlReader->lineNumber(), edgeTypeName);
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
                        throw Core::RuntimeError(
                            "Topology XML: Line {}: Encountered invalid subelement '{}' within Edge element",
                            xmlReader->lineNumber(), elementName);
                    }
                }

                // Assert that we have found both a from and to link
                if (!fromLink.has_value() || !toLink.has_value())
                {
                    throw Core::RuntimeError(
                        "Topology XML: Line {}: Edge '{}' requires both a From and To link element",
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
            throw Core::RuntimeError("Topology XML: Line {}: Edge link element has no port attribute",
                                     xmlReader->lineNumber());
        }

        const auto port = parsePort(portName);

        if (!port.has_value())
        {
            throw Core::RuntimeError("Topology XML: Line {}: Edge link element has invalid port attribute value '{}'",
                                     xmlReader->lineNumber(), portName);
        }

        link.NodePort = port.value();

        // The referenced node is specified by its identifier, given as the element text.
        const auto nodeIdentifier = xmlReader->readElementText();

        // Try to find the node object pointer
        const auto node = this->findNode(nodeIdentifier);

        if (!node.has_value())
        {
            throw Core::RuntimeError("Topology XML: Line {}: Link refers to nonexistant Node '{}'",
                                     xmlReader->lineNumber(), nodeIdentifier);
        }

        link.TargetNode = node.value();

        // Look up its vertex descriptor
        const auto nodeDescriptor = this->lookupNode(link.TargetNode);

        if (!nodeDescriptor.has_value())
        {
            throw Core::RuntimeError(
                "Topology XML: Line {}: Link refers to Node '{}' which is not part of the topology graph",
                xmlReader->lineNumber(), nodeIdentifier);
        }

        link.TargetNodeDescriptor = nodeDescriptor.value();

        // Now, perform validation.
        // First, check if the node actually has a matching port.
        if (!link.TargetNode->hasPort(link.NodePort))
        {
            throw Core::RuntimeError("Topology XML: Line {}: Edge refers to nonexisting source port '{}' on node '{}'",
                                     xmlReader->lineNumber(), portToString(link.NodePort),
                                     link.TargetNode->identifier());
        }

        // Then, check that the port of the node is not already used.
        // XXX use boost::out_edges

        return link;
    }

    const Topology::VertexDescriptor& Topology::PathFindingStateKey::vertex() const
    {
        return this->first;
    }

    Topology::PathFindingStateKey::PathFindingStateKey()
        : PathFindingStateKey({}, Port::None)
    {
    }

    Topology::PathFindingStateKey::PathFindingStateKey(const VertexDescriptor& vertex, Port entryPort)
        : BaseType{vertex, entryPort}
    {
    }

    Port Topology::PathFindingStateKey::entryPort() const
    {
        return this->second;
    }

    bool Topology::PathFindingState::operator>(const PathFindingState& other) const
    {
        return this->distance > other.distance;
    }
}
