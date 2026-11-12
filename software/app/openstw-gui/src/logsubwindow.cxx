#include <QHeaderView>
#include <QScrollBar>
#include <QVBoxLayout>

#include "logmanager.hxx"
#include "logsubwindow.hxx"

// Reference to the global log manager instance.
extern LogManager* g_LogManager;

LogSubWindow::LogSubWindow(QWidget* parent)
    : SubWindow{true, parent}
{
    this->setWindowTitle("Log Messages");
    this->resize(850, 250);

    this->m_messageView = new QTreeView();
    this->m_messageView->setModel(g_LogManager);

    // All log items will have the exact same height.
    // This allows the list view to do some internal
    // optimizations.
    this->m_messageView->setUniformRowHeights(true);

    // Do not render the root item, and do not add indentation
    // to our items (which is usually meant to allow the user to
    // tell apart root items from items on the first level)
    this->m_messageView->setRootIsDecorated(false);

    // Always select the whole row.
    this->m_messageView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Make sure that the last column stretches the rest of the controls width.
    this->m_messageView->header()->setStretchLastSection(true);

    auto* layout = new QVBoxLayout();
    layout->addWidget(this->m_messageView);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* layoutContent = new QWidget();
    layoutContent->setLayout(layout);
    this->setWidget(layoutContent);

    // Implement auto scroll behaviour, that respects the users manual
    // scrolling - it will only scroll to the bottom on log message arrival
    // if the current scroll position was already at the bottom.

    // React to a new item being about to be inserted, and check if the log view
    // is currently scrolled to the very bottom.
    // clang-format off
    connect(g_LogManager, &QAbstractTableModel::rowsAboutToBeInserted,
            this,
            [this]()
            {
                // Retrieve scroll bar
                auto* scrollbar = this->m_messageView->verticalScrollBar();

                // Check if its very near of the bottom, just to be sure.
                this->m_viewAtBottom = (scrollbar->value() >= scrollbar->maximum() - 1);
            }
    );
    // clang-format on

    // Also react to the signal emitted after a new item has been inserted,
    // and scroll the view to the new bottom position if we previously
    // determined that the view was already scrolled to the very bottom.
    // clang-format off
    connect(g_LogManager, &QAbstractTableModel::rowsInserted,
            this,
            [this]()
            {
                if(this->m_viewAtBottom)
                {
                    this->m_messageView->scrollToBottom();
                }
            },
            Qt::QueuedConnection // Make sure that the item has been fully displayed before scrolling.
    );
    // clang-format on
}
