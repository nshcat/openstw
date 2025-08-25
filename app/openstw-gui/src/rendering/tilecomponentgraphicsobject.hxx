#pragma once

#include <QGraphicsObject>
#include <QObject>
#include <optional>
#include <tile.hxx>

class TileGraphicsObject;

namespace Rendering
{
    class TileComponentGraphicsObject : public QGraphicsObject
    {
        Q_OBJECT
    public:
        TileComponentGraphicsObject(TileGraphicsObject* parent);

    public:
        /**
         * @brief Finishes setting up the graphics object to be ready for rendering.
         * Is called from the main tile graphics object.
         */
        void setup();
        TileGraphicsObject* tileGraphicsObject() const;

    protected:
        /**
         * @brief Called when the tile component renderer needs this component
         * to position itself relative to the parent tile component graphics object.
         * This is only called once at the beginning of the lifetime of each
         * component grapgics object.
         */
        virtual void positionSelf() = 0;

        /**
         * @brief Whether this tile component graphics objects wants to receive
         * mouse input, and if so, from which mouse buttons.
         * @return std::nullopt if no mouse input is needed, otherwise a set of mouse
         * buttons to react to.
         */
        virtual std::optional<Qt::MouseButtons> acceptsMouseInput() const;

    public:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        Openstw::Simulation::Tile* m_tile{};
        TileGraphicsObject* m_tileGraphicsObj{};
    };
}
