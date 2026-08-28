#include "SubscriptionTrackerGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SubscriptionTrackerGUI window;
    window.show();
    return app.exec();
}
