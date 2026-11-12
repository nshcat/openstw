#pragma once

#include <QGraphicsObject>
#include <QVector>
#include <spdr/featurelayer.hxx>
#include <spdr/tile.hxx>

#include "blinkgenerator.hxx"
#include "rendering/renderinghelpers.hxx"
#include "rendering/tilerenderingconstants.hxx"
#include "rendering/trackindicatorfeatureobject.hxx"

class TileGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    constexpr static qreal tileWidth = Rendering::TileRenderingConstants::tileWidth;
    constexpr static qreal tileHeight = Rendering::TileRenderingConstants::tileHeight;

public:
    TileGraphicsObject(Openstw::SpDr::Tile* tile, BlinkGenerator* blink);

public:
    // The tile this graphics object is associated with.
    Openstw::SpDr::Tile* tile() const;

    // The global blink generator used to render blinking apertures.
    BlinkGenerator* blinkGenerator() const;

    // The bounding rect of the inner tile, without the border.
    // Use this when rendering tile contents.
    QRectF innerBoundingRect() const;

public: // == QGraphicsItem implementation
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

public:
    void setup();

    /**
     * @brief Calculates the horizontal area over or under the track marker, used for rendering
     * tile contents such as signals, lamps and buttons.
     */
    QRectF horizontalDrawingAreaRect(Rendering::VerticalDirection position) const;

protected:
    // The SpDr tile we are representing. That object is essentially the view model,
    // and this class is the view.
    Openstw::SpDr::Tile* m_tile{};

    // All feature layers we need to render the tile associated with this
    // graphics object.
    QVector<Rendering::TileFeatureGraphicsObject*> m_featureRenderers{};

    // The global blink generator used to render blinking apertures.
    BlinkGenerator* m_blink;
};
