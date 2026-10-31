#pragma once

#include <QObject>
#include <element.hxx>

namespace Openstw::Interlocking
{
    class SignalMast : public Element
    {
        Q_OBJECT

    public:
        explicit SignalMast(QStringView identifier, QObject* parent = nullptr);

    protected:
    };
}