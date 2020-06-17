#include"TestInputProcessor.h"

#include<QtTest/QtTest>

#include<stdexcept>
using std::logic_error;

#include<sstream>
using ostring_s = std::ostringstream;

#include<string>
using std::to_string;
using std::string;

void TestInputProcessor::test_parseInputTime() noexcept try{

	// Test the interpretation without zeros at left
	for( int h = 0; h != 24; ++h){

		for( int m = 0; m != 60; ++m){

			ostring_s param;
			param << h << ':' << m;

			QCOMPARE(parseInputTime(param.str()),60*h + m);
		}
	}

	// Test the interpretation with zeros at left
	for( int h = 0; h != 24; ++h){

		for( int m = 0; m != 60; ++m){

			ostring_s param;
			param << (to_string(h).size() == 1 ? "0" : "") << h << ':' << (to_string(m).size() == 1 ? "0" : "") << m;

			QCOMPARE(parseInputTime(param.str()),60*h + m);
		}
	}

	// Out of range [0,24[ hour values
	for( int h = -1; h != -5; --h){

		ostring_s param;
		param << h << ":00";

		QVERIFY_EXCEPTION_THROWN(parseInputTime(param.str()),logic_error);
	}

	QVERIFY_EXCEPTION_THROWN(parseInputTime("-1000:00"),logic_error);

	for( int h = 24; h != 29; ++h){

		ostring_s param;
		param << h << ":00";

		QVERIFY_EXCEPTION_THROWN(parseInputTime(param.str()),logic_error);
	}

	QVERIFY_EXCEPTION_THROWN(parseInputTime("1000:00"),logic_error);

	// Out of range [0,60[ minute values
	for( int m = -1; m != -5; --m){

		ostring_s param;
		param << "00:" << m;

		QVERIFY_EXCEPTION_THROWN(parseInputTime(param.str()),logic_error);
	}

	QVERIFY_EXCEPTION_THROWN(parseInputTime("00:-1000"),logic_error);

	for( int m = 60; m != 65; ++m){

		ostring_s param;
		param << "00:" << m;

		QVERIFY_EXCEPTION_THROWN(parseInputTime(param.str()),logic_error);
	}

	QVERIFY_EXCEPTION_THROWN(parseInputTime("00:1000"),logic_error);

	// Test the bad parse detection
	QVERIFY_EXCEPTION_THROWN(parseInputTime("a0:00"),logic_error);
	QVERIFY_EXCEPTION_THROWN(parseInputTime("00:a0"),logic_error);

	QVERIFY_EXCEPTION_THROWN(parseInputTime("0a:00"),logic_error);
	QVERIFY_EXCEPTION_THROWN(parseInputTime("00:0a"),logic_error);

	QVERIFY_EXCEPTION_THROWN(parseInputTime("00:"),logic_error);
	QVERIFY_EXCEPTION_THROWN(parseInputTime(":00"),logic_error);

	QVERIFY_EXCEPTION_THROWN(parseInputTime("0000"),logic_error);
	QVERIFY_EXCEPTION_THROWN(parseInputTime(":"),logic_error);
	QVERIFY_EXCEPTION_THROWN(parseInputTime(""),logic_error);

}catch( const logic_error& error){

	ostring_s strs;
	strs << "Unexpected logic_error thrown...\nMessage: " << error.what() << '\n';

	QFAIL(strs.str().data());

}catch(...){

	QFAIL("Unknow exception thrown");
}


void TestInputProcessor::test_getHomePath() noexcept try{

	InputProcessor obj;
	QCOMPARE(obj.getHomePath(),QDir::homePath());

}catch(...){

	QFAIL("Unknow exception thrown");
}


