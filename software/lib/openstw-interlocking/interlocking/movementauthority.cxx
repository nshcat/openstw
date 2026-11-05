#include "movementauthority.hxx"
#include <core/exception.hxx>

namespace Openstw::Interlocking
{
    MovementAuthority::MovementAuthority(MovementAuthorityType type, std::optional<double> maxSpeed)
        : m_type{type}, m_maxSpeed{maxSpeed}
    {
    }

    MovementAuthority MovementAuthority::FullSpeed()
    {
        return MovementAuthority{MovementAuthorityType::FullSpeed, std::nullopt};
    }

    MovementAuthority MovementAuthority::ReducedSpeed(double maxSpeed)
    {
        return MovementAuthority{MovementAuthorityType::ReducedSpeed, {maxSpeed}};
    }

    MovementAuthority MovementAuthority::OnSight()
    {
        return MovementAuthority{MovementAuthorityType::OnSight, std::nullopt};
    }

    MovementAuthority MovementAuthority::Shunting()
    {
        return MovementAuthority{MovementAuthorityType::Shunting, std::nullopt};
    }

    MovementAuthorityType MovementAuthority::type() const
    {
        return this->m_type;
    }

    bool MovementAuthority::forTrain() const
    {
        return this->type() != MovementAuthorityType::Shunting;
    }

    bool MovementAuthority::forShunting() const
    {
        return !this->forTrain();
    }

    double MovementAuthority::speedRestriction() const
    {
        if (!this->m_maxSpeed.has_value())
        {
            throw Core::RuntimeError("Movement authority has no speed restriction");
        }

        return this->m_maxSpeed.value();
    }

    bool MovementAuthority::speedRestricted() const
    {
        return this->m_maxSpeed.has_value();
    }
}
