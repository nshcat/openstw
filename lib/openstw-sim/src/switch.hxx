#pragma once

#include <memory>
#include <pugixml.hpp>
#include <stdexcept>
#include <type_traits>

namespace Openstw::Simulation
{
    enum class SwitchType
    {
        // Just a simple, two-way switch
        Simple = 0,

        // Switch that derails the train if not in straight position
        GleisSperre = 1
    };

    /**
     * @brief Base class for the various types of switches in the route network.
     */
    class Switch
    {
    protected:
        Switch(const SwitchType type);

    public:
        virtual ~Switch() = default;

        Switch(const Switch&) = delete;
        Switch(Switch&&) = default;

        Switch& operator=(const Switch&) = delete;
        Switch& operator=(Switch&&) = default;

    public:
        static std::unique_ptr<Switch> CreateFrom(const pugi::xml_node&);

    public:
        SwitchType type() const;

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Switch, std::decay_t<T>>>>
        T* as()
        {
            T* ptr = dynamic_cast<T*>(this);
            if (ptr == nullptr)
                throw std::runtime_error("Dynamic type error");

            return ptr;
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Switch, std::decay_t<T>>>>
        const T* as() const
        {
            const T* ptr = dynamic_cast<const T*>(this);
            if (ptr == nullptr)
                throw std::runtime_error("Dynamic type error");

            return ptr;
        }

    protected:
        SwitchType m_type;
    };
}
