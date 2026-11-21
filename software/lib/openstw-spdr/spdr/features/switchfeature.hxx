#pragma once

#include <QDomElement>
#include <QObject>

#include <interlocking/elements/switch.hxx>
#include <interlocking/system.hxx>

#include <orientedfeaturelayer.hxx>
#include <switchbranchorientation.hxx>
#include <switchstyle.hxx>

namespace Openstw::SpDr
{
    // SpDr tile feature that represents a switch.
    class SwitchFeature : public OrientedFeatureLayer
    {
        Q_OBJECT

    protected:
        // Aperture identifiers for all orientations.
        struct Identifiers
        {
            constexpr static QStringView stemAperture{u"switch-stem"};
            constexpr static QStringView straightAperture{u"switch-straight"};
            constexpr static QStringView branchAperture{u"switch-branch"};

            constexpr static QStringView weichenTasteButton{u"switch-weichentaste"};
        };

    public:
        explicit SwitchFeature(SwitchStyle style, SwitchBranchOrientation branchOrientation,
                               FeatureOrientation orientation, Interlocking::Switch* switchElement,
                               QObject* parent = nullptr);

        virtual ~SwitchFeature() override = default;

    public:
        // Parse switch feature from given XML element.
        static SwitchFeature* createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                            QObject* parent = nullptr);

    public:
        // General style of the switch representation.
        SwitchStyle style() const;

        // Orientation of the switch branch.
        SwitchBranchOrientation branchOrientation() const;

        // Aperture for the lamp under the switch stem.
        Aperture* stemLamp() const;

        // Aperture for the lamp under the switch straight arm.
        Aperture* straightLamp() const;

        // Aperture for the lamp under the switch branch arm.
        Aperture* branchLamp() const;

        // Button representing the Weichentaste for the switch.
        Button* weichenTaste() const;

    protected slots:

    protected:
        // Reference to the switch element within the interlocking layer.
        Interlocking::Switch* m_switchElement;

        // General style of the switch representation.
        SwitchStyle m_style;

        // Orientation of the switch branch. Together with the general feature orientation
        // stored within the base class, this uniquely describes the overall switch
        // orientation.
        SwitchBranchOrientation m_branchOrientation;

        // Aperture for the switch stem.
        Aperture* m_stem;

        // Aperture for the switch branch arm.
        Aperture* m_branch;

        // Aperture for the switch straight arm.
        Aperture* m_straight;

        // Button for the Weichentaste
        Button* m_weichenTaste;
    };
}
