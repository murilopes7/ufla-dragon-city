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
// FUNÇÃO DE VERIFICAÇÃO E CONVERSÃO DE TIPOS
// ====================================================================

string caseSensitive(const string& s) {
	string temp;
	for (const char* p = s.c_str(); *p; ++p) {
		if (isupper(*p))
			temp += tolower(*p);
		else
			temp += *p;
	}

	return temp;
}

bool comparaEntrada(const Dragon* dragao, const int fim, const int j,
					const int entrada) {
	switch (entrada) {
		case 0:
			return dragao[j].id < dragao[fim].id;
		case 1:
			return caseSensitive(dragao[j].nome) <
				   caseSensitive(dragao[fim].nome);
		case 2:
			return caseSensitive(dragao[j].tipo) <
				   caseSensitive(dragao[fim].tipo);
		case 3:
			return dragao[j].nivel < dragao[fim].nivel;
		case 4:
			return dragao[j].vida < dragao[fim].vida;
		case 5:
			return dragao[j].ataque < dragao[fim].ataque;
		case 6:
			return dragao[j].chance_critico < dragao[fim].chance_critico;
		case 7:
			return caseSensitive(dragao[j].habilidade_critico) <
				   caseSensitive(dragao[fim].habilidade_critico);
		default:
			return false;
	}
}

// ====================================================================
// PROCEDIMENTO DE LIMPEZA DO TERMINAL
// ====================================================================

void limparEntrada() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ====================================================================
// FUNÇÕES DE BUSCA (BINARY SEARCH)
// ====================================================================

int buscaBinaria(Dragon* dragao, int key, int inicio, int fim) {
	if (inicio > fim) return -1;

	int meio = inicio + (fim - inicio) / 2;

	if (dragao[meio].id == key) return meio;

	if (dragao[meio].id < key)
		return buscaBinaria(dragao, key, meio + 1, fim);
	else
		return buscaBinaria(dragao, key, inicio, meio - 1);
}

int partition(Dragon* dragao, const int inicio, const int fim,
			  const int entrada) {
	int i = inicio;

	for (int j = inicio; j < fim; j++) {
		if (comparaEntrada(dragao, fim, j, entrada)) {
			const Dragon aux = dragao[i];
			dragao[i] = dragao[j];
			dragao[j] = aux;
			i++;
		}
	}
	const Dragon aux = dragao[i];
	dragao[i] = dragao[fim];
	dragao[fim] = aux;

	return i;
}

void quickSort(Dragon* dragao, const int inicio, const int fim,
			   const int entrada) {
	if (inicio < fim) {
		const int p = partition(dragao, inicio, fim, entrada);
		quickSort(dragao, inicio, p - 1, entrada);
		quickSort(dragao, p + 1, fim, entrada);
	}
}

// ====================================================================
// FUNÇÃO DE REDIMENSIONAMENTO COM INCREMENTAÇÃO DE 10
// ====================================================================

void redimensionar_vetor(Dragon*& dragoes, int& tamanho, int& capacidade) {
	const int incremento = 10;
	int nova_capacidade = capacidade + incremento;
	Dragon* novo_dragao = new Dragon[nova_capacidade];

	for (int i = 0; i < tamanho; i++) novo_dragao[i] = dragoes[i];

	delete[] dragoes;

	dragoes = novo_dragao;
	capacidade = nova_capacidade;
}

