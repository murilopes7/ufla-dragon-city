# 🐉 Sistema de Gerenciamento de Dragões - Dragon City Dex

Nesse código, desenvolvemos um programa que funciona como um “Dex” do jogo **Dragon City**. O sistema consome um arquivo `.csv`, permitindo realizar operações como ordenação, inserção de dados, remoção lógica, busca e exibição total ou parcial do conteúdo.

---

## Visão Geral

* Leitura de um arquivo CSV contendo dados dos dragões
* Inserção de novos registros (com validação)
* Remoção lógica (ID se torna negativo)
* Ordenação (QuickSort) por ID, Nome ou Tipo
* Busca Binária por ID e Nome
* Exibição total ou parcial dos dados
* Salvamento das alterações no CSV
* Recarregamento completo do CSV original
* Redimensionamento automático do vetor base (+10 posições quando necessário)

---

## 🧩 Estrutura dos Dados

### `struct database`

```cpp
struct database {
    int id;                        // Identificador único do Dragão
    string nome;                   // Nome do Dragão 
    string tipo;                   // Tipo(s) do Dragão (separados por '/')
    int nivel;                     // Nível atual do Dragão
    int vida;                      // Vida (HP) atual
    int ataque;                    // Dano base
    float chanceCritico;           // Valor decimal no formato x.xxx
    string habEspecial;            // Habilidade especial
};
```

---

## 🔧 Funções Implementadas

### Funções de Utilidade e Entrada

<ol>

<li>

```cpp
string caseSensitive(const string&)
```

| Parâmetro         | Descrição                                                |
| ----------------- | -------------------------------------------------------- |
| `const string& s` | String original a ser convertida para letras minúsculas. |

► **Objetivo:** Normalizar strings para comparação em ordenação/busca.

► **Retorno:** String em minúsculas (ignora espaços internos).

</li>

<li>

```cpp
string trataEspacos()
```

| Parâmetro | Descrição                                              |
| --------- | ------------------------------------------------------ |
| *nenhum*  | Lê a entrada do usuário e converte espaços para `'/'`. |

► **Objetivo:** Padronizar campos textuais sem espaços.

► **Retorno:** String com `/` no lugar de espaços.

</li>

<li>

```cpp
int trataEntradaInt()
```

► **Objetivo:** Garantir que a entrada contenha apenas números.

► **Retorno:** Inteiro convertido após validação.

</li>

<li>

```cpp
float trataEntradaFloat()
```

► **Objetivo:** Validar números no formato `x.xxx`.

► **Retorno:** Float convertido.

</li>

<li>

```cpp
void aperteEnter()
```

► **Objetivo:** Pausar a execução até ENTER ser pressionado.

</li>

</ol>

---

## Funções Principais

### Funções de Gerenciamento de Memória

<ol>

<li>

```cpp
void redimensionarVetor(database* &dragao, int &tamanhoVetor);
```

► **Objetivo:** Aumentar o vetor em +10 posições.

► **Algoritmo:** Cria novo vetor, copia conteúdo e atualiza ponteiro.

► **Retorno:** Vetor expandido.

</li>

<li>

```cpp
database* lerValores(ifstream &dados, int &tamanhoVetor, int &quantidadeDados);
```

| Parâmetro         | Descrição                   |
|-------------------| --------------------------- |
| `ifstream& dados` | Arquivo CSV aberto          |
| `capacidadeVetor` | Capacidade atual do vetor   |
| `quantidadeDados` | Quantidade de dragões lidos |

► **Objetivo:** Carregar todo o CSV para memória.

► **Retorno:** Vetor preenchido com todos os registros válidos.

</li>

<li>

```cpp
int maiorID(const database* dragao, const int quantidadeDragoes);
```

► **Objetivo:** Encontrar o maior ID do vetor.

► **Retorno:** Maior ID encontrado.

</li>

</ol>

---

## Inserção e Remoção

<ol>

<li>

```cpp
void inserirDragao(database* &dragao, int quantidade, int &capacidadeVetor, int &quantidadeDragoes);
```

► **Objetivo:** Inserir novos dragões com ID automático (`maiorID + 1`).

► **Algoritmo:** Lê dados do usuário, valida entradas e expande vetor se necessário.

</li>

<li>

```cpp
void removerDragao(database* &dragao, const int tamanhoVetor, const int id);
```

► **Objetivo:** Remover dragões (remoção lógica).

► **Algoritmo:** Multiplica o ID por `-1`.

► **Observação:** A remoção só é efetivada no CSV após confirmação.

</li>

<li>

```cpp
void cancelarRemocao(database* dragao, const int tamanhoVetor);
```

► **Objetivo:** Reverter remoções caso o usuário opte por não salvar.

► **Efeito:** IDs negativos voltam a ser positivos.

</li>

</ol>

---

## Ordenação (QuickSort)

<ol>

<li>

```cpp
bool comparaEntradaQuickSort(const database* dragao, const int fim, const int j, const int entrada);
```

