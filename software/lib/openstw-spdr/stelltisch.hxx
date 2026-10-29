#pragma once

#include "tile.hxx"
#include <QObject>
#include <QVector>
#include <interlocking.hxx>
#include <topology.hxx>

namespace Openstw::SpDr
{
    class Stelltisch : public QObject
    {
        Q_OBJECT

    public:
        Stelltisch(Interlocking::Interlocking* interlocking, QObject* parent = nullptr);

    public:
        // The interlocking system associatedw with this stelltisch
        Interlocking::Interlocking* interlocking() const;

    protected:
        Interlocking::Interlocking* m_interlocking;
        QVector<Tile*> m_tiles{};
    };
}
