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
        dados >> dragao[i].id;
        dados >> lixo;
        dados >> lixo;
        getline(dados, dragao[i].nome, '"');
        dados >> lixo;
        getline(dados, dragao[i].tipo, ',');
        dados >> dragao[i].nivel;
        dados >> lixo;
        dados >> dragao[i].vida;
        dados >> lixo;
        dados >> dragao[i].ataque;
        dados >> lixo;
        dados >> dragao[i].chanceCritico;
        dados >> lixo;
        dados >> lixo;
        getline(dados, dragao[i].habEspecial, '"');
        dados.ignore();
    }
    return dragao;
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
        cout << dragao[buscaBinaria(dragao, 0, numDados - 1, dragID)].nome;
    }

    return 0;
}