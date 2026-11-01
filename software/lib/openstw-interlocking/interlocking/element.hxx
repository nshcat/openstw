#pragma once

#include "elementtype.hxx"
#include <QObject>
#include <QString>
#include <QStringView>

namespace Openstw::Interlocking
{
    // Base class for all elements within the interlocking network.
    // Specific elements such as signals and track sections
    // derive from this.
    class Element : public QObject
    {
        Q_OBJECT

    public:
        explicit Element(ElementType type, QStringView identifier, QObject* parent = nullptr);

        // Virtual destructor to ensure that this class is abstract.
        virtual ~Element() = 0;

    public:
        // Specific type of this element.
        ElementType type() const;

        // Unique identifier of this element.
        QStringView identifier() const;

        // Whether this element is a signal.
        bool isSignal() const;

    protected:
        // Specific type of this element.
        ElementType m_type;

        // Unique identifier of this element. Has to only be
        // unique within the interlocking system - i.e. a signal element
        // within the interlocking system can share the same identifier as
        // the signal node within the topology graph that it is referencing.
        QString m_identifier;
    };
}
