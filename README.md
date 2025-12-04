# 🐉 Sistema de Gerenciamento de Dragões -- Dragon City Dex

Nesse código, desenvolvemos um programa que funciona como um "Dex", do jogo Dragon City. Durante sua execução um .csv é consumido, nele podemos realizar alterações como ordenação, inserção de dados e remoção. Além disso, conseguimos expor seu conteúdo completo ou em um determinado intervalo.

## Visão Geral

-   Leitura de um arquivo CSV contendo dados dos dragões
-   Inserção de novos registros (com validação)
-   Remoção lógica (ID negativo)
-   Ordenação (QuickSort) por ID, Nome ou Tipo
-   Busca Binária por ID, Nome e Tipo
-   Exibição total ou parcial dos dados
-   Salvamento das alterações no CSV

## Estrutura dos Dados

### `struct database`

``` cpp
struct database {
    int id;
    string nome;
    string tipo;
    int nivel;
    int vida;
    int ataque;
    float chanceCritico;
    string habEspecial;
};
```

## 🔨 Funções Implementadas

### Funções de Utilidade e Entrada

```cpp
string caseSensitive(const string&)
```

| Parâmetro         | Descrição                                                           |
| ----------------- | ------------------------------------------------------------------- |
| `const string& s` | String original cuja versão minúscula e sem espaços será retornada. |

Converte a entrada para letras minúsculas e remove espaços. Essa função é utilizada dentro da função `partition`, visando uma comparação mais certeira.

```cpp
string trataEspacos()
```
 
| Parâmetro | Descrição                                                            |
| --------- | -------------------------------------------------------------------- |
| *nenhum*  | A função apenas lê entrada do usuário e substitui espaços por `'/'`. |

Converte os espaços `' '` em `'/'`.

```cpp
int trataEntradaInt()
```

| Parâmetro | Descrição                                          |
| --------- | -------------------------------------------------- |
| *nenhum*  | A função lê e valida um número inteiro da entrada. |

Lê e valida um número inteiro.

```cpp
float trataEntradaFloat()
```

| Parâmetro | Descrição                                                            |
| --------- | -------------------------------------------------------------------- |
| *nenhum*  | A função lê e valida um valor de ponto flutuante no formato `x.xxx`. |

Lê e valida um número de ponto flutuante (garantindo que esteja adequado ao formato `x.xxx`)

```cpp
void aperteEnter()
```

| Parâmetro | Descrição                                                        |
| --------- | ---------------------------------------------------------------- |
| *nenhum*  | A função apenas pausa o programa até o usuário pressionar ENTER. |

Pausa a execução até o usuário pressionar ENTER.

---
### Funções de Gerenciamento de Memória

``` cpp
void redimensionarVetor(database*& dragao, int& tamanho);
```

Aumenta a capacidade do vetor dinâmico de dragões em 10 posições, implementando a realocação dinâmica: um novo vetor é criado, os dados são copiados para ele e o vetor antigo é removido da memória.

``` cpp
database* lerValores(ifstream& dados, int& capacidadeVetor, int& qntDados);
```

| Parâmetro              | Descrição                                                             |
| ---------------------- | --------------------------------------------------------------------- |
| `ifstream& dados`      | Arquivo CSV aberto de onde os registros serão lidos.                  |
| `int& capacidadeVetor` | Capacidade atual do vetor; aumenta automaticamente quando necessário. |
| `int& qntDados`        | Quantidade de dragões lidos e inseridos no vetor.                     |

Lê todos os dados do arquivo CSV (dragoes.csv) para o vetor dinâmico na memória, tratando corretamente o formato CSV (delimitadores por vírgula e aspas) e chamando redimensionarVetor caso o limite seja atingido.

``` cpp
int maiorID(const database* dragao, const int quantidadeDragoes);
```

| Parâmetro                     | Descrição                             |
| ----------------------------- | ------------------------------------- |
| `const database* dragao`      | Vetor contendo os dragões carregados. |
| `const int quantidadeDragoes` | Quantidade total de dragões no vetor. |

Encontra o maior ID atualmente registrado no banco de dados, sendo utilizado para garantir que novos dragões recebam um identificador único (maior ID + 1).

### Inserção e Remoção

``` cpp
void inserirDragao(database*& dragao, int quantidade, int& capacidadeVetor, int& quantidadeDragoes);
```

| Parâmetro                | Descrição                                                             |
| ------------------------ | --------------------------------------------------------------------- |
| `database*& dragao`      | Ponteiro para o vetor dinâmico onde os novos dragões serão inseridos. |
| `int quantidade`         | Quantidade de dragões que o usuário deseja inserir.                   |
| `int& capacidadeVetor`   | Capacidade atual do vetor; pode ser aumentada durante a inserção.     |
| `int& quantidadeDragoes` | Contador total de dragões no vetor após as inserções.                 |

