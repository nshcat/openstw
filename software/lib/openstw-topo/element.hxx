#pragma once

#include "elementcategory.hxx"
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
            explicit Element(ElementCategory category, const QString& identifier, QObject* parent = nullptr);

        public:
            const QString& identifier() const;
            void setIdentifier(const QString& identifier);

            bool isNode() const;
            bool isEdge() const;
            ElementCategory category() const;

        protected:
            ElementCategory m_category;
            QString m_identifier; //< Unique identifier for this element.
        };
    }
}
