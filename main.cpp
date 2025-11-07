#include <notcurses/notcurses.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

struct Dragon {
	int id{};
	string nome{};

	string tipo{};

	int nivel{};
	int vida{};
	int ataque{};

	float chance_critico{};

	string habilidade_critico{};
	bool removido_logicamente = false;
};

int buscaBinaria(Dragon* dragao, int key, int inicio, int fim) {
	if (inicio > fim) return -1;

	int meio = inicio + (fim - inicio) / 2;

	if (dragao[meio].id == key) return meio;

	if (dragao[meio].id < key)
		return buscaBinaria(dragao, key, meio + 1, fim);
	else
		return buscaBinaria(dragao, key, inicio, meio - 1);
}

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
	int nova_capacidade = capacidade + incremento;
	Dragon* novo_dragao = new Dragon[nova_capacidade];

	for (int i = 0; i < capacidade; i++) novo_dragao[i] = dragoes[i];

	delete[] dragoes;

	dragoes = novo_dragao;
	capacidade = nova_capacidade;
}

// ====================================================================
// FUNÇÃO DE IMPRESSÃO DOS DADOS DO ARQUIVO
// ====================================================================

void imprimirVetor(Dragon* v, int n) {
	cout << left << setw(4) << "ID" << setw(25) << "Nome" << setw(12) << "Tipo"
		 << setw(8) << "Nível" << setw(12) << "Vida" << setw(11) << "Poder"
		 << setw(12) << "Crít" << setw(8) << "Habilidade Especial" << "\n";

	for (int i = 0; i < n; i++) {
		cout << left << setw(4) << v[i].id << setw(25) << v[i].nome << setw(12)
			 << v[i].tipo << setw(8) << v[i].nivel << setw(12) << v[i].vida
			 << setw(11) << v[i].ataque << setw(12) << fixed << setprecision(2)
			 << v[i].chance_critico << setw(8) << v[i].habilidade_critico
			 << "\n";
	}
}

// ====================================================================
// PROCEDIMENTO DE LIMPEZA DO TERMINAL
// ====================================================================

void limparEntrada() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ====================================================================
// FUNÇÃO DE MENU PRINCIPAL UTILIZANDO NOTCURSES
// ====================================================================

int menuNotcurses(struct notcurses* nc) {
	ncplane* stdplane = notcurses_stdplane(nc);

	const char* opcoes[] = {"Mostrar todos os dragões",
							"Inserir novo dragão",
							"Remover dragão",
							"Buscar dragão",
							"Ordenar dragões",
							"Salvar alterações",
							"Sair"};

	const int total = sizeof(opcoes) / sizeof(opcoes[0]);

	int selecionado = 0;
	bool rodando = true;

	while (rodando) {
		ncplane_erase(stdplane);

		ncplane_set_fg_rgb8(stdplane, 255, 0, 255);
		ncplane_printf_yx(stdplane, 1, 2,
						  "╔════════════════════════════════════════╗");
		ncplane_printf_yx(stdplane, 4 + total, 2,
						  "╚════════════════════════════════════════╝");

		ncplane_set_fg_rgb8(stdplane, 255, 255, 0);
		ncplane_printf_yx(stdplane, 2, 6, "      MENU DRAGON CITY      ");

		for (int i = 0; i < total; i++) {
			if (i == selecionado) {
				ncplane_set_fg_rgb8(stdplane, 0, 255, 0);
				ncplane_printf_yx(stdplane, 4 + i, 4, "► %s", opcoes[i]);
			} else {
				ncplane_set_fg_rgb8(stdplane, 255, 255, 255);
				ncplane_printf_yx(stdplane, 4 + i, 6, "%s", opcoes[i]);
			}
		}

		notcurses_render(nc);

		ncinput ni;
		uint32_t key = notcurses_get(nc, NULL, &ni);

		bool valido = true;
		if (key == (uint32_t)-1) valido = false;
		if (ni.evtype == NCTYPE_RELEASE) valido = false;

		if (valido) {
			if (ni.id == NCKEY_UP) {
				selecionado = (selecionado - 1 + total) % total;
			} else {
				if (ni.id == NCKEY_DOWN) {
					selecionado = (selecionado + 1) % total;
				} else {
					if (ni.id == NCKEY_ENTER || ni.id == '\n') {
						rodando = false;
					}
				}
			}
		}
	}

	return selecionado;
}

// ====================================================================
// FUNÇÃO DE CARREGAMENTO (Combina CSV Parsing e Redimensionamento)
// ====================================================================

void carregar_dados_csv(Dragon*& dragoes, int& tamanho, int& capacidade,
						ifstream& nome_arquivo) {
	if (dragoes == nullptr) {
		capacidade = 40;
		dragoes = new Dragon[capacidade];
		tamanho = 0;
	}

	string linha;

	if (!getline(nome_arquivo, linha))
		cerr << "Arquivo vazio ou erro de leitura.";

	while (getline(nome_arquivo, linha)) {
		if (!linha.empty()) {
			if (tamanho == capacidade) {
				redimensionar_vetor(dragoes, tamanho, capacidade);
			}

			Dragon novo_dragao;
			string campos[7];

			// ✅ NEW PARSER HERE (no substr, no npos, uses erase)
			for (int c = 0; c < 7; c++) {
				campos[c] = "";

				if (!linha.empty() && linha[0] == '"') {
					linha.erase(0, 1);	// remove a primeira aspa "
					unsigned int pos = 0;
					while (pos < linha.size() && linha[pos] != '"') pos++;

					campos[c] = "";
					for (unsigned int k = 0; k < pos; k++)
						campos[c] += linha[k];

					if (pos < linha.size()) {
						linha.erase(0, pos + 2);
					} else {
						linha = "";
					}
				} else {
					unsigned int pos = 0;
					while (pos < linha.size() && linha[pos] != ',') pos++;

					campos[c] = "";
					for (unsigned int k = 0; k < pos; k++)
						campos[c] += linha[k];

					if (pos < linha.size())
						linha.erase(0, pos + 1);
					else
						linha = "";
				}
			}

			novo_dragao.id = tamanho + 1;
			novo_dragao.nome = campos[0];
			novo_dragao.tipo = campos[1];
			novo_dragao.nivel = string_para_int(campos[2]);
			novo_dragao.vida = string_para_int(campos[3]);
			novo_dragao.ataque = string_para_int(campos[4]);
			novo_dragao.chance_critico = string_para_float(campos[5]);
			novo_dragao.habilidade_critico = campos[6];

			dragoes[tamanho] = novo_dragao;
			tamanho++;
		}
	}
}

int main() {
	int tamanho = 0, capacidade = 40;
	Dragon* dragoes = new Dragon[capacidade];

	ifstream nome_arquivo("./dragon_city_60_dragons_real_names.csv");
	if (!nome_arquivo.is_open()) {
		cerr << "Erro: Não foi possível carregar o arquivo.";
		return 1;
	}

	carregar_dados_csv(dragoes, tamanho, capacidade, nome_arquivo);

	notcurses_options opts = {};
	opts.flags = NCOPTION_SUPPRESS_BANNERS;
	struct notcurses* nc = notcurses_init(&opts, nullptr);
	if (!nc) {
		cout << "Erro ao iniciar notcurses\n";
		return -1;
	}

	int opcao = menuNotcurses(nc);
	notcurses_stop(nc);
	switch (opcao) {
		case 0:
			imprimirVetor(dragoes, tamanho);
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			cout << "Aperte ENTER para sair!";
			limparEntrada();
			cout << "Saindo..." << endl;
			break;
	}

	nome_arquivo.close();
	delete[] dragoes;
	return 0;
}
