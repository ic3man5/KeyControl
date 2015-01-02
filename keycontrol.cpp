#include "keycontrol.h"
#include <windows.h>
#include <tuple>
#include <QDebug>
#include <utility>
#include <QThread>

KeyControl::KeyControl(QObject *parent) : QObject(parent)
{
    caps_option = Option::DONT_TOUCH;
    num_option = Option::DONT_TOUCH;
    scroll_option = Option::DONT_TOUCH;
}

KeyControl::~KeyControl()
{

}
void KeyControl::quit()
{
    this->quit_ = true;
}

void KeyControl::process_keys()
{
    /*
    caps_option = Option::OFF;
    num_option = Option::DONT_TOUCH;
    scroll_option = Option::DONT_TOUCH;
    quit_ = false;
    */
    auto is_key_enabled =
        [](auto& state) -> auto { return ((state & 0x80) != 0x80 && (state & 0x01) == 0x01); };
    auto is_key_disabled =
        [](auto& state) -> auto { return ((state & 0x80) != 0x80 && (state & 0x01) != 0x01); };
    auto toggle_key =
        [](auto& key) {
            keybd_event(key, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
            keybd_event(key, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        };
    auto keys = std::vector<std::pair<int, KeyControl::Option> > {
        std::make_pair(VK_CAPITAL, caps_option),
        std::make_pair(VK_NUMLOCK, num_option),
        std::make_pair(VK_SCROLL, scroll_option)
    };
    for (auto& key : keys) {
        auto keystate = GetKeyState(key.first);
        switch (key.second) {
        case Option::DONT_TOUCH:
            break;
        case Option::OFF:
            if (is_key_enabled(keystate)) {
                toggle_key(key.first);
            }
            break;
        case Option::ON:
            if (is_key_disabled(keystate)) {
                toggle_key(key.first);
            }
            break;
        default:
            qDebug() << "Error: Unknown Key Option...";
        }
    }
}

void KeyControl::setCapsOption(Option option)
{
    caps_option = option;
}

void KeyControl::setNumOption(Option option)
{
    num_option = option;
}
void KeyControl::setScrollOption(Option option)
{
    scroll_option = option;
}


/*
 * bool is_caps_enabled(short& state) {
    return ((state & 0x80) != 0x80 && (state & 0x01) == 0x01);
}

int main()
{
    while (true) {
        short keystate = 0;
        Sleep(10);
        keystate = GetKeyState(VK_CAPITAL);
        if (is_caps_enabled(keystate)) {
            cout << "Turning off Capslock" << endl;
            keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
            keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }
    }
    return 0;
}
*/
