#pragma once

#include <initializer_list>
#include <memory>
#include <type_traits>

namespace Openstw::Simulation
{
    template <class T, class U> std::unique_ptr<T> dynamic_pointer_cast(std::unique_ptr<U>&& r) noexcept
    {
        (void)dynamic_cast<T*>(static_cast<U*>(0));

        T* p = dynamic_cast<T*>(r.get());

        if (p)
            r.release();
        return std::unique_ptr<T>(p);
    }

    struct GridPosition
    {
        GridPosition() = default;
        GridPosition(std::size_t x, std::size_t y);

        std::size_t x{};
        std::size_t y{};
    };

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

        FlagField(std::initializer_list<E> values) : FlagField()
        {
            storage_t val{};
            for (const auto value : values)
            {
                val |= static_cast<storage_t>(value);
            }
            this->m_flags = val;
        }

    public:
        bool hasAny() const
        {
            return this->m_flags != storage_t{};
        }

        bool has(const E value) const
        {
            return (this->m_flags & static_cast<storage_t>(value)) != storage_t{};
        }

        void set(const E value)
        {
            this->m_flags |= static_cast<storage_t>(value);
        }

        void clear(const E value)
        {
            this->m_flags &= ~static_cast<storage_t>(value);
        }

    protected:
        storage_t m_flags;
    };
}
