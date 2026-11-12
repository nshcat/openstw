#pragma once

#include <QMdiSubWindow>
#include <QObject>

// Common base class for all sub windows within the application.
// For example, takes care of sending out proper visibility change
// events.
class SubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    SubWindow(bool withCloseButton, QWidget* parent = nullptr);

signals:
    void visibilityChanged(bool visible);

protected:
    // Intercept close events triggered by the user pressing X,
    // and turn those into visibility changes instead.
    virtual void closeEvent(QCloseEvent* event) override;
};
