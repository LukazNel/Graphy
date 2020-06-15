#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTextStream>

#include "../include/adjacencymatrix.h"

int main(int argc, char *argv[])
{
    AdjacencyMatrix matrix({
                               { 0, 1, 7, 0, 0, 0 },
                               { 0, 0, 0, 9, 0, 0 },
                               { 0, 0, 0, 0, 4, 0 },
                               { 0, 0, 0, 0, 10, 5 },
                               { 0, 0, 0, 0, 0, 3 },
                               { 0, 0, 0, 0, 3, 0 }
                           }, true);
    /*QTextStream out(stdout);
    out << matrix;

    matrix.dijkstra(0, 5);*/

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
    //return 0;
}
