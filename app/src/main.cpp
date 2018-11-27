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

        const auto editor = new Editor();
        return editor;
    }
    );

}

void openDatabase()
{

}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    openDatabase();
    registerQmlTypes();
    qmlRegisterType<DatasetHandler>("App.Controllers", 1, 0, "DatasetHandler");
    qmlRegisterType<DatabaseHandler>("App.Controllers", 1, 0, "DatabaseHandler");
//    qmlRegisterType<Dataset>("App.Controllers", 1, 0, "Dataset");
    qmlRegisterType<ApiHandler>("App.Controllers", 1, 0, "ApiHandler");
    qmlRegisterType<TableModel>("App.Controllers", 1, 0, "TableModel");
    qmlRegisterType<Project>("App.Controllers", 1, 0, "Project");
    qmlRegisterType<TaskManager>("App.Controllers", 1, 0, "TaskManager");
    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("SaltyStudio");
    QGuiApplication::setOrganizationDomain("dnai.io");
    QGuiApplication::setApplicationName("ML Studio");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/resources/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return QGuiApplication::exec();
}