// ====================================================================
// PROCEDIMENTO QUE SALVA AS MUDANÇAS NO .CSV
// ====================================================================
void salvarMudancas(Dragon*& dragao, int& tamanho, int& capacidade,
					const string& arquivo_string, bool overwrite,
					int qtdNovos = 0) {
	ofstream documento;

	// Se for overwrite, reescreve o arquivo com todos os dados
	if (overwrite) {
		documento.open(arquivo_string);
		if (!documento) {
			cerr << "Erro ao abrir arquivo para escrita: " << arquivo_string
				 << "\n";
			return;
		}

		// Write CSV Header
		documento << "\"ID\",\"Nome do Dragao\",\"Tipo\",\"Nivel "
					 "atual\",\"Vida (HP)\",\"Ataque (dano base)\",\"Chance "
					 "Ataque Critico\",\"Habilidade Especial\""
				  << '\n';

		int contagem_salvar = 0;
		for (int i = 0; i < tamanho; ++i) {
			if (dragao[i].id != -1) {
				contagem_salvar++;
			}
		}

		// Create a temporary array for dragons to be saved
		Dragon* dragoes_salvar = new Dragon[contagem_salvar];
		int novo_tamanho = 0;
		for (int i = 0; i < tamanho; ++i) {
			if (dragao[i].id != -1) {
				dragao[i].id = novo_tamanho + 1;
				dragoes_salvar[novo_tamanho] = dragao[i];
				novo_tamanho++;
			}
		}

		// Write the data to the file
		for (int i = 0; i < novo_tamanho; ++i) {
			documento << '"' << dragoes_salvar[i].id << '"' << ',' << '"'
					  << dragoes_salvar[i].nome << '"' << ',' << '"'
					  << dragoes_salvar[i].tipo << '"' << ',' << '"'
					  << dragoes_salvar[i].nivel << '"' << ',' << '"'
					  << dragoes_salvar[i].vida << '"' << ',' << '"'
					  << dragoes_salvar[i].ataque << '"' << ',' << '"' << fixed
					  << setprecision(2) << dragoes_salvar[i].chance_critico
					  << '"' << ',' << '"'
					  << dragoes_salvar[i].habilidade_critico << '"' << '\n';
		}
		documento.close();

		int nova_capacidade = capacidade;

		// Lógica de redimensionamento para diminuir a capacidade se necessário
		if (novo_tamanho < capacidade / 2) {
			if (capacidade > 40) {
				nova_capacidade = novo_tamanho + 10;
				if (nova_capacidade < 40) {
					nova_capacidade = 40;
				}
			}
		} else if (novo_tamanho >= capacidade) {
			nova_capacidade = novo_tamanho + 10;
		}

		Dragon* novo_dragao_array = new Dragon[nova_capacidade];
		for (int i = 0; i < novo_tamanho; ++i) {
			novo_dragao_array[i] = dragoes_salvar[i];
		}

		delete[] dragao;
		delete[] dragoes_salvar;

		dragao = novo_dragao_array;
		tamanho = novo_tamanho;
		capacidade = nova_capacidade;

	} 
	
	else {

		// Se não for overwrite, apenas adiciona os novos no final
		documento.open(arquivo_string, ios::app);
		if (!documento) {
			cerr << "Erro ao abrir arquivo para append: " << arquivo_string
				 << "\n";
			return;
		}

		int inicio = tamanho - qtdNovos;
		for (int i = inicio; i < tamanho; ++i) {
			documento << '"' << dragao[i].id << '"' << ',' << '"'
					  << dragao[i].nome << '"' << ',' << '"' << dragao[i].tipo
					  << '"' << ',' << '"' << dragao[i].nivel << '"' << ','
					  << '"' << dragao[i].vida << '"' << ',' << '"'
					  << dragao[i].ataque << '"' << ',' << '"' << fixed
					  << setprecision(2) << dragao[i].chance_critico << '"'
					  << ',' << '"' << dragao[i].habilidade_critico << '"'
					  << '\n';
		}
		documento.close();
	}
}

void inserirDragoes(Dragon*& dragao, int& tamanho, int& capacidade, int qtd,
					bool& salvo) {
	for (int i = 0; i < qtd; i++) {
		if (tamanho == capacidade)
			redimensionar_vetor(dragao, tamanho, capacidade);

		cout << "\n =--- Inserindo dragão " << (i + 1) << " de " << qtd
			 << " ---=\n";

		dragao[tamanho].id = tamanho + 1;
		cin.ignore();

		cout << "Nome: ";
		getline(cin, dragao[tamanho].nome);

		cout << "Tipo: ";
		getline(cin, dragao[tamanho].tipo);

		cout << "Nivel: ";
		cin >> dragao[tamanho].nivel;

		cout << "Vida: ";
		cin >> dragao[tamanho].vida;

		cout << "Ataque: ";
		cin >> dragao[tamanho].ataque;

		cout << "Chance critico (0.00): ";
		cin >> dragao[tamanho].chance_critico;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Habilidade critico: ";
		getline(cin, dragao[tamanho].habilidade_critico);

		dragao[tamanho].removido_logicamente = false;

		tamanho++;
	}

	salvo = false;
}

void removerDragao(Dragon* dragao, int tamanho, int qtd, bool& salvo) {
	for (int k = 0; k < qtd; k++) {
		int id;

		cout << "ID:  ";
		cin >> id;

		int i = 0;
		bool encontrou = false;
		bool condicao_busca = true;

		while (i < tamanho && condicao_busca) {
			if (dragao[i].id == id) {
				dragao[i].id = -1;
				dragao[i].removido_logicamente = true;
				salvo = false;
				encontrou = true;
				cout << endl << "Dragão adicionado para a fila de exclusão.";
				condicao_busca = false;
			}
			i++;
		}

		if (!encontrou) {
			cout << "Dragão não encontrado.\n";
		}
	}
}

void cancelarRemocao(Dragon* dragao, const int tamanho) {
	int novo_id = 1;
	for (int i = 0; i < tamanho; i++) {
		if (dragao[i].id == -1) {
			dragao[i].id = novo_id;
			dragao[i].removido_logicamente = false;
		}
		if (dragao[i].id != -1) {
			novo_id++;
		}
	}
}

