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

void salvarMudancas(const database* dragao, const int tamanho, const string &nomeDocumento) {
    ofstream documento(nomeDocumento);

    for (int i = 0; i < tamanho; i++) {
        if (dragao[i].id > 0) {
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
}

void removerDragao(database* dragao, const int numDados, const int id) {
    if (id > numDados) {
        cout << "Dragao nao encontrado no banco de dados.";
    }else{
        int posicao = 0;
        while (dragao[posicao].id != id) {
            posicao += 1;
        }
        dragao[posicao].id *= -1;
        cout << endl << "Dragao " << id << " adicionado para a fila de exclusao.";
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
    if (inicio > -1 && fim > 0) {
        escreveVetor(dragao, fim, inicio - 1);
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

    cout << endl << "Lista ordenada por " << "id:" << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, numDados - 1, 0);
    escreveVetor(dragao, numDados);

    cout << endl << "Lista ordenada por " << "nome:" << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, numDados - 1, 1);
    escreveVetor(dragao, numDados);

    cout << endl << "Lista ordenada por " << "tipo:" << endl;
    cout << "---------------------------------------------" << endl;
    quickSort(dragao, 0, numDados - 1, 2);
    escreveVetor(dragao, numDados);

    removerDragao(dragao, numDados - 1, 2);
    removerDragao(dragao, numDados - 1, 3);
    removerDragao(dragao, numDados - 1, 4);

    cout << endl << endl << "Salvar remocoes?" << endl;
    bool salvar = false;
    cin >> salvar;

    if (salvar) salvarMudancas(dragao, numDados, "teste.txt");
    else cancelarRemocao(dragao, numDados - 1);

    cout << endl << "Lista com dragoes para exclusao:" << endl;
    cout << "---------------------------------------------" << endl;
    escreveVetor(dragao, numDados);

    cout << endl << "Lista no intervalo 2 a 8:" << endl;
    cout << "---------------------------------------------" << endl;
    escreveParteVetor(dragao, 2, 8);

    return 0;
}