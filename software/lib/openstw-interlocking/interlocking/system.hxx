#pragma once

#include "elements/tracksection.hxx"
#include "request.hxx"
#include "route.hxx"
#include <QDomElement>
#include <QHash>
#include <QList>
#include <QObject>
#include <QQueue>
#include <QVector>
#include <topo/topology.hxx>

namespace Openstw::Interlocking
{
    // Class representing an interlocking system that handles
    // all safety critical railway operations on a given
    // railway topology network.
    class System : public QObject
    {
        Q_OBJECT

    public:
        System(Topology::Topology* topology, QObject* parent = nullptr);

    public:
        // Create interlocking system based on given topology graph and
        // the data in given XML document node.
        static System* createFromXml(Topology::Topology* topology, QDomElement xmlElement, QObject* parent = nullptr);

    public:
        // Topology object associated with the interlocking system
        Topology::Topology* topology() const;

        // Retrieve interlocking element with given identifier.
        std::optional<Element*> element(QStringView identifier);

    public slots:
        // Handle given interlocking request.
        // The interlocking system will acquire ownership of it.
        void handleRequest(Request* request);

    protected: // == Deserializing
        // Populate interlocking network contents from given XML element.
        void populateFromXml(QDomElement xmlElement);

        // Populate track sections from given XML element
        void populateSections(QDomElement xmlElement);

        // Populate switches from given XML element
        void populateSwitches(QDomElement xmlElement);

        // Populate signal masts from given XML element
        void populateSignalMasts(QDomElement xmlElement);

    protected:
        // Topology graph representing the railway infrastructure
        Topology::Topology* m_topology;

        // Collection of all elements making up the interlocking
        // network, such as signals, track sections and switches.
        QHash<QString, Element*> m_elements{};

        // Queue containing all active interlocking requests.
        QQueue<Request*> m_activeRequests{};

        // Collection of all routes that havent been released yet.
        QList<Route*> m_activeRoutes{};
    };
}
