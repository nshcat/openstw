#include <QPainter>

#include "../tilegraphicsobject.hxx"
#include "erlaubnisfeldgraphicsobject.hxx"
#include "renderinghelpers.hxx"
#include "trackgraphicsobject.hxx"

namespace Rendering
{
    ErlaubsnisFeldGraphicsObject::ErlaubsnisFeldGraphicsObject(TileGraphicsObject* parent)
        : TileComponentGraphicsObject(parent)
    {
    }

    QRectF ErlaubsnisFeldGraphicsObject::boundingRect() const
    {
        // We span the whole inner tile.
        return this->tileGraphicsObject()->innerBoundingRect();
    }

    void ErlaubsnisFeldGraphicsObject::positionSelf()
    {
        this->setPos(this->boundingRect().topLeft());
    }

    QRectF ErlaubsnisFeldGraphicsObject::calculateHemiRect(const VerticalDirection location) const
    {
        const auto tileBoundingRect = this->tileGraphicsObject()->innerBoundingRect();

        constexpr auto rectHeight =
            TileRenderingConstants::halfTileHeight - (TrackGraphicsObject::trackThickness / 2.0f);

        if (location == VerticalDirection::Top)
        {
            return QRectF{tileBoundingRect.left(), tileBoundingRect.top(), tileBoundingRect.width(), rectHeight};
        }
        else
        {
            return QRectF{tileBoundingRect.left(), tileBoundingRect.bottom() - rectHeight, tileBoundingRect.width(),
                          rectHeight};
        }
    }

    void ErlaubsnisFeldGraphicsObject::drawButton(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        const QRectF buttonRect{
            centerWithin(ErlaubsnisFeldGraphicsObject::buttonDiameter, location.width(), location.left()),
            centerWithin(ErlaubsnisFeldGraphicsObject::buttonDiameter, location.height(), location.top()),
            ErlaubsnisFeldGraphicsObject::buttonDiameter, ErlaubsnisFeldGraphicsObject::buttonDiameter};

        painter->setPen(QPen{ErlaubsnisFeldGraphicsObject::buttonOuterColor, 0.5f});
        painter->setBrush(ErlaubsnisFeldGraphicsObject::buttonInnerColor);
        painter->drawEllipse(buttonRect);

        painter->restore();
    }

    void ErlaubsnisFeldGraphicsObject::drawEaTLabel(QPainter* painter, const QRectF& location) const
    {
        painter->save();

        auto currentFont = painter->font();
        currentFont.setBold(true);
        painter->setFont(currentFont);

        drawTextBox(painter, location, "EaT", Qt::white, Qt::transparent, 0.0f, Qt::black);

        painter->restore();
    }

    void ErlaubsnisFeldGraphicsObject::drawAusfahrLabel(QPainter* painter, const QRectF& location,
                                                        const QString& text) const
    {
        painter->save();

        drawTextBox(painter, location, text, Qt::white, Qt::transparent, 0.0f, Qt::black);

        painter->restore();
    }

    void ErlaubsnisFeldGraphicsObject::drawLamp(QPainter* painter, const QRectF& location, bool lampOn)
    {
        painter->save();

        // XXX Implement active state
        const QColor lampInnerColor = ErlaubsnisFeldGraphicsObject::inactiveLampColor;

        const QRectF lampRect{
            centerWithin(ErlaubsnisFeldGraphicsObject::lampDiameter, location.width(), location.left()),
            centerWithin(ErlaubsnisFeldGraphicsObject::lampDiameter, location.height(), location.top()),
            ErlaubsnisFeldGraphicsObject::lampDiameter, ErlaubsnisFeldGraphicsObject::lampDiameter};

        painter->setPen(QPen{Qt::black, 0.5f});
        painter->setBrush(lampInnerColor);
        painter->drawEllipse(lampRect);

        painter->restore();
    }

    void ErlaubsnisFeldGraphicsObject::drawErlaubnisMelder(QPainter* painter, const QRectF& location,
                                                           QColor melderColor,
                                                           const HorizontalDirection direction) const
    {
        painter->save();

        const auto center = location.center();

        QPolygonF melderPolygon{};
        if (direction == HorizontalDirection::Left)
        {
            melderPolygon << location.topRight() << location.bottomRight() << QPointF{location.left(), center.y()}
                          << location.topRight();
        }
        else
        {
            melderPolygon << location.topLeft() << QPointF{location.right(), center.y()} << location.bottomLeft()
                          << location.topLeft();
        }

        painter->setPen(rectanglePen(Qt::black, 0.5f));
        painter->setBrush(melderColor);
        painter->drawPolygon(melderPolygon);

        painter->restore();
    }

    void ErlaubsnisFeldGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        if (!this->m_tile->hasErlaubnisFeld())
            return;

        // Base class method call sets up clipping region
        TileComponentGraphicsObject::paint(painter, option, widget);

