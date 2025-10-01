#pragma once

#include <QGraphicsObject>
#include <tile.hxx>

#include "rendering/arrowgraphicsobject.hxx"
#include "rendering/erlaubnisfeldgraphicsobject.hxx"
#include "rendering/fsmgraphicsobject.hxx"
#include "rendering/labelgraphicsobject.hxx"
#include "rendering/platformgraphicsobject.hxx"
#include "rendering/renderinghelpers.hxx"
#include "rendering/signalgraphicsobject.hxx"
#include "rendering/tilerenderingconstants.hxx"
#include "rendering/trackgraphicsobject.hxx"
#include "rendering/znagraphicsobject.hxx"
#include "rendering/zstgraphicsobject.hxx"

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

    /**
     * @brief The bounding rect of the inner tile, without the border.
     * Use this when rendering tile contents.
     */
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
    void createComponentRenderers();

protected slots:
    void tileInvalidated();

protected:
    Openstw::Simulation::Tile* m_tile{};

protected: // == Component renderers
    Rendering::ArrowGraphicsObject* m_arrowRenderer;
    Rendering::TrackGraphicsObject* m_trackRenderer;
    Rendering::SignalGraphicsObject* m_fwdSignalRenderer;
    Rendering::SignalGraphicsObject* m_bkwdSignalRenderer;
    Rendering::ErlaubsnisFeldGraphicsObject* m_erlaubnisFeldRenderer;
    Rendering::ZNAGraphicsObject* m_znaRenderer;
    Rendering::ZSTGraphicsObject* m_zstRenderer;
    Rendering::LabelGraphicsObject* m_labelRenderer;
    Rendering::FSMGraphicsObject* m_fsmRenderer;
    Rendering::PlatformGraphicsObject* m_platformRenderer;
};
