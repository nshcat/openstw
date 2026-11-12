#pragma once

#include <QDateTime>
#include <QObject>
#include <QtLogging>

// Class representing a single log message received from the
// Qt logging infrastructure.
class LogMessage : public QObject
{
    Q_OBJECT

public:
    LogMessage(QtMsgType type, const QString& category, const QString& text, QObject* parent = nullptr);

public:
    // The timestamp of the message.
    const QDateTime& timestamp() const;

    // Severity type of this message.
    QtMsgType type() const;

    // The logging category that emitted the log message.
    const QString& category() const;

    // The text contents of the log message.
    const QString& text() const;

protected:
    // The timestamp of the message.
    QDateTime m_timestamp{QDateTime::currentDateTime()};

    // Severity type of this message.
    QtMsgType m_type;

    // The logging category that emitted the log message.
    QString m_category;

    // The text contents of the log message.
    QString m_text;
};
