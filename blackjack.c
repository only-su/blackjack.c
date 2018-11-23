/* Codigos Unicode para os simbolos dos naipes */
#define ESPADA "\xE2\x99\xA4"
#define PAUS "\xE2\x99\xA7"
#define COPAS "\xE2\x99\xA5"
#define OURO "\xE2\x99\xA6"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>

/* Estrutura da carta */
typedef struct _CARTA {
	int	valor;
	char	naipe[20];
} CARTA;

/* Estrutura do baralho */
typedef struct _BARALHO {
	CARTA	cartas[52];
	int	qtd;
} BARALHO;

/* Estrutura do jogador */
typedef struct _JOGADOR {
	CARTA	mao[15];
	int	qtd_mao;
	int	dinheiro;
} JOGADOR;

/* Traduz o valor inteiro da carta em uma string (A,1,2,3,4,5,6,7,8,9,10,J,Q ou K) */
char *traduz_valor(int valor, char *retorno)
{
	switch (valor) {
	case 0:
		strcpy(retorno, "A");
		break;
	case 10:
		strcpy(retorno, "J");
		break;
	case 11:
		strcpy(retorno, "Q");
		break;
	case 12:
		strcpy(retorno, "K");
		break;
	default:
		sprintf(retorno, "%d", valor + 1);
	}
	return retorno;
}

/* Traduz o valor interio do naipe da carta na string Unicode do simbolo do
 * naipe */
char *traduz_naipe(int naipe, char *retorno)
{
	switch (naipe) {
	case 0:
		strcpy(retorno, ESPADA);
		break;
	case 1:
		strcpy(retorno, COPAS);
		break;
	case 2:
		strcpy(retorno, OURO);
		break;
	case 3:
		strcpy(retorno, PAUS);
		break;
	}
	return retorno;
}

/* Avalia a pontuacao de uma mao */
int pontuaMao(CARTA c[], int qtd)
{
	int i, pts = 0;

	for (i = 0; i < qtd; i++) {
		if (c[i].valor == 0) {
			/* De acordo com as regras do 21, o valor da carta A pode ser 1 ou 11
			 * dependendo do que beneficia mais o jogador */
			if (pts + 11 <= 21)
				pts += 11;
			else
				pts += 1;
		} else if (c[i].valor >= 9) {
			pts += 10;
		} else {
			pts += c[i].valor + 1;
		}
	}
	return pts;
}

/* Gera e retorna a string de uma carta */
char *string_carta(CARTA car)
{
	char *str;
	char scarta[5];

	str = (char *)malloc(sizeof(char) * 15);
	traduz_valor(car.valor, scarta);
	sprintf(str, "%s%s", scarta, car.naipe);
	return str;
}

void imprime_mao(JOGADOR *jg, char ident)
{
	int i;
	char scarta[5];
	char lines [5][5000];

	for (i = 0; i < 5; i++) strcpy(lines[i], " ");

	if (ident == 'm') {
		sprintf(lines[0], " ┌─────┐");
		sprintf(lines[1], " │░░░░░│");
		sprintf(lines[2], " │░░░░░│");
		sprintf(lines[3], " │░░░░░│");
		sprintf(lines[4], " └─────┘");
	}

	for (i = (ident == 'm') ? 1 : 0; i < jg->qtd_mao; i++) {
		traduz_valor(jg->mao[i].valor, scarta);
		sprintf(lines[0], "%s┌─────┐", lines[0]);
		sprintf(lines[1], "%s│%s    │", lines[1], jg->mao[i].naipe);
		sprintf(lines[2], "%s│  %c%c │", lines[2], scarta[0], (strcmp(scarta, "10")) ? ' ' : scarta[1]);
		sprintf(lines[3], "%s│    %s│", lines[3], jg->mao[i].naipe);
		sprintf(lines[4], "%s└─────┘", lines[4]);
	}

	puts(lines[0]);
	puts(lines[1]);
	puts(lines[2]);
	puts(lines[3]);
	puts(lines[4]);
}

/* Inicia e retorna um baralho */
BARALHO iniciar_baralho()
{
	BARALHO bar;
	int i;

	for (i = 0; i < 52; i++) {
		traduz_naipe(i / 13, bar.cartas[i].naipe);
		bar.cartas[i].valor = i % 13;
	}
	bar.qtd = 52;
	return bar;
}

/* Imprime um baralho inteiro em ordem (Usada para testes) */
void imprimir_baralho(BARALHO bar)
{
	int i;

	for (i = 0; i < bar.qtd; i++)
		printf("%s", string_carta(bar.cartas[i]));
}

