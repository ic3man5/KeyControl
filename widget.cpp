#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QSettings>

auto construct_name = [](auto* w) -> auto {
    auto* parent = w->parent();
    if (auto* temp = qobject_cast<QGroupBox*>(parent))
        return QString("%1 %2").arg(temp->title(), w->text());
    qDebug() << "NOPE!";
    return w->text();
};

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QSettings settings;
    ui->setupUi(this);
    mTrayIcon = new QSystemTrayIcon(QIcon(":/icons/default_icon"), this);
    connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    mTrayIcon->show();
    mKeyControlThread = new QThread(this);
    mKeyControl = new KeyControl(this);
    mKeyControl->moveToThread(mKeyControlThread);
    mKeyControlTimer = new QTimer();
    connect(this, SIGNAL(capsOptionChanged(KeyControl::Option)), mKeyControl, SLOT(setCapsOption(Option)), Qt::QueuedConnection);
    connect(mKeyControlThread, SIGNAL(finished()), mKeyControl, SLOT(deleteLater()));
    connect(mKeyControlTimer, SIGNAL(timeout()), mKeyControl, SLOT(process_keys()));
    mKeyControlThread->start();
    mKeyControlTimer->start(10);


    QList<QRadioButton*> caps_widgets =
        { ui->radioButtonCapsNone, ui->radioButtonCapsOn, ui->radioButtonCapsOff };
    for (auto* widget : caps_widgets) {
        connect(widget, SIGNAL(clicked()), this, SLOT(caps_lock_option_changed()));
        if (settings.value(construct_name(widget), true).toBool()) {
            widget->setChecked(true);
        }
    }
    QList<QRadioButton*> num_widgets =
        { ui->radioButtonNumNone, ui->radioButtonNumOn, ui->radioButtonNumOff };
    for (auto* widget : num_widgets) {
        connect(widget, SIGNAL(clicked()), this, SLOT(num_lock_option_changed()));
        if (settings.value(construct_name(widget), true).toBool()) {
            widget->setChecked(true);
        }
    }
    QList<QRadioButton*> scroll_widgets =
        { ui->radioButtonScrollNone, ui->radioButtonScrollOn, ui->radioButtonScrollOff };
    for (auto* widget : scroll_widgets) {
        connect(widget, SIGNAL(clicked()), this, SLOT(scoll_lock_option_changed()));
        if (settings.value(construct_name(widget), true).toBool()) {
            widget->setChecked(true);
        }
    }
    restoreGeometry(settings.value("WindowGeometry", geometry()).toByteArray());
    caps_lock_option_changed();
    num_lock_option_changed();
    scroll_lock_option_changed();
}

Widget::~Widget()
{
    QSettings settings;

    mKeyControlTimer->stop();
    mKeyControlThread->quit();
    mKeyControlThread->wait(5);
    settings.setValue("WindowGeometry", saveGeometry());

    // Copied from constructor...
    QList<QRadioButton*> caps_widgets =
        { ui->radioButtonCapsNone, ui->radioButtonCapsOn, ui->radioButtonCapsOff };
    for (auto* widget : caps_widgets) {
        connect(widget, SIGNAL(clicked()), this, SLOT(caps_lock_option_changed()));
        settings.setValue(construct_name(widget), widget->isChecked());
    }
    QList<QRadioButton*> num_widgets =
        { ui->radioButtonNumNone, ui->radioButtonNumOn, ui->radioButtonNumOff };
    for (auto* widget : num_widgets) {
        connect(widget, SIGNAL(clicked()), this, SLOT(num_lock_option_changed()));
        settings.setValue(construct_name(widget), widget->isChecked());
    }
    QList<QRadioButton*> scroll_widgets =
        { ui->radioButtonScrollNone, ui->radioButtonScrollOn, ui->radioButtonScrollOff };
    for (auto* widget : scroll_widgets) {
        connect(widget, SIGNAL(clicked()), this, SLOT(scoll_lock_option_changed()));
        settings.setValue(construct_name(widget), widget->isChecked());
    }

    delete ui;
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Context:
        qDebug() << "Context";
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "Double Click";
        break;
    case QSystemTrayIcon::Trigger:
        qDebug() << "Trigger";
        if (isVisible())
            this->hide();
        else
            this->show();
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "Middle Click";
        qApp->quit();
        break;
    default:
        qDebug() << "Unknown Reason...";
    }
}

void Widget::caps_lock_option_changed()
{
    if (ui->radioButtonCapsNone->isChecked()) {
        mKeyControl->setCapsOption(KeyControl::DONT_TOUCH);
    } else if (ui->radioButtonCapsOn->isChecked()) {
        mKeyControl->setCapsOption(KeyControl::ON);
    } else if (ui->radioButtonCapsOff->isChecked()) {
        mKeyControl->setCapsOption(KeyControl::OFF);
    }
}

void Widget::num_lock_option_changed()
{
    if (ui->radioButtonNumNone->isChecked()) {
        mKeyControl->setNumOption(KeyControl::DONT_TOUCH);
    } else if (ui->radioButtonNumOn->isChecked()) {
        mKeyControl->setNumOption(KeyControl::ON);
    } else if (ui->radioButtonNumOff->isChecked()) {
        mKeyControl->setNumOption(KeyControl::OFF);
    }
}

void Widget::scroll_lock_option_changed()
{
    if (ui->radioButtonScrollNone->isChecked()) {
        mKeyControl->setScrollOption(KeyControl::DONT_TOUCH);
    } else if (ui->radioButtonScrollOn->isChecked()) {
        mKeyControl->setScrollOption(KeyControl::ON);
    } else if (ui->radioButtonScrollOff->isChecked()) {
        mKeyControl->setScrollOption(KeyControl::OFF);
    }
}

void Widget::on_pushButtonHide_clicked()
{
    hide();
}
