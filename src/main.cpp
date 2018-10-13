#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "editor.h"

void registerQmlTypes()
{
    //REGISTER EDITOR TO SINGLETON FOR QML
    qmlRegisterSingletonType<Editor>
            (
                "App.Controllers",
                1, 0, "Editor",
                [](QQmlEngine *e, QJSEngine *js) -> QObject * {
        Q_UNUSED(e)
        Q_UNUSED(js)

        auto editor = new Editor();
        return editor;
    }
    );

}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    registerQmlTypes();
    qmlRegisterType<DatasetHandler>("App.Controllers", 1, 0, "DatasetHandler");
    qmlRegisterType<Dataset>("App.Controllers", 1, 0, "Dataset");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/resources/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
