<h1> Sistema de Gerenciamento de dados de dragões de Dragon City </h1>
<h2>Visão Geral</h2>
<ul>
  <li>Mostrar os dragões para o usuário.<br/>
       <p>   ► Mostrar os dragões em um intervalo [a,b] - sendo
            sendo a e b IDs digitados pelo usuário
       </p>
  </li>
  <li> Inserção de dragões (com validação rigorosa de tipos) </li>
  <li> Busca por ID, Nome ou Tipo (case-sensitive) </li>   
  <li> Remoção lógica de registros </li>  
  <li> Ordenação por diferentes critérios - ID, Nome ou Tipo </li>
  <li> Gravação em arquivo CSV </li> 
  <li> Interface TUI interativa com Notcurses </li>
</ul>
<h2>Estrutura dos Dados</h2>
<h3>A estrutura Dragon armazena as seguintes informações: </h3>

```cpp
struct Dragon {
	int id{};                     // Identificador único do Dragão
    string nome{};                // Nome do Dragão 
	string tipo{};                // Tipo(s) do Dragão (máximo de dois tipos separados por /) 
	int nivel{};                  // Nível do Dragão
	int vida{};                   // Vida Máxima do Dragão no Nível atual (HP)
	int ataque{};                 // Ataque máximo do Dragão no nível atual
	float chance_critico{};       // Um número decimal no intervalo [0.0, 1.0] representando a chance do ataque ser crítico  
	string habilidade_critico{};  // Nome da habilidade a qual o ataque crítico é ativada (podendo ter espaços)
};
```

<h2> Funções Implementadas </h2>
<h4> ► Funções de Utilidade e Entrada/Saída</h4>
	  
| Função | Descrição | Parâmetros |
| :--- | :--- | :--- |
| `void limparEntrada()` | Limpa o buffer de entrada (`std::cin.clear()` e `std::cin.ignore()`) após a leitura de números ou para evitar problemas com `getline()` após `cin >>`. | Nenhum. |
| `string caseSensitive(const string& s)` | Converte uma string para minúsculas (case-insensitive) para ser usada em comparações e ordenação. | `const string& s`: String a ser convertida. |
| `string trataEspacos()` | Lê uma linha de entrada do usuário e substitui todos os espaços (`' '`) por barras (`'/'`). Útil para formatar o campo `tipo`. | Nenhum. |
| `int verifyIfNumber()` | Solicita uma entrada ao usuário e garante que é um número inteiro positivo, tratando exceções (`invalid_argument`, `out_of_range`). | Nenhum. |
| `bool isInteger(const string& s)` | Verifica se uma string representa um número inteiro válido. | `const string& s`: String a ser verificada. |
| `bool isFloat(const string& s)` | Verifica se uma string representa um número de ponto flutuante válido. | `const string& s`: String a ser verificada. |

<h3> ► Funções Principais</h3>
<ol>
  <li>
    <h4> Função para carregar os dados do .CSV e importar estes dados no ponteiro</h4>
	  
```cpp
void carregar_dados_csv(Dragon*& dragoes, int& tamanho, int& capacidade, const string& nome_arquivo) 
```
<h4> Carrega os dados do csv e armazena em um vetor local </h4>
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`| Referência para o ponteiro do array dinâmico de objetos `Dragon`. <br/> O ponteiro é atualizado para a nova área de memória alocada. | 
| `int& tamanho` | Referência para o número atual de elementos. Usado para copiar os dados. | 
| `int& capacidade:` | Referência para o tamanho máximo atual do array. <br/> É atualizada com a nova capacidade. | 
| `const string& arquivo_string`| Referência para o fluxo de entrada de dados (ifstream) já aberto, associado ao arquivo CSV.  | 

<p>Retornos</p>

```cpp
true;  // Se o arquivo CSV foi carregado com sucesso
false; // Se houve falha ao abrir o arquivo
```
  </li>
  <li>
	<h4> Função para redimensionar o array dinâmico de dragões </h4>
	  
```cpp
void redimensionar_vetor(Dragon*& dragao, int& tamanho, int& capacidade)
```
<h4> Aumenta a capacidade do array dinâmico, geralmente em 10, para acomodar mais dragões </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :----- | :----- | 
| `Dragon*& dragao`| Referência para o ponteiro do array dinâmico de objetos `Dragon`. <br/> O ponteiro é atualizado para a nova área de memória alocada. | 
| `int& tamanho` | Referência para o número atual de elementos. Usado para copiar os dados. | 
| `int& capacidade:` | Referência para o tamanho máximo atual do array. <br/> É atualizada com a nova capacidade.. | 


<p>Retornos</p>

```cpp
void  // A função não retorna valor, apenas modifica os parâmetros por referência.
```
  </li>
  <li>
	  <h4> Função para salvar as alterações (inserção/remoção) no arquivo CSV </h4>
	  
```cpp
void salvarMudancas (Dragon*& dragao, int& tamanho, int& capacidade, const string& arquivo_string,
					 bool overwrite, int qtdNovos = 0)
