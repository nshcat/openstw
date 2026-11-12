#pragma once

#include <QGraphicsObject>
#include <QObject>
#include <optional>
#include <spdr/featurelayer.hxx>
#include <spdr/featuretype.hxx>

#include "../blinkgenerator.hxx"
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
        // Base class constructor.
        // Parameter `requiresBlinkImpulse` controls whether the global blink
        // timer will cause this feature to be invalidated and redrawn.
        // Parameter `providesContextMenu` controls whether the user can right click
        // the feature to interact with a context menu.
        TileFeatureGraphicsObject(Openstw::SpDr::FeatureType featureType, TileGraphicsObject* parent,
                                  bool requiresBlinkImpulse, bool providesContextMenu = false);

    public:
        // The specific type of feature layer this graphics object renders.
        Openstw::SpDr::FeatureType featureType() const;

        // Retrieve the tile graphics object this feature object belongs to.
        TileGraphicsObject* tileGraphicsObject() const;

    public:
        // Finishes setting up the graphics object to be ready for rendering.
        // Is called from the main tile graphics object.
        void setup();     

    public:
        // Implements `IContextMenuProvider`.
        virtual bool providesContextMenu() const override;

        // Main graphics object painting routine.
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        // The global blink generator used to render blinking apertures.
        BlinkGenerator* blinkGenerator() const;

        // Determine the colour the given aperture should be rendered in.
        // This also incorporates the global blink generator state.
        QColor apertureRenderColor(const Openstw::SpDr::Aperture* aperture) const;

        // Determine whether the given aperture is currently on. This correctly
        // incorporates the global blink timer when it comes to blinking apertures.
        bool apertureIsOn(const Openstw::SpDr::Aperture* aperture) const;

    protected:
        // Called when the tile component renderer needs this component
        // to position itself relative to the parent tile component graphics object.
        // This is only called once at the beginning of the lifetime of each
        // component grapgics object.
        virtual void positionSelf() = 0;

        // Whether this tile component graphics objects wants to receive
        // mouse input, and if so, from which mouse buttons.
        // Returns std::nullopt if no mouse input is needed, otherwise a set of mouse
        // buttons to react to.
        virtual std::optional<Qt::MouseButtons> acceptsMouseInput() const;

        // Called by the base class right after the component was layouted.
        // Can be used by subclasses to setup state that depends on the object bounding box
        // or location.
        virtual void afterSetup();

    protected slots:
        // Called whenever the global blink timer fires.
        // This slot is only connected to if `m_requiresBlinkImpulse`
        // is true.
        void onBlinkImpulse(BlinkState state);

    protected:
        // Reference to the graphics object for the tile this feature layer belongs to.
        TileGraphicsObject* m_tileGraphicsObj{};

        // Whether this feature provides a context menu to the user.
        bool m_providesContextMenu{false};

        // The specific type of feature layer this graphics object represents.
        Openstw::SpDr::FeatureType m_featureType;

        // Whether this feature has apertures that can blink, and thus needs to
        // be rerendered whenever the global blink timer fires.
        bool m_requiresBlinkImpulse{false};
    };
}
