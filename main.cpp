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

int buscaBinaria(database* dragao, const int inicio, const int fim, const int k) {
    const int meio = (inicio + fim) / 2;

    if(inicio <= fim){
        if (dragao[meio].id == k) {
            return meio;
        }
        if (dragao[meio].id < k) {
            return buscaBinaria(dragao, meio + 1, fim, k);
        }
        return buscaBinaria(dragao, inicio, meio - 1, k);
    }
    return -1;
}

database* lerValores(ifstream &dados, const int tamanho) {
    char lixo;
    database* dragao = new database[tamanho];

    for (int i = 0; i < tamanho; i++) {
        dados >> dragao[i].id >> lixo >> lixo;
        getline(dados, dragao[i].nome, '"');
        dados >> lixo;
        getline(dados, dragao[i].tipo, ',');
        dados >> dragao[i].nivel >> lixo;
        dados >> dragao[i].vida >> lixo;
        dados >> dragao[i].ataque >> lixo;
        dados >> dragao[i].chanceCritico >> lixo >> lixo;
        getline(dados, dragao[i].habEspecial, '"');
        dados.ignore();
    }
    return dragao;
}

string caseSensitive(const string &s) {
    string temp;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') temp += static_cast<char>(tolower(s[i]));
        temp += s[i];
    }
    return temp;
}

bool comparaEntrada(const database* dragao, const int fim, const int j, const int entrada) {
    switch (entrada) {
        case 0:
            return dragao[j].id < dragao[fim].id;
        case 1:
            return caseSensitive(dragao[j].nome) < caseSensitive(dragao[fim].nome);
        case 2:
            return caseSensitive(dragao[j].tipo) < caseSensitive(dragao[fim].tipo);
        case 3:
            return dragao[j].nivel < dragao[fim].nivel;
        case 4:
            return dragao[j].vida < dragao[fim].vida;
        case 5:
            return dragao[j].ataque < dragao[fim].ataque;
        case 6:
            return dragao[j].chanceCritico < dragao[fim].chanceCritico;
        case 7:
            return caseSensitive(dragao[j].habEspecial) < caseSensitive(dragao[fim].habEspecial);
        default:
            return false;
    }
}

int partition(database* dragao, const int inicio, const int fim, const int entrada) {
    int i = inicio;

    for (int j = inicio; j < fim; j++) {
        if (comparaEntrada(dragao, fim, j, entrada)) {
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
    dados << tamanho << endl;

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

void removerDragao(database* dragao, const int numDados, const int id) {
    if (id > numDados) {
        cout << "Dragao nao encontrado no banco de dados." << endl;
    }else{
        int posicao = 0;
        while (dragao[posicao].id != id) {
            posicao += 1;
        }
        dragao[posicao].id *= -1;
        cout << "Dragao " << id << " adicionado para a fila de exclusao." << endl;
    }
}

void cancelarRemocao(database* dragao, const int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (dragao[i].id == -1) dragao[i].id *= -1;
    }
}

void escreveVetor(const database* dragao, const int tamanho, const int inicio = 0) {
    for (int i = inicio; i < tamanho; i++) {
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

void escreveParteVetor(const database* dragao, const int inicio, const int fim) {
    if (inicio > -1 && fim > 0 && inicio <= fim) {
        cout << endl << "Lista no intervalo " << inicio << " ate " << fim << endl;
        cout << "---------------------------------------------" << endl;
        escreveVetor(dragao, fim, inicio - 1);
    }else {
        cout << "Intervalo invalido." << endl;
    }
}

bool verificaSalvar(const database *dragao, const int tamanho, const string &s1, const string &s2 = "") {
    bool salvar = false;

    cout << endl << s1 << s2 << "? (Digite 0 para NAO ou 1 para SIM)" << endl;
    cin >> salvar;

    if (salvar) {
        salvarMudancas(dragao, tamanho);
        cout << "Salvo com sucesso!" << endl << endl;
    }else {
        cout << "Operacao cancelada." << endl << endl;
    }
    return salvar;
}

void escreverESalvar(database *dragao, const int tamanho, const string &s, const int opcao) {
    cout << endl << "Lista ordenada por " << s << ':' << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, tamanho - 1, opcao);
    escreveVetor(dragao, tamanho);
    verificaSalvar(dragao, tamanho, "Salvar ordenado por ", s);
}

int main() {
    string linha;
    string quantidade;
    ifstream dados("dragoes.csv");

    getline(dados, linha);
    getline(dados, quantidade);
    const int numDados = stoi(quantidade);

    database* dragao = lerValores(dados, numDados);

    int entrada = 0;
    int dragID = 0;
    int inicioIntervalo = 0;
    int fimIntervalo = 0;
    int qntRemover = 0;

    while (entrada != 9) {
        cout << "===== MENU DRAGON CITY DEX =====" << endl
         << "0. Escrever vetor" << endl
         << "1. Procurar dragao por ID" << endl
         << "2. Ordenar por ID" << endl
         << "3. Ordenar por Nome" << endl
         << "4. Ordenar por Tipo" << endl
         << "5. Remover dragao" << endl
         << "6. Listar intervalo X -> Y" << endl
         << "9. Sair" << endl
         << "================================" << endl;

        cin >> entrada;

        switch (entrada) {
            case 0:
                cout << endl << "Vetor:" << endl;
                cout << "---------------------------------------------" << endl;
                escreveVetor(dragao, numDados);
                cout << endl;
                break;
            case 1:
                cout << "Digite o ID do Dragao que deseja procurar:" << endl;
                cin >> dragID;
                if (dragID > 0 && dragID <= numDados) {
                    cout << "Dragao de ID " << dragID << " -> "
                    << dragao[buscaBinaria(dragao, 0, numDados - 1, dragID)].nome << endl;
                }else {
                    cout << "Dragao de ID " << dragID << " -> " << "Dragao nao registrado" << endl;
                }
                cout << endl;
                break;
            case 2:
                escreverESalvar(dragao, numDados, "ID", 0);
                break;
            case 3:
                escreverESalvar(dragao, numDados, "nome", 1);
                break;
            case 4:
                escreverESalvar(dragao, numDados, "tipo", 2);
                break;
            case 5:
                cout << "Quantos dragoes deseja remover?" << endl;
                cin >> qntRemover;
                if (qntRemover > 0) {
                    cout << "Digite o(s) dragao(oes) que deseja remover:" << endl;
                    while (qntRemover > 0) {
                        cin >> dragID;
                        removerDragao(dragao, numDados, dragID);
                        qntRemover -= 1;
                    }
                    if (!verificaSalvar(dragao, numDados, "Concluir remocao")) cancelarRemocao(dragao, numDados);
                }else {
                    cout << "Quantidade invalida." << endl;
                }
                break;
            case 6:
                cout << "Digite o inicio do intervalo:" << endl;
                cin >> inicioIntervalo;
                cout << "Digite o fim do intervalo:" << endl;
                cin >> fimIntervalo;
                escreveParteVetor(dragao, inicioIntervalo, fimIntervalo);
                cout << endl;
                break;
            default:
                cout << "Obrigado por utilizar!";
        }
    }
    dados.close();
    delete[] dragao;
    return 0;
}