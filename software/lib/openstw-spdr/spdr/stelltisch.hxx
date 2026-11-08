#pragma once

#include "tile.hxx"
#include <QDomElement>
#include <QObject>
#include <QSize>
#include <QTimer>
#include <QVector>
#include <interlocking/system.hxx>
#include <optional>
#include <topo/topology.hxx>

namespace Openstw::SpDr
{
    class Stelltisch : public QObject
    {
        Q_OBJECT

    public:
        Stelltisch(QSize dimensions, Interlocking::System* interlocking, QObject* parent = nullptr);

    public:
        // Create stelltisch based on data from given XML element.
        static Stelltisch* createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                         QObject* parent = nullptr);

    public:
        // Dimensions of the stelltisch, in tiles.
        QSize dimensions() const;

        // Width of the stelltisch, in tiles
        unsigned width() const;

        // Height of the stelltisch, in tiles
        unsigned height() const;

        // Retrieve tile at given location on the stelltisch.
        std::optional<Tile*> operator[](unsigned x, unsigned y);

        // Retrieve tile at given location on the stelltisch.
        std::optional<const Tile*> operator[](unsigned x, unsigned y) const;

        // Retrieve tile at given location on the stelltisch.
        std::optional<Tile*> tile(unsigned x, unsigned y);

        // Retrieve tile at given location on the stelltisch.
        std::optional<const Tile*> tile(unsigned x, unsigned y) const;

    signals:
        // Emitted whenever blinking apertures should toggle their state.
        // This is provided such that all apertures of the associated
        // Stelltisch can blink in a synchronized fashion (as is the case
        // with the real world SpDr Stelltisch)
        void blinkStep();

        // Emitted whenever the state of a button on a tile changes.
        void buttonStateChanged(const Openstw::SpDr::Tile* tile, const Openstw::SpDr::Button* button,
                                Openstw::SpDr::ButtonState newState);

        // Emitted whenever the state of an aperture on a tile changes.
        void apertureStateChanged(const Openstw::SpDr::Tile* tile, const Openstw::SpDr::Aperture* aperture,
                                  Openstw::SpDr::ApertureState newState);

    protected:
        // Calculate the (x, y) location of a tile from its linear index
        // within the tile vector.
        TileLocation tilePosition(qsizetype tileIndex) const;

        // Calculate a tiles linear index within the tile vector from
        // its (x, y) location.
        qsizetype tileIndex(TileLocation location) const;

    protected:
        // Populate Stelltisch data from given XML element.
        void populateFromXml(QDomElement xmlElement);

        // Populate tile info and features from given XML element.
        void populateTiles(QDomElement xmlElement);

        // Read feature layers form given XML element, and add them to
        // given tile.
        void populateLayers(QDomElement xmlElement, Tile* tile);

    protected:
        // Dimensions of the Stelltisch, in tiles.
        QSize m_dimensions;

        // Reference to the interlocking system
        Interlocking::System* m_interlocking;

        // Container holding the actual tiles. This is a flat matrix
        // of tiles, stored in row-major order.
        QVector<Tile*> m_tiles{};

        QTimer* m_blinkTimer;
    };
}