```
<h4> Persiste os dados atuais do array de dragões no arquivo CSV. </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`| Referência para o ponteiro do array dinâmico de objetos `Dragon`. <br/> O ponteiro é atualizado para a nova área de memória alocada. | 
| `int& tamanho` | Referência para o número atual de elementos. Usado para copiar os dados. | 
| `int& capacidade:` | Referência para o tamanho máximo atual do array. <br/> É atualizada com a nova capacidade. | 
| `const string& arquivo_string`| Nome do arquivo CSV onde os dados serão salvos. | 
| `bool overwrite` | Se `true`, o arquivo é sobrescrito (usado após remoção/ordenação). <br/> Se `false`, novos registros são anexados. |
| `int qtdNovos = 0` | Usado no modo `apppend` (`overwrite = false`) para saber a partir de qual índice começar a salvar os novos dragões. |


<p>Retornos</p>

```cpp
void  // A função não retorna valor.
```

  </li>
<h3>  ► Funções de Inserção e Remoção (CRUD)</h3>
  <li>
	  
<h4> Função para inserir um ou mais novos dragões </h4>

```cpp
void inserirDragoes (Dragon*& dragao, int& tamanho, int& capacidade, int qtd, bool& salvo)
```
<h4> Permite a entrada de dados de novos dragões, incluindo validação e redimensionamento automático do array. </h4>
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`| Referência para o ponteiro do array dinâmico de objetos `Dragon`. <br/> O ponteiro é atualizado para a nova área de memória alocada. | 
| `int& tamanho` | O número atual de dragões no array.| 
| `int& capacidade:` | Referência para o tamanho máximo atual do array. <br/> É atualizada com a nova capacidade. | 
| `int qtd`| Quantidade de dragões que o usuário deseja inserir. | 
| `bool& salvo` | Flag que é definida como `false` para indicar alterações não salvas.|

<p>Retornos</p>

```cpp
void  // A função não retorna valor.
```
<h4> Função para marcar dragões para remoção lógica </h4>
	  
```cpp
void removerDragao(Dragon* dragao, int tamanho, int qtd, bool& salvo)
```

<h4> <strong> Marca um ou mais dragões para remoção. </strong> <br/> ◌ A remoção física (do arquivo CSV) ocorre apenas no salvamento. </h4>
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`| Referência para o ponteiro do array dinâmico de objetos `Dragon`. <br/> O ponteiro é atualizado para a nova área de memória alocada. | 
| `int tamanho` | Referência para o número atual de elementos. Usado para copiar os dados. | 
| `int qtd`| Quantidade de dragões que o usuário deseja inserir. | 
| `bool& salvo` | Flag que é definida como `false` para indicar alterações não salvas.|

<p>Retornos</p>

```cpp
void  // A função não retorna valor.
```
<h4> Função para cancelar a remoção lógica de dragões </h4>
		  
```cpp
void cancelarRemocao(Dragon* dragao, const int tamanho)
```
<h4> Reverte a flag <code style="padding: 5px;font-size: 130%;"> removido_logicamente </code> de volta para <code style="padding: 5px;font-size: 130%;"> false </code> para dragões marcados para exclusão. </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`| Referência para o ponteiro do array dinâmico de objetos `Dragon`. <br/> O ponteiro é atualizado para a nova área de memória alocada. | 
| `const int tamanho` | O número atual de dragões no array. | 

<p>Retornos</p>

```cpp
void  // A função não retorna valor.
```

</li>
<li>
<h3>  ► Funções de Ordenação (QuickSort) </h3>

<h4> Função auxiliar de comparação para o QuickSort </h4>

```cpp
bool comparaEntrada(const Dragon* dragao, const int fim, const int j, const int entrada)
```

<h4> Compara dois elementos do array de dragões (em <code style="padding: 5px;font-size: 130%;">fim </code> e <code style="padding: 5px;font-size: 130%;">j</code>) com base no critério de ordenação selecionado. </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`| Ponteiro para o array de dragões. | 
| `const int fim` | Índice do elemento pivô.| 
| `const int j`| Índice do elemento atual sendo comparado. | 
| `const int entrada` |  Critério de ordenação (0=ID, 1=Nome, 2=Tipo, etc).|

<p>Retornos</p>

```cpp
true;  // Se o dragão em 'j' deve vir antes do dragão em 'fim'.
false; // Caso contrário.
```

<h4> Função de partição do algoritmo QuickSort </h4>

```cpp
int partition(Dragon* dragao, const int inicio, const int fim, const int entrada)
```

