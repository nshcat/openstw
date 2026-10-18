#pragma once

#include <QString>
#include <format>
#include <stdexcept>

template <>
struct std::formatter<QString> : std::formatter<std::string>
{
    auto format(const QString& str, auto& ctx) const
    {
        auto stdStr = str.toStdString();
        return std::formatter<std::string>::format(stdStr, ctx);
    }
};

template <>
struct std::formatter<QStringView> : std::formatter<QString>
{
    auto format(const QStringView& strView, auto& ctx) const
    {
        auto str = QString{strView};
        return std::formatter<QString>::format(str, ctx);
    }
};

namespace Openstw
{
    namespace Topology
    {
        // Wrapper around std::runtime_error that allows easy
        // use of std::format to set the exception message.
        class RuntimeError : public std::runtime_error
        {
        public:
            template <class... Args>
            RuntimeError(std::format_string<Args...> what_arg_fmt, Args&&... args)
                : runtime_error{std::format(what_arg_fmt, std::forward<Args>(args)...)}
            {
            }
        };
    }
}