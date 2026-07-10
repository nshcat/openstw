#pragma once

#include <QObject>
#include <QTimer>

class Application : QObject
{
    Q_OBJECT

public:
    Application();

public:
    QTimer* timer1Hz();
    QTimer* timer2Hz();

protected:
    void startTimers();
    void setupTimer(QTimer* timer, int periodMs);

protected:
    QTimer* m_timer1Hz;
    QTimer* m_timer2Hz;
};

extern Application* gApplication;

void createApplication();
