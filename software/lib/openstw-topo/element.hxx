#pragma once

#include <QObject>

namespace Openstw
{
    namespace Topology
    {
        // Base class for all elements that are part of the topology
        // network.
        class Element : public QObject
        {
            Q_OBJECT

        public:
            explicit Element(const QString& identifier, QObject* parent = nullptr);

        public:
            const QString& identifier() const;
            void set_identifier(const QString& identifier);

        protected:
            QString m_identifier{}; //< Unique identifier for this element.
        };
    }
}