► **Comparações possíveis:**
`0 = id` | `1 = nome` | `2 = tipo`

► **Objetivo:** Critério de comparação para QuickSort.

</li>

<li>

```cpp
int partition(database* dragao, const int inicio, const int fim, const int tipoOrdenacao);
```

► **Objetivo:** Particionar vetor com base no pivô.

► **Retorno:** Índice final do pivô.

</li>

<li>

```cpp
void quickSort(database* dragao, const int inicio, const int fim, const int entrada);
```

► **Objetivo:** Ordenar o vetor completo recursivamente.

</li>

</ol>

---

## Busca Binária

<ol>

<li>

```cpp
int buscaBinariaID(database* dragao, const int inicio, const int fim, const int k);
```

► **Objetivo:** Buscar dragão pelo ID.

► **Retorno:** Índice ou `-1`.

</li>

<li>

```cpp
int buscaBinariaNome(database* dragao, const int inicio, const int fim, string &k);
```

► **Objetivo:** Buscar dragão pelo nome (case-insensitive).

► **Retorno:** Índice ou `-1`.

</li>

</ol>

---

## Arquivos e Saída

<ol>

<li>

```cpp
void salvarMudancas(const database* dragao, const int tamanhoVetor);
```

► **Objetivo:** Reescrever completamente o arquivo `dragoes.csv`, ignorando IDs negativos.

</li>

<li>

```cpp
void escreveVetor(const database* dragao, const int tamanhoVetor, const int inicio = 0);
```

► **Objetivo:** Exibir o vetor completo ou parcial.

</li>

<li>

```cpp
void escreveParteVetor(const database* dragao, const int inicio, const int fim, const int quantidadeDragoes);
```

► **Objetivo:** Exibir apenas o intervalo escolhido pelo usuário.

</li>

<li>

```cpp
void escreverOrdenado(database* dragao, const int tamanhoVetor, const string &s, const int tipoOrdenacao);
```

► **Objetivo:** Ordenar, exibir e solicitar confirmação de salvamento.

</li>

<li>

```cpp
void verificaArquivo(const ifstream &dados, int &entrada);
```

► **Objetivo:** Checar se o CSV abriu corretamente.

</li>

</ol>

---

## User Interface (UI) do Menu

```cpp
void escreverMenu();
```

```
                    $$$                                                 
                   $   $                                                
                    $$$                                                 
                    $ $                                                 
                    $ $                                                 
                  $$$ $$$                                               
                $$  $$$  $$$                                            
              $$  $$$$$$$   $                                           
             $               $                                          
            $                 $      ===== MENU DRAGON CITY DEX =====   
            $                 $      0. Escrever arquivo                
            $     $$$$$$$$$$$$$$$    1. Procurar dragao por ID          
            $    $               $   2. Procurar dragao por Nome        
            $    $   $$$$$$$$$$$$$   3. Ordenar por ID                  
            $   $   $           $$$  4. Ordenar por Nome                
            $   $   $ $$$   $$$  $$  5. Ordenar por Tipo                
            $   $   $ $$$   $$$  $$  6. Remover dragao                  
            $   $   $           $$$  7. Listar intervalo X -> Y         
            $    $   $$$$$$$$$$$$$   8. Inserir dragao                  
            $     $$$$$$$$$$$$$$     9. Sair                            
            $                 $      ================================   
            $    $$$$$$$$$$$$$$                                         
            $   $  $  $  $  $                                           
            $  $$$$$$$$$$$$$$                                           
            $  $   $  $  $  $                                           
            $   $$$$$$$$$$$$$$$                                         
           $$$                 $$$                                      
         $$   $$$         $$$$$   $$                                    
       $$        $$$$$$$$$          $$$                                 
      $  $$                     $$$$   $$                               
   $$$$$   $$$$$$$$      $$$$$$$       $ $                              
```

---

## ⚙️ Como Compilar o Projeto

```bash
g++ main.cpp -o dragoncitydex
./dragoncitydex
```

---

## 📁 Estrutura do Arquivo CSV (`dragoes.csv`)

Formato esperado:

```
ID,"Nome do Dragao",Tipo,Nivel,Vida,Ataque,ChanceCritico,"Habilidade Especial"
```

### Regras:

* Strings com espaços devem estar entre aspas `" "`
* Tipos com mais de um elemento devem usar `/`
* `chanceCritico` deve estar no formato decimal `x.xxx`
* IDs negativos indicam remoção lógica

### Exemplo:

```csv
1000,"Dragão Cleitinho",Fogo,2,900,150,0.02,"Reclamão"
1001,"Dragão Maria",Água,4,1400,210,0.03,"Fofoqueira"
```

---

## Autoria

Projeto desenvolvido por:

* **Felipe Eduardo Carvalho Pimenta**
* **Pedro Augusto Castro Oliveira**
* **Murilo Santos Lopes Carvalho**

Estudantes do *Bacharelado em Sistemas de Informação* da **Universidade Federal de Lavras (UFLA)**.