void TestInputProcessor::test_readCSV() noexcept try{

	static const string basePath = string("file://") + QDir::homePath().toStdString() + "/hotelListStorage/";
	auto getPath = []( const char * const cstr) noexcept(0) -> QString{ return QString((basePath + cstr).data()); };

// ################################
// ### Begin of parse test zone ###
// ################################

	// Test nonexistent file detection
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("nonexistent.csv")),QString);

	try{

		readCSV(getPath("nonexistent.csv"));

	}catch( const QString& error){

		QCOMPARE(error,"O arquivo CSV não pode ser aberto");
	}

	// Test empty file detection
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("empty_01.csv")),QString);

	try{

		readCSV(getPath("empty_01.csv"));

	}catch( const QString& error){

		QCOMPARE(error,"O arquivo CSV está vazio");
	}

	// Test file with no hotels
	for( int h = 0; h != 24; ++h){

		for( int m = 0; m != 60; ++m){

			QVERIFY_EXCEPTION_THROWN(readCSV(getPath("empty_02.csv"),60*h + m),QString);

			try{

				readCSV(getPath("empty_02.csv"),60*h + m);

			}catch( const QString& error){

				ostring_s strs;

				strs << "Nenhum hotel aberto às " << ( to_string(h).size() == 1 ? "0" : "" ) << h
					<< ':' << ( to_string(m).size() == 1 ? "0" : "" ) << m << '\n';

				QCOMPARE(error,strs.str().data());
			}
		}
	}

	// Test a file missing a hotel name
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_01.csv")),QString);

	try{

		readCSV(getPath("missing_01.csv"));

	}catch( const QString& error){

		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 1")));
		QVERIFY(error.contains(QString("Nenhum nome de hotel encontrado")));
	}

	// Test file without column separator
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_02.csv")),QString);

	try{

		readCSV(getPath("missing_02.csv"));

	}catch( const QString& error){

		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 1")));
		QVERIFY(error.contains(QString("Nenhum separador ';' encontrado")));
	}

	// Test file without opening hour
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_03.csv")),QString);

	try{

		readCSV(getPath("missing_03.csv"));

	}catch( const QString& error){

		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.1")));
		QVERIFY(error.contains(QString("Valor da hora não encontrado")));
	}

	// Test file without first time separator
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_04.csv")),QString);

	try{

		readCSV(getPath("missing_04.csv"));

	}catch( const QString& error){

		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.1")));
		QVERIFY(error.contains(QString("Separador ':' não encontrado")));
	}

	// Test file without opening minute
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_05.csv")),QString);

	try{

		readCSV(getPath("missing_05.csv"));

	}catch( const QString& error){


		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.1")));
		QVERIFY(error.contains(QString("Valor dos minutos não encontrado")));
	}

	// Test file without opening time
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_06.csv")),QString);

	try{

		readCSV(getPath("missing_06.csv"));

	}catch( const QString& error){


		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.1")));
		QVERIFY(error.contains(QString("Hora de abertura não encontrada")));
	}

	// Test file without opening and closing time separator
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_07.csv")),QString);

	try{

		readCSV(getPath("missing_07.csv"));

	}catch( const QString& error){

		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.1")));
		QVERIFY(error.contains(QString("Nenhum separador '-' encontrado")));
	}

	// Test file without closing hour
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_08.csv")),QString);

	try{

		readCSV(getPath("missing_08.csv"));

	}catch( const QString& error){

		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.2")));
		QVERIFY(error.contains(QString("Valor da hora não encontrado")));
	}

	// Test file without closing time separator
	QVERIFY_EXCEPTION_THROWN(readCSV(getPath("missing_09.csv")),QString);

	try{

		readCSV(getPath("missing_09.csv"));

	}catch( const QString& error){

		QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.2")));
		QVERIFY(error.contains(QString("Separador ':' não encontrado")));
	}

	// Test file without closing minute
	for( int i = 10; i != 12; ++i){

		const string name = string("missing_") + to_string(i) + string(".csv");
		QVERIFY_EXCEPTION_THROWN(readCSV(getPath(name.data())),QString);

		try{

			readCSV(getPath(name.data()));

		}catch( const QString& error){

			QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.2")));
			QVERIFY(error.contains(QString("Valor dos minutos não encontrado")));
		}
	}

	// Test file without closing time
	for( int i = 12; i != 14; ++i){

		const string name = string("missing_") + to_string(i) + string(".csv");
		QVERIFY_EXCEPTION_THROWN(readCSV(getPath(name.data())),QString);

		try{

			readCSV(getPath(name.data()));

		}catch( const QString& error){

			QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.2")));
			QVERIFY(error.contains(QString("Hora de fechamento não encontrada")));
		}
	}

	// Test file without time column
	for( int i = 14; i != 16; ++i){

		const string name = string("missing_") + to_string(i) + string(".csv");
		QVERIFY_EXCEPTION_THROWN(readCSV(getPath(name.data())),QString);

		try{

			readCSV(getPath(name.data()));

		}catch( const QString& error){

			QVERIFY(error.startsWith(QString("Error de parse no arquivo CSV na linha: 2 coluna: 2.1")));
			QVERIFY(error.contains(QString("Hora de abertura não encontrada")));
		}
	}

// ################################
// ### Begin of range test zone ###
// ################################

	// Test general T1 < T2 detection
	for( int h = 0; h != 24; ++h){

		auto hotelList = readCSV(getPath("valid_01.csv"),60*h + 15);
		QCOMPARE(hotelList.size(),size_t(24-h));
	}

	// Test general T2 < T1 detection
	for( int h = 0; h != 24; ++h){

		auto hotelList = readCSV(getPath("valid_02.csv"),60*h + 45);
		QCOMPARE(hotelList.size(),size_t(h+1));
	}

	// Test range for 24 -> 23 hotels open on T1 < T2 (upper bound)
	for( int m = -1; m != 2; ++m){

		auto hotelList = readCSV(getPath("valid_01.csv"),30 + m);

		if( m != 1 )
			QCOMPARE(hotelList.size(),size_t(24));
		else
			QCOMPARE(hotelList.size(),size_t(23));
	}

	// Test range for 0 -> 24 hotels open on T1 < T2 (lower bound)
	for( int m = -1; m != 2; ++m){

		try{

			auto hotelList = readCSV(getPath("valid_01.csv"),(1440+m)%1440);
			QCOMPARE(hotelList.size(),size_t(24));

		}catch( const QString& error){

			if( m == -1 )
				QCOMPARE(error,"Nenhum hotel aberto às 23:59\n");
			else
				throw;
		}
	}

	// Test range for 1 -> 2 hotels open on T2 < T1 (upper bound)
	for( int m = -1; m != 2; ++m){

		auto hotelList = readCSV(getPath("valid_02.csv"),90 + m);

		if( m == -1 )
			QCOMPARE(hotelList.size(),size_t(1));
		else
			QCOMPARE(hotelList.size(),size_t(2));
	}

	// Test range for 24 -> 0 hotels open on T2 < T1 (lower bound)
	for( int m = -1; m != 2; ++m){

		try{

			auto hotelList = readCSV(getPath("valid_02.csv"),(1440+m)%1440);
			QCOMPARE(hotelList.size(),size_t(24));

		}catch( const QString& error){

			if( m == 1 )
				QCOMPARE(error,"Nenhum hotel aberto às 00:01\n");
			else
				throw;
		}
	}

}catch( const QString& error){

	ostring_s strs;
	strs << "Unexpected error thrown...\nMessage: " << error.toStdString() << '\n';

	QFAIL(strs.str().data());

}catch(...){

	QFAIL("Unknow exception thrown");
}
