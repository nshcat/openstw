#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include <QString>
#include <QStringView>
#include <QVector>

#include "logmessage.hxx"

// Class that collects all messages emitted by the Qt logging
// infrastrucutre, and acts as an abstract table model for the Ui.
class LogManager : public QAbstractTableModel
{
    Q_OBJECT

public:
    // The different data items that a log message is comprised of.
    enum Column
    {
        TimestampColumn = 0,
        CategoryColumn,
        MessageTypeColumn,
        TextColumn
    };

public:
    explicit LogManager(QObject* parent = nullptr);

public:
    // Add a new log message to the collection.
    void addMessage(QtMsgType type, const QString& category, const QString& text);

public: // QAbstractListModel implementation
    // Retrieves the total number of log messages currently in the collection.
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    // Retrieves the number of columns for the table.
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    // Supply the table model with a specific data item.
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Describes the header information for the table view.
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

protected:
    // Convert given Qt message severity type to its string representation.
    static QStringView messageTypeToString(QtMsgType type);

protected:
    // Collection of all log messages received so far.
    QVector<LogMessage*> m_messages{};
};