/* Embaralha um baralho */
void embaralhar(BARALHO *bar)
{
	int i, r;
	CARTA aux;

	srand(time(0));
	for (i = 0; i < bar->qtd; i++) {
		r = rand() % 52;
		aux = bar->cartas[i];
		bar->cartas[i] = bar->cartas[r];
		bar->cartas[r] = aux;
	}
}

/* Retira e retorna uma carta do baralho */
CARTA puxaCarta(BARALHO *bar)
{
	CARTA c = bar->cartas[--bar->qtd];

	return c;
}

/* Cria a mao inicial de um jogador */
void iniciar_mao(JOGADOR *jg, BARALHO *bar)
{
	jg->mao[0] = puxaCarta(bar);
	jg->mao[1] = puxaCarta(bar);
	jg->qtd_mao = 2;
}

void titulo()
{
	system("clear");
	printf("  /$$$$$$$  /$$                     /$$          /$$$$$               "
	       "      /$$                    \n ");
	printf("| $$__  $$| $$                    | $$         |__  $$               "
	       "     | $$                    \n ");
	printf("| $$  \\ $$| $$  /$$$$$$   /$$$$$$$| $$   /$$      | $$  /$$$$$$   "
	       "/$$$$$$$| $$   /$$      /$$$$$$$\n ");
	printf("| $$$$$$$ | $$ |____  $$ /$$_____/| $$  /$$/      | $$ |____  $$ "
	       "/$$_____/| $$  /$$/     /$$_____/\n ");
	printf("| $$__  $$| $$  /$$$$$$$| $$      | $$$$$$/  /$$  | $$  /$$$$$$$| $$ "
	       "     | $$$$$$/     | $$      \n ");
	printf("| $$  \\ $$| $$ /$$__  $$| $$      | $$_  $$ | $$  | $$ /$$__  $$| "
	       "$$      | $$_  $$     | $$      \n ");
	printf("| $$$$$$$/| $$|  $$$$$$$|  $$$$$$$| $$ \\  $$|  $$$$$$/|  $$$$$$$|  "
	       "$$$$$$$| $$ \\  $$ /$$|  $$$$$$$\n ");
	printf("|_______/ |__/ \\_______/ \\_______/|__/  \\__/ \\______/  "
	       "\\_______/ \\_______/|__/  \\__/|__/ \\_______/\n");
	printf("\n");
}

