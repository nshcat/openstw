#include "tilegraphicsobject.hxx"
#include "logging.hxx"
#include "rendering/renderinghelpers.hxx"
#include "rendering/tilerenderingconstants.hxx"
#include <QGraphicsScene>
#include <QPainter>
#include <optional>

TileGraphicsObject::TileGraphicsObject(Openstw::SpDr::Tile* tile, BlinkGenerator* blink)
    : m_tile(tile), m_blink{blink}
{
}

Openstw::SpDr::Tile* TileGraphicsObject::tile() const
{
    return this->m_tile;
}

BlinkGenerator* TileGraphicsObject::blinkGenerator() const
{
    return this->m_blink;
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
    QColor backgroundBorderColor = QColor{0x65, 0x65, 0x65}; // QColor{0x3e, 0x3e, 0x3e};

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
    // Setup all all feature layer renderers.
    for (auto* featureLayer : this->m_tile->layers())
    {
        std::optional<Rendering::TileFeatureGraphicsObject*> layerRenderer{};

        // Figure out which type of feature layer this is, and construct
        // the appropiate graphics object for it.
        switch (featureLayer->type())
        {
        case Openstw::SpDr::FeatureType::TrackIndicator:
            {
                auto* feature = dynamic_cast<const Openstw::SpDr::TrackIndicatorFeature*>(featureLayer);

                if (!feature)
                {
                    // clang-format off
                    qCWarning(catGui)
                        << "Encountered unexpected dynamic type mismatch for feature of type '"
                        << featureLayer->type()
                        << "', ignoring"
                        << Qt::endl;
                    // clang-format on

                    continue;
                }

                layerRenderer = new Rendering::TrackIndicatorFeatureObject{feature, this};
                break;
            }

        default:
            // clang-format off
            qCWarning(catGui)
                << "Encountered unknown feature layer type '"
                << featureLayer->type()
                << "', ignoring"
                << Qt::endl;
            // clang-format on

            break;
        }

        // Have we actually constructed a new graphics object?
        // If so, set it up and remember it for later.
        if (layerRenderer.has_value())
        {
            (*layerRenderer)->setup();
            this->m_featureRenderers.append(*layerRenderer);
        }
    }
}

QRectF TileGraphicsObject::horizontalDrawingAreaRect(Rendering::VerticalDirection position) const
{
    const auto tileBoundingRect = this->innerBoundingRect();

    // XXX The track constant should NOT be part of the feature renderer class..
    // It should be moved to a general constants file.
    constexpr auto signalBoundingRectHeight = Rendering::TileRenderingConstants::halfTileHeight -
                                              (Rendering::TrackIndicatorFeatureObject::trackThickness / 2.0f);

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