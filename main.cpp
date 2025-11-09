#include <iostream>
#include <fstream>

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

bool comparaEntrada(const database* dragao, const int fim, const int j, const int entrada){
    switch (entrada) {
        case 0:
            return caseSensitive(dragao[j].tipo) < caseSensitive(dragao[fim].tipo);
        case 1:
            return caseSensitive(dragao[j].nome) < caseSensitive(dragao[fim].nome);
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

void salvarMudancas(const database* dragao, const int tamanho, const string &nomeDocumento) {
    ofstream documento(nomeDocumento);

    for (int i = 0; i < tamanho; i++) {
        documento << dragao[i].id << ','
        << '"' << dragao[i].nome << '"' << ','
        << dragao[i].tipo << ','
        << dragao[i].nivel << ','
        << dragao[i].vida << ','
        << dragao[i].ataque << ','
        << dragao[i].chanceCritico << ','
        << '"' << dragao[i].habEspecial << '"' << endl;
    }
}

void escreveVetor(const database* dragao, const int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        cout << dragao[i].nome << " "
        << dragao[i].tipo << " "
        << dragao[i].nivel << " "
        << dragao[i].vida << " "
        << dragao[i].ataque << " "
        << dragao[i].chanceCritico << " "
        << dragao[i].habEspecial << endl;
    }
}

int main() {
    string linha;
    string quantidade;
    ifstream dados("dragoes.csv");

    getline(dados, linha);
    getline(dados, quantidade);
    const int numDados = stoi(quantidade);

    database* dragao = lerValores(dados, numDados);

    int dragID;
    cin >> dragID;

    if (dragID > 0 && dragID <= numDados) {
        cout << "Dragao de ID " << dragID << " -> "
        << dragao[buscaBinaria(dragao, 0, numDados - 1, dragID)].nome << endl;
    }else {
        cout << "Dragao de ID " << dragID << " -> " << "Dragao nao registrado" << endl;
    }

    cout << endl << "Lista ordenada por " << "tipo: " << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, numDados - 1, 0);
    escreveVetor(dragao, numDados);

    cout << endl << "Lista ordenada por " << "nome: " << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, numDados - 1, 1);
    escreveVetor(dragao, numDados);

    salvarMudancas(dragao, numDados, "teste.txt");

    return 0;
}