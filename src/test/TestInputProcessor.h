#ifndef TESTINPUTPROCESSOR_H
#define TESTINPUTPROCESSOR_H

#include"../classes/InputProcessor.h"

#include<QtTest/QtTest>

class TestInputProcessor: public InputProcessor{

	Q_OBJECT

private slots:

	static void test_parseInputTime() noexcept;
	static void test_getHomePath() noexcept;
	static void test_readCSV() noexcept;
};

#endif // TESTINPUTPROCESSOR_H
