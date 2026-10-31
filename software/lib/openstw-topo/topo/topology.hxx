#pragma once

#include "edge.hxx"
#include "element.hxx"
#include "node.hxx"
#include "path.hxx"
#include "port.hxx"

#include <QHash>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QXmlStreamReader>
#include <boost/graph/adjacency_list.hpp>
#include <optional>

namespace Openstw::Topology
{
    // Class containing the topology graph, which describes
    // the physical layout of the railway network.
    // It holds direct ownership of all node and edge object instances,
    // the graph is just a an adjacent view on those objects that describes
    // their relationships.
    class Topology : public QObject
    {
        Q_OBJECT;

    protected: // Structures and type alias for the topology graph
        struct VertexProperties
        {
            // Node object associated with this vertex
            Node* node;

            // Node identifier
            QString identifier;
        };

        struct EdgeProperties
        {
            // Edge object associated with this edge
            Edge* edge;

            // Edge identifier
            QString identifier;

            // Edge length
            double length;

            // Port the edge is connected to at the source
            Port fromPort;

            // Port the edge is connected to at the destination
            Port toPort;
        };

        using GraphType =
            boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperties, EdgeProperties>;
        using VertexDescriptor = boost::graph_traits<GraphType>::vertex_descriptor;
        using EdgeDescriptor = boost::graph_traits<GraphType>::edge_descriptor;

    protected: // Internal helper structures
        // Object describing a link between an edge and a port on a node
        struct EdgeLink
        {
            // Vertex descriptor for the node that is referred
            // to by this link
            VertexDescriptor TargetNodeDescriptor;

            // Node this link refers to
            Node* TargetNode;

            // Which port of the target node this link refers to
            Port NodePort;
        };

        // State of graph path finding is unique if we attach the port from which we have entered to
        // the vertex we are in.
        struct PathFindingStateKey : public std::pair<VertexDescriptor, Port>
        {
            using BaseType = std::pair<VertexDescriptor, Port>;

            PathFindingStateKey();

            PathFindingStateKey(const VertexDescriptor& vertex, Port entryPort);

            // The vertex we are currently at.
            const VertexDescriptor& vertex() const;

            // The port from which we have entered the vertex.
            Port entryPort() const;
        };

        // State of path finding algorithm within topology graph
        struct PathFindingState
        {
            // Uniquely identifies the path finding state - since our graph
            // is augmented by the port system, we have to remember on which
            // port we entered the current vertex.
            PathFindingStateKey key;

            // Accumulated distance so far
            double distance;

            bool operator>(const PathFindingState& other) const;
        };

        // Information about a states predecessor
        struct PathFindingPredecessorInfo
        {
            // The state the algorithm was in before arriving at the state
            // that has this predecessor info associated with it
            PathFindingStateKey previousState;

            // Edge the algorithm has taken to get from the predecessor state to
            // next state
            EdgeDescriptor takenEdge;

            // Port used to leave vertex associated with previous state
            Port exitPort;
        };

    protected:
        Topology(QObject* parent);

    public: // == Static methods
        // Read topology data from given XML file and return topology network object
        // instance.
        static Topology* readFromFile(const QString& filePath, QObject* parent = nullptr);

    public: // == Interacting with the topology network
        // Retrieve topology network element by its identifier.
        std::optional<Element*> findElement(const QString& identifier) const;

        // Retrieve node within topology network by its identifier.
        std::optional<Node*> findNode(const QString& identifier) const;

        // Find a path from given node to given other node, with given ports as exit
        // and entry.
        std::optional<Path> findPath(const Node* from, const Node* to, Port fromExitPort, Port toEntryPort) const;

    public: // == Debug methods
        QString dumpGraph();

    protected: // == Interaction with topology graph
        // Retrieve graph vertex descriptor for given node, if the node
        // is part of the topology graph.
        std::optional<VertexDescriptor> lookupNode(const Node* node) const;

        // Retrieve graph vertex descriptor for given node identifier, if the node
        // is part of the topology graph.
        std::optional<VertexDescriptor> lookupNode(QStringView nodeIdentifier) const;

        // Retrieve graph edge descriptor for given edge, if the edge
        // is part of the topology graph.
        std::optional<EdgeDescriptor> lookupEdge(const Edge* edge) const;

        // Retrieve the port that the given edge is connected to given vertex on.
        std::optional<Port> lookupLinkPort(EdgeDescriptor edge, VertexDescriptor vertex) const;

    protected: // == Deserializing
        // Populate topology network contents from given XML reader.
        void populateFromXml(QXmlStreamReader* xmlReader);

        // Read node collection from XML document.
        void populateNodes(QXmlStreamReader* xmlReader);

        // Read edges between nodes from XML document.
        void populateEdges(QXmlStreamReader* xmlReader);

        // Parse a "From" or "To" connection element within an Edge element.
        // This requires that the tag was already entered - the method
        // does not care whether this is a "From" or "To" element!
        EdgeLink parseAndVerifyLink(QXmlStreamReader* xmlReader);

    protected: // == Attributes
        // Collection of all topology elements associated with the railway network.
        // Keyed with the elements unique identifier.
        QHash<QString, Element*> m_elements{};

        // Specific collections for the nodes and edges of the topology graph.
        // The objects contained within these collections are also contained
        // within the general element collection, but the pairs also contain
        // the graph vertex or edge descriptors that can be used to retrieve
        // the graph vertex or edge corresponding to a specific element.
        // These collections are also keyed with the unique identifiers of
        // the nodes and edges.
        QHash<QString, QPair<Node*, VertexDescriptor>> m_nodes{};
        QHash<QString, QPair<Edge*, EdgeDescriptor>> m_edges{};

        // The graph describing our topology network.
        GraphType m_topologyGraph{};
    };
}
