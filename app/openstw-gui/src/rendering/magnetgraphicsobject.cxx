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

        painter->setRenderHint(QPainter::Antialiasing, true);

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
        case MagnetType::TrackClosed:
            this->drawTrackClosed(painter, innerContentRect);
            break;

        case MagnetType::CatenaryPowerless:
            this->drawCatenaryPowerless(painter, innerContentRect);
            break;

        case MagnetType::CounterTrackInUse:
            this->drawCounterTrackInUse(painter, innerContentRect);
            break;

        case MagnetType::AbschnittsPruefung:
            this->drawTextMagnet(painter, innerContentRect, "AP");
            break;

        case MagnetType::RaeumungsPruefung:
            this->drawTextMagnet(painter, innerContentRect, "RP");
            break;

        case MagnetType::Kleinwagen:
            this->drawTextMagnet(painter, innerContentRect, "KL");
            break;

        case MagnetType::LueTrain:
            this->drawTextMagnet(painter, innerContentRect, "LUE");
            break;

        case MagnetType::NoCounterTrackUse:
            this->drawTextMagnet(painter, innerContentRect, "NLF");
            break;

        case MagnetType::WorkInProgress:
            this->drawTextMagnet(painter, innerContentRect, "ARB");
            break;

        case MagnetType::LevelCrossingMalfunction:
            this->drawTextMagnet(painter, innerContentRect, "BUE");
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
            this->m_isBeingDragged = true;
        }
    }

    void MagnetGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        QGraphicsObject::mouseReleaseEvent(event);

        if (event->button() == Qt::MouseButton::LeftButton)
        {
            this->setTransformOriginPoint(this->boundingRect().center());
            this->setScale(1.0f);
            this->m_isBeingDragged = false;
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

    void MagnetGraphicsObject::drawCatenaryPowerless(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        painter->translate(location.center());
        painter->rotate(45.0f);
        painter->translate(-location.center());

        const qreal outerRectSize = location.height() - 18.0f;
        const QRectF outerRect{centerWithin(outerRectSize, location.width(), location.left()),
                               centerWithin(outerRectSize, location.height(), location.top()), outerRectSize,
                               outerRectSize};

        painter->setPen(rectanglePen(Qt::blue, MagnetGraphicsObject::catenaryPowerlessOuterThickness));
        painter->setBrush(Qt::white);
        painter->drawRect(adjustRectForBorder(outerRect, MagnetGraphicsObject::catenaryPowerlessOuterThickness));

        const qreal innerRectSize = outerRectSize - 2.0f * 2.0f * MagnetGraphicsObject::catenaryPowerlessOuterThickness;

        const QRectF innerRect{centerWithin(innerRectSize, location.width(), location.left()),
                               centerWithin(innerRectSize, location.height(), location.top()), innerRectSize,
                               innerRectSize};

        painter->setPen(rectanglePen(Qt::blue, 1.0f));
        painter->setBrush(Qt::blue);
        painter->drawRect(adjustRectForBorder(innerRect, 1.0f));

        painter->restore();
    }

    void MagnetGraphicsObject::drawCounterTrackInUse(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        const QRectF leftArrowHeadRect{
            location.left() + MagnetGraphicsObject::counterTrackInUsePadding,
            centerWithin(MagnetGraphicsObject::counterTrackInUseArrowHeadHeight, location.height(), location.top()),
            MagnetGraphicsObject::counterTrackInUseArrowHeadWidth,
            MagnetGraphicsObject::counterTrackInUseArrowHeadHeight};

        QPolygonF leftArrowHead{};
        leftArrowHead << leftArrowHeadRect.topRight()
                      << QPointF{leftArrowHeadRect.left(), leftArrowHeadRect.center().y()}
                      << leftArrowHeadRect.bottomRight() << leftArrowHeadRect.topRight();

        painter->setPen(QPen{Qt::black, 0.5f});
        painter->setBrush(Qt::black);
        painter->drawPolygon(leftArrowHead);

        const QRectF rightArrowHeadRect{
            location.right() - MagnetGraphicsObject::counterTrackInUseArrowHeadWidth -
                MagnetGraphicsObject::counterTrackInUsePadding,
            centerWithin(MagnetGraphicsObject::counterTrackInUseArrowHeadHeight, location.height(), location.top()),
            MagnetGraphicsObject::counterTrackInUseArrowHeadWidth,
            MagnetGraphicsObject::counterTrackInUseArrowHeadHeight};

        QPolygonF rightArrowHead{};
        rightArrowHead << rightArrowHeadRect.topLeft()
                       << QPointF{rightArrowHeadRect.right(), rightArrowHeadRect.center().y()}
                       << rightArrowHeadRect.bottomLeft() << rightArrowHeadRect.topLeft();

        painter->setPen(QPen{Qt::black, 0.5f});
        painter->setBrush(Qt::black);
        painter->drawPolygon(rightArrowHead);

        const QRectF arrowStemRect{
            leftArrowHeadRect.right(),
            centerWithin(MagnetGraphicsObject::counterTrackInUseArrowThickness, location.height(), location.top()),
            rightArrowHeadRect.left() - leftArrowHeadRect.right(),
            MagnetGraphicsObject::counterTrackInUseArrowThickness};

        painter->setPen(rectanglePen(Qt::black, 1.0f));
        painter->setBrush(Qt::black);
        painter->drawRect(adjustRectForBorder(arrowStemRect, 1.0f));

        painter->restore();
    }

    void MagnetGraphicsObject::drawTextMagnet(QPainter* painter, const QRectF& location, const QString& text) const
    {
        painter->save();

        drawTextBox(painter, location, text, Qt::white, Qt::transparent, 0.0f, Qt::black);

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

    void MagnetGraphicsObject::wheelEvent(QGraphicsSceneWheelEvent* event)
    {
        if (this->m_isBeingDragged)
        {
            const auto currentRotation = this->rotation();
            this->setRotation(currentRotation + event->delta() * 0.15f);
        }
    }
}
