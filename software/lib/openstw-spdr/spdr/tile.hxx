#pragma once

#include "button.hxx"
#include "buttonstate.hxx"
#include "featurelayer.hxx"
#include "tilelocation.hxx"
#include <QObject>
#include <QSpan>
#include <QVector>
#include <optional>

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
        explicit Tile(const TileLocation& location, QObject* parent = nullptr);

    public:
        // Location of this tile on the Stelltisch.
        const TileLocation& location() const;

        // The feature layers that make up this tile.
        QSpan<FeatureLayer const* const> layers() const;

        // Set the state for a SpDr button on this tile, via its identifier.
        // This method is meant to be called from whatever Ui is displaying
        // the Stelltisch and its tiles.
        void setButtonState(QStringView identifier, ButtonState newState);

    signals:
        // Emitted whenever the state of a button on this tile changes.
        void buttonStateChanged(const Openstw::SpDr::Button* button, Openstw::SpDr::ButtonState newState);

        // Emitted whenever the state of an aperture on this tile changes.
        void apertureStateChanged(const Openstw::SpDr::Aperture* aperture, Openstw::SpDr::ApertureState newState);

    protected:
        // Location of the tile on the Stelltisch.
        TileLocation m_location;

        // Collection of feature layers that make up this tile.
        QVector<FeatureLayer*> m_featureLayers{};
    };
}
