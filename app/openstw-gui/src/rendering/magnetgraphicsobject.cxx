#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QObject>
#include <QPainter>
#include <QtCore>

#include "magnetgraphicsobject.hxx"
#include "renderinghelpers.hxx"

namespace Rendering
{
    MagnetGraphicsObject::MagnetGraphicsObject(const MagnetType type) : QGraphicsObject(), m_type{type}
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable);
        this->setFlag(QGraphicsItem::ItemIsMovable);
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        this->setAcceptedMouseButtons(Qt::MouseButton::LeftButton | Qt::MouseButton::RightButton);
    }

    void MagnetGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        painter->save();

        const auto boundingRect = this->boundingRect();

        painter->setPen(rectanglePen(Qt::black, MagnetGraphicsObject::magnetBorderThickness));
        painter->setBrush(Qt::white);
        painter->drawRect(adjustRectForBorder(boundingRect, MagnetGraphicsObject::magnetBorderThickness));

        const QRectF innerBorderRect{
            boundingRect.left() + MagnetGraphicsObject::magnetBorderThickness +
                MagnetGraphicsObject::magnetBorderPadding,
            boundingRect.top() + MagnetGraphicsObject::magnetBorderThickness +
                MagnetGraphicsObject::magnetBorderPadding,
            boundingRect.width() - 2.0f * MagnetGraphicsObject::magnetBorderThickness -
                2.0f * MagnetGraphicsObject::magnetBorderPadding,
            boundingRect.height() - 2.0f * MagnetGraphicsObject::magnetBorderThickness -
                2.0f * MagnetGraphicsObject::magnetBorderPadding,
        };

        painter->setPen(rectanglePen(Qt::black, MagnetGraphicsObject::magnetInnerBorderThickness));
        painter->setBrush(Qt::white);
        painter->drawRect(adjustRectForBorder(innerBorderRect, MagnetGraphicsObject::magnetInnerBorderThickness));

        const qreal margin = MagnetGraphicsObject::magnetInnerBorderThickness + 1.0f;
        const QRectF innerContentRect = innerBorderRect.marginsRemoved(QMarginsF{margin, margin, margin, margin});

        switch (this->m_type)
        {
        case (MagnetType::TrackClosed):
            this->drawTrackClosed(painter, innerContentRect);
            break;

        default:
            break;
        }

        painter->restore();
    }

    QRectF MagnetGraphicsObject::boundingRect() const
    {
        return QRectF{0.0f, 0.0f, MagnetGraphicsObject::magnetWidth, MagnetGraphicsObject::magnetHeight};
    }

    bool MagnetGraphicsObject::providesContextMenu() const
    {
        return true;
    }

    void MagnetGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
    {
        QMenu contextMenu{event->widget()};
        QAction* removeAction = contextMenu.addAction("Remove");

        connect(removeAction, &QAction::triggered, this, &MagnetGraphicsObject::onRemoveMagnet);

        contextMenu.exec(event->screenPos());
    }

    void MagnetGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        QGraphicsObject::mousePressEvent(event);

        if (event->button() == Qt::MouseButton::LeftButton)
        {
            this->setTransformOriginPoint(this->boundingRect().center());
            this->setScale(1.1f);
        }
    }

    void MagnetGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        QGraphicsObject::mouseReleaseEvent(event);

        if (event->button() == Qt::MouseButton::LeftButton)
        {
            this->setTransformOriginPoint(this->boundingRect().center());
            this->setScale(1.0f);
        }
    }

    void MagnetGraphicsObject::drawTrackClosed(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        const QRectF crossRect{
            centerWithin(MagnetGraphicsObject::trackClosedSymbolWidth, location.width(), location.left()),
            centerWithin(MagnetGraphicsObject::trackClosedSymbolHeight, location.height(), location.top()),
            MagnetGraphicsObject::trackClosedSymbolWidth, MagnetGraphicsObject::trackClosedSymbolHeight};

        painter->setPen(QPen{Qt::black, MagnetGraphicsObject::trackClosedSymbolThickness, Qt::SolidLine, Qt::FlatCap});
        painter->drawLine(crossRect.bottomLeft(), crossRect.topRight());
        painter->drawLine(crossRect.topLeft(), crossRect.bottomRight());

        painter->restore();
    }

    void MagnetGraphicsObject::onRemoveMagnet()
    {
        auto* scene = this->scene();
        if (scene != nullptr)
        {
            scene->removeItem(this);
        }

        this->deleteLater();
    }
}
