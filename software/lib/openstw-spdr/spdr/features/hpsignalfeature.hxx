#pragma once

#include <QDomElement>
#include <QObject>
#include <interlocking/elements/signalmast.hxx>
#include <interlocking/hauptsignalschirm.hxx>
#include <interlocking/system.hxx>

#include "../aperture.hxx"
#include "../featureorientation.hxx"
#include "../featuretype.hxx"
#include "../orientedfeaturelayer.hxx"

namespace Openstw::SpDr
{
    // A feature representing a single main signal.
    class HpSignalFeature : public OrientedFeatureLayer
    {
        Q_OBJECT

    public:
        HpSignalFeature(Interlocking::SignalMast* signalMast, FeatureOrientation orientation,
                        QObject* parent = nullptr);

        virtual ~HpSignalFeature() override = default;

    public:
        // Parse main signal feature from given XML element.
        static HpSignalFeature* createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                              QObject* parent = nullptr);

    public:
        // Aperture for the red signal lamp.
        Aperture* redLamp() const;

        // Aperture for the green signal lamp.
        Aperture* greenLamp() const;

    protected:
        // Update the state of the lamp apertures, based on the
        // aspect the referenced Hauptsignalschirm is currently showing.
        void updateLampApertures();

    protected slots:
        // Called whenever the current aspect of the referenced Hauptsignalschirm changes.
        void hauptSignalBildChanged(Interlocking::HauptSignalBild newSignalBild);

    protected:
        // Reference to the Hauptsignalschirm whose state we are mirroring
        // using our apertures.
        Interlocking::HauptSignalSchirm* m_signalSchirm;

        // Aperture for the red signal lamp.
        Aperture* m_redLamp{};

        // Aperture for the green signal lamp.
        Aperture* m_greenLamp{};
    };
}
