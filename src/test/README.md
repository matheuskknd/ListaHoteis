## Instructions

Aqui há uma explicação de como rodar os testes usando Linux. Primeiro é importante que você tenha toda a parte de build comum funcionando, explicado em ../../README.md
Tendo isso funcionando, abra o terminal e entre no diretório onde estes testes se encontram. Depois siga as instruções em ordem:

* Cheque se a macro do arquivo mainText.cpp vai executar o teste que você deseja
  * Basta alterar o valor da macro para corresponder à classe que deseja testar

 * Execute 'qmake -project "QT += testlib"';
  * Nota: este qmake é mesmo da sua instalação do QtCreator. Se outro binário for usado, a build pode falhar!

 * Modifique o arquivo '.pro' da seguinte forma:
  * Adicione uma linha "QT += X", para cada entrada equivalente no arquivo .pro do projeto alvo do teste
  * Adiciona uma entrada do tipo "$PROJECT_DIR/$CPP_DIR/*.cpp" na propriedade SOURCES para adicionar todos os seus .cpp (cuidado para não adicionar a main.cpp)
  * Adiciona uma entrada do tipo "$PROJECT_DIR/$H_DIR/*.h" na propriedade HEADERS para adicionar todos os seus .h

 * Execute 'qmake'
 * Execute 'make'
 * Rode o executável
