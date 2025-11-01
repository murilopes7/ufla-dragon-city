#include <fstream>
#include <iostream>
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
// FUNÇÃO DE CONVERSÃO DE STRING PARA INT
// ====================================================================
int string_para_int(const string& s) {
	int num = 0;
	bool negativo = false;
	unsigned int i = 0;
	if (!s.empty() && s[0] == '-') {
		negativo = true;
		i = 1;
	}

	while (i < s.size()) {
		char c = s[i];
		if (c >= '0' && c <= '9') {
			num = num * 10 + (c - '0');
		}

		i++;
	}

	if (negativo) num = -num;
	return num;
}

// ====================================================================
// FUNÇÃO DE CONVERSÃO DE STRING PARA FLOAT (com ponto '.')
// ====================================================================
float string_para_float(const string& s) {
	float num = 0;
	bool negativo = false;
	unsigned int i = 0;
	bool decimal = false;
	float fator = 0.1;

	if (!s.empty() && s[0] == '-') {
		negativo = true;
		i = 1;
	}

	for (; i < s.size(); i++) {
		char c = s[i];
		if (c == '.') {
			decimal = true;
		} else if (c >= '0' && c <= '9') {
			if (!decimal) {
				num = num * 10 + (c - '0');
			} else {
				num += (c - '0') * fator;
				fator /= 10;
			}
		}
	}

	if (negativo) num = -num;
	return num;
}

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

	// ler e descartar header (se houver)
	if (!getline(arquivo, linha)) {
		cerr << "Arquivo vazio ou erro de leitura: " << arquivo_path << "\n";
		return;
	}

	// Loop Principal
	while (getline(arquivo, linha)) {
		if (!linha.empty()) {
			if (tamanho == capacidade) {
				redimensionar_vetor(dragoes, tamanho, capacidade);
			}

			Dragon novo_dragao;
			bool valido = true;

			// Encontrando a vírgula
			unsigned int pos = linha.find(',');

			// Verificação da vírgula
			if (pos == std::string::npos) {
				cerr << "Linha " << linha << " inválida. ";
				valido = false;
			}

			string campos[7];
			int campos_atual = 0;
			string atual = "";
			if (valido) {
				for (unsigned int i = 0; i < linha.size(); i = i + 1) {
					if (linha[i] == ',') {
						if (campo_atual < 7) {
							campos[campo_atual] = atual;
							campo_atual++;
							atual = "";
						}
					}

					else
						atual += linha[i];
				}

				if (campo_atual < 7) {
					campos[campo_atual] = atual;  // último campo
					campo_atual++;
				}

				if (campo_atual < 7) {
					linha_valida = false;
					cerr << "Linha inválida: " << linha << "\n";
				}

				if (linha_valida) {
					dragoes[tamanho].nome = campos[0];
					dragoes[tamanho].tipo = campos[1];
					dragoes[tamanho].nivel = string_para_int(campos[2]);
					dragoes[tamanho].vida = string_para_int(campos[3]);
					dragoes[tamanho].ataque = string_para_int(campos[4]);
					dragoes[tamanho].chance_critico =
						string_para_float(campos[5]);
					dragoes[tamanho].habilidade = campos[6];
					tamanho++;
				}
			}
		}
	}
}

int main() {
	int tamanho = 0, capacidade = 40;
	Dragon* dragao = new Dragon[capacidade];
	return 0;
}
