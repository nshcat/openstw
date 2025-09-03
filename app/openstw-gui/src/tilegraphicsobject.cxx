#include "tilegraphicsobject.hxx"
#include "rendering/renderinghelpers.hxx"
#include "rendering/tilerenderingconstants.hxx"
#include <QGraphicsScene>
#include <QPainter>

TileGraphicsObject::TileGraphicsObject(Openstw::Simulation::Tile* tile) : m_tile(tile)
{
}

Openstw::Simulation::Tile* TileGraphicsObject::tile() const
{
    return this->m_tile;
}

QRectF TileGraphicsObject::innerBoundingRect() const
{
    return QRectF{Rendering::TileRenderingConstants::tileInsetSize / 2.0f,
                  Rendering::TileRenderingConstants::tileInsetSize / 2.0f,
                  Rendering::TileRenderingConstants::innerTileWidth,
                  Rendering::TileRenderingConstants::innerTileHeight};
}

QRectF TileGraphicsObject::boundingRect() const
{
    return QRectF{0.0f, 0.0f, TileGraphicsObject::tileWidth, TileGraphicsObject::tileHeight};
}

void TileGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QColor backgroundColor = QColor{0x96, 0x96, 0x96};
    QColor backgroundBorderColor = QColor{0x3e, 0x3e, 0x3e};

    painter->setBrush(QBrush{backgroundColor});
    painter->setPen(Rendering::rectanglePen(backgroundBorderColor, Rendering::TileRenderingConstants::borderThickness));

    // Qt renders rectangles in such a way that the outline stroke is centered on the 'ideal'
    // rectangles outline, thus the stroke extends 'outside' our bounding box.
    // We can fix this by making the rectangle smaller by the amount of expected overdraw, shrinking
    // it to lie entirely inside our bounding box.
    painter->drawRect(
        Rendering::adjustRectForBorder(this->boundingRect(), Rendering::TileRenderingConstants::borderThickness));
}

void TileGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    this->QGraphicsItem::mousePressEvent(event);
}

void TileGraphicsObject::setup()
{
    this->createComponentRenderers();
}

void TileGraphicsObject::createComponentRenderers()
{
    this->m_arrowRenderer = new Rendering::ArrowGraphicsObject{this};
    this->m_arrowRenderer->setup();
    this->scene()->addItem(this->m_arrowRenderer);

    this->m_trackRenderer = new Rendering::TrackGraphicsObject{this};
    this->m_trackRenderer->setup();
    this->scene()->addItem(this->m_trackRenderer);

    this->m_bkwdSignalRenderer =
        new Rendering::SignalGraphicsObject{this, Openstw::Simulation::TileElementDirection::Backward};
    this->m_bkwdSignalRenderer->setup();
    this->scene()->addItem(this->m_bkwdSignalRenderer);

    this->m_fwdSignalRenderer =
        new Rendering::SignalGraphicsObject{this, Openstw::Simulation::TileElementDirection::Forward};
    this->m_fwdSignalRenderer->setup();
    this->scene()->addItem(this->m_fwdSignalRenderer);
}
