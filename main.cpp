#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Dragon {
	int id;
	string nome;

	string tipo;

	int nivel;
	int vida;
	int ataque;

	float chance_critico;

	string habilidade_critico;
	bool removido_logicamente = false;
};

// ====================================================================
// FUNÇÃO DE REDIMENSIONAMENTO COM INCREMENTAÇÃO DE 10
// ====================================================================

void redimensionar_vetor(Dragon*& dragoes, int& tamanho, int& capacidade) {
	const int incremento = 10;
	int nova_capacidade = capacidade_inicial + incremento;
	Dragon* novo_dragao = new Dragon[nova_capacidade];

	for (int i = 0; i < tamanho; i++) novo_dragao[i] = dragoes[i];

	delete[] dragoes;

	dragoes = novo_dragao;
	capacidade_inicial = nova_capacidade;
}

// ====================================================================
// FUNÇÃO DE CARREGAMENTO (Combina CSV Parsing e Redimensionamento)
// ====================================================================

void carregar_dados_csv(Dragon*& dragoes, int& tamanho, int& capacidade,
						const string& nome_arquivo) {
	if (dragoes == nullptr) {
		capacidade = 40;
		dragoes = new Dragon[capacidade];
		tamanho = 0;
	}

	ifstream arquivo("~/Downloads/dragon_city_60_dragons_real_names.csv");
	if (!arquivo.is_open()) {
		cerr << "Erro: Não foi possível carregar o arquivo " << arquivo << endl;
		return;
	}

	string linha;
	getline(arquivo, linha);

	// Loop Principal
	while (getline(arquivo, linha)) {
		if (tamanho == capacidade) {
			redimensionar_vetor(dragoes, tamanho, capacidade);
		}

		Dragon novo_dragao;
		bool valido = true;

		// Encontrando a vírgula
		unsigned int pos = linha.find(',');

		// Verificação da vírgula
		if (pos == npos) {
			cerr << "Linha " << linha << " inválida. ";
			valido = false;
		}

		if (valido) {
			novo_dragao.nome = linha.substr(0, pos);
			string valor_str = linha.substr(pos + 1);
		}
	}
}

int main() {
	int tamanho = 0, capacidade = 40;
	Dragon* dragao = new Dragon[capacidade];
	return 0;
}
