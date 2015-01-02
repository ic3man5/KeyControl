#ifndef KEYCONTROL_H
#define KEYCONTROL_H

#include <QObject>

class KeyControl : public QObject
{
    Q_OBJECT
public:
    explicit KeyControl(QObject *parent = 0);
    ~KeyControl();

    enum Option {
        DONT_TOUCH = 0,
        OFF,
        ON
    };

signals:

public slots:
    void process_keys();
    void quit();
    void setCapsOption(Option option);
    void setNumOption(Option option);
    void setScrollOption(Option option);

private:
    bool quit_;
    Option caps_option;
    Option num_option;
    Option scroll_option;
};

#endif // KEYCONTROL_H
