#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  // Qt 5.6 이상
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);  // 이미지 DPI 지원
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
