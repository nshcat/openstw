#pragma once

#include <QString>
#include <QStringView>
#include <array>

#include "enums.hxx"

namespace Openstw::Simulation
{
    /**
     * @brief Class managing the up to six digits of a Zugnummer
     */
    class Zugnummer
    {
    public:
        Zugnummer();

        explicit Zugnummer(const QString& str);
        explicit Zugnummer(QStringView str);

    public:
        const Digit& operator[](const std::size_t index) const;
        Digit& operator[](const std::size_t index);
        QString toString() const;

    protected:
        std::array<Digit, 6> m_digits{};
    };
}
