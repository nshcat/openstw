#include "xmlutility.hxx"

namespace Openstw::Core::Xml
{
    std::optional<QString> stringAttribute(QDomElement xmlElement, QStringView attributeName)
    {
        const auto attributeValue = xmlElement.attribute(attributeName.toString());
        if (attributeValue.isEmpty())
        {
            return std::nullopt;
        }

        return {attributeValue};
    }

    std::optional<int> intAttribute(QDomElement xmlElement, QStringView attributeName)
    {
        // First, try to retrieve the attributes value as a string.
        const auto attributeValue = stringAttribute(xmlElement, attributeName);

        // Bail out early if the attribute doesnt exist.
        if (!attributeValue.has_value())
        {
            return std::nullopt;
        }

        // Try to convert the attributes value to an integer.
        bool conversionOkay{false};
        const auto attributeIntValue = attributeValue->toInt(&conversionOkay);

        // Handle conversion failure
        if (!conversionOkay)
        {
            return std::nullopt;
        }

        // If conversion was successful, we are done.
        return attributeIntValue;
    }

    std::optional<double> doubleAttribute(QDomElement xmlElement, QStringView attributeName)
    {
        // First, try to retrieve the attributes value as a string.
        const auto attributeValue = stringAttribute(xmlElement, attributeName);

        // Bail out early if the attribute doesnt exist.
        if (!attributeValue.has_value())
        {
            return std::nullopt;
        }

        // Try to convert the attributes value to a double.
        bool conversionOkay{false};
        const auto attributeDoubleValue = attributeValue->toDouble(&conversionOkay);

        // Handle conversion failure
        if (!conversionOkay)
        {
            return std::nullopt;
        }

        // If conversion was successful, we are done.
        return attributeDoubleValue;
    }
}