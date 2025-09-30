#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include "gridModel.h"

// #include "gameoflifemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register GridModel type as uncreatable in QML
    qmlRegisterUncreatableType<Grid>("GOL", 1, 0, "GOLGrid", "Cannot create Grid in QML");
    //qDebug() << QMetaEnum::fromType<Grid::State>().valueToKey(Grid::ALIVE);
    //qDebug() << QMetaEnum::fromType<Grid::State>().valueToKey(Grid::DEAD);

    GridModel gridModel;

    // Expose instance as context property
    engine.rootContext()->setContextProperty("gridModel", &gridModel);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
