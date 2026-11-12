#include "logmessage.hxx"

LogMessage::LogMessage(QtMsgType type, const QString& category, const QString& text, QObject* parent)
    : QObject{parent}, m_type{type}, m_category{category}, m_text{text}
{
}

const QDateTime& LogMessage::timestamp() const
{
    return this->m_timestamp;
}

QtMsgType LogMessage::type() const
{
    return this->m_type;
}

const QString& LogMessage::category() const
{
    return this->m_category;
}

const QString& LogMessage::text() const
{
    return this->m_text;
}
