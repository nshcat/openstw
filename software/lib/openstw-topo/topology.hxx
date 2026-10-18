#pragma once

#include "edge.hxx"
#include "element.hxx"
#include "node.hxx"

#include <QHash>
#include <QMap>
#include <QObject>
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

    protected:
        Topology(QObject* parent);

    public:
        // Read topology data from given XML file and return topology network object
        // instance.
        static Topology* readFromFile(const QString& filePath, QObject* parent = nullptr);

    public:
        // Retrieve topology network element by its identifier.
        std::optional<Element*> findElement(const QString& identifier);

    protected:
        // Populate topology network contents from given XML reader.
        void populateFromXml(QXmlStreamReader* xmlReader);

        // Read node collection from XML document.
        void populateNodes(QXmlStreamReader* xmlReader);

        // Read edges between nodes from XML document.
        void populateEdges(QXmlStreamReader* xmlReader);

    protected:
        // Collection of all topology elements associated with the railway network.
        // Keyed with the elements unique identifier.
        QHash<QString, Element*> m_elements{};

        // Specific collections for the nodes and edges of the topology graph.
        // The objects contained within these collections are also contained
        // within the general element collection.
        // These collections are also keyed with the unique identifiers of
        // the nodes and edges.
        QHash<QString, Node*> m_nodes{};
        QHash<QString, Edge*> m_edges{};
    };
}
