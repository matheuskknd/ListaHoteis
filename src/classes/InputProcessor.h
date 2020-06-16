#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include<QStringList>
#include<QJSEngine>
#include<QObject>
#include<QDir>

#include<string>
#include<tuple>
#include<list>

class InputProcessor : public QObject{

	static QJSEngine* engine;
	Q_OBJECT

private:

	// Read the SCV file and return a list of hotel names
	static std::list<std::tuple<QString,int,int>> readCSV( const QString& fileName, int searchT = 0) noexcept(0);

	// Parses the hour and minutes passed as parameters
	static int parseInputTime( const std::string& time) noexcept(0);

public:

	explicit InputProcessor( QObject* parent = nullptr) : QObject(parent) {}

	Q_INVOKABLE QStringList availableHours( const QString& uri, const QString& time) noexcept;

	Q_INVOKABLE QString getHomePath() noexcept{ return QDir::homePath();}

	static void setEngine( QJSEngine& _engine) noexcept{ engine = &_engine; }
};

#endif // INPUTPROCESSOR_H