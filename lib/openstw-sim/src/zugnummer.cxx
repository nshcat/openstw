#include "zugnummer.hxx"

namespace Openstw::Simulation
{
    Zugnummer::Zugnummer() : m_digits{Digit::None, Digit::None, Digit::None, Digit::None, Digit::None, Digit::None}
    {
    }

    Zugnummer::Zugnummer(const QString& str) : Zugnummer{QStringView{str}}
    {
    }

    Zugnummer::Zugnummer(QStringView str) : Zugnummer()
    {
        for (int strIdx = 0; strIdx < qMin(str.length(), 6); ++strIdx)
        {
            const auto reversedIdx = str.length() - strIdx;
            const auto chr = str[reversedIdx];

            Digit digit{Digit::None};
            if (chr.isDigit())
            {
                digit = static_cast<Digit>(chr.digitValue());
            }

            this->m_digits[reversedIdx] = digit;
        }
    }

    const Digit& Zugnummer::operator[](const std::size_t index) const
    {
        return this->m_digits[index];
    }

    Digit& Zugnummer::operator[](const std::size_t index)
    {
        return this->m_digits[index];
    }

    QString Zugnummer::toString() const
    {
        std::array<QChar, 6> characterBuffer{};

        for (int idx = 0; idx < 6; ++idx)
        {
            const auto digit = this->m_digits[idx];
            if (digit == Digit::None)
            {
                characterBuffer[idx] = ' ';
            }
            else
            {
                characterBuffer[idx] = static_cast<QChar>('0' + static_cast<std::int8_t>(digit));
            }
        }

        return QString{QStringView{characterBuffer.data(), characterBuffer.size()}};
    }
}
