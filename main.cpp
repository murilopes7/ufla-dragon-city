#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct database {
    int id{};
    string nome;
    string tipo;
    int nivel{};
    int vida{};
    int ataque{};
    float chanceCritico{};
    string habEspecial;
};

string caseSensitive(const string &entrada) {
    string temp;
    for (int i = 0; i < static_cast<int>(entrada.length()); i++) {
        if (entrada[i] != ' ') {
            if (entrada[i] >= 'A' && entrada[i] <= 'Z') temp += static_cast<char>(tolower(entrada[i]));
            else temp += entrada[i];
        }
    }
    return temp;
}

string trataEspacos() {
    string entrada;
    getline(cin, entrada);
    for (int i = 0; i <  static_cast<int>(entrada.length()); i++) {
        if (entrada[i] == ' ') entrada[i] = '/';
    }
    return entrada;
}

int trataEntradaInt() {
    string entrada;
    cin >> entrada;
    for (int i = 0; i < static_cast<int>(entrada.length()); i++) {
        if (!isdigit(entrada[i])) {
            cout << "Valor invalido, insira apenas numeros." << endl;
            return trataEntradaInt();
        }
    }
    return stoi(entrada);
}

float trataEntradaFloat() {
    string entrada;
    cin >> entrada;
    if (!isdigit(entrada[0]) || entrada[1] != '.'){
        cout << "Valor invalido! Apenas numeros no formato 0.xxx." << endl;
        return trataEntradaFloat();
    }
    for (int i = 2; i < static_cast<int>(entrada.length()); i++) {
        if (!isdigit(entrada[i])) {
            cout << "Valor invalido! Apenas numeros no formato 0.xxx." << endl;
            return trataEntradaFloat();
        }
    }
    return stof(entrada);
}

void redimensionarVetor(database* &dragao, int &tamanho) {
    database* temp = new database[tamanho + 10];
    for (int i = 0; i < tamanho; i++) temp[i] = dragao[i];
    delete[] dragao;
    dragao = temp;
    tamanho += 10;
}

database* lerValores(ifstream &dados, int &capacidadeVetor, int &qntDados) {
    qntDados = 0;
    char lixo = ' ';
    string linha = " ";

    dados.clear();
    dados.seekg(0);
    getline(dados, linha);

    database* dragao = new database[capacidadeVetor];

    while (dados >> dragao[qntDados].id) {
        dados >> lixo >> lixo;
        getline(dados, dragao[qntDados].nome, '"');
        dados >> lixo;
        getline(dados, dragao[qntDados].tipo, ',');
        dados >> dragao[qntDados].nivel >> lixo;
        dados >> dragao[qntDados].vida >> lixo;
        dados >> dragao[qntDados].ataque >> lixo;
        dados >> dragao[qntDados].chanceCritico >> lixo >> lixo;
        getline(dados, dragao[qntDados].habEspecial, '"');
        dados.ignore();

        qntDados += 1;
        if (capacidadeVetor == qntDados) redimensionarVetor(dragao, capacidadeVetor);
    }
    return dragao;
}

int buscaBinariaID(database* dragao, const int inicio, const int fim, const int k) {
    const int meio = (inicio + fim) / 2;

    if(inicio <= fim){
        if (dragao[meio].id == k) {
            return meio;
        }
        if (dragao[meio].id < k) {
            return buscaBinariaID(dragao, meio + 1, fim, k);
        }
        return buscaBinariaID(dragao, inicio, meio - 1, k);
    }
    return -1;
}

int buscaBinariaNome(database* dragao, const int inicio, const int fim, string &k) {
    const int meio = (inicio + fim) / 2;

    if(inicio <= fim){
        if (caseSensitive(dragao[meio].nome) == caseSensitive(k)) {
            return meio;
        }
        if (caseSensitive(dragao[meio].nome) < caseSensitive(k)) {
            return buscaBinariaNome(dragao, meio + 1, fim, k);
        }
        return buscaBinariaNome(dragao, inicio, meio - 1, k);
    }
    return -1;
}

int maiorID(const database* dragao, const int quantidadeDragoes) {
    int maior = 0;
    for (int i = 0; i < quantidadeDragoes; i++) {
        if (dragao[i].id > maior) maior = dragao[i].id;
    }
    return maior;
}

