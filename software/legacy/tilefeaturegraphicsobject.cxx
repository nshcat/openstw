#include "tilefeaturegraphicsobject.hxx"
#include "../tilegraphicsobject.hxx"
#include <QCursor>
#include <QPainter>

namespace Rendering
{
    TileFeatureGraphicsObject::TileFeatureGraphicsObject(Openstw::SpDr::FeatureType featureType,
                                                         TileGraphicsObject* parent, bool providesContextMenu)
        : QGraphicsObject(parent),
          IContextMenuProvider(),
          m_tileGraphicsObj{parent},
          m_providesContextMenu{providesContextMenu},
          m_featureType{featureType}
    {
    }

    Openstw::SpDr::FeatureTypeNS::FeatureType TileFeatureGraphicsObject::featureType() const
    {
        return this->m_featureType;
    }

    void TileFeatureGraphicsObject::setup()
    {
        const auto mouseButtons = this->acceptsMouseInput();
        this->setAcceptedMouseButtons(mouseButtons.value_or(Qt::MouseButton::NoButton));
        if (mouseButtons.has_value())
            this->setCursor(Qt::PointingHandCursor);

        this->positionSelf();
        this->afterSetup();
    }

    TileGraphicsObject* TileFeatureGraphicsObject::tileGraphicsObject() const
    {
        return this->m_tileGraphicsObj;
    }

    bool TileFeatureGraphicsObject::providesContextMenu() const
    {
        return this->m_providesContextMenu;
    }

    std::optional<Qt::MouseButtons> TileFeatureGraphicsObject::acceptsMouseInput() const
    {
        return std::nullopt;
    }

    void TileFeatureGraphicsObject::afterSetup()
    {
        return;
    }

    void TileFeatureGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        // Always clip to the bounding rectangle to make painting diagonal items
        // easier (by being able to use rotated rectangles)
        painter->setClipRect(this->boundingRect());

        // Enable antialiasing
        painter->setRenderHint(QPainter::Antialiasing);
    }
}
