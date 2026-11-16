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

    protected:
        // Unique aperture identifiers that depend on the orientation
        // of this feature. This is required since there might be two
        // signal features on a single tile, and not including the orientation
        // would result in name clashes, which would make signals ambiguous
        // on any level higher than individual feature layer objects!

        // Aperture identifiers for forward orientation
        struct ForwardIdentifiers
        {
            constexpr static QStringView greenAperture{u"hpsignal-fwd-green"};
            constexpr static QStringView redAperture{u"hpsignal-fwd-red"};
        };

        // Aperture identifiers for backward orientation
        struct BackwardIdentifiers
        {
            constexpr static QStringView greenAperture{u"hpsignal-bck-green"};
            constexpr static QStringView redAperture{u"hpsignal-bck-red"};
        };

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
