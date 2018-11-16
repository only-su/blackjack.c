/* Codigos Unicode para os simbolos dos naipes */
#define ESPADA "\xE2\x99\xA4"
#define PAUS   "\xE2\x99\xA7"
#define COPAS  "\xE2\x99\xA5"
#define OURO   "\xE2\x99\xA6"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Estrutura da carta */
typedef struct _CARTA {
  int valor;
  char naipe[20];
} CARTA;

/* Estrutura do baralho */
typedef struct _BARALHO {
  CARTA cartas[52];
  int qtd;
} BARALHO;

/* Estrutura do jogador */
typedef struct _JOGADOR {
  CARTA mao[15];
  int qtd_mao;
  int dinheiro;
} JOGADOR;

/* Traduz o valor inteiro da carta em uma string (A,1,2,3,4,5,6,7,8,9,10,J,Q ou K) */
char* traduz_valor(int valor, char *retorno) {
  switch(valor) {
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
      sprintf(retorno, "%d", valor +1);
  }
  return retorno;
}

/* Traduz o valor interio do naipe da carta na string Unicode do simbolo do naipe */
char* traduz_naipe(int naipe, char *retorno) {
  switch(naipe) {
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
int pontuaMao(CARTA c[], int qtd) {
  int i, pts = 0;
  for(i = 0; i < qtd; i++) {
    if(c[i].valor == 0) {
      /* De acordo com as regras do 21, o valor da carta A pode ser 1 ou 11
       dependendo do que beneficia mais o jogador */
      if(pts + 11 <= 21) pts += 11;
      else pts += 1;
    }
    else if(c[i].valor >= 9) pts += 10;
    else pts += c[i].valor + 1;
  }
  return pts;
}

/* Gera e retorna a string de uma carta */
char *string_carta(CARTA car) {
  char *str;
  char scarta[5];
  str = (char *) malloc(sizeof(char)*15);
  traduz_valor(car.valor, scarta);
  sprintf(str, "%2s%s", scarta, car.naipe);
  return str;
}

/* Inicia e retorna um baralho */
BARALHO iniciar_baralho() {
  BARALHO bar;
  int i;
  for(i = 0; i < 52; i++) {
    traduz_naipe(i/13, bar.cartas[i].naipe);
    bar.cartas[i].valor = i%13;
  }
  bar.qtd = 52;
  return bar;
}

/* Imprime um baralho inteiro em ordem (Usada para testes) */
void imprimir_baralho(BARALHO bar) {
  int i;
  for(i = 0; i < bar.qtd;i++)
  {
    printf("%s", string_carta(bar.cartas[i]));
  }
}

/* Embaralha um baralho */
void embaralhar(BARALHO *bar) {
  int i, r;
  CARTA aux;
  srand(time(0));
  for(i = 0; i < bar->qtd;i++) {
    r = rand()%52;
    aux = bar->cartas[i];
    bar->cartas[i] = bar->cartas[r];
    bar->cartas[r] = aux;
  }
}

/* Retira e retorna uma carta do baralho */
CARTA puxaCarta(BARALHO *bar) {
  CARTA c = bar->cartas[--bar->qtd];
  return c;
}

/* Cria a mao inicial de um jogador */
void iniciar_mao(JOGADOR *jg, BARALHO *bar) {
  jg->mao[0] = puxaCarta(bar);
  jg->mao[1] = puxaCarta(bar);
  jg->qtd_mao = 2;
}

int main(void) {
  int i, pts, pts_mesa, aposta, rodada;
  char c;
  BARALHO bar;
  JOGADOR jg, mesa;
  system("clear");

  /* O jogo inicia com um menu tendo como opções começar o jogo ou sair do programa */
  printf("Bem-vindo ao Blackjack em C\n\nDigite 1 para comecar o jogo\nDigite qualquer outra tecla para sair\n");
  c = getchar();
  while('\n' != getchar()); /* Limpa a entrada */
  system("clear");
  if(c != '1') return 0;

  /* O jogador deve digitar um quantidade de dinheiro coma a qual iniciará o jogo. O programa repete até que um valor válido seja digitado */
  while(1) {
    printf("Digite um montante inicial para você:\n");
    if(!scanf("%d", &jg.dinheiro) || jg.dinheiro <= 0) {
      while('\n' != getchar()); /* Limpa a entrada */
      system("clear");
      printf("Este valor não é válido para a aposta!\n");
    }
    else break;
  }
  system("clear");

  rodada = 1; /* O programa começa a contar as rodadas */
  while(1) {
    bar = iniciar_baralho();
    embaralhar(&bar);
    system("clear");

    /* O programa pede um valor para ser apostado nessa rodada.
       Se o jogador ganhar a rodada ele ganha o dobro da aposta, se perder ele não ganha nada.
       O programa repete até que um valor válido seja digitado. */
    while(1) {
      printf("Rodada %d\nMontante atual: $%d\nDigite um valor para a aposta dessa rodada:\n", rodada, jg.dinheiro);
      if(!scanf("%d", &aposta) || aposta > jg.dinheiro || aposta <= 0) {
        while('\n' != getchar()); /* Limpa a entrada */
        system("clear");
        printf("Este valor não é válido para a aposta!\n");
      }
      else break;
    }
    while('\n' != getchar()); /* Limpa a entrada */
    jg.dinheiro -= aposta; /* Remove a aposta do montante do jogador */

    /* Inicia a mao do jogador e da mesa */
    iniciar_mao(&jg, &bar);
    iniciar_mao(&mesa, &bar);

    /* O programa repete ate que o jogador deseje abaixar a mao, que a pontuacao seja igual a 21 ou que a pontuacao passe de 21 */
    while(1) {
      system("clear");
      /* Mostra a mao do jogador e da mesa. Uma das cartas da mesa fica virada. */
      printf("Rodada %d\nAposta atual: $%d\n\nMão da mesa:\n ??\n%s\n\nSua mão:\n", rodada, aposta, string_carta(mesa.mao[0]));
      for(i = 0; i < jg.qtd_mao; i++) {
        printf("%s\n", string_carta(jg.mao[i]));
      }
      pts = pontuaMao(jg.mao, jg.qtd_mao);
      printf("\nPontuação atual: %d\n", pts);

      /* Se a pontuacao do jogar for igual a 21 ele automaticamente ganha, se for maior que 21 ele automaticamente perde */
      if(pts > 21) {
        printf("\nA pontuação estourou!\nVocê perdeu!\n");
        break;
      }
      else if(pts == 21) {
        printf("\nA pontuação deu 21!\nVocê ganhou!\n");
        break;
      }

      while(1) {
        printf("\nDigite 1 para puxar outra carta\nDigite 2 para abaixar a mao\n");
        c = getchar();
        while('\n' != getchar()); /* Limpa a entrada */
        if(c == '1') {
          jg.mao[jg.qtd_mao++] = puxaCarta(&bar);
          break;
        }
        else if (c == '2') break;
      }
      if(c == '2') break;
    }
    if(pts == 21) jg.dinheiro += aposta*2;
    else if(pts < 21) {
      /* codigo da pontuacao da mesa */
    }

    while(1) {
      /* adicionar verificacao para ver se o montante eh 0 */
      printf("\nMontante resultante:%d\nDeseja jogar outra rodada(s/n):\n", jg.dinheiro);
      c = getchar();
      while('\n' != getchar()); /* Limpa a entrada */
      if(c == 's') {
        rodada++;
        break;
      }
      else if (c == 'n') break;
    }
    if(c == 'n') break;
  }
  return 0;
}
