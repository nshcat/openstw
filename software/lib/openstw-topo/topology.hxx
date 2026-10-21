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

    protected:
        Topology(QObject* parent);

    public: // == Static methods
        // Read topology data from given XML file and return topology network object
        // instance.
        static Topology* readFromFile(const QString& filePath, QObject* parent = nullptr);

    public: // == Interacting with the topology network
        // Retrieve topology network element by its identifier.
        std::optional<Element*> findElement(const QString& identifier);

        // Find a path from given node to given other node.
        std::optional<Path> findPath(Node* from, Node* to) const;

    public: // == Debug methods
        QString dumpAsGraphVizDot();

    protected: // == Interaction with topology graph
        // Retrieve graph vertex descriptor for given node, if the node
        // is part of the topology graph.
        std::optional<VertexDescriptor> lookupNode(Node* node) const;

        // Retrieve graph vertex descriptor for given node identifier, if the node
        // is part of the topology graph.
        std::optional<VertexDescriptor> lookupNode(QStringView nodeIdentifier) const;

        // Retrieve graph edge descriptor for given edge, if the edge
        // is part of the topology graph.
        std::optional<EdgeDescriptor> lookupEdge(Edge* edge) const;

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
        QPair<QString, Port> parseConnection(QXmlStreamReader* xmlReader);

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
