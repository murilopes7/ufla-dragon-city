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
<h3> Manipulação dos dados do CSV </h3>
<ol>
  <li>
    
```cpp
void carregar_dados_csv(Dragon*& dragoes, int& tamanho, int& capacidade,
    const string& nome_arquivo) 
```
<h4> Carrega os dados do csv e armazena em um vetor local </h4>
<p>Parâmetros</p>

```cpp
Dragon *& dragoes:           Referência para o ponteiro do array dinâmico de objetos Dragon. 
                             Permite que a função realoque o array e atualize o ponteiro.
int& tamanho:                Referência para o número atual de elementos válidos no array. 
                             Será atualizado com a quantidade de itens carregados.
int& capacidade:             Referência para o tamanho máximo atual do array. 
                             Será atualizado se o array for redimensionado.
const string& nome_arquivo:  Referência para o fluxo de entrada de dados (ifstream) 
                             já aberto, associado ao arquivo CSV. 
```
<p>Retornos</p>

```cpp
true;  // Se o arquivo CSV foi carregado com sucesso
false; // Se houve falha ao abrir o arquivo
```
  </li>
  <li>
    
  </li>
  
</ol>


