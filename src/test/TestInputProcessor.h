#ifndef TESTINPUTPROCESSOR_H
#define TESTINPUTPROCESSOR_H

#include<QtTest/QtTest>

class TestInputProcessor: public QObject{

	Q_OBJECT

private slots:

	static void parseInputTime() noexcept;
	static void getHomePath() noexcept;
	static void readCSV() noexcept;
};

#endif // TESTINPUTPROCESSOR_H
