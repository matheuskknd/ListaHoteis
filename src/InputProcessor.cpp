#include"InputProcessor.h"

#include<QtGlobal>
#include<QString>

#include<stdexcept>
using std::logic_error;

#include<fstream>
using std::ifstream;
using std::ios_base;
using std::getline;

#include<sstream>
using string_s = std::stringstream;

#include<string>
using std::to_string;
using std::string;
using std::stoi;

#include<tuple>
using std::tuple;

#include<list>
using TupleList = std::list<tuple<QString,int,int>>;

#ifndef NDEBUG

	#include<iostream>
	using std::cout;

#endif

QJSEngine* InputProcessor::engine = nullptr;

TupleList InputProcessor::readCSV( const QString& fileName, const int searchT) noexcept(0){

	ifstream file(fileName.toStdString().substr(7),ios_base::in | ios_base::binary);
	TupleList hotelList;

	if( !file.is_open() )
		throw QString("O arquivo CSV não pode ser aberto");

	size_t line = 1;
	uint8_t col = 0;

	try{

		// Auxiliar variables
		string str;
		char ch;

		getline(file,str);	// Ignores the file's first line
		line = 2;

		if( str.empty() && file.eof() )
			throw QString("O arquivo CSV está vazio");

		// Temporary variables
		QString qstr;
		int t1, t2;
		int tmp;

		while( !file.eof() ){

			qstr.clear();
			t1 = t2 = -1;

			// Get the first column
			col = 1;
			str.clear();
			getline(file,str,';');
			qstr = str.data();

			if( str.empty() ){

				if( file.eof() )
					break;
				else
					throw logic_error("Nenhum nome de hotel encontrado");
			}

			if( str.find('\n') != string::npos || file.eof() )
				throw logic_error("Nenhum separador ';' encontrado");

			// Get the second column starting hour
			col = 2;
			file >> tmp;

			if( !file.good() )
				throw logic_error("Erro de parse na primeira hora");

			if( tmp < 0 || tmp > 23 )
				throw logic_error("Valor da primeira hora fora do intervalo");

			t1 = 60 * tmp;
			file >> ch;

			if( !file.good() )
				throw logic_error("Erro de parse no primeiro separador de hora");

			if( ch != ':' )
				throw logic_error("Primeiro separador de hora ':' não encontrado");

			// Get the second column starting minute
			file >> tmp;

			if( !file.good() )
				throw logic_error("Erro de parse no primeiro minuto");

			if( tmp < 0 || tmp > 59 )
				throw logic_error("Valor do primeiro minuto fora do intervalo");

			t1 += tmp;
			file >> ch;

			if( !file.good() )
				throw logic_error("Erro de parse no separador de primeira e segunda hora");

			if( ch != '-' )
				throw logic_error("Separador de primeira e segunda hora '-' não encontrado");

			// Get the second column closing hour
			file >> tmp;

			if( !file.good() )
				throw logic_error("Erro de parse na segunda hora");

			if( tmp < 0 || tmp > 23 )
				throw logic_error("Valor da segunda hora fora do intervalo");

			t2 = 60 * tmp;
			file >> ch;

			if( !file.good() )
				throw logic_error("Erro de parse no segundo separador de hora");

			if( ch != ':' )
				throw logic_error("Segundo separador de hora ':' não encontrado");

			// Get the second column closing minute
			file >> tmp;

			if( file.fail() )
				throw logic_error("Erro de parse no segundo minuto");

			if( tmp < 0 || tmp > 59 )
				throw logic_error("Valor do segundo minuto fora do intervalo");

			t2 += tmp;

			str.clear();
			getline(file,str);	// Ignores the rest of this line

			if( !str.empty() )
				throw logic_error("A formatação do CSV não condiz com ('.+','[0-9]{1,2}:[0-9]{1,2}-[0-9]{1,2}:[0-9]{1,2}')");

			// Logical selection part
			if( t1 > t2 ){

				if( searchT >= t1 || searchT <= t2 )
					hotelList.emplace_back(std::move(qstr),t1,t2);

			}else{

				if( searchT >= t1 && searchT <= t2 )
					hotelList.emplace_back(std::move(qstr),t1,t2);
			}

#ifndef NDEBUG

cout << "Hotel: " << qstr.toStdString() << "\n";
cout << "Hora 1: " << t1/60 << "\n";
cout << "min 1: " << t1%60 << "\n";
cout << "Hora 2: " << t2/60 << "\n";
cout << "min 2: " << t2%60 << "\n";
cout << "Olhando: (" << str << ")\n";
cout << "Line: " << line << "\n\n";

#endif

			++line;
		}

	}catch( const ios_base::failure& error){

		string_s error_msg;

		error_msg << "Error lendo arquivo CSV na linha: " << line << " coluna: " << int(col)
			<< "\nMensagem: " << error.what() << "\nCódigo: " << error.code() << '\n';

		throw QString(error_msg.str().data());

	}catch( const logic_error& error){

		string_s error_msg;

		error_msg << "Error de parse no arquivo CSV na linha: " << line << " coluna: " << int(col)
			<< "\nMensagem: " << error.what() << '\n';

		throw QString(error_msg.str().data());
	}

	if( hotelList.empty() ){

		auto h = to_string(searchT/60);
		auto m = to_string(searchT%60);
		string_s error_msg;

		error_msg << "Nenhum hotel aberto às " << ( h.size() == 1 ? "0" : "" ) << h
			<< ':' << ( m.size() == 1 ? "0" : "" ) << m << '\n';

		throw QString(error_msg.str().data());
	}

	return hotelList;
}

QStringList InputProcessor::availableHours( const QString& uri, const QString& time) noexcept(0) try{

	Q_ASSERT(!uri.isEmpty() && !time.isEmpty());
	Q_ASSERT(time.length() == 5);
	Q_ASSERT(engine != nullptr);

	int searchT;

	try{

		const string& aux = time.toStdString();
		int tmp;

		// Parse the input hour
		tmp = stoi(aux.substr(0,2),nullptr,10);

		if( tmp < 0 || tmp > 23 )
			throw logic_error("A hora inserida está fora de intervalo");

		searchT = 60 * tmp;

		// Parse the input minute
		tmp = stoi(aux.substr(3,2),nullptr,10);

		if( tmp < 0 || tmp > 59 )
			throw logic_error("Os minutos inseridos estão fora de intervalo");

		searchT += tmp;

	}catch( const logic_error& error){

		string_s error_msg;
		error_msg << "Erro de parse na hora inserida..\nMensagem: " << error.what() << '\n';
		throw QString(error_msg.str().data());
	}

	TupleList hotels = readCSV(uri,searchT);
	QStringList qlist;

	// Unfortunately the time complexity can't be better than n*log2(n)
	for( const auto& tup : hotels )
		qlist.append(std::get<0>(tup));

	return qlist;

}catch( const QString& msg){

	engine->throwError(msg);
	return QStringList();

}catch(...){

	engine->throwError(QString("Unknown error occurred while processing input"));
	return QStringList();
}