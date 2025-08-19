#pragma once

#include <QGraphicsItem>
#include <tile.hxx>

#include "rendering/tilerenderingconstants.hxx"
#include "rendering/trackrenderer.hxx"

class TileGraphicsItem : public QGraphicsItem
{
public:
    constexpr static qreal tileWidth = Rendering::TileRenderingConstants::tileWidth;
    constexpr static qreal tileHeight = Rendering::TileRenderingConstants::tileHeight;

public:
    TileGraphicsItem(Openstw::Simulation::Tile* tile);

public:
    Openstw::Simulation::Tile* tile() const;

public: // == QGraphicsItem implementation
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    Openstw::Simulation::Tile* m_tile{};

    Rendering::TrackRenderer m_trackRenderer;
};
