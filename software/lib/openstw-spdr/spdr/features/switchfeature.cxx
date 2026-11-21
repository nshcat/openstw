#include "switchfeature.hxx"

namespace Openstw::SpDr
{

    SwitchFeature::SwitchFeature(SwitchStyle style, SwitchBranchOrientation branchOrientation,
                                 FeatureOrientation orientation, Interlocking::Switch* switchElement,
                                 Interlocking::TrackSection* sectionElement, QObject* parent)
        : OrientedFeatureLayer{FeatureType::Switch, orientation, parent},
          m_style{style},
          m_branchOrientation{branchOrientation},
          m_switchElement{switchElement},
          m_sectionElement{sectionElement}
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

        // Start listening to state change events emitted by the interlocking switch.
        // clang-format off
        connect(this->m_switchElement, &Interlocking::Switch::stateChanged,
                this, &SwitchFeature::onSwitchStateChanged);
        // clang-format on

        // Also listen for button presses on the Weichentaste.
        // clang-format off
        connect(this->m_weichenTaste, &Button::stateChanged,
                this, &SwitchFeature::onWeichenTasteStateChanged);
        // clang-format on

        // And finally, listen to occupancy changed events
        // coming from the track section we reference for occupancy information.
        // clang-format off
        connect(this->m_sectionElement, &Interlocking::TrackSection::occupancyChanged,
                this, &SwitchFeature::onSectionOccupancyChanged);
        // clang-format on

        // Perform initial aperture update
        this->updateApertures();
    }

    SwitchFeature* SwitchFeature::createFromXml(QDomElement xmlElement, Interlocking::System* interlocking,
                                                QObject* parent)
    {
        // Parse the referenced switch. This will throw if any step fails.
        auto* switchElement =
            FeatureLayer::parseInterlockingReference<Interlocking::Switch>(xmlElement, u"Switch", interlocking);

        // Parse the referenced track section.
        auto* sectionElement = FeatureLayer::parseInterlockingReference<Interlocking::TrackSection>(
            xmlElement, u"TrackSection", interlocking);

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
        return new SwitchFeature{*style, *branchOrientation, orientation, switchElement, sectionElement, parent};
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

    void SwitchFeature::updateApertures()
    {
        // First, check whether the switch is currently locked or
        // whether its associated track section is occupied.
        // If so, it will keep both its stem and active branch lit up.
        if (this->m_switchElement->locked() || this->m_sectionElement->occupied())
        {
            const auto apertureColor = (this->m_sectionElement->occupied() ? ApertureColor::Red : ApertureColor::White);

            // Whether we light up the stem or not depends on whether the switch
            // is locked for flankenschutz or fahrstrasse. With flankenschutz,
            // the stem is not lit up.
            bool isFlankenSchutz =
                (this->m_switchElement->locked() &&
                 this->m_switchElement->lockReason() == Interlocking::LockingReason::FlankProtection);

            if (!isFlankenSchutz)
            {
                this->m_stem->updateSolid(apertureColor);
            }
            else
            {
                this->m_stem->updateOff();
            }

            // Light up the currently active switch branch, either in white if the switch
            // is merely locked, or red if the associated track section is currently occupied.
            if (this->m_switchElement->currentPosition() == Interlocking::SwitchPosition::Straight)
            {
                this->m_branch->updateOff();
                this->m_straight->updateSolid(apertureColor);
            }
            else
            {
                this->m_branch->updateSolid(apertureColor);
                this->m_straight->updateOff();
            }
        }
        else
        {
            // Check if we are currently changing the switch position!
            if (this->m_switchElement->isAdjusting())
            {
                // Let the target branch blink.
                this->m_stem->updateOff();

                if (this->m_switchElement->targetPosition() == Interlocking::SwitchPosition::Straight)
                {
                    this->m_branch->updateOff();
                    this->m_straight->updateBlinking(ApertureColor::White);
                }
                else
                {
                    this->m_branch->updateBlinking(ApertureColor::White);
                    this->m_straight->updateOff();
                }
            }
            else
            {
                // Otherwise, we only show the currently active branch if the Weichentaste
                // is pressed down.
                this->m_stem->updateOff();

                if (this->m_weichenTaste->pressed())
                {
                    if (this->m_switchElement->currentPosition() == Interlocking::SwitchPosition::Straight)
                    {
                        this->m_branch->updateOff();
                        this->m_straight->updateSolid(ApertureColor::White);
                    }
                    else
                    {
                        this->m_branch->updateSolid(ApertureColor::White);
                        this->m_straight->updateOff();
                    }
                }
                else
                {
                    this->m_branch->updateOff();
                    this->m_straight->updateOff();
                }
            }
        }
    }

    void SwitchFeature::onSwitchStateChanged(Interlocking::SwitchPositionNS::SwitchPosition currentPosition,
                                             Interlocking::SwitchPositionNS::SwitchPosition targetPosition,
                                             bool isAdjusting)
    {
        this->updateApertures();
    }

    void SwitchFeature::onWeichenTasteStateChanged(ButtonState newState)
    {
        this->updateApertures();
    }

    void SwitchFeature::onSectionOccupancyChanged(bool occupied)
    {
        this->updateApertures();
    }
}