<h4> Rearranja os elementos da submatriz em torno de um elemento pivô (o último elemento). </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`     | Ponteiro para o array de dragões. | 
| `const int inicio`    | Índice de início da submatriz. | 
| `const int fim`		| Índice de fim da submatriz (o elemento pivô). | 
| `const int entrada`   |  Critério de ordenação (0=ID, 1=Nome, 2=Tipo, etc).|

<p>Retornos</p>

```cpp
int // O índice final do pivô na submatriz particionada.
```

<h4> Função principal de ordenação QuickSort </h4>

```cpp
void quickSort(Dragon* dragao, const int inicio, const int fim, const int entrada)
```

<h4> Implementa o algoritmo QuickSort de forma recursiva para ordenar o array de dragões. </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`     | Ponteiro para o array de dragões. | 
| `const int inicio`    | Índice de início da submatriz. | 
| `const int fim`		| Índice de fim da submatriz (o elemento pivô). | 
| `const int entrada`   | Critério de ordenação (0=ID, 1=Nome, 2=Tipo, etc).|

<p>Retornos</p>

```cpp
void 	 // A função não retorna valor, ordena o array in-place.
```
</li>
<li>
	<h3> Funções de Busca (Busca Binária)</h3>
<h4> ► Função de busca binária recursiva por ID </h4>
	
```cpp
int buscaBinaria(Dragon* dragao, int key, int inicio, int fim)
```

<h4> Localiza um dragão pelo seu ID (chave numérica). Requer que o array esteja ordenado por ID. </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`     | Ponteiro para o array de dragões (ordenado por ID). | 
| `int key`    			| ID do dragão a ser buscado. | 
| `int inicio`			|  Índice inicial para a busca. | 
| `int fim`   			| Índice final para a busca.|

<p>Retornos</p>

```cpp
int 	// O índice do dragão encontrado, ou -1 se não for encontrado.
```

<h4> Função de busca binária recursiva por Nome </h4>

```cpp
int buscaBinariaPeloNome (Dragon* dragao, const string& dragonName, int inicio, int fim)
```

<h4> Localiza um dragão pelo seu nome. Requer que o array esteja ordenado por Nome. </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`     		| Ponteiro para o array de dragões (ordenado por Nome).| 
| `const string& dragonName` 	| O nome do dragão a ser buscado. | 
| `int inicio`					| Índice inicial para a busca. | 
| `int fim`   					| Índice final para a busca.|

<p>Retornos</p>

```cpp
int 	// O índice do dragão encontrado, ou -1 se não for encontrado.
```

<h4> Função de busca binária recursiva por Tipo </h4>

```cpp
int buscaBinariaPeloTipo(Dragon* dragao, const string& tipoBuscado, int inicio, int fim)
```

<h4> Localiza um dragão pelo seu tipo. Requer que o array esteja ordenado por Tipo. </h4> 
<p>Parâmetros</p>

| Parâmetro | Descrição | 
| :------- | :------- | 
| `Dragon*& dragao`     		| Ponteiro para o array de dragões (ordenado por Tipo).| 
| `const string& tipoBuscado` 	| O tipo de dragão a ser buscado. | 
| `int inicio`					| Índice inicial para a busca. | 
| `int fim`   					| Índice final para a busca.|

<p>Retornos</p>

```cpp
int 	// O índice do dragão encontrado, ou -1 se não for encontrado.
```
</li>
</ol>

<h3>Interface TUI (Terminal User Interface) com Notcurses</h3>
<p> O Notcurses é essencial para transformar o programa de console tradicional em uma aplicação interativa com menus visuais, navegação por teclado e atualização dinâmica de tela. </p>

| Aspecto | Detalhes Técnicos e Uso no Projeto | 
| :------- | :------- | 
| `Inicialização e Setup`     		| A TUI é iniciada com `notcurses_init()`. <br/> A estrutura `notcurses_options` é configurada com a flag `NCOPTION_SUPPRESS_BANNERS` para garantir que o terminal seja controlado exclusivamente pela aplicação, sem mensagens iniciais padrão do Notcurses, assegurando uma experiência visual limpa. O ponteiro nc gerencia todo o ambiente gráfico.| 
| `Desenho de Conteúdo (ncplane)` 	| O Notcurses utiliza `ncplane (planos)` para gerenciar o desenho. <br/> O plano principal é usado para desenhar caixas, títulos e opções de menu. <br/> Funções como `ncplane_putstr_aligned()` são usadas para imprimir o texto do menu de forma centralizada ou alinhada, e funções de cor `(ncplane_set_bg_rgb8()` e `ncplane_set_fg_rgb8())` aplicam cores de fundo e texto personalizadas (usando RGB), permitindo um design rico no terminal.| 
| `Tratamento de Entrada`					| A interatividade é implementada através de `notcurses_get(nc, NULL, &input)`, que aguarda eventos de teclado (capturados na `estrutura ncinput`). <br/> O programa interpreta os valores de eventos como `NCKEY_UP`, `NCKEY_DOWN` e `NCKEY_ENTER` para controlar a navegação dos menus e a seleção de opções.| 
| `Renderização e Destaque`   					| Após cada mudança de estado (ex: mover o cursor no menu), o código manipula as cores da linha de texto que representa a opção atual para criar o efeito de destaque visual (highlighting). <br/> A função `notcurses_render(nc)` é chamada a cada ciclo para atualizar o terminal, aplicando todas as mudanças desenhadas e proporcionando a sensação de uma interface de usuário dinâmica.|