void inserirDragao(database* &dragao, int quantidade, int &capacidadeVetor, int &quantidadeDragoes) {
    while (quantidade > 0) {
        if (quantidadeDragoes == capacidadeVetor) redimensionarVetor(dragao, capacidadeVetor);

        int const i = quantidadeDragoes;
        dragao[i].id = maiorID(dragao, quantidadeDragoes) + 1;

        cout << "Escreva o nome do dragao: " << endl;
        getline(cin >> ws, dragao[i].nome);

        cout << "Escreva o tipo do dragao: " << endl;
        dragao[i].tipo = trataEspacos();
        cout << "Escreva o nivel do dragao: " << endl;
        dragao[i].nivel = trataEntradaInt();
        cout << "Escreva a quantidade de vida do dragao: " << endl;
        dragao[i].vida = trataEntradaInt();
        cout << "Escreva os pontos de ataque do dragao: " << endl;
        dragao[i].ataque = trataEntradaInt();
        cout << "Escreva a chance de ataque critico do dragao: " << endl;
        dragao[i].chanceCritico = trataEntradaFloat();

        cout << "Escreva a habilidade especial do dragao: " << endl;
        getline(cin >> ws, dragao[i].habEspecial);

        quantidadeDragoes += 1;
        quantidade --;
    }
}

bool comparaEntradaQuickSort(const database* dragao, const int fim, const int j, const int entrada) {
    switch (entrada) {
        case 0:
            return dragao[j].id < dragao[fim].id;
        case 1:
            return caseSensitive(dragao[j].nome) < caseSensitive(dragao[fim].nome);
        case 2:
            return caseSensitive(dragao[j].tipo) < caseSensitive(dragao[fim].tipo);
        default:
            return false;
    }
}

int partition(database* dragao, const int inicio, const int fim, const int entrada) {
    int i = inicio;

    for (int j = inicio; j < fim; j++) {
        if (comparaEntradaQuickSort(dragao, fim, j, entrada)) {
            const database aux = dragao[i];
            dragao[i] = dragao[j];
            dragao[j] = aux;
            i++;
        }
    }
    const database aux = dragao[i];
    dragao[i] = dragao[fim];
    dragao[fim] = aux;

    return i;
}

void quickSort(database* dragao, const int inicio, const int fim, const int entrada) {
    if (inicio < fim) {
        const int p = partition(dragao, inicio, fim, entrada);
        quickSort(dragao, inicio, p - 1, entrada);
        quickSort(dragao, p + 1, fim, entrada);
    }
}

void salvarMudancas(const database* dragao, const int tamanho) {
    ofstream dados("dragoes.csv");

    dados << "ID,Nome do Dragao,Tipo,Nivel atual,Vida (HP),Ataque (dano base),"
             "Chance Ataque Critico,Habilidade Especial" << endl;

    for (int i = 0; i < tamanho; i++) {
        if (dragao[i].id > 0) {
            dados << dragao[i].id << ','
            << '"' << dragao[i].nome << '"' << ','
            << dragao[i].tipo << ','
            << dragao[i].nivel << ','
            << dragao[i].vida << ','
            << dragao[i].ataque << ','
            << dragao[i].chanceCritico << ','
            << '"' << dragao[i].habEspecial << '"' << endl;
        }
    }
    dados.close();
}

bool verificaSalvar(const database* dragao, const int tamanhoVetor, const string &s1, const string &s2 = "") {
    bool salvar = false;

    cout << s1 << s2 << "? (Digite 0 para NAO ou 1 para SIM)" << endl;
    cin >> salvar;

    if (salvar) {
        salvarMudancas(dragao, tamanhoVetor);
        cout << "Salvo com sucesso!" << endl;
    }else {
        cout << "Operacao cancelada." << endl;
    }
    return salvar;
}

void verificaArquivo(const ifstream &dados, int &entrada) {
    if (!dados.is_open()) {
        cout << "DEU RUIM, NAO CARREGOU O CSV!";
        entrada = 9;
    }
}

void removerDragao(database* &dragao, const int tamanhoVetor, const int id) {
    int i = 0;
    bool encontrou = false;
    while (i < tamanhoVetor && id > 0) {
        if (dragao[i].id == id) {
            dragao[i].id *= -1;
            cout << "Dragao " << id << " adicionado para a fila de exclusao." << endl;
            encontrou = true;
        }
        i++;
    }
    if (!encontrou) cout << "Dragao nao encontrado no banco de dados." << endl;
}

void cancelarRemocao(database* dragao, const int tamanhoVetor) {
    for (int i = 0; i < tamanhoVetor; i++) {
        if (dragao[i].id == -1) dragao[i].id *= -1;
    }
}

