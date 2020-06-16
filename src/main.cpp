#include<QQmlApplicationEngine>
#include<QGuiApplication>

#include"InputProcessor.h"

int main( int argc, char** argv){

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc,argv);

	// Engine initialization
	QQmlApplicationEngine engine;
	InputProcessor::setEngine(engine);

	// Register an object of this type into QML
	qmlRegisterType<InputProcessor>("My.Lib",1,0,"InputProcessor");

	// Register this information to avoid errors
	app.setOrganizationDomain("orgDomain");
	app.setOrganizationName("orgName");

	const QUrl url(QStringLiteral("qrc:/QML/main.qml"));

	QObject::connect(&engine,&QQmlApplicationEngine::objectCreated,&app, [url]( QObject* obj, const QUrl& objUrl) noexcept(0) -> void{

		if( !obj && url == objUrl )
			QCoreApplication::exit(-1);

	},Qt::QueuedConnection);

	engine.load(url);
	return app.exec();
}
