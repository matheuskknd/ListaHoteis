#include"TestInputProcessor.h"
#include"../classes/InputProcessor.h"

#include<QtTest/QtTest>

void TestInputProcessor::parseInputTime() noexcept{

	InputProcessor obj;
}


void TestInputProcessor::getHomePath() noexcept{

	InputProcessor obj;
	QCOMPARE(obj.getHomePath(),QDir::homePath());
}


void TestInputProcessor::readCSV() noexcept{

	InputProcessor obj;
}