Insere um ou mais novos dragões no vetor de dados. Para isso, é solicitado os atributos do dragão, por meio das funções que tratam as entradas.

``` cpp
void removerDragao(database*& dragao, const int tamanhoVetor, const int id);
```

| Parâmetro                | Descrição                                         |
| ------------------------ | ------------------------------------------------- |
| `database*& dragao`      | Vetor onde o dragão será marcado com ID negativo. |
| `const int tamanhoVetor` | Quantidade total de dragões armazenados.          |
| `const int id`           | ID do dragão que deve ser marcado como removido.  |

Marca um dragão para remoção. Altera o ID dos dragões marcados para remoção para negativos, multiplicando-os por `* -1`. Após isso, é chamado a função de salvar dados, caso o usuário confirme, os dados são apagados definitivamente do `.csv`.

``` cpp
void cancelarRemocao(database* dragao, const int tamanhoVetor);
```

| Parâmetro                | Descrição                                                                    |
| ------------------------ | ---------------------------------------------------------------------------- |
| `database* dragao`       | Vetor contendo os dragões, incluindo os que estavam marcados como removidos. |
| `const int tamanhoVetor` | Quantidade total de elementos no vetor.                                      |

Restaura o ID original de todos os dragões marcados para exclusão, quando o usuário não confirma `Salvar Dados`.

### Ordenação (QuickSort)

``` cpp
bool comparaEntradaQuickSort(const database* dragao, const int fim, const int j, const int entrada);
```

| Parâmetro                | Descrição                                                                 |
| ------------------------ | ------------------------------------------------------------------------- |
| `const database* dragao` | Vetor com os dados usados na comparação.                                  |
| `const int fim`          | Índice do pivô utilizado para comparação.                                 |
| `const int j`            | Índice do elemento que está sendo comparado com o pivô.                   |
| `const int entrada`      | Código que indica qual campo comparar (`0 = id`, `1 = nome`, `2 = tipo`). |

* **Objetivo:** Realizar a comparação de valores e retornar `true` ou `false`, para ser utilizando dentro do `Partition`.
* **Códigos de Comparação:**
    * `0`: Compara por **ID**.
    * `1`: Compara por **Nome** (case-insensitive).
    * `2`: Compara por **Tipo** (case-insensitive).

``` cpp
int partition(database* dragao, const int inicio, const int fim, const int entrada);
```

| Parâmetro           | Descrição                             |
| ------------------- | ------------------------------------- |
| `database* dragao`  | Vetor que será particionado.          |
| `const int inicio`  | Índice inicial da partição.           |
| `const int fim`     | Índice final da partição (pivô).      |
| `const int entrada` | Campo usado como base para ordenação. |

* **Objetivo:** Realiza o passo de partição do QuickSort, posicionando o pivô (último elemento) no seu lugar correto.
* **Retorno:** O índice final do pivô.

``` cpp
void quickSort(database* dragao, const int inicio, const int fim, const int entrada);
```

| Parâmetro           | Descrição                               |
| ------------------- | --------------------------------------- |
| `database* dragao`  | Vetor a ser ordenado.                   |
| `const int inicio`  | Índice inicial do intervalo a ordenar.  |
| `const int fim`     | Índice final do intervalo.              |
| `const int entrada` | Campo usado como critério de ordenação. |

* **Objetivo:** Implementa o algoritmo de ordenação QuickSort de forma recursiva.

### Busca Binária

``` cpp
int buscaBinariaID(database* dragao, const int inicio, const int fim, const int k);
```

| Parâmetro          | Descrição                 |
| ------------------ | ------------------------- |
| `database* dragao` | Vetor já ordenado por ID. |
| `const int inicio` | Início da busca.          |
| `const int fim`    | Fim da busca.             |
| `const int k`      | ID procurado.             |

* **Objetivo:** Procura um dragão por seu **ID**.
* **Algoritmo:** Implementação recursiva da Busca Binária.
* **Retorno:** O índice (posição) do dragão encontrado ou **-1**.

``` cpp
int buscaBinariaNome(database* dragao, const int inicio, const int fim, string& k);
```

| Parâmetro          | Descrição                                               |
| ------------------ | ------------------------------------------------------- |
| `database* dragao` | Vetor de dragões ordenado por nome.                     |
| `const int inicio` | Início da busca.                                        |
| `const int fim`    | Fim da busca.                                           |
| `string& k`        | Nome buscado (convertido para minúsculas internamente). |

* **Objetivo:** Procura um dragão por seu **Nome**.
* **Algoritmo:** Implementação recursiva da Busca Binária, usando a função `caseSensitive` para comparação de strings.
* **Retorno:** O índice do dragão encontrado ou **-1**.

### Arquivos e Saída

``` cpp
void salvarMudancas(const database* dragao, const int tamanho);
```

