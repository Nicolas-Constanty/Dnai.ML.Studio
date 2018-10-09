#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "editor.h"

void registerQmlTypes()
{
	//REGISTER EDITOR TO SINGLETON FOR QML
	qmlRegisterSingletonType<dnai::mlstudio::Editor>
		(
			"App.Controllers",
			1, 0, "Editor",
			[](QQmlEngine *e, QJSEngine *js) -> QObject * {
		Q_UNUSED(e)
			Q_UNUSED(js)

			auto editor = new dnai::mlstudio::Editor();
		return editor;
	}
	);
	qmlRegisterType<dnai::mlstudio::DatasetHandler>("App.Controllers", 1, 0, "DatasetHandler");
	qmlRegisterType<dnai::mlstudio::Dataset>("App.Controllers", 1, 0, "Dataset");
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	registerQmlTypes();
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
