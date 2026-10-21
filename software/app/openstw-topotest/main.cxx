#include <QCoreApplication>
#include <QDir>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <sstream>
#include <topology.hxx>

using namespace Openstw::Topology;

std::ostream& operator<<(std::ostream& stream, const QString& str)
{
    stream << str.toStdString();
    return stream;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    const auto xmlPath = QDir::current().filePath("topology.xml");

    if (!QFile::exists(xmlPath))
    {
        std::cerr << "Topology XML file does not exist at path '" << xmlPath << "'" << std::endl;
        return -1;
    }

    std::cout << "Loading topology data from XML file '" << xmlPath << "'" << std::endl;

    Topology* topo = Topology::readFromFile(xmlPath, &a);

    std::cout << topo->dumpAsGraphVizDot() << std::endl;

    return 0;
}
