#include <QCoreApplication>
#include <QDir>
#include <boost/graph/graphviz.hpp>
#include <interlocking/system.hxx>
#include <iostream>
#include <sstream>
#include <topo/topology.hxx>

using namespace Openstw;

std::ostream& operator<<(std::ostream& stream, const QString& str)
{
    stream << str.toStdString();
    return stream;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    const auto xmlPath = QDir::current().filePath("teststadt.xml");

    if (!QFile::exists(xmlPath))
    {
        std::cerr << "Topology XML file does not exist at path '" << xmlPath << "'" << std::endl;
        return -1;
    }

    std::cout << "Loading topology data from XML file '" << xmlPath << "'" << std::endl;

    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cerr << "Could not open Topology XML file" << std::endl;
        return -1;
    }

    QDomDocument doc;
    const auto result = doc.setContent(&file);
    if (!result)
    {
        std::cerr << "Failed to parse Topology XML file: " << result.errorMessage << "at line" << result.errorLine
                  << "column" << result.errorColumn << std::endl;

        return -1;
    }

    auto topologyElement = doc.documentElement().namedItem("Topology");

    if (topologyElement.isNull() || !topologyElement.isElement())
    {
        std::cerr << "Topology XML file does not contain Topology data" << std::endl;
        return -1;
    }

    Topology::Topology* topo = Topology::Topology::createFromXml(topologyElement.toElement(), &a);

    std::cout << topo->dumpGraph() << std::endl << std::endl;

    // Try to find a path from S1 to S5. This should be successful.
    const auto* nodeS1 = *topo->findNode("S1");
    const auto* nodeS5 = *topo->findNode("S5");

    const auto path = topo->findPath(nodeS1, nodeS5, Topology::Port::Front, Topology::Port::Back);
    if (!path)
    {
        std::cout << "No path found from S1 -> S5" << std::endl;
    }
    else
    {
        std::cout << "Found path from S1 -> S5" << std::endl;

        const auto pathDump = path->dumpGraph();

        std::cout << pathDump << std::endl;
    }

    // Try to find a path from S5 to S4. This should not be possible,
    // since it would require traversing W2 or W1 in an illegal way.
    const auto* nodeS4 = *topo->findNode("S4");
    const auto path2 = topo->findPath(nodeS5, nodeS4, Topology::Port::Front, Topology::Port::Front);

    if (!path2)
    {
        std::cout << "No path found from S5 -> S4" << std::endl;
    }
    else
    {
        std::cout << "Found path from S5 -> S4" << std::endl;

        const auto pathDump = path2->dumpGraph();

        std::cout << pathDump << std::endl;
    }

    // Try to find a path from S5 to S6. This should be successful.
    const auto* nodeS6 = *topo->findNode("S6");

    const auto path3 = topo->findPath(nodeS5, nodeS6, Topology::Port::Front, Topology::Port::Front);
    if (!path3)
    {
        std::cout << "No path found from S5 -> S6" << std::endl;
    }
    else
    {
        std::cout << "Found path from S5 -> S6" << std::endl;

        const auto pathDump = path3->dumpGraph();

        std::cout << pathDump << std::endl;
    }

    // == Try creating the interlocking system from XML.
    auto interlockingElement = doc.documentElement().namedItem("Interlocking");

    if (interlockingElement.isNull() || !interlockingElement.isElement())
    {
        std::cerr << "Interlocking XML file does not contain Interlocking data" << std::endl;
        return -1;
    }

    Interlocking::System* interlocking = Interlocking::System::createFromXml(topo, interlockingElement.toElement(), &a);

    return 0;
}
