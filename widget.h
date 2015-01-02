#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QThread>
#include <QTimer>
#include "keycontrol.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QSystemTrayIcon* mTrayIcon;
    KeyControl* mKeyControl;
    QThread* mKeyControlThread;
    QTimer* mKeyControlTimer;

signals:

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private slots:
    void caps_lock_option_changed();
    void num_lock_option_changed();
    void scroll_lock_option_changed();
    void on_pushButtonHide_clicked();
};

#endif // WIDGET_H
