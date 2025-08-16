#pragma once

#include <type_traits>

namespace Openstw::Simulation
{
    template <typename E> class FlagField
    {
        using storage_t = std::underlying_type_t<E>;

    public:
        FlagField() : FlagField(storage_t{})
        {
        }

        explicit FlagField(const storage_t value) : m_flags{value}
        {
        }

    protected:
        storage_t m_flags;
    };
}
