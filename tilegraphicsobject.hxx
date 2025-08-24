#pragma once

#include <QGraphicsObject>
#include <tile.hxx>

#include "rendering/tilerenderingconstants.hxx"
#include "rendering/trackgraphicsobject.hxx"

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

public:
    void setup();

    // XXX Implement signals that are triggered when user clicks on tile elements

protected:
    void createComponentRenderers();

protected:
    Openstw::Simulation::Tile* m_tile{};

protected: // == Component renderers
    Rendering::TrackGraphicsObject* m_trackRenderer;
};
