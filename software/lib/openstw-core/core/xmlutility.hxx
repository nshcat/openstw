#pragma once

#include "enums.hxx"
#include <QDomElement>
#include <QString>
#include <QStringView>
#include <optional>
#include <type_traits>

// Utilities for XML parsing
namespace Openstw::Core::Xml
{
    // Retrieve XML element attribute value as string, if it exists.
    // Will return std::nullopt if the attribute doesn't exist.
    std::optional<QString> stringAttribute(QDomElement xmlElement, QStringView attributeName);

    // Retrieve XML element attribute value as integer, if it exists.
    // Will return std::nullopt if the attribute doesn't exist, or if
    // its value can't be converted to an integer.
    std::optional<int> intAttribute(QDomElement xmlElement, QStringView attributeName);

    // Retrieve XML element attribute value as double, if it exists.
    // Will return std::nullopt if the attribute doesn't exist, or if
    // its value can't be converted to a double.
    std::optional<double> doubleAttribute(QDomElement xmlElement, QStringView attributeName);

    // Retrieve XML element attribute value as enum of type E, if it exists.
    // Will return std::nullopt if the attribute doesn't exist, or if
    // its value can't be converted to an enum of type E.
    template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
    std::optional<E> enumAttribute(QDomElement xmlElement, QStringView attributeName)
    {
        // Retrieve attribute value as string.
        const auto stringValue = stringAttribute(xmlElement, attributeName);

        // Bail out early if the attribute doesnt exist.
        if (!stringValue.has_value())
        {
            return std::nullopt;
        }

        // Try to convert the attributes string value to the requested
        // enum type E. This will return std::nullopt if the conversion
        // was unsuccessful.
        return fromString<E>(stringValue.value());
    }
}