void escreveVetor(const database* dragao, const int tamanhoVetor, const int inicio = 0) {
    for (int i = inicio; i < tamanhoVetor; i++) {
        if (dragao[i].id > 0) {
            cout << dragao[i].id << ' '
            << dragao[i].nome << ' '
            << dragao[i].tipo << ' '
            << dragao[i].nivel << ' '
            << dragao[i].vida << ' '
            << dragao[i].ataque << ' '
            << dragao[i].chanceCritico << ' '
            << dragao[i].habEspecial << endl;
        }
    }
}

void escreveParteVetor(const database* dragao, const int inicio, const int fim, const int qntDragoes) {
    if (inicio > 0 && fim > 0 && inicio <= fim && fim <= qntDragoes) {
        cout << "Lista no intervalo " << inicio << " ate " << fim << endl;
        cout << "---------------------------------------------" << endl;
        escreveVetor(dragao, fim, inicio - 1);
    }else {
        cout << "Intervalo invalido." << endl;
    }
}

void escreverOrdenado(database* dragao, const int tamanhoVetor, const string &s, const int opcao) {
    cout << endl << "Lista ordenada por " << s << ':' << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, tamanhoVetor - 1, opcao);
    escreveVetor(dragao, tamanhoVetor);
    cout << endl;
    verificaSalvar(dragao, tamanhoVetor, "Salvar ordenado por ", s);
}

void escreverMenu() {
    cout << "                    $$$                                                 " << endl;
    cout << "                   $   $                                                " << endl;
    cout << "                    $$$                                                 " << endl;
    cout << "                    $ $                                                 " << endl;
    cout << "                    $ $                                                 " << endl;
    cout << "                  $$$ $$$                                               " << endl;
    cout << "                $$  $$$  $$$                                            " << endl;
    cout << "              $$  $$$$$$$   $                                           " << endl;
    cout << "             $               $                                          " << endl;
    cout << "            $                 $      ===== MENU DRAGON CITY DEX =====   " << endl;
    cout << "            $                 $      0. Escrever arquivo                " << endl;
    cout << "            $     $$$$$$$$$$$$$$$    1. Procurar dragao por ID          " << endl;
    cout << "            $    $               $   2. Procurar dragao por Nome        " << endl;
    cout << "            $    $   $$$$$$$$$$$$$   3. Ordenar por ID                  " << endl;
    cout << "            $   $   $           $$$  4. Ordenar por Nome                " << endl;
    cout << "            $   $   $ $$$   $$$  $$  5. Ordenar por Tipo                " << endl;
    cout << "            $   $   $ $$$   $$$  $$  6. Remover dragao                  " << endl;
    cout << "            $   $   $           $$$  7. Listar intervalo X -> Y         " << endl;
    cout << "            $    $   $$$$$$$$$$$$$   8. Inserir dragao                  " << endl;
    cout << "            $     $$$$$$$$$$$$$$     9. Sair                            " << endl;
    cout << "            $                 $      ================================   " << endl;
    cout << "            $    $$$$$$$$$$$$$$                                         " << endl;
    cout << "            $   $  $  $  $  $                                           " << endl;
    cout << "            $  $$$$$$$$$$$$$$                                           " << endl;
    cout << "            $  $   $  $  $  $                                           " << endl;
    cout << "            $   $$$$$$$$$$$$$$$                                         " << endl;
    cout << "           $$$                 $$$                                      " << endl;
    cout << "         $$   $$$         $$$$$   $$                                    " << endl;
    cout << "       $$        $$$$$$$$$          $$$                                 " << endl;
    cout << "      $  $$                     $$$$   $$                               " << endl;
    cout << "   $$$$$   $$$$$$$$      $$$$$$$       $ $                              " << endl << endl;
}

void aperteEnter() {
    cout << endl << "Pressione ENTER...";
    cin.ignore();
    cin.get();
}

