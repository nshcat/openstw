#pragma once

#include <QGraphicsObject>
#include <tile.hxx>

#include "rendering/testgraphicsobject.hxx"
#include "rendering/tilerenderingconstants.hxx"

class TileGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    constexpr static qreal tileWidth = Rendering::TileRenderingConstants::tileWidth;
    constexpr static qreal tileHeight = Rendering::TileRenderingConstants::tileHeight;

public:
    TileGraphicsObject(Openstw::Simulation::Tile* tile);

public:
    Openstw::Simulation::Tile* tile() const;

public: // == QGraphicsItem implementation
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    // XXX Implement signals that are triggered when user clicks on tile elements

protected:
    Openstw::Simulation::Tile* m_tile{};

    Rendering::TestGraphicsObject* m_testObj;
};
