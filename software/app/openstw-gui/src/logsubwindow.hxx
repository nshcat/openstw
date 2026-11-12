#pragma once

#include <QObject>
#include <QTreeView>

#include "subwindow.hxx"

class LogSubWindow : public SubWindow
{
    Q_OBJECT

public:
    LogSubWindow(QWidget* parent);

protected:
    // The tree view that shows the log messages.
    // We use a QTreeView here because it just looks nicer -
    // we are not actually displaying a tree of items, just a flat
    // list of log messages.
    QTreeView* m_messageView;
};
