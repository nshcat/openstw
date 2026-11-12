#include <QCloseEvent>

#include "subwindow.hxx"

SubWindow::SubWindow(bool withCloseButton, QWidget* parent)
    : QMdiSubWindow{parent, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint |
                                (withCloseButton ? Qt::WindowCloseButtonHint : (Qt::WindowFlags)0)}
{
    // Turn window close operations into hiding instead,
    // to always keep the window living.
    this->setAttribute(Qt::WA_DeleteOnClose, false);
}

void SubWindow::closeEvent(QCloseEvent* event)
{
    // Hide instead of actually fully closing the window
    this->hide();

    // Notify our clients about the changed visibility.
    emit this->visibilityChanged(false);

    // This keeps the MdiArea from hiding the sub windows
    // main widget.
    event->ignore();
}
