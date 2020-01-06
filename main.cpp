#include "mainwindow.h"

#include <QApplication>
#include <QWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    w.setWindowTitle("Amnesty 2019 - ZSK [v1.0]");
    w.setWindowIcon(QIcon("../prog_proj1/Amnesty-icon.ico"));

    w.show();


    return a.exec();
}
