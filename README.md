# ListaHoteis
UI QML com binds em C++ para carregar e fazer parse de dados

## Funcionamento do projeto

Para buildar o projeto, é necessário instalar o QtCreator 4.12.2 ou superior e abrir o arquivo '*.pro*' nesta página.
A configuração deve suportar a biblioteca do QML QtQuick 2.15 TLS, Qt 5.15.0 e um compilador de C++11 ou superiores.

Além disso, deve haver um link para o diretório '*storage/*' deste repositório no diretório <font color="green">$HOME</font> chamado "hotelListStorage/".

## Decisões de UI

A tela inicial tem o visual bastante simples, ela é separada em três componentes principais: "hourInput", "advancedButton", "csvInput"

* A área onde a hora a ser consultada pode ser inserida (com filtro e valor padrão para melhor IHC);
* Um botão de expanção da tela para opções avançadas;
* Um área para buscar arquivos CSV para a aplicação (apenas via FileDialog com filtros para melhor IHC).

A tela de exibição dos hoteis abertos também tem um design simplificado

* Um botão de voltar para a tela anterior;
* Uma label do valor consultado;
* Uma lista de rolagem com nome dos hoteis.

O design é intuitivo e utiliza janelas de diálogo padrão do SO, tanto para procurar arquivos, quanto para mostrar erros.

## Documentação básica

A principal parte da API é a classe InputProcessor, que interage com o QML como uma classe de controle, realizando a lógica e intermediando o acesso a dados.

A classe InputProcessor possui os seguintes métodos públicos:

* QStringList availableHours( const QString& uri, const QString& time) noexcept:
	* uri -> uma URI do tipo "file://" com o caminho absoluto para um arquivo CSV válido
	* time -> um valor de hora e minuto no formato "HH:MM"

	* Retorna -> uma QStringList contendo os nomes dos hoteis abertos naquele horário, segundo o arquivo CSV;
		* Pode ser chamado via QML

	* Exceções -> lança um erro do tipo QString contendo uma mensagem explicativa ao objeto InputProcessor::engine setado anteriormente
		* Se houver qualquer erro de parse da hora passada por parâmetro;
		* Se houver qualquer erro de parse do CSV.
		* Não lança exeções na stack do C++

* QString getHomePath() noexcept:
	* Retorna -> QDir::homePath()

* static void setEngine( QJSEngine& _engine) noexcept:
	* _engine _> um objeto do tipo QJSEngine (assim como QQmlApplicationEngine), o qual pode executar um arquivo .qml onde métodos dessa classe podem ser chamados
	* NOTA: se qualquer outro método não estático desta classe for chamado sem que antes sua QJSEngine tenha sido definida com este método, o comportamento é indefinido.
