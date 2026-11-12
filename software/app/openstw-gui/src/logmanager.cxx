#include <QStringLiteral>
#include <qstringliteral.h>

#include "logmanager.hxx"

LogManager::LogManager(QObject* parent)
    : QAbstractTableModel{parent}
{
}

void LogManager::addMessage(QtMsgType type, const QString& category, const QString& text)
{
    const auto newRow = this->m_messages.size();
    beginInsertRows(QModelIndex(), newRow, newRow);
    this->m_messages.append(new LogMessage{type, category, text, this});
    endInsertRows();
}

int LogManager::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        // Entries have no child items, we are a flat list.
        return 0;
    }

    return this->m_messages.count();
}

int LogManager::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        // Entries have no child items, we are a flat list.
        return 0;
    }

    return 4;
}

QVariant LogManager::data(const QModelIndex& index, int role) const
{
    // Perform bounds check
    if (!index.isValid() || index.row() < 0 || index.row() >= this->m_messages.count())
    {
        // No data available for given index.
        return {};
    }

    // Retrieve the log message at given index.
    const auto* logMessage = this->m_messages.at(index.row());

    // Determine the exact column requested by the caller.
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case TimestampColumn:
            return logMessage->timestamp().toString(u"HH:mm:ss");

        case MessageTypeColumn:
            return LogManager::messageTypeToString(logMessage->type()).toString();

        case CategoryColumn:
            return logMessage->category();

        case TextColumn:
            return logMessage->text();

        default:
            return {};
        }
    }
    else
    {
        return {};
    }
}

QVariant LogManager::headerData(int section, Qt::Orientation orientation, int role) const
{
    // We only support horizontal display orientation.
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    {
        return {};
    }

    // Return column header information.
    switch (section)
    {
    case TimestampColumn:
        return "Time";

    case CategoryColumn:
        return "Category";

    case MessageTypeColumn:
        return "Level";

    case TextColumn:
        return "Message";

    default:
        return {};
    }
}

QStringView LogManager::messageTypeToString(QtMsgType type)
{
    switch (type)
    {
    case QtMsgType::QtDebugMsg:
        return {u"Debug"};

    case QtMsgType::QtInfoMsg:
        return {u"Info"};

    case QtMsgType::QtWarningMsg:
        return {u"Warning"};

    case QtMsgType::QtFatalMsg:
        return {u"Fatal"};

    case QtMsgType::QtCriticalMsg:
        return {u"Critical"};

    default:
        return {u"unknown"};
    }
}