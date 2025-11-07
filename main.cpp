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

int buscaBinaria(database dragao[], const int inicio, const int fim, const int k) {
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

bool comparaEntrada(const database* dragao, string& pivo, const int fim, const int j, const string& entrada){
    if (entrada == "tipo") {
        pivo = dragao[fim].tipo;
        if (dragao[j].tipo < pivo) {
            return true;
        }
        return false;
    }

    if (entrada == "nome") {
        pivo = dragao[fim].nome;
        if (dragao[j].nome < pivo) {
            return true;
        }
        return false;
    }
    return false;
}

int partition(database* dragao, const int inicio, const int fim, const string& entrada) {
    string pivo;
    int i = inicio;

    for (int j = inicio; j < fim; j++) {
        if (comparaEntrada(dragao, pivo, fim, j, entrada)) {
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

void quickSort(database* dragao, const int inicio, const int fim, const string& entrada) {
    if (inicio < fim) {
        const int p = partition(dragao, inicio, fim, entrada);
        quickSort(dragao, inicio, p - 1, entrada);
        quickSort(dragao, p + 1, fim, entrada);
    }
}

void escreveVetor(const database* dragao, const int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        cout << dragao[i].nome << " "
        << dragao[i].tipo << " "
        << dragao[i].nivel << " "
        << dragao[i].vida << " "
        << dragao[i].ataque << " "
        << dragao[i].chanceCritico << endl;
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
    quickSort(dragao, 0, numDados - 1, "tipo");
    escreveVetor(dragao, numDados);

    cout << endl << "Lista ordenada por " << "nome: " << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, numDados - 1, "nome");
    escreveVetor(dragao, numDados);

    return 0;
}