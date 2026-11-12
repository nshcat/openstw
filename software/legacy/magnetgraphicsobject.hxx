#pragma once

#include <QGraphicsItem>
#include <QObject>

#include "../magnettype.hxx"
#include "icontextmenuprovider.hxx"

namespace Rendering
{
    class MagnetGraphicsObject : public QGraphicsObject, public IContextMenuProvider
    {
        Q_OBJECT

        constexpr static qreal magnetWidth = 140.0f;
        constexpr static qreal magnetHeight = 80.0f;
        constexpr static qreal magnetBorderThickness = 1.0f;
        constexpr static qreal magnetBorderPadding = 2.0f;
        constexpr static qreal magnetInnerBorderThickness = 6.0f;

        constexpr static qreal trackClosedSymbolWidth = 80.0f;
        constexpr static qreal trackClosedSymbolHeight = 45.0f;
        constexpr static qreal trackClosedSymbolThickness = 14.0f;

        constexpr static qreal catenaryPowerlessOuterThickness = 5.0f;

        constexpr static qreal counterTrackInUseArrowHeadWidth = 30.0f;
        constexpr static qreal counterTrackInUseArrowHeadHeight = 25.0f;
        constexpr static qreal counterTrackInUseArrowThickness = 8.0f;
        constexpr static qreal counterTrackInUsePadding = 5.0f;

    public:
        MagnetGraphicsObject(const MagnetType type);

    public:
        MagnetType magnetType() const;

    public:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        virtual QRectF boundingRect() const override;

    public:
        virtual bool providesContextMenu() const override;

    protected:
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void wheelEvent(QGraphicsSceneWheelEvent* event) override;

    protected:
        void drawTrackClosed(QPainter* painter, const QRectF& location) const;
        void drawCatenaryPowerless(QPainter* painter, const QRectF& location) const;
        void drawCounterTrackInUse(QPainter* painter, const QRectF& location) const;
        void drawTextMagnet(QPainter* painter, const QRectF& location, const QString& text) const;

    protected slots:
        void onRemoveMagnet();

    protected:
        MagnetType m_type;
        bool m_isBeingDragged{false};
    };
}
