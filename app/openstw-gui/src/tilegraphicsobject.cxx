#include "tilegraphicsobject.hxx"
#include "rendering/renderinghelpers.hxx"
#include "rendering/tilerenderingconstants.hxx"
#include <QGraphicsScene>
#include <QPainter>

TileGraphicsObject::TileGraphicsObject(Openstw::Simulation::Tile* tile) : m_tile(tile)
{
    connect(tile, &Openstw::Simulation::Tile::invalidated, this, &TileGraphicsObject::tileInvalidated);
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

QRectF TileGraphicsObject::horizontalDrawingAreaRect(Rendering::VerticalDirection position) const
{
    const auto tileBoundingRect = this->innerBoundingRect();

    constexpr auto signalBoundingRectHeight =
        Rendering::TileRenderingConstants::halfTileHeight - (Rendering::TrackGraphicsObject::trackThickness / 2.0f);

    QPointF topLeft;

    if (position == Rendering::VerticalDirection::Top)
    {
        topLeft = QPointF{tileBoundingRect.topLeft()};
    }
    else
    {
        topLeft = QPointF{tileBoundingRect.left(), tileBoundingRect.bottom() - signalBoundingRectHeight};
    }

    return QRectF{topLeft.x(), topLeft.y(), tileBoundingRect.width(), signalBoundingRectHeight};
}

void TileGraphicsObject::createComponentRenderers()
{
    this->m_arrowRenderer = new Rendering::ArrowGraphicsObject{this};
    this->m_arrowRenderer->setup();

    this->m_erlaubnisFeldRenderer = new Rendering::ErlaubsnisFeldGraphicsObject{this};
    this->m_erlaubnisFeldRenderer->setup();

    this->m_trackRenderer = new Rendering::TrackGraphicsObject{this};
    this->m_trackRenderer->setup();

    this->m_bkwdSignalRenderer =
        new Rendering::SignalGraphicsObject{this, Openstw::Simulation::TileElementDirection::Backward};
    this->m_bkwdSignalRenderer->setup();

    this->m_fwdSignalRenderer =
        new Rendering::SignalGraphicsObject{this, Openstw::Simulation::TileElementDirection::Forward};
    this->m_fwdSignalRenderer->setup();

    this->m_znaRenderer = new Rendering::ZNAGraphicsObject{this};
    this->m_znaRenderer->setup();

    this->m_labelRenderer = new Rendering::LabelGraphicsObject{this};
    this->m_labelRenderer->setup();

    this->m_zstRenderer = new Rendering::ZSTGraphicsObject{this};
    this->m_zstRenderer->setup();

    this->m_fsmRenderer = new Rendering::FSMGraphicsObject{this};
    this->m_fsmRenderer->setup();
}

void TileGraphicsObject::tileInvalidated()
{
    this->update();
}
