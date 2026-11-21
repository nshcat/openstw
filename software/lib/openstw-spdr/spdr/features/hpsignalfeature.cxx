#include <core/enums.hxx>
#include <core/exception.hxx>
#include <core/xmlutility.hxx>

#include "hpsignalfeature.hxx"
#include "logging.hxx"

namespace Openstw::SpDr
{
    HpSignalFeature::HpSignalFeature(Interlocking::SignalMast* signalMast, FeatureOrientation orientation,
                                     QObject* parent)
        : OrientedFeatureLayer{FeatureType::HpSignal, orientation, parent}
    {
        // First, create apertures used to display the signal state.
        if (this->orientation() == FeatureOrientation::Forward)
        {
            this->m_redLamp = this->createAperture(ForwardIdentifiers::redAperture);
            this->m_greenLamp = this->createAperture(ForwardIdentifiers::greenAperture);
        }
        else
        {
            this->m_redLamp = this->createAperture(BackwardIdentifiers::redAperture);
            this->m_greenLamp = this->createAperture(BackwardIdentifiers::greenAperture);
        }

        // Then, retrieve the Hauptsignalschirm attached to given Signalmast.
        if (!signalMast->hasHauptSignalSchirm())
        {
            throw Core::ArgumentError("Signal mast '{}' has no Hauptsignalschirm", signalMast->identifier());
        }

        this->m_signalSchirm = signalMast->hauptSignalSchirm();

        // Listen to aspect change signals coming from the Hauptsignalschirm.
        // clang-format off
        connect(this->m_signalSchirm, &Interlocking::HauptSignalSchirm::signalBildChanged,
                this, &HpSignalFeature::hauptSignalBildChanged);
        // clang-format on

        // Perform initial synchronization
        this->updateLampApertures();
    }

    HpSignalFeature* HpSignalFeature::createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                                    QObject* parent)
    {
        // Retrieve the signal mast that fills this feature with data.
        auto* mast =
            FeatureLayer::parseInterlockingReference<Interlocking::SignalMast>(xmlElement, u"SignalMast", interlocking);

        // Parse feature orientation.
        const auto orientation = OrientedFeatureLayer::parseOrientation(xmlElement);

        // Create feature layer object and return
        return new HpSignalFeature{mast, orientation, parent};
    }

    Aperture* HpSignalFeature::redLamp() const
    {
        return this->m_redLamp;
    }

    Aperture* HpSignalFeature::greenLamp() const
    {
        return this->m_greenLamp;
    }

    void HpSignalFeature::updateLampApertures()
    {
        // Retrieve current Signalbild
        const auto currentAspect = this->m_signalSchirm->signalBild();

        // Update lamps according to the aspect
        switch (currentAspect)
        {
            // Both Fahrt and Langsamfahrt are represented as a single
            // green lamp on SpDr tiles.
        case Interlocking::HauptSignalBild::Fahrt:
        case Interlocking::HauptSignalBild::LangsamFahrt:
            this->m_greenLamp->updateSolid(ApertureColor::Green);
            this->m_redLamp->updateOff();
            break;

        case Interlocking::HauptSignalBild::Halt:
            this->m_greenLamp->updateOff();
            this->m_redLamp->updateSolid(ApertureColor::Red);
            break;

        default:
            // clang-format off
            qCCritical(catSpdr)
                << "HpSignal feature encountered unknown Hauptsignalbild '"
                << Core::toString(currentAspect)
                << "', ignoring"
                << Qt::endl;
            // clang-format on

            return;
        }
    }

    void HpSignalFeature::hauptSignalBildChanged(Interlocking::HauptSignalBild newSignalBild)
    {
        // clang-format off
        qCDebug(catSpdr)
            << "Hauptsignalschirm on mast '"
            << this->m_signalSchirm->mast()->identifier()
            << "' has changed aspect to '"
            << Core::toString(newSignalBild)
            << "', updating HpSignalFeature apertures"
            << Qt::endl;
        // clang-format on

        this->updateLampApertures();
    }
}
