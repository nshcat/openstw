#pragma once

#include "tile.hxx"
#include <QObject>
#include <QTimer>
#include <QVector>
#include <interlocking.hxx>
#include <optional>
#include <topology.hxx>

namespace Openstw::SpDr
{
    class Stelltisch : public QObject
    {
        Q_OBJECT

    public:
        Stelltisch(Interlocking::Interlocking* interlocking, QObject* parent = nullptr);

    public:
        // Dimensions of the stelltisch, in tiles.
        QSize dimensions() const;

        // Width of the stelltisch, in tiles
        int width() const;

        // Height of the stelltisch, in tiles
        int height() const;

        // Retrieve tile at given location on the stelltisch.
        std::optional<Tile*> operator[](int x, int y);

        // Retrieve tile at given location on the stelltisch.
        std::optional<const Tile*> operator[](int x, int y) const;

        // Retrieve tile at given location on the stelltisch.
        std::optional<Tile*> tile(int x, int y);

        // Retrieve tile at given location on the stelltisch.
        std::optional<const Tile*> tile(int x, int y) const;

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
        Interlocking::Interlocking* m_interlocking;
        QVector<Tile*> m_tiles{};
        QTimer* m_blinkTimer;
    };
}
