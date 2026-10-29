#pragma once

#include "featurelayer.hxx"
#include <QObject>
#include <QSpan>
#include <QVector>

namespace Openstw::SpDr
{

    // A class describing a single tile in a Stelltisch.
    // It acts somewhat as a view model for the tile - accepting
    // abstract input events, and managing the state of the tile
    // lights and display elements.
    // The tile view model always deals with the concept of
    // "display elements" - an abstraction over the actual
    // physical display elements on SpDr tiles, which do not care
    // about what physical lamps or render elements actually show
    // the element state.
    // A tile manages a collection of feature layers, which describe
    // individual, self-contained features a tile can have, such as
    // a signal, a track or a switch, and handle interacting with
    // the corresponding logical elements living inside the interlocking
    // layer.
    class Tile : public QObject
    {
        Q_OBJECT

    public:
        explicit Tile(QObject* parent = nullptr);

    public:
        // The feature layers that make up this tile.
        QSpan<FeatureLayer const* const> layers() const;

    signals:

    protected:
        // Collection of feature layers that make up this tile.
        QVector<FeatureLayer*> m_featureLayers{};
    };
}
