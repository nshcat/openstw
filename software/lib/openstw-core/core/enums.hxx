#pragma once

#include <QObject>
#include <QString>
#include <QStringView>
#include <optional>
#include <qmetaobject.h>
#include <type_traits>

namespace Openstw::Core
{
    // Convert given enumeration value to its string representation.
    // This requires the enumeration to be marked with Q_ENUM.
    template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
    QString toString(E value)
    {
        // Retrieve the meta object associated with the enum.
        const auto& metaEnum = QMetaEnum::fromType<E>();

        // Then, using that meta object, retrieve the enum values string
        // representation.
        const auto* rawString = metaEnum.valueToKey(static_cast<std::underlying_type_t<E>>(value));

        return QString{rawString};
    }

    // Convert given enumeration value string representation to its
    // typed enumeration value.
    template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
    std::optional<E> fromString(QStringView value)
    {
        // Retrieve the meta object associated with the enum.
        const auto& metaEnum = QMetaEnum::fromType<E>();

        // Try to convert the given string representation to
        // its corresponding raw enumeration value.
        const auto rawStringValue = value.toLocal8Bit();
        bool wasOkay{false};
        const auto rawValue = metaEnum.keyToValue(rawStringValue.data(), &wasOkay);

        // Verify that the conversion was successful.
        if (!wasOkay)
        {
            return std::nullopt;
        }

        // If it was, cast the raw enum value to the enumeration type.
        return {static_cast<E>(rawValue)};
    }
}