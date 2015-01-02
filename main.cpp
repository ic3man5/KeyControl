#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Fusion");
    QCoreApplication::setApplicationName("KeyControl");

    Widget w;
    w.show();
    w.hide();
    return a.exec();
}
