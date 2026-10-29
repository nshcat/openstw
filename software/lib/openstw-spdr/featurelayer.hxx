#pragma once

#include "displayelement.hxx"
#include "featuretype.hxx"
#include <QObject>
#include <QSpan>
#include <QVector>

namespace Openstw::SpDr
{
    // A single, self-contained feature found on a SpDr tile.
    // Features take care of listening to their corresponding
    // interlocking objects and deriving display element state
    // from the state of those objects.
    class FeatureLayer : public QObject
    {
        Q_OBJECT

    public:
        explicit FeatureLayer(FeatureType type, QObject* parent = nullptr);

        // Pure virtual destructor to force the class to be abstract.
        virtual ~FeatureLayer() = 0;

    public:
        // Specific type of feature this layer represents.
        FeatureType type() const;

        // All display elements this feature layer manages.
        QSpan<DisplayElement const* const> displayElements() const;

    protected:
        // Add given display element to the collection of elements
        // this feature layer manages. This should be called while
        // constructing a specific feature layer sub class to populate
        // the layer with elements.
        // It takes care of listening to the elements state change
        // signals, and routing them to the feature layers `elementStateChanged`
        // signal.
        void addElement(DisplayElement* element);

        // Notify our clients via a signal that the state of a display element managed
        // by this feature layer has changed.
        void notifyElementStateChanged(DisplayElement* element, const ElementState& newState);

    signals:
        // Emitted whenever the state of an display element managed by this
        // feature layer changes.
        void elementStateChanged(Openstw::SpDr::DisplayElement* element, Openstw::SpDr::ElementState newState);

    protected:
        // Type of feature this layer represents.
        FeatureType m_type;

        // All display elements managed by this layer.
        // They are owned by this class, and are thus children of it!
        QVector<DisplayElement*> m_displayElements{};
    };
}