int main(void)
{
	int pts, pts_mesa, pts_split, pts_aux, aposta, rodada, dobrou = 0, split = 0, mao_atual, desistiu_split = 0;
	int dft_montante = 200;
	char c, ident;
	char msg[50];
	char input[255];
	BARALHO bar;
	JOGADOR jg;
	JOGADOR mesa;
	JOGADOR jg_split;
	FILE *inst;

	/* O jogo inicia com um menu tendo como opções começar o jogo, configurar
	 * o jogo ou sair do programa */
	while (1) {
		titulo();
		printf("Digite e para começar o jogo (¢%d)\n", dft_montante);
		printf("Digite c para configurar\n");
		printf("Digite i para instruções\n");
		printf("Digite qualquer outra coisa para sair\n");
		c = getchar();
		while ('\n' != getchar());      /* Limpa a entrada */
		if (c == 'e') {
			rodada = 1;             /* O programa começa a contar as rodadas */
			jg.dinheiro = dft_montante;

			while (1) {
				bar = iniciar_baralho();
				embaralhar(&bar);
				ident = 'm';
				strcpy(msg, "");

				/* O programa pede um valor para ser apostado nessa rodada.
				 * Se o jogador ganhar a rodada ele ganha o dobro da aposta, se perder ele
				 * não ganha nada. O programa repete até que um valor válido seja
				 * digitado. */
				while (1) {
					desistiu_split = 0;
					split = 0;
					dobrou = 0;
					mao_atual = 0;
					titulo();
					printf("Rodada %d\n", rodada);
					printf("Montante atual: ¢%d\n", jg.dinheiro);
					printf("%s\nDigite um valor para a aposta dessa rodada:\n¢", msg);
					if (!scanf("%d", &aposta) || aposta > jg.dinheiro || aposta <= 0) {
						while ('\n' != getchar());      /* Limpa a entrada */
						sprintf(msg, "Este valor não é válido para a aposta! ");
					} else {
						break;
					}
				}

				strcpy(msg, "");

				while ('\n' != getchar());      /* Limpa a entrada */
				jg.dinheiro -= aposta;          /* Remove a aposta do montante do jogador */

				/* Inicia a mao do jogador e da mesa */
				iniciar_mao(&jg, &bar);
				iniciar_mao(&mesa, &bar);

				/* O programa repete ate que o jogador deseje abaixar a mao,
				 * que a pontuacao seja igual a 21 ou que a pontuacao passe de 21 */
				while (1) {
					/* Mostra a mao do jogador e da mesa. Uma das cartas da mesa fica
					 * virada. */
					titulo();
					printf("Rodada %d\n", rodada);
					printf("Montante atual: ¢%d\n", jg.dinheiro);
					printf("Aposta atual: ¢%d\n", aposta);
					if (split) printf("Aposta do split: ¢%d\n", aposta);
					printf("\nMão da mesa:\n");
					imprime_mao(&mesa, ident);
					pts_mesa = pontuaMao(mesa.mao, mesa.qtd_mao);
					printf("\n Pontuação atual: %d\n", (ident != 'm') ? pts_mesa : pts_mesa - pontuaMao(mesa.mao, 1));

					if (mao_atual == 1) {
						printf("\nSua mão (2):\n");
						imprime_mao(&jg_split, 'j');
						pts_split = pontuaMao(jg_split.mao, jg_split.qtd_mao);
						printf("\n Pontuação atual: %d\n", pts_split);
					} else if (mao_atual == 0) {
						printf("\nSua mão");
						(split) ? printf(" (1):\n") : printf(":\n");
						imprime_mao(&jg, 'j');
						pts = pontuaMao(jg.mao, jg.qtd_mao);
						printf("\n Pontuação atual: %d\n", pts);
					} else {
						printf("\nSua mão (1):\n");
						imprime_mao(&jg, 'j');
						pts = pontuaMao(jg.mao, jg.qtd_mao);
						printf("\n Pontuação atual: %d\n", pts);
						printf("\nSua mão (2):\n");
						imprime_mao(&jg_split, 'j');
						pts_split = pontuaMao(jg_split.mao, jg_split.qtd_mao);
						printf("\n Pontuação atual: %d\n", pts_split);
					}
					printf("%s", msg);

					if (ident == 'j') break;

					/* Se a pontuacao do jogar for igual a 21 ele pode ainda empatar com a
					 * mesa, se for maior que 21 ele automaticamente perde */

					/* Decide se deve usar a pontuação da mão normal ou a pontuação do split */
					if (mao_atual == 0) pts_aux = pts;
					else pts_aux = pts_split;

					if (pts_aux > 21) {
						printf("\nA pontuação estourou!\n");
						if (mao_atual == 1 || !split) {
							if (pts <= 21) {
								while (pts_mesa < 17 && c != '0') {
									mesa.mao[mesa.qtd_mao++] = puxaCarta(&bar);
									pts_mesa = pontuaMao(mesa.mao, mesa.qtd_mao);
								}
							}
							if (split) mao_atual = 2;
							ident = 'j';
							printf("Digite qualquer coisa para continuar o jogo...");
							getch();
							while ('\n' != getchar());
						} else {
							printf("Digite qualquer coisa para jogar com sua 2ª mão...");
							getch();
							while ('\n' != getchar());
							mao_atual = 1;
							continue;
						}
					} else if (pts_aux == 21) {
						if (jg.qtd_mao == 2)
							printf("\nBlackJack! Alguém vai jantar frango essa noite!\n");
						else
							printf("\nA pontuação deu 21!\n");
						if (mao_atual == 1 || !split) {
							while (pts_mesa < 17 && c != '0') {
								mesa.mao[mesa.qtd_mao++] = puxaCarta(&bar);
								pts_mesa = pontuaMao(mesa.mao, mesa.qtd_mao);
							}
							if (split) mao_atual = 2;
							ident = 'j';
							printf("Digite qualquer coisa para continuar o jogo...");
							getch();
							while ('\n' != getchar());
						} else {
							printf("Digite qualquer coisa para jogar com sua 2ª mão...");
							getch();
							while ('\n' != getchar());
							mao_atual = 1;
							continue;
						}
					}

					if (ident == 'm') {
						printf("\nDigite s para finalizar a rodada (stand)\n");
						printf("%s", (dobrou) ? "" : "Digite h para pedir mais cartas (hit)\n");
						printf("%s", (dobrou || split) ? "" : ((aposta > jg.dinheiro) ? "Seu montante é insuficiente para dobrar a aposta!\n" : "Digite d para dobrar a aposta (double down)\n"));
						printf("%s", (!split && jg.qtd_mao == 2 && (jg.mao[0].valor == jg.mao[1].valor || (jg.mao[0].valor >= 9 && jg.mao[1].valor >= 9))) ? "Digite 2 para separar as cartas (split)\n" : "");
						printf("%s", ((jg.qtd_mao == 2 && mao_atual == 0) || (jg_split.qtd_mao == 2 && mao_atual == 1)) ? "Digite 0 para desistir (surrender)\n" : "");
						c = getchar();
						while ('\n' != getchar()); /* Limpa a entrada */
						if (!dobrou) {
							if (c == 'd') {
								if (!split) {
									if (aposta <= jg.dinheiro) {
										jg.mao[jg.qtd_mao++] = puxaCarta(&bar);
										jg.dinheiro -= aposta;
										aposta += aposta;
										dobrou = 1;
										sprintf(msg, "\nVocê dobrou sua aposta! Agora é tudo ou nada!!!\n");
									} else {
										sprintf(msg, "\nVocê não tem dinheiro suficiente para dobrar sua aposta!\n");
										c = 0;
									}
								} else {
									sprintf(msg, "\nVocê não pode dobrar sua aposta depois de separar suas cartas!\n");
									c = 0;
								}
							} else if (c == 'h') {
								if (mao_atual == 0) jg.mao[jg.qtd_mao++] = puxaCarta(&bar);
								else jg_split.mao[jg_split.qtd_mao++] = puxaCarta(&bar);
							} else if (c == '2' && !split && jg.qtd_mao == 2 && (jg.mao[0].valor == jg.mao[1].valor || (jg.mao[0].valor >= 9 && jg.mao[1].valor >= 9))) {
								if (!split) {
									split = 1;
									jg_split.qtd_mao = 2;
									jg_split.mao[0] = jg.mao[1];
									jg.mao[1] = puxaCarta(&bar);
									jg_split.mao[1] = puxaCarta(&bar);
									jg.dinheiro -= aposta;
								}
							}
						} else {
							if (c == 'h' || c == 'd' || c == '2') {
								sprintf(msg, "\nVocê já dobrou sua aposta! Você não pode puxar mais cartas!\n");
								c = 0;
							}
						}
					}

					if ((c == 's' || (c == '0' && ((jg.qtd_mao == 2 && mao_atual == 0) || (jg_split.qtd_mao == 2 && mao_atual == 1))))) {
						if (mao_atual == 1 || !split) {
							while (pts_mesa < 17 && c != '0') {
								mesa.mao[mesa.qtd_mao++] = puxaCarta(&bar);
								pts_mesa = pontuaMao(mesa.mao, mesa.qtd_mao);
							}
							if (split) mao_atual = 2;
							ident = 'j';
							printf("Digite qualquer coisa para continuar o jogo...");
							getch();
							while ('\n' != getchar());
						} else {
							if (c == '0') desistiu_split = 1;
							printf("Digite qualquer coisa para jogar com sua 2ª mão...");
							getch();
							while ('\n' != getchar());
							mao_atual = 1;
							continue;
						}
						titulo();
					}
					/*else if (c != 'h' && c != 'd' && c != '2') continue;*/
				}

				pts_mesa = pontuaMao(mesa.mao, mesa.qtd_mao);
				if (!split) {
					if (c == '0') {
						printf("\nVocê desistiu...\n");
						jg.dinheiro += aposta / 2;
					} else if ((pts > pts_mesa || pts_mesa > 21) && pts <= 21) {
						printf("\nParabéns você ganhou essa rodada!!\n");
						jg.dinheiro += (jg.qtd_mao == 2 && pts == 21) ? (aposta * 2) + (aposta / 2) : aposta * 2;
					} else if ((pts == pts_mesa) && pts <= 21) {
						if (pts != 21 || (jg.qtd_mao != 2 && mesa.qtd_mao != 2)) {
							printf("\nEmpate!\n");
							jg.dinheiro += aposta;
						} else {
							if (jg.qtd_mao == 2 && mesa.qtd_mao != 2) {
								printf("\nParabéns você ganhou essa rodada!!\n");
								jg.dinheiro += (aposta * 2) + (aposta / 2);
							} else if (jg.qtd_mao != 2 && mesa.qtd_mao == 2) {
								printf("\nQue pena a mesa ganhou dessa vez 😢 😢\n");
							} else {
								printf("\nEmpate!\n");
								jg.dinheiro += aposta;
							}
						}
					} else {
						printf("\nQue pena a mesa ganhou dessa vez 😢 😢\n");
					}
				} else {
					printf("\nMão 1: ");
					if (c == '0') {
						printf("Você desistiu...\n");
						jg.dinheiro += aposta / 2;
					} else if ((pts > pts_mesa || pts_mesa > 21) && pts <= 21) {
						printf("Parabéns você ganhou essa rodada!!\n");
						jg.dinheiro += (jg.qtd_mao == 2 && pts == 21) ? (aposta * 2) + (aposta / 2) : aposta * 2;
					} else if ((pts == pts_mesa) && pts <= 21) {
						if (pts != 21 || (jg.qtd_mao != 2 && mesa.qtd_mao != 2)) {
							printf("Empate!\n");
							jg.dinheiro += aposta;
						} else {
							if (jg.qtd_mao == 2 && mesa.qtd_mao != 2) {
								printf("Parabéns você ganhou essa rodada!!\n");
								jg.dinheiro += (aposta * 2) + (aposta / 2);
							} else if (jg.qtd_mao != 2 && mesa.qtd_mao == 2) {
								printf("Que pena a mesa ganhou dessa vez 😢 😢\n");
							} else {
								printf("Empate!\n");
								jg.dinheiro += aposta;
							}
						}
					} else {
						printf("Que pena a mesa ganhou dessa vez 😢 😢\n");
					}
					printf("\nMão 2: ");
					if (desistiu_split) {
						printf("Você desistiu...\n");
					} else if ((pts_split > pts_mesa || pts_mesa > 21) && pts_split <= 21) {
						printf("Parabéns você ganhou essa rodada!!\n");
						jg.dinheiro += (jg_split.qtd_mao == 2 && pts_split == 21) ? (aposta * 2) + (aposta / 2) : aposta * 2;
					} else if ((pts_split == pts_mesa) && pts_split <= 21) {
						if (pts_split != 21 || (jg_split.qtd_mao != 2 && mesa.qtd_mao != 2)) {
							printf("Empate!\n");
							jg.dinheiro += aposta;
						} else {
							if (jg_split.qtd_mao == 2 && mesa.qtd_mao != 2) {
								printf("Parabéns você ganhou essa rodada!!\n");
								jg.dinheiro += (aposta * 2) + (aposta / 2);
							} else if (jg_split.qtd_mao != 2 && mesa.qtd_mao == 2) {
								printf("Que pena a mesa ganhou dessa vez 😢 😢\n");
							} else {
								printf("Empate!");
								jg.dinheiro += aposta;
							}
						}
					} else {
						printf("Que pena a mesa ganhou dessa vez 😢 😢\n");
					}
				}

				while (1) {
					printf("\nMontante resultante: ¢%d\n\n", jg.dinheiro);
					if (jg.dinheiro == 0) {
						printf("Parece que você faliu 😢 😢\n");
						break;
					} else {
						printf("Digite c para continuar ou s para sair: ");
						c = getchar();
						while ('\n' != getchar()); /* Limpa a entrada */
					}
					if (c == 'c') {
						rodada++;
						break;
					} else if (c == 's') {
						break;
					}
				}
				if (jg.dinheiro == 0) {
					printf("Digite qualquer coisa para voltar ao menu principal...");
					getch();
					while ('\n' != getchar()); /* Limpa a entrada */
					break;
				} else if (c == 's') {
					break;
				}
			}
		} else if (c == 'c') {
			strcpy(msg, "");
			/* O jogador deve digitar um quantidade de dinheiro com a
			 * a qual iniciará o jogo. O programa repete até que um valor
			 * válido seja digitado */
			while (1) {
				titulo();
				printf("%s\n", msg);
				printf("Digite um montante padrão para as partidas:\n¢");
				fgets(input, 255, stdin);
				if (!sscanf(input, "%d", &dft_montante) || dft_montante <= 0)
					strcpy(msg, "Este valor não é válido!");
				else
					break;
			}
		} else if (c == 'i') {
			inst = fopen("inst.txt", "r");
			titulo();
			fgets(input, 255, inst);
			while(!feof(inst)) {
				printf("%s", input);
				fgets(input, 255, inst);
			}
			printf("\nDigite qualquer coisa para voltar ao menu principal...");

			getch();
			while ('\n' != getchar()); /* Limpa a entrada */
		} else {
			break;
		}
	}
	return 0;
}