<br/>
<h3> ► Funções de Menu (Interface TUI Notcurses) </h3>
<p>  Estas funções são responsáveis por apresentar as opções ao usuário e gerenciar a navegação e a chamada das operações principais do sistema através da interface Notcurses.</p>

| Função | Descrição | Parâmetros | Explicação do Uso | 
| :------- | :------- | :------- | :------- | 
| `int menuNotcurses(...)`     		| Menu Principal: Exibe o menu principal TUI, controlando a navegação com setas e ENTER. Exibe o aviso de alterações não salvas.| struct notcurses* nc, bool salvo | É a função central do loop principal da aplicação. Gerencia o estado da interface, recebe a escolha do usuário e exibe o status de persistência (salvo ou não) através da flag bool salvo.|
| `void menuMostrarDragoes(...)` 	| Visualização: Permite ao usuário escolher se deseja mostrar todos os dragões ou um intervalo personalizado por ID.| struct notcurses*& nc, Dragon* dragoes, int tamanho | Lida com a exibição dos dados. Utiliza os recursos do Notcurses para exibir a listagem em um formato que permite rolagem (scrolling) dentro da TUI, otimizando a leitura de grandes volumes de dados.| Guia o usuário pela TUI e chama as funções lógicas de entrada de dados do console (std::cin), garantindo a validação e o redimensionamento dinâmico. Ao final, define salvo como false. |
| `void menuInserirDragoes(...)`	| Inserção: Inicia o processo de inserção de novos dragões, solicitando a quantidade e os dados.| struct notcurses*& nc, Dragon*& dragao, int& tamanho, int& capacidade, bool& salvo, const string& arquivo_string |Guia o usuário pela TUI e chama as funções lógicas de entrada de dados do console (std::cin), garantindo a validação e o redimensionamento dinâmico. Ao final, define salvo como false.|
| `void menuRemoverDragoes(...)`   					| Remoção Lógica: Permite que o usuário marque um ou mais dragões para remoção lógica.| struct notcurses*& nc, Dragon*& dragao, int& tamanho, int& capacidade, bool& salvo, const string& arquivo_string| Gerencia a interação TUI para que o usuário informe os IDs. Em seguida, chama a lógica que atualiza o campo removido_logicamente. Define salvo como false.|
| `void menuBuscarDragoes(...)`   					| Busca: Menu TUI para selecionar o critério de busca (ID, Nome, Tipo). <br/> A busca é mostrada na interface.| struct notcurses*& nc, Dragon* dragoes, int tamanho, const string& arquivo_string| Garante que o array esteja ordenado (se necessário), chama a lógica de busca binária e o resultado é exibido de forma formatada diretamente na interface Notcurses.|
| `void menuOrdenarDragoes(...)`   					| Ordenação: Menu TUI para selecionar o critério de ordenação (ID, Nome, Tipo), aplicando o QuickSort.| struct notcurses*& nc, Dragon* dragao, int tamanho, bool& salvo	| Apresenta as opções de ordenação e executa o quickSort no array em memória. Define salvo como false, pois a ordenação altera a sequência de dados em relação ao arquivo salvo. |
| `void menuSalvarDragoes(...)`   					| Persistência: Menu TUI para confirmar o salvamento das alterações no arquivo CSV.	| struct notcurses*& nc, Dragon*& dragao, int& tamanho, int& capacidade, const string& arquivo_string, bool& salvo, int inicioInseridos	| É o ponto de controle para a persistência. Chama a função salvarMudancas, que lida com a reescrita do arquivo (removendo os registros marcados). Se for bem-sucedido, define salvo como true. |

<br/>
<h2> Como Compilar e Executar </h2>

<h3>Pré-requisitos: </h3>
<ul>
	<li>Compilador C++ (g++ recomendado)</li>
	<li>Biblioteca Notcurses instalada.</li>
</ul>
<h3>Compilação</h3>

```bash
# Exemplo de comando de compilação (pode variar dependendo da configuração do Notcurses)
g++ main.cpp -lnotcurses-core -lnotcurses -o ./main
```

Execução:
```bash
./main
```
