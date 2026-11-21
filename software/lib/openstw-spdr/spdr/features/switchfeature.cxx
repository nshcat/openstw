#include "switchfeature.hxx"

namespace Openstw::SpDr
{

    SwitchFeature::SwitchFeature(SwitchStyle style, SwitchBranchOrientation branchOrientation,
                                 FeatureOrientation orientation, Interlocking::Switch* switchElement, QObject* parent)
        : OrientedFeatureLayer{FeatureType::Switch, orientation, parent},
          m_style{style},
          m_branchOrientation{branchOrientation},
          m_switchElement{switchElement}
    {
        // Create apertures. Since there can only ever be at most one switch
        // feature on any given SpDr tile, we do not need to include the orientation
        // in the aperture identifiers.
        this->m_stem = this->createAperture(Identifiers::stemAperture);
        this->m_straight = this->createAperture(Identifiers::straightAperture);
        this->m_branch = this->createAperture(Identifiers::branchAperture);

        // Create the button representing the Weichentaste.
        this->m_weichenTaste =
            this->createButton(Identifiers::weichenTasteButton, ButtonRole::WeichenTaste, this->m_switchElement);
    }

    SwitchFeature* SwitchFeature::createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                                QObject* parent)
    {
        // Parse the referenced switch. This will throw if any step fails.
        auto* switchElement =
            FeatureLayer::parseInterlockingElement<Interlocking::Switch>(xmlElement, u"Switch", interlocking);

        // Parse the general feature orientation.
        auto orientation = OrientedFeatureLayer::parseOrientation(xmlElement);

        // Parse switch style
        const auto style = Core::Xml::enumAttribute<SwitchStyle>(xmlElement, u"Style");
        if (!style.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Switch feature element missing style attribute",
                                     xmlElement.lineNumber());
        }

        // Parse branch orientation
        const auto branchOrientation = Core::Xml::enumAttribute<SwitchBranchOrientation>(xmlElement, u"Branch");
        if (!branchOrientation.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Switch feature element missing branch orientation attribute",
                                     xmlElement.lineNumber());
        }

        // Finally, construct switch feature layer object
        return new SwitchFeature{*style, *branchOrientation, orientation, switchElement, parent};
    }

    SwitchStyle SwitchFeature::style() const
    {
        return this->m_style;
    }

    SwitchBranchOrientation SwitchFeature::branchOrientation() const
    {
        return this->m_branchOrientation;
    }

    Aperture* SwitchFeature::stemLamp() const
    {
        return this->m_stem;
    }

    Aperture* SwitchFeature::straightLamp() const
    {
        return this->m_straight;
    }

    Aperture* SwitchFeature::branchLamp() const
    {
        return this->m_branch;
    }

    Button* SwitchFeature::weichenTaste() const
    {
        return this->m_weichenTaste;
    }
}