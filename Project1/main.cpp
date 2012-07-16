#include <QtGui/QApplication>
#include "projectwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProjectWidget w;
    w.show();
    
    return a.exec();
}