        // XXX Put most of this into methods
        // XXX Implement the other direction! Everything _besides_ text needs to be rotated.
        // Maybe pass down a bool indicating whether rotation is active, and if rotate text
        // again inside the text draw methods here in this class

        painter->save();

        const auto& erlaubnisFeld = this->m_tile->erlaubnisFeld();

        const auto boundingRect = this->tileGraphicsObject()->innerBoundingRect();

        // Rect containing the EaT button and incoming melder
        const auto incomingRect = this->calculateHemiRect(VerticalDirection::Top);

        // EaT button and label
        const QRectF eatButtonRect{
            incomingRect.left() + ErlaubsnisFeldGraphicsObject::horzPaddingToBorder,
            centerWithin(ErlaubsnisFeldGraphicsObject::buttonDiameter, incomingRect.height(), incomingRect.top()),
            ErlaubsnisFeldGraphicsObject::buttonDiameter, ErlaubsnisFeldGraphicsObject::buttonDiameter};
        this->drawButton(painter, eatButtonRect);

        const QRectF eatLabelRect{
            eatButtonRect.right() + ErlaubsnisFeldGraphicsObject::buttonLabelPadding,
            centerWithin(ErlaubsnisFeldGraphicsObject::textBoxHeight, incomingRect.height(), incomingRect.top()),
            ErlaubsnisFeldGraphicsObject::textBoxWidth, ErlaubsnisFeldGraphicsObject::textBoxHeight};

        this->drawEaTLabel(painter, eatLabelRect);

        // Erlaubnismelder for einfahrt
        // Einfahrmelder lamp is on if other Stellwerk has the Erlaubnis
        const auto einfahrMelderColor =
            (erlaubnisFeld.erlaubnisState() == Openstw::Simulation::AusfahrErlaubnisState::OtherStellwerk)
                ? ErlaubsnisFeldGraphicsObject::activeErlaubnisMelderColor
                : inactiveErlaubnisMelderColor;

        const QRectF einfahrMelderRect{incomingRect.right() - ErlaubsnisFeldGraphicsObject::horzPaddingToBorder -
                                           ErlaubsnisFeldGraphicsObject::erlaubnisMelderWidth,
                                       centerWithin(ErlaubsnisFeldGraphicsObject::erlaubnisMelderHeight,
                                                    incomingRect.height(), incomingRect.top()),
                                       ErlaubsnisFeldGraphicsObject::erlaubnisMelderWidth,
                                       ErlaubsnisFeldGraphicsObject::erlaubnisMelderHeight};
        this->drawErlaubnisMelder(painter, einfahrMelderRect, einfahrMelderColor, HorizontalDirection::Right);

        // Rect containing the outgoing melder
        const auto outgoingRect = this->calculateHemiRect(VerticalDirection::Bottom);

        // Outgoing melder
        // Ausfahrmelder lamp is on if this Stellwerk has the Erlaubnis
        const auto ausfahrMelderColor =
            (erlaubnisFeld.erlaubnisState() == Openstw::Simulation::AusfahrErlaubnisState::ThisStellwerk)
                ? ErlaubsnisFeldGraphicsObject::activeErlaubnisMelderColor
                : inactiveErlaubnisMelderColor;

        const QRectF ausfahrMelderRect{outgoingRect.left() + ErlaubsnisFeldGraphicsObject::horzPaddingToBorder,
                                       centerWithin(ErlaubsnisFeldGraphicsObject::erlaubnisMelderHeight,
                                                    outgoingRect.height(), outgoingRect.top()),
                                       ErlaubsnisFeldGraphicsObject::erlaubnisMelderWidth,
                                       ErlaubsnisFeldGraphicsObject::erlaubnisMelderHeight};

        this->drawErlaubnisMelder(painter, ausfahrMelderRect, ausfahrMelderColor, HorizontalDirection::Left);

        // Lamp
        // XXX implement active state
        const QRectF lampRect{
            ausfahrMelderRect.right() + ErlaubsnisFeldGraphicsObject::lampPadding,
            centerWithin(ErlaubsnisFeldGraphicsObject::lampDiameter, outgoingRect.height(), outgoingRect.top()),
            ErlaubsnisFeldGraphicsObject::lampDiameter, ErlaubsnisFeldGraphicsObject::lampDiameter};

        this->drawLamp(painter, lampRect, false);

        // Outgoing label
        const QRectF outgoingLabelRect{
            lampRect.right() + ErlaubsnisFeldGraphicsObject::lampPadding,
            centerWithin(ErlaubsnisFeldGraphicsObject::textBoxHeight, outgoingRect.height(), outgoingRect.top()),
            ErlaubsnisFeldGraphicsObject::textBoxWidth, ErlaubsnisFeldGraphicsObject::textBoxHeight};
        this->drawAusfahrLabel(painter, outgoingLabelRect, erlaubnisFeld.ausfahrRichtungLabel());

        painter->restore();
    }
}