// ====================================================================
// FUNÇÃO DE MENU PRINCIPAL UTILIZANDO NOTCURSES
// ====================================================================

int menuNotcurses(struct notcurses* nc, bool salvo) {
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

		// Linha em vermelho para alterações não salvas
		if (!salvo) {
			ncplane_set_fg_rgb8(stdplane, 255, 0, 0);
			ncplane_printf_yx(stdplane, 5 + total, 4,
							  "ATENÇÃO: Há alterações não salvas!");
		}
		notcurses_render(nc);

		// AGUARDA ENTRADA DO USUÁRIO PARA O MENU
		ncinput ni;
		uint32_t key = notcurses_get_blocking(nc, &ni);

		if (ni.evtype != NCTYPE_RELEASE) {
			if (key == NCKEY_UP) {
				selecionado = (selecionado - 1 + total) % total;
			} else if (key == NCKEY_DOWN) {
				selecionado = (selecionado + 1) % total;
			} else if (key == NCKEY_ENTER || key == '\n') {
				rodando = false;
				return selecionado;
			}
		}
	}
	return selecionado;
}

// ====================================================================
// Menu TUI: Mostrar Dragões (Lógica de Input com cout/cin )
// ====================================================================

void menuMostrarDragoes(struct notcurses*& nc, Dragon* dragoes, int tamanho) {
	ncplane* stdplane = notcurses_stdplane(nc);
	const char* opcoes[] = {"Mostrar todos os dragões",
							"Mostrar intervalo personalizado", "Voltar"};
	const int total = sizeof(opcoes) / sizeof(opcoes[0]);
	int selecionado = 0;
	bool rodando = true;

	while (rodando) {
		ncplane_erase(stdplane);
		ncplane_set_fg_rgb8(stdplane, 150, 220, 255);
		ncplane_printf_yx(stdplane, 1, 2,
						  "╔════════════════════════════════════════╗");
		ncplane_printf_yx(stdplane, 5 + total, 2,
						  "╚════════════════════════════════════════╝");
		ncplane_printf_yx(stdplane, 2, 10, "==== MENU - MOSTRAR DRAGÕES ====");
		for (int i = 0; i < total; i++) {
			if (i == selecionado) {
				ncplane_set_fg_rgb8(stdplane, 0, 255, 255);
				ncplane_printf_yx(stdplane, 4 + i, 6, "► %s", opcoes[i]);
			} else {
				ncplane_set_fg_rgb8(stdplane, 200, 230, 255);
				ncplane_printf_yx(stdplane, 4 + i, 8, "%s", opcoes[i]);
			}
		}
		notcurses_render(nc);

		// AGUARDA ENTRADA DO USUÁRIO PARA O MENU
		ncinput ni;
		uint32_t key = notcurses_get_blocking(nc, &ni);

		if (ni.evtype != NCTYPE_RELEASE) {
			if (key == NCKEY_UP) {
				selecionado = (selecionado - 1 + total) % total;
			} else if (key == NCKEY_DOWN) {
				selecionado = (selecionado + 1) % total;
			} else if (key == NCKEY_ENTER || key == '\n') {
				if (selecionado == 2) {
					rodando = false;
				} else {
					int indice_inicio;
					int indice_fim;
					if (selecionado == 0) {
						indice_inicio = 0;
						indice_fim = tamanho - 1;
					}

					else if (selecionado == 1) {
						notcurses_stop(nc);
						system("clear");

						int id_inicio, id_fim;
						cout << "--- EXIBINDO DRAGÕES EM UM INTERVALO "
								"---\n\n";
						cout << "Digite o índice inicial (1 a " << tamanho
							 << "): ";
						cin >> id_inicio;
						cout << "Digite o índice final (" << id_inicio << " a "
							 << tamanho << "): ";
						cin >> id_fim;
						limparEntrada();  // Limpa o buffer de entrada

						indice_inicio = id_inicio - 1;
						indice_fim = id_fim - 1;

						if (indice_inicio < 0) indice_inicio = 0;
						if (indice_fim >= tamanho) indice_fim = tamanho - 1;
						if (indice_inicio > indice_fim) {
							int aux = indice_inicio;
							indice_inicio = indice_fim;
							indice_fim = aux;
						}

						// Reinicializa o notcurses para a visualização
						notcurses_options opts = {};
						opts.flags = NCOPTION_SUPPRESS_BANNERS;
						nc = notcurses_init(&opts, nullptr);
						stdplane = notcurses_stdplane(nc);
					}

					bool visualizando_lista = true;
					int offset = indice_inicio;
					int linhas_por_pagina = 25;

					while (visualizando_lista) {
						ncplane_erase(stdplane);
						ncplane_set_fg_rgb8(stdplane, 220, 220, 220);
						ncplane_printf_yx(stdplane, 1, 2,
										  "Lista de Dragões (↑ ↓ para rolar, "
										  "ENTER/ESC p/ voltar)");

						ncplane_set_fg_rgb8(stdplane, 173, 216,
											230);  // RGB para Light Blue
						for (int i = 0; i < linhas_por_pagina; i++) {
							int idx = i + offset;
							if (idx <= indice_fim) {
								if (dragoes[idx].id != -1) {
									ncplane_printf_yx(
										stdplane, 3 + i, 2,
										"%3d %-20s %-10s Nv:%2d HP:%4d Atq:%4d "
										"Crt:%.2f Hab:%s",
										dragoes[idx].id,
										dragoes[idx].nome.c_str(),
										dragoes[idx].tipo.c_str(),
										dragoes[idx].nivel, dragoes[idx].vida,
										dragoes[idx].ataque,
										dragoes[idx].chance_critico,
										dragoes[idx]
											.habilidade_critico.c_str());
								}
							}
						}
						notcurses_render(nc);

						ncinput ni_lista;
						uint32_t key_lista =
							notcurses_get_blocking(nc, &ni_lista);
						if (ni_lista.evtype != NCTYPE_RELEASE) {
							if (key_lista == NCKEY_UP) {
								if (offset > indice_inicio) {
									offset--;
								}
							} else if (key_lista == NCKEY_DOWN) {
								if ((offset + linhas_por_pagina - 1) <
									indice_fim) {
									offset++;
								}
							} else if (key_lista == NCKEY_ENTER ||
									   key_lista == '\n' ||
									   key_lista == NCKEY_ESC) {
								visualizando_lista = false;
							}
						}
					}
				}
			}
		}
	}
}

