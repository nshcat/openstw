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
        auto* redLamp = new Aperture("signal-red-lamp", this);
        this->addAperture(redLamp);
        this->m_redLamp = redLamp;

        auto* greenLamp = new Aperture("signal-green-lamp", this);
        this->addAperture(greenLamp);
        this->m_greenLamp = greenLamp;

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
        const auto signalName = Core::Xml::stringAttribute(xmlElement, u"SignalMast");
        if (!signalName.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: HpSignal feature element missing signal mast name attribute",
                                     xmlElement.lineNumber());
        }

        // Try to find the element within the interlocking system
        // that represents the referenced signal mast
        auto mastElement = interlocking->element(signalName.value());
        if (!mastElement.has_value())
        {
            throw Core::RuntimeError(
                "SpDr XML: Line {}: HpSignal feature element references non-existent signal mast '{}'",
                xmlElement.lineNumber(), signalName.value());
        }

        // Validate that the element is, in fact, a signal mast
        if (mastElement.value()->type() != Interlocking::ElementType::SignalMast)
        {
            throw Core::RuntimeError("SpDr XML: Line {}: HpSignal feature element references interlocking "
                                     "element '{}', which is not a signal mast",
                                     xmlElement.lineNumber(), signalName.value());
        }

        // Cast it.
        auto* mast = dynamic_cast<Interlocking::SignalMast*>(mastElement.value());

        // Validate the cast.
        if (!mast)
        {
            throw Core::RuntimeError("SpDr XML: Line {}: HpSignal feature element references interlocking "
                                     "element '{}', which unexpectedly is not a SignalMast object",
                                     xmlElement.lineNumber(), signalName.value());
        }

        // Parse feature orientation.
        const auto orientation = Core::Xml::enumAttribute<FeatureOrientation>(xmlElement, u"Orientation");
        if (!orientation.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: HpSignal feature element missing orientation attribute",
                                     xmlElement.lineNumber());
        }

        // Create feature layer object and return
        return new HpSignalFeature{mast, orientation.value(), parent};
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
