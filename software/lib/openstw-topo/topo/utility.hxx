#pragma once

#include <QString>
#include <iostream>

static std::ostream& operator<<(std::ostream& stream, const QString& str)
{
    stream << str.toStdString();
    return stream;
}

static std::istream& operator>>(std::istream& stream, QString& str)
{
    std::string tmp;
    stream >> tmp;
    str = QString::fromStdString(tmp);
    return stream;
}