| Parâmetro                | Descrição                                        |
| ------------------------ | ------------------------------------------------ |
| `const database* dragao` | Vetor contendo os dados atualizados dos dragões. |
| `const int tamanho`      | Quantidade total de dragões no vetor.            |

* **Objetivo:** Salva o estado atual do vetor de dragões no arquivo `dragoes.csv`.
* **Funcionamento:** **Sobrescreve** o arquivo, escreve o cabeçalho e, em seguida, escreve os dados de cada dragão com `id > 0` no formato `.csv`.

``` cpp
void escreveVetor(const database* dragao, const int tamanhoVetor, const int inicio = 0);
```

| Parâmetro                | Descrição                                     |
| ------------------------ | --------------------------------------------- |
| `const database* dragao` | Vetor contendo os dragões a serem exibidos.   |
| `const int tamanhoVetor` | Número total de dragões armazenados (id > 0). |
| `const int inicio`       | Índice inicial da exibição (default = 0).     |

* **Objetivo:** Exibe os dados de todos os dragões com `id > 0` na tela.

``` cpp
void escreveParteVetor(const database* dragao, const int inicio, const int fim, const int qntDragoes);
```

| Parâmetro                | Descrição                                   |
| ------------------------ | ------------------------------------------- |
| `const database* dragao` | Vetor contendo os dragões a serem exibidos. |
| `const int inicio`       | Índice inicial da faixa exibida.            |
| `const int fim`          | Índice final da faixa exibida.              |
| `const int qntDragoes`   | Quantidade total de dragões existentes.     |

* **Objetivo:** Exibe os dragões em um intervalo específico (índices) fornecido pelo usuário.

``` cpp
void escreverOrdenado(database* dragao, const int tamanhoVetor, const string& s, const int opcao);
```

| Parâmetro                | Descrição                                              |
| ------------------------ | ------------------------------------------------------ |
| `database* dragao`       | Vetor que será ordenado temporariamente.               |
| `const int tamanhoVetor` | Quantidade de dragões no vetor.                        |
| `const string& s`        | Nome do campo exibido na mensagem ao usuário.          |
| `const int opcao`        | Campo usado na ordenação (`0=id`, `1=nome`, `2=tipo`). |

* **Objetivo:** Ordena o vetor conforme o campo escolhido pelo usuário, utilizando `QuickSort`, exibe a lista e pergunta se o usuário deseja salvar a nova ordem no `.csv`.

``` cpp
void escreverMenu();
```

| Parâmetro | Descrição                       |
| --------- | ------------------------------- |
| *nenhum*  | Apenas exibe as opções do menu. |

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
* **Objetivo:** Exibe o menu principal de opções do sistema.

``` cpp
void verificaArquivo(const ifstream& dados, int& entrada);
```

| Parâmetro               | Descrição                                                     |
| ----------------------- | ------------------------------------------------------------- |
| `const ifstream& dados` | Arquivo CSV que está sendo verificado.                        |
| `int& entrada`          | Variável de controle do menu; recebe `9` caso abertura falhe. |

* **Objetivo:** Verifica se o arquivo CSV foi aberto corretamente. Se não, força a saída do programa (entrada = 9).

## ⚒️ Função Principal

``` cpp
int main();
```

---

## 🔧 Como Compilar o Projeto

Para compilar o programa em C++, utilize o `g++` (ou qualquer compilador compatível com C++11);

```bash
# Compile
g++ main.cpp -o dragon_city_dex

# Execute
./dragon_city_dex
```
---

## 📁 Estrutura do Arquivo CSV (`dragoes.csv`)

O programa utiliza um arquivo CSV no seguinte formato:

```
id,nome,tipo,nivel,vida,ataque,chanceCritico,habEspecial
```

### ⚙️ Regras importantes:

* Campos de texto podem conter espaços e devem estar entre aspas (`"`).
* `id` deve ser um número inteiro positivo.
* `chanceCritico` utiliza ponto no formato `x.xxx`.

### ⚙️ Exemplo válido:

```csv
id,nome,tipo,nivel,vida,ataque,chanceCritico,habEspecial
1,"Dragão Joãozinho","Preguiça",3,1200,150,0.015,"Bocejo Devastador"
2,"Dragão Marianão","Carisma",7,2400,320,0.072,"Abraço Supersônico"
3,"Dragão Zé Bolacha","Farinha",5,1800,210,0.048,"Esfarelamento Total"
4,"Dragão Bicudo","Fofoqueiro",9,2600,330,0.099,"Língua de Fogo (Literal)"
5,"Dragão Cleitin da Firma","Café",12,3400,420,0.143,"Virada Madrugada"
```

## Autoria

Projeto desenvolvido por **Felipe Pimenta**, **Pedro Oliveira** e **Murilo Carvalho**,  
estudantes do *Bacharelado em Sistemas de Informação* da  
**Universidade Federal de Lavras (UFLA)**.

