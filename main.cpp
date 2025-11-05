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

int buscaBinaria(database dragao[], int inicio, int fim, int k) {
    int meio = (inicio + fim) / 2;

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

database* lerValores(ifstream &dados, int tamanho) {
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

int partition(database* dragao, int inicio, int fim) {
    string pivo = dragao[fim].nome;
    int i = inicio;

    for (int j = inicio; j < fim; j++) {
        if (dragao[j].nome < pivo) {
            string aux = dragao[i].nome;
            dragao[i].nome = dragao[j].nome;
            dragao[j].nome = aux;
            i++;
        }
    }
    string aux = dragao[i].nome;
    dragao[i].nome = dragao[fim].nome;
    dragao[fim].nome = aux;

    return i;
}

void quickSort(database* dragao, int inicio, int fim) {
    if (inicio < fim) {
        int p = partition(dragao, inicio, fim);
        quickSort(dragao, inicio, p - 1);
        quickSort(dragao, p + 1, fim);
    }
}

void escreveVetor(database* dragao, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        cout << dragao[i].nome << " "
        << dragao[i].tipo << " "
        << dragao[i].nivel << " "
        << dragao[i].vida << " "
        << dragao[i].ataque << " "
        << dragao[i].chanceCritico << endl;
    }
}

int main(){
    string linha;
    string quantidade;
    ifstream dados("dragoes.csv");

    getline(dados, linha);
    getline(dados, quantidade);
    int numDados = stoi(quantidade);

    database* dragao = lerValores(dados, numDados);

    int dragID;
    cin >> dragID;

    if (dragID > 0) {
        cout << " Dragao de ID " << dragID << " ->"
        << dragao[buscaBinaria(dragao, 0, numDados - 1, dragID)].nome << endl;
    }

    quickSort(dragao, 0, numDados - 1);
    escreveVetor(dragao, numDados);

    return 0;
}