// ====================================================================
// Menu TUI: Salvar os Dragões (Lógica de Input com cout/cin )
// ====================================================================

void menuSalvarDragoes(struct notcurses*& nc, Dragon*& dragoes, int& tamanho,
					   int& capacidade, const string& arquivo_string,
					   bool& salvo, int inicioInseridos) {
	ncplane* stdplane = notcurses_stdplane(nc);
	const char* opcoes[] = {"Sim", "Não"};
	const int total = 2;
	int selecionado = 0;
	bool rodando = true;

	while (rodando) {
		ncplane_erase(stdplane);
		ncplane_set_fg_rgb8(stdplane, 150, 255, 200);
		ncplane_printf_yx(stdplane, 1, 2,
						  "╔══════════════════════════════════════╗");
		ncplane_printf_yx(stdplane, 6, 2,
						  "╚══════════════════════════════════════╝");
		ncplane_printf_yx(stdplane, 2, 8, "Deseja salvar no arquivo?");

		for (int i = 0; i < total; i++) {
			if (i == selecionado) {
				if (i == 0)
					ncplane_set_fg_rgb8(stdplane, 0, 255, 0);
				else
					ncplane_set_fg_rgb8(stdplane, 255, 0, 0);

				ncplane_printf_yx(stdplane, 4 + i, 6, "► %s", opcoes[i]);
			} else {
				ncplane_set_fg_rgb8(stdplane, 255, 255, 255);
				ncplane_printf_yx(stdplane, 4 + i, 8, "%s", opcoes[i]);
			}
		}

		notcurses_render(nc);

		ncinput ni;
		uint32_t key = notcurses_get_blocking(nc, &ni);

		if (ni.evtype != NCTYPE_RELEASE) {
			if (key == NCKEY_UP)
				selecionado = (selecionado - 1 + total) % total;

			else if (key == NCKEY_DOWN)
				selecionado = (selecionado + 1) % total;

			else if (key == NCKEY_ENTER || key == '\n') {
				if (selecionado == 0) {
					if (inicioInseridos == -1) 
						salvarMudancas(dragoes, tamanho, capacidade,
									   arquivo_string, true);
					else {
						int qtdNovos = tamanho - inicioInseridos;
						salvarMudancas(dragoes, tamanho, capacidade,
									   arquivo_string, false, qtdNovos);
					}

					salvo = true;

					ncplane_erase(stdplane);
					ncplane_set_fg_rgb8(stdplane, 0, 255, 0);
					ncplane_printf_yx(stdplane, 2, 6,
									  "✔ Arquivo salvo com sucesso!");
					ncplane_printf_yx(stdplane, 4, 4,
									  "Pressione ENTER para voltar...");
					notcurses_render(nc);

					bool condicao_espera = true;
					while (condicao_espera) {
						ncinput fim;
						uint32_t key2 = notcurses_get_blocking(nc, &fim);
						if (fim.evtype != NCTYPE_RELEASE) {
							if (key2 == NCKEY_ENTER || key2 == '\n') {
								rodando = false;
								condicao_espera = false;
							}
						}
					}
				}

				else {
					if (inicioInseridos == -1) 
						cancelarRemocao(dragoes, tamanho);
			
					ncplane_erase(stdplane);
					ncplane_set_fg_rgb8(stdplane, 255, 0, 0);
					ncplane_printf_yx(stdplane, 2, 8, "✖ Salvar cancelado.");
					ncplane_printf_yx(stdplane, 4, 4,
									  "Pressione ENTER para voltar...");
					notcurses_render(nc);

					bool condicao_espera = true;
					while (condicao_espera) {
						ncinput fim;
						uint32_t key2 = notcurses_get_blocking(nc, &fim);
						if (fim.evtype != NCTYPE_RELEASE) {
							if (key2 == NCKEY_ENTER || key2 == '\n') {
								rodando = false;
								condicao_espera = false;
							}
						}
					}
				}
			}
		}
	}
}

// ====================================================================
// Menu TUI: Inserir os Dragões (Lógica de Input com cout/cin )
// ====================================================================

void menuInserirDragoes(struct notcurses*& nc, Dragon*& dragoes, int& tamanho,
						int& capacidade, bool& salvo,
						const string& arquivo_string) {
	ncplane* stdplane = notcurses_stdplane(nc);

	const char* opcoes[] = {"Inserir novo(s) dragao(es)", "Voltar"};
	int total = 2;
	int selecionado = 0;
	bool rodando = true;

	while (rodando) {
		ncplane_erase(stdplane);
		ncplane_set_fg_rgb8(stdplane, 180, 255, 150);
		ncplane_printf_yx(stdplane, 1, 2,
						  "╔══════════════════════════════════════╗");
		ncplane_printf_yx(stdplane, 6, 2,
						  "╚══════════════════════════════════════╝");
		ncplane_printf_yx(stdplane, 2, 8, "==== MENU - INSERIR DRAGOES ====");

		for (int i = 0; i < total; i++) {
			if (i == selecionado) {
				ncplane_set_fg_rgb8(stdplane, 0, 255, 0);
				ncplane_printf_yx(stdplane, 4 + i, 6, "► %s", opcoes[i]);
			} else {
				ncplane_set_fg_rgb8(stdplane, 255, 255, 255);
				ncplane_printf_yx(stdplane, 4 + i, 8, "%s", opcoes[i]);
			}
		}

		notcurses_render(nc);

		ncinput ni;
		uint32_t key = notcurses_get_blocking(nc, &ni);

		if (ni.evtype != NCTYPE_RELEASE) {
			if (key == NCKEY_UP)
				selecionado = (selecionado - 1 + total) % total;

			else if (key == NCKEY_DOWN)
				selecionado = (selecionado + 1) % total;

			else if (key == NCKEY_ENTER || key == '\n') {
				if (selecionado == 0) {
					notcurses_stop(nc);
					system("clear");

					int qtd;
					cout << "Quantos dragões deseja inserir? ";
					cin >> qtd;
					limparEntrada();

					int inicioInseridos = tamanho;
					inserirDragoes(dragoes, tamanho, capacidade, qtd, salvo);

					// Reinicializa o notcurses
					notcurses_options opts = {};
					opts.flags = NCOPTION_SUPPRESS_BANNERS;
					nc = notcurses_init(&opts, nullptr);
					stdplane = notcurses_stdplane(nc);

					// Chama o menu de salvar após a inserção
					menuSalvarDragoes(nc, dragoes, tamanho, capacidade,
									  arquivo_string, salvo, inicioInseridos);

				} else {
					rodando = false;
				}
			}
		}
	}
}

// ====================================================================
// Menu TUI: Remover Dragões (Lógica de Input com cout/cin )
// ====================================================================

void menuRemoverDragoes(struct notcurses*& nc, Dragon*& dragoes, int& tamanho,
						int& capacidade, bool& salvo,
						const string& arquivo_string) {
	ncplane* stdplane = notcurses_stdplane(nc);

	const char* opcoes[] = {"Remover dragão(ões)", "Voltar"};
	int total = 2;
	int selecionado = 0;
	bool rodando = true;

	while (rodando) {
		ncplane_erase(stdplane);
		ncplane_set_fg_rgb8(stdplane, 255, 150, 150);
		ncplane_printf_yx(stdplane, 1, 2,
						  "╔══════════════════════════════════════╗");
		ncplane_printf_yx(stdplane, 6, 2,
						  "╚══════════════════════════════════════╝");
		ncplane_printf_yx(stdplane, 2, 8, "==== MENU - REMOVER DRAGÕES ====");

		for (int i = 0; i < total; i++) {
			if (i == selecionado) {
				ncplane_set_fg_rgb8(stdplane, 255, 0, 0);
				ncplane_printf_yx(stdplane, 4 + i, 6, "► %s", opcoes[i]);
			} else {
				ncplane_set_fg_rgb8(stdplane, 255, 255, 255);
				ncplane_printf_yx(stdplane, 4 + i, 8, "%s", opcoes[i]);
			}
		}

		notcurses_render(nc);

		ncinput ni;
		uint32_t key = notcurses_get_blocking(nc, &ni);

		if (ni.evtype != NCTYPE_RELEASE) {
			if (key == NCKEY_UP)
				selecionado = (selecionado - 1 + total) % total;

			else if (key == NCKEY_DOWN)
				selecionado = (selecionado + 1) % total;

			else if (key == NCKEY_ENTER || key == '\n') {
				if (selecionado == 0) {
					notcurses_stop(nc);
					system("clear");

					int qtd;
					cout << "Quantos dragões deseja remover? ";
					cin >> qtd;
					limparEntrada();

					removerDragao(dragoes, tamanho, qtd, salvo);

					// Reinicializa o notcurses
					notcurses_options opts = {};
					opts.flags = NCOPTION_SUPPRESS_BANNERS;
					nc = notcurses_init(&opts, nullptr);
					stdplane = notcurses_stdplane(nc);

					// Chama o menu de salvar após a remoção. inicioInseridos =
					// -1 signals removal.
					menuSalvarDragoes(nc, dragoes, tamanho, capacidade,
									  arquivo_string, salvo, -1);

				} else {
					rodando = false;
				}
			}
		}
	}
}

// ====================================================================
// Menu TUI: Buscar Dragões (Lógica de Input com cout/cin )
// ====================================================================

void menuBuscarDragoes(struct notcurses*& nc, Dragon* dragoes, int tamanho,
					   const string& arquivo_string) {
	ncplane* stdplane = notcurses_stdplane(nc);

	const char* opcoes[] = {"Buscar por ID", "Voltar"};
	int total = 2;
	int selecionado = 0;
	bool rodando = true;

	while (rodando) {
		ncplane_erase(stdplane);
		ncplane_set_fg_rgb8(stdplane, 255, 255, 150);
		ncplane_printf_yx(stdplane, 1, 2,
						  "╔══════════════════════════════════════╗");
		ncplane_printf_yx(stdplane, 6, 2,
						  "╚══════════════════════════════════════╝");
		ncplane_printf_yx(stdplane, 2, 8, "==== MENU - BUSCAR DRAGÕES ====");

		for (int i = 0; i < total; i++) {
			if (i == selecionado) {
				ncplane_set_fg_rgb8(stdplane, 255, 255, 0);
				ncplane_printf_yx(stdplane, 4 + i, 6, "► %s", opcoes[i]);
			} else {
				ncplane_set_fg_rgb8(stdplane, 255, 255, 255);
				ncplane_printf_yx(stdplane, 4 + i, 8, "%s", opcoes[i]);
			}
		}

		notcurses_render(nc);

		ncinput ni;
		uint32_t key = notcurses_get_blocking(nc, &ni);

		if (ni.evtype != NCTYPE_RELEASE) {
			if (key == NCKEY_UP)
				selecionado = (selecionado - 1 + total) % total;

			else if (key == NCKEY_DOWN)
				selecionado = (selecionado + 1) % total;

			else if (key == NCKEY_ENTER || key == '\n') {
				if (selecionado == 0) {
					notcurses_stop(nc);
					system("clear");

					int idDragon;
					cout << "Digite o ID do Dragão que deseja procurar:"
						 << endl;
					cin >> idDragon;
					limparEntrada();
					quickSort(dragoes, 0, tamanho - 1, 0);

					int indice =
						buscaBinaria(dragoes, idDragon, 0, tamanho - 1);

					// Reinicializa o notcurses
					notcurses_options opts = {};
					opts.flags = NCOPTION_SUPPRESS_BANNERS;
					nc = notcurses_init(&opts, nullptr);
					stdplane = notcurses_stdplane(nc);

					if (indice != -1) {
						bool visualizando_lista = true;
						int offset = indice;
						int linhas_por_pagina = 1;	

						while (visualizando_lista) {
							ncplane_erase(stdplane);
							ncplane_set_fg_rgb8(stdplane, 220, 220, 220);
							ncplane_printf_yx(
								stdplane, 1, 2,
								"Dragão Encontrado (ENTER/ESC p/ voltar)");

							ncplane_set_fg_rgb8(stdplane, 173, 216, 230);

							// Display the single dragon
							if (dragoes[indice].id != -1) {
								ncplane_printf_yx(
									stdplane, 3, 2,
									"%3d %-20s %-10s Nv:%2d HP:%4d Atq:%4d "
									"Crt:%.2f Hab:%s",
									dragoes[indice].id,
									dragoes[indice].nome.c_str(),
									dragoes[indice].tipo.c_str(),
									dragoes[indice].nivel, dragoes[indice].vida,
									dragoes[indice].ataque,
									dragoes[indice].chance_critico,
									dragoes[indice].habilidade_critico.c_str());
							}

							notcurses_render(nc);

							ncinput ni_lista;
							uint32_t key_lista =
								notcurses_get_blocking(nc, &ni_lista);
							if (ni_lista.evtype != NCTYPE_RELEASE) {
								if (key_lista == NCKEY_ENTER ||
									key_lista == '\n' ||
									key_lista == NCKEY_ESC) {
									visualizando_lista = false;
								}
							}
						}

					} else {
						ncplane_erase(stdplane);
						ncplane_set_fg_rgb8(stdplane, 255, 0, 0);
						ncplane_printf_yx(stdplane, 2, 6,
										  "✖ Dragão com ID %d não encontrado.",
										  idDragon);
						ncplane_printf_yx(stdplane, 4, 4,
										  "Pressione ENTER para voltar...");
						notcurses_render(nc);

						bool condicao_espera = true;
						while (condicao_espera) {
							ncinput fim;
							uint32_t key2 = notcurses_get_blocking(nc, &fim);
							if (fim.evtype != NCTYPE_RELEASE) {
								if (key2 == NCKEY_ENTER || key2 == '\n') {
									condicao_espera = false;
								}
							}
						}
					}

					rodando = false;
				}

				else {
					ncplane_erase(stdplane);
					ncplane_set_fg_rgb8(stdplane, 255, 0, 0);
					ncplane_printf_yx(stdplane, 2, 8, "✖ Busca cancelada.");
					ncplane_printf_yx(stdplane, 4, 4,
									  "Pressione ENTER para voltar...");
					notcurses_render(nc);

					bool condicao_espera = true;
					while (condicao_espera) {
						ncinput fim;
						uint32_t key2 = notcurses_get_blocking(nc, &fim);
						if (fim.evtype != NCTYPE_RELEASE) {
							if (key2 == NCKEY_ENTER || key2 == '\n') {
								rodando = false;
								condicao_espera = false;
							}
						}
					}
				}
			}
		}
	}
}

// ====================================================================
// Menu TUI: Ordenar Dragões (Novo)
// ====================================================================

void menuOrdenarDragoes(struct notcurses*& nc, Dragon* dragoes, int tamanho,
						bool& salvo) {
	ncplane* stdplane = notcurses_stdplane(nc);
	const char* opcoes[] = {"Ordenar por ID", "Ordenar por Nome",
							"Ordenar por Tipo", "Voltar"};
	const int total = sizeof(opcoes) / sizeof(opcoes[0]);
	int selecionado = 0;
	bool rodando = true;

	while (rodando) {
		ncplane_erase(stdplane);
		ncplane_set_fg_rgb8(stdplane, 255, 150, 255);
		ncplane_printf_yx(stdplane, 1, 2,
						  "╔══════════════════════════════════════╗");
		ncplane_printf_yx(stdplane, 5 + total, 2,
						  "╚══════════════════════════════════════╝");
		ncplane_printf_yx(stdplane, 2, 8, "==== MENU - ORDENAR DRAGÕES ====");

		for (int i = 0; i < total; i++) {
			if (i == selecionado) {
				ncplane_set_fg_rgb8(stdplane, 255, 0, 255);
				ncplane_printf_yx(stdplane, 4 + i, 6, "► %s", opcoes[i]);
			} else {
				ncplane_set_fg_rgb8(stdplane, 255, 255, 255);
				ncplane_printf_yx(stdplane, 4 + i, 8, "%s", opcoes[i]);
			}
		}

		notcurses_render(nc);

		ncinput ni;
		uint32_t key = notcurses_get_blocking(nc, &ni);

		if (ni.evtype != NCTYPE_RELEASE) {
			if (key == NCKEY_UP)
				selecionado = (selecionado - 1 + total) % total;

			else if (key == NCKEY_DOWN)
				selecionado = (selecionado + 1) % total;

			else if (key == NCKEY_ENTER || key == '\n') {
				if (selecionado < 3) {
					// 0: ID, 1: Nome, 2: Tipo. Corresponde aos cases 0, 1, 2
					// em comparaEntrada
					quickSort(dragoes, 0, tamanho - 1, selecionado);
					salvo = false;

					ncplane_erase(stdplane);
					ncplane_set_fg_rgb8(stdplane, 0, 255, 0);
					ncplane_printf_yx(stdplane, 2, 6,
									  "✔ Ordenação concluída. Pressione ENTER "
									  "para voltar...");
					notcurses_render(nc);

					bool condicao_espera = true;
					while (condicao_espera) {
						ncinput fim;
						uint32_t key2 = notcurses_get_blocking(nc, &fim);
						if (fim.evtype != NCTYPE_RELEASE) {
							if (key2 == NCKEY_ENTER || key2 == '\n') {
								rodando = false;
								condicao_espera = false;
							}
						}
					}
				} else {
					rodando = false;
				}
			}
		}
	}
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

	// Read and discard header line
	if (!getline(nome_arquivo, linha))
		cerr << "Arquivo vazio ou erro de leitura.";

	bool condicao_leitura = true;
	while (condicao_leitura) {
		if (getline(nome_arquivo, linha)) {
			if (!linha.empty()) {
				if (tamanho == capacidade) {
					redimensionar_vetor(dragoes, tamanho, capacidade);
				}

				Dragon novo_dragao;
				string campos[8];  // Array size to 8 to read all fields

				for (int c = 0; c < 8; c++) {
					campos[c] = "";
					if (!linha.empty() && linha[0] == '"') {
						linha.erase(0, 1);
						unsigned int pos = 0;
						bool condicao_pos = true;
						while (pos < linha.size() && condicao_pos) {
							if (linha[pos] != '"') {
								pos++;
							} else {
								condicao_pos = false;
							}
						}

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
						bool condicao_pos = true;
						while (pos < linha.size() && condicao_pos) {
							if (linha[pos] != ',') {
								pos++;
							} else {
								condicao_pos = false;
							}
						}

						campos[c] = "";
						for (unsigned int k = 0; k < pos; k++)
							campos[c] += linha[k];

						if (pos < linha.size())
							linha.erase(0, pos + 1);
						else
							linha = "";
					}
				}

				// Assign fields, ignoring campos[0] (the CSV ID)
				novo_dragao.id = tamanho + 1; 
				novo_dragao.nome = campos[1];
				novo_dragao.tipo = campos[2];
				novo_dragao.nivel = string_para_int(campos[3]);
				novo_dragao.vida = string_para_int(campos[4]);
				novo_dragao.ataque = string_para_int(campos[5]);
				novo_dragao.chance_critico = string_para_float(campos[6]);
				novo_dragao.habilidade_critico = campos[7];

				dragoes[tamanho] = novo_dragao;
				tamanho++;
			}
		} else {
			condicao_leitura = false;
		}
	}
}

