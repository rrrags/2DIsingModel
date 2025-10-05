#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include "latticeModel.hpp"

// #include "gameoflifemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<Simulator>("simulator",
                                          1,
                                          0,
                                          "simulator",
                                          "Cannot create Simulator in QML");

    LatticeModel latticeModel;

    engine.rootContext()->setContextProperty("latticeModel", &latticeModel);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
