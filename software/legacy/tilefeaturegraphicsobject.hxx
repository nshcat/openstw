#pragma once

#include <QGraphicsObject>
#include <QObject>
#include <optional>
#include <spdr/featurelayer.hxx>
#include <spdr/featuretype.hxx>

#include "icontextmenuprovider.hxx"

class TileGraphicsObject;

namespace Rendering
{
    // A Qt graphics object that takes care of rendering a single feature layer
    // within a SpDr tile.
    class TileFeatureGraphicsObject : public QGraphicsObject, public IContextMenuProvider
    {
        Q_OBJECT

    public:
        TileFeatureGraphicsObject(Openstw::SpDr::FeatureType featureType, TileGraphicsObject* parent,
                                  bool providesContextMenu = false);

    public:
        // The specific type of feature layer this graphics object renders.
        Openstw::SpDr::FeatureType featureType() const;

        // Retrieve the tile graphics object this feature object belongs to.
        TileGraphicsObject* tileGraphicsObject() const;

    public:
        /**
         * @brief Finishes setting up the graphics object to be ready for rendering.
         * Is called from the main tile graphics object.
         */
        void setup();     

    public:
        virtual bool providesContextMenu() const override;

    protected:
        /**
         * @brief Called when the tile component renderer needs this component
         * to position itself relative to the parent tile component graphics object.
         * This is only called once at the beginning of the lifetime of each
         * component grapgics object.
         */
        virtual void positionSelf() = 0;

        /**
         * @brief Whether this tile component graphics objects wants to receive
         * mouse input, and if so, from which mouse buttons.
         * @return std::nullopt if no mouse input is needed, otherwise a set of mouse
         * buttons to react to.
         */
        virtual std::optional<Qt::MouseButtons> acceptsMouseInput() const;

        /**
         * @brief Called by the base class right after the component was layouted.
         * Can be used by subclasses to setup state that depends on the object bounding box
         * or location.
         */
        virtual void afterSetup();

    public:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        TileGraphicsObject* m_tileGraphicsObj{};
        bool m_providesContextMenu{false};
        Openstw::SpDr::FeatureType m_featureType;
    };
}