int main() {
	int tamanho = 0, capacidade = 40;
	Dragon* dragoes = new Dragon[capacidade];

	const string arquivo_string = "./dragon_city_60_dragon_real_names.csv";
	ifstream nome_arquivo(arquivo_string);
	if (!nome_arquivo.is_open()) {
		cerr << "Erro fatal: Não foi possível carregar o arquivo "
			 << arquivo_string << ".\n";
		delete[] dragoes;
		return 1;
	}

	carregar_dados_csv(dragoes, tamanho, capacidade, nome_arquivo);
	nome_arquivo.close();

	notcurses_options opts = {};
	opts.flags = NCOPTION_SUPPRESS_BANNERS;
	struct notcurses* nc = notcurses_init(&opts, nullptr);

	if (nc == nullptr) {
		cerr << "Erro fatal: Não foi possível inicializar a biblioteca "
				"notcurses.\n";
		delete[] dragoes;
		return 1;
	}

	bool salvo = true;

	bool app_rodando = true;
	while (app_rodando) {
		int opcao = menuNotcurses(nc, salvo);
		if (opcao == 0) {
			// Imprimir os dados - podendo ser todos os dados
			// ou num intervalo personalizado
			menuMostrarDragoes(nc, dragoes, tamanho);
		} else if (opcao == 1) {
			// Inserir um ou mais novos Dragões
			menuInserirDragoes(nc, dragoes, tamanho, capacidade, salvo,
							   arquivo_string);
		} else if (opcao == 2) {
			// Remover
			menuRemoverDragoes(nc, dragoes, tamanho, capacidade, salvo,
							   arquivo_string);
		} else if (opcao == 3) {
			// Buscar
			menuBuscarDragoes(nc, dragoes, tamanho, arquivo_string);
		} else if (opcao == 4) {
			// Ordenar
			menuOrdenarDragoes(nc, dragoes, tamanho, salvo);
		} else if (opcao == 5) {
			// Salvar
			// Call menuSalvarDragoes with -1 to signal a general save
			// (overwrite)
			menuSalvarDragoes(nc, dragoes, tamanho, capacidade, arquivo_string,
							  salvo, -1);
		} else if (opcao == 6) {
			// Sair
			app_rodando = false;
		}
	}

	// --- Finalização Limpa ---
	notcurses_stop(nc);
	delete[] dragoes;
	cout << "Programa finalizado com sucesso." << endl;

	return 0;
}
