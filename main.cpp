#include <QApplication>
#include "dir.h"

// ----------------------------------------------------------------------
int main (int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Поиск каталогов");
    FileFinder fileFinder;

    fileFinder.resize(480, 480);
    fileFinder.show();

    return app.exec();
}

