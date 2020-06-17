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
using ostring_s = std::ostringstream;
using istring_s = std::istringstream;

#include<string>
using std::to_string;
using std::string;
using std::stoi;

#include<tuple>
using std::tuple;

#include<list>
using TupleList = std::list<tuple<QString,int,int>>;


QJSEngine* InputProcessor::engine = nullptr;


TupleList InputProcessor::readCSV( const QString& fileName, const int searchT) noexcept(0){

	ifstream file(fileName.toStdString().substr(7),ios_base::in | ios_base::binary);
	TupleList hotelList;

	if( !file.is_open() )
		throw QString("O arquivo CSV não pode ser aberto");

	size_t line = 1;
	float col = 0;

	try{

		// Auxiliar variables
		string str;

		getline(file,str);	// Ignores the file's first line
		line = 2;

		if( str.empty() && file.eof() )
			throw QString("O arquivo CSV está vazio");

		// Temporary variables
		QString qstr;
		int t1, t2;

		while( !file.eof() ){

			qstr.clear();
			t1 = t2 = -1;

			// Get the first column
			col = 1;
			getline(file,str,';');

			if( str.empty() ){

				if( file.eof() )
					break;
				else
					throw logic_error("Nenhum nome de hotel encontrado");
			}

			if( str.find('\n') != string::npos || file.eof() )
				throw logic_error("Nenhum separador ';' encontrado");

			qstr = str.data();

			// Get the second column starting hour
			col = 2.1;
			getline(file,str,'-');

			if( str.empty() || str[0] == '\n' )
				throw logic_error("Hora de abertura não encontrada");

			if( str.find('\n') != string::npos || file.eof() )
				throw logic_error("Nenhum separador '-' encontrado");

			t1 = parseInputTime(str);

			// Get the second column closing hour
			col = 2.2;
			getline(file,str);

			if( str.empty() )
				throw logic_error("Hora de fechamento não encontrada");

			t2 = parseInputTime(str);

			// Logical selection part
			if( t1 > t2 ){

				if( searchT >= t1 || searchT <= t2 )
					hotelList.emplace_back(std::move(qstr),t1,t2);

			}else{

				if( searchT >= t1 && searchT <= t2 )
					hotelList.emplace_back(std::move(qstr),t1,t2);
			}

			++line;
		}

	}catch( const ios_base::failure& error){

		ostring_s error_msg;

		error_msg << "Error lendo arquivo CSV na linha: " << line << " coluna: " << col
			<< "\nMensagem: " << error.what() << "\nCódigo: " << error.code() << '\n';

		throw QString(error_msg.str().data());

	}catch( const logic_error& error){

		ostring_s error_msg;

		error_msg << "Error de parse no arquivo CSV na linha: " << line << " coluna: " << col
			<< "\nMensagem: " << error.what() << '\n';

		throw QString(error_msg.str().data());
	}

	if( hotelList.empty() ){

		auto h = to_string(searchT/60);
		auto m = to_string(searchT%60);
		ostring_s error_msg;

		error_msg << "Nenhum hotel aberto às " << ( h.size() == 1 ? "0" : "" ) << h
			<< ':' << ( m.size() == 1 ? "0" : "" ) << m << '\n';

		throw QString(error_msg.str().data());
	}

	return hotelList;
}


int InputProcessor::parseInputTime( const string& time) noexcept(0){

	istring_s strs(time);
	string aux;

	int value;
	int tmp;

	getline(strs,aux,':');

	if( aux.empty() )
		throw logic_error("Valor da hora não encontrado");

	if( aux.size() == time.size() )
		throw logic_error("Separador ':' não encontrado");

	for( decltype(aux.size()) i = 0; i != aux.size(); ++i)
		if( aux[i] < '0' || aux[i] > '9')
			throw logic_error("A hora contém um valor não numérico");

	// Parse the input hour
	tmp = stoi(aux,nullptr,10);

	if( tmp < 0 || tmp > 23 )
		throw logic_error("A hora inserida está fora de intervalo");

	value = 60 * tmp;
	getline(strs,aux);

	if( aux.empty() )
		throw logic_error("Valor dos minutos não encontrado");

	for( decltype(aux.size()) i = 0; i != aux.size(); ++i)
		if( aux[i] < '0' || aux[i] > '9')
			throw logic_error("Os minutos contém um valor não numérico");

	// Parse the input minutes
	tmp = stoi(aux,nullptr,10);

	if( tmp < 0 || tmp > 59 )
		throw logic_error("Os minutos inseridos estão fora de intervalo");

	return value + tmp;
}


QStringList InputProcessor::availableHours( const QString& uri, const QString& time) noexcept try{

	Q_ASSERT(!uri.isEmpty() && !time.isEmpty());
	Q_ASSERT(time.length() == 5);
	Q_ASSERT(engine != nullptr);

	int searchT;

	try{

		searchT = parseInputTime(time.toStdString());

	}catch( const logic_error& error){

		ostring_s error_msg;
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