int main() {
    ifstream dados("dragoes.csv");
    int tamanhoVetor = 40;
    int entrada = 0;
    int dragaoID = 0;
    int inicioIntervalo = 0;
    int fimIntervalo = 0;
    int qntRemover = 0;
    int quantidadeInserir = 0;
    int quantidadeDragoes = 0;
    int posEncontrado = 0;
    string dragaoNome;

    verificaArquivo(dados, entrada);
    database* dragao = lerValores(dados, tamanhoVetor, quantidadeDragoes);

    while (entrada != 9) {
        escreverMenu();
        cin >> entrada;

        switch (entrada) {
            case 0:
                delete[] dragao;
                dragao = lerValores(dados, tamanhoVetor, quantidadeDragoes);
                cout << endl << "Arquivo:" << endl;
                cout << "---------------------------------------------" << endl;
                escreveVetor(dragao, tamanhoVetor);
                aperteEnter();
                break;
            case 1:
                quickSort(dragao, 0, tamanhoVetor - 1, 0);
                cout << "Digite o ID do Dragao que deseja procurar:" << endl;
                dragaoID = trataEntradaInt();
                if (dragaoID <= 0) {
                    posEncontrado = -1;
                }else {
                    posEncontrado = buscaBinariaID(dragao, 0, tamanhoVetor - 1, dragaoID);
                }
                if (posEncontrado < 0) {
                    cout << "Dragao de ID " << dragaoID << " -> " << "Dragao nao registrado" << endl;
                }else {
                    cout << "Dragao de ID " << dragaoID << " -> " << endl
                    << "Nome: " << dragao[posEncontrado].nome << endl
                    << "Tipo: " << dragao[posEncontrado].tipo << endl
                    << "Nivel: " << dragao[posEncontrado].nivel << endl
                    << "Vida: " << dragao[posEncontrado].vida << endl
                    << "Ataque: " << dragao[posEncontrado].ataque << endl
                    << "Chanc. Critico: " << dragao[posEncontrado].chanceCritico << endl
                    << "Hab. Especial: " << dragao[posEncontrado].habEspecial << endl;
                }
                aperteEnter();
                break;
            case 2:
                quickSort(dragao, 0, tamanhoVetor - 1, 1);
                cout << "Digite o nome do Dragao que deseja procurar:" << endl;
                getline(cin >> ws, dragaoNome);
                posEncontrado = buscaBinariaNome(dragao, 0, tamanhoVetor - 1, dragaoNome);
                if (posEncontrado > 0) {
                    cout << "Dragao encontrado -> " << endl
                        << "Nome: " << dragao[posEncontrado].nome << endl
                        << "Tipo: " << dragao[posEncontrado].tipo << endl
                        << "Nivel: " << dragao[posEncontrado].nivel << endl
                        << "Vida: " << dragao[posEncontrado].vida << endl
                        << "Ataque: " << dragao[posEncontrado].ataque << endl
                        << "Chanc. Critico: " << dragao[posEncontrado].chanceCritico << endl
                        << "Hab. Especial: " << dragao[posEncontrado].habEspecial << endl;
                }else {
                    cout << "Dragao nao encontrado, verifique a ortografia." << endl;
                }
                aperteEnter();
                break;
            case 3:
                escreverOrdenado(dragao, tamanhoVetor, "ID", 0);
                aperteEnter();
                break;
            case 4:
                escreverOrdenado(dragao, tamanhoVetor, "nome", 1);
                aperteEnter();
                break;
            case 5:
                escreverOrdenado(dragao, tamanhoVetor, "tipo", 2);
                aperteEnter();
                break;
            case 6:
                cout << "Quantos dragoes deseja remover?" << endl;
                qntRemover = trataEntradaInt();
                if (qntRemover < 1) {
                    cout << "Quantidade invalida." << endl;
                }else {
                    cout << "Digite o(s) ID do(s) dragao(oes) que deseja remover:" << endl;
                    while (qntRemover > 0) {
                        dragaoID = trataEntradaInt();
                        removerDragao(dragao, tamanhoVetor, dragaoID);
                        qntRemover -= 1;
                    }
                    cout << endl;
                    if (!verificaSalvar(dragao, tamanhoVetor, "Concluir remocao")) cancelarRemocao(dragao, tamanhoVetor);
                }
                aperteEnter();
                break;
            case 7:
                delete[] dragao;
                dragao = lerValores(dados, tamanhoVetor, quantidadeDragoes);
                cout << "Digite o inicio do intervalo:" << endl;
                inicioIntervalo = trataEntradaInt();
                cout << "Digite o fim do intervalo:" << endl;
                fimIntervalo = trataEntradaInt();
                escreveParteVetor(dragao, inicioIntervalo, fimIntervalo, quantidadeDragoes);
                aperteEnter();
                break;
            case 8:
                delete[] dragao;
                dragao = lerValores(dados, tamanhoVetor, quantidadeDragoes);
                cout << "Quantos dragoes quer inserir?" << endl;
                quantidadeInserir = trataEntradaInt();
                if (quantidadeInserir <= 0) {
                    cout << "Quantidade invalida." << endl;
                }else {
                    inserirDragao(dragao, quantidadeInserir, tamanhoVetor, quantidadeDragoes);
                    verificaSalvar(dragao, tamanhoVetor, "Salvar");
                }
                aperteEnter();
                break;
            default:
                cout << "Obrigado por utilizar!";
        }
    }
    dados.close();
    delete[] dragao;
    return 0;
}