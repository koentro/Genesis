// ============================================================
// SPRITE COM CONTROLE
// Um personagem que anda pela tela quando você aperta a
// cruzeta do controle.
//
// Ideia: um SPRITE é uma imagem independente do fundo. O
// Mega Drive guarda a posição dele numa lista e o desenha por
// cima de tudo. Para mover, basta mudar a posição.
// ============================================================

#include <genesis.h>

// "Apresenta" ao código C as imagens e sprites do resources.res
// (arquivo gerado automaticamente na compilação).
#include "resources.h"

// Tamanho do sprite em pixels (4 tiles x 8 pixels).
#define TAM 32

// Tamanho da tela em pixels.
#define LARGURA_TELA 320
#define ALTURA_TELA  224

// Quantos pixels o personagem anda por quadro.
// 60 quadros por segundo x 2 pixels = 120 pixels por segundo.
#define VELOCIDADE 2

// (No SGDK 1.x, escreva apenas: int main())
int main(bool hardReset)
{
    // ----------------------------------------------------------
    // 1) PREPARAR O MOTOR DE SPRITES
    // Liga o sistema que gerencia a lista de sprites.
    // Precisa ser chamado UMA vez antes de usar qualquer sprite.
    // ----------------------------------------------------------
    SPR_init();

    // ----------------------------------------------------------
    // 2) CARREGAR AS CORES DO SPRITE
    // O sprite traz a própria paleta (até 16 cores). Copiamos
    // para a PAL1, para não misturar com a PAL0 (usada pelo
    // texto e pelo fundo).
    // ----------------------------------------------------------
    PAL_setPalette(PAL1, heroi.palette->data, DMA);

    // ----------------------------------------------------------
    // 3) POSIÇÃO INICIAL (centro da tela)
    // "s16" é um número com sinal, que pode ser negativo.
    // Subtraímos metade do tamanho para o CENTRO do personagem
    // ficar no centro da tela (a posição marca o canto
    // superior esquerdo do sprite).
    // ----------------------------------------------------------
    s16 x = (LARGURA_TELA - TAM) / 2;
    s16 y = (ALTURA_TELA - TAM) / 2;

    // ----------------------------------------------------------
    // 4) CRIAR O SPRITE NA TELA
    // SPR_addSprite devolve um "controle remoto" (ponteiro) para
    // esse sprite. Guardamos em "personagem" para mexer nele depois.
    //   &heroi     -> a imagem do resources.res
    //   x, y       -> posição inicial, em pixels
    //   TILE_ATTR  -> informa como desenhar:
    //     PAL1  -> usar a paleta 1
    //     FALSE -> prioridade baixa (fica atrás do plano A)
    //     FALSE -> sem espelhamento vertical
    //     FALSE -> sem espelhamento horizontal
    // ----------------------------------------------------------
    Sprite *personagem = SPR_addSprite(&heroi, x, y,
                                       TILE_ATTR(PAL1, FALSE, FALSE, FALSE));

    // Título fixo, escrito uma vez só.
    VDP_drawText("USE AS SETAS", 13, 1);

    // Caixinha para montar os textos dos números.
    char texto[8];

    while (TRUE)
    {
        // Lê os botões do controle 1 (como no projeto 2).
        u16 estado = JOY_readJoypad(JOY_1);

        // ------------------------------------------------------
        // 5) MOVER CONFORME O BOTÃO APERTADO
        // Os "if" são separados (e não "else if") de propósito:
        // assim dá para andar na DIAGONAL, apertando duas
        // direções ao mesmo tempo.
        //
        // O eixo Y cresce para BAIXO: o topo da tela é y = 0.
        // Por isso, "para cima" DIMINUI o y.
        // ------------------------------------------------------
        if (estado & BUTTON_LEFT)  x -= VELOCIDADE;
        if (estado & BUTTON_RIGHT) x += VELOCIDADE;
        if (estado & BUTTON_UP)    y -= VELOCIDADE;
        if (estado & BUTTON_DOWN)  y += VELOCIDADE;

        // ------------------------------------------------------
        // 6) NÃO DEIXAR SAIR DA TELA
        // Se passou do limite, voltamos para o limite.
        // O limite direito/inferior desconta o tamanho do sprite,
        // porque a posição marca o canto superior esquerdo.
        // ------------------------------------------------------
        if (x < 0) x = 0;
        if (x > LARGURA_TELA - TAM) x = LARGURA_TELA - TAM;
        if (y < 0) y = 0;
        if (y > ALTURA_TELA - TAM) y = ALTURA_TELA - TAM;

        // ------------------------------------------------------
        // 7) ATUALIZAR O SPRITE
        // Dois passos:
        //   SPR_setPosition -> anota a nova posição.
        //   SPR_update      -> envia a lista de sprites ao
        //                      console, de uma vez só.
        // Sem o SPR_update, o sprite não se mexe na tela.
        // ------------------------------------------------------
        SPR_setPosition(personagem, x, y);
        SPR_update();

        // Mostra a posição na tela (como no projeto 1).
        // Completamos com zeros (3 posições) para não sobrar
        // sujeira quando o número muda de tamanho.
        VDP_drawText("X:", 1, 26);
        intToStr(x, texto, 3);
        VDP_drawText(texto, 4, 26);

        VDP_drawText("Y:", 9, 26);
        intToStr(y, texto, 3);
        VDP_drawText(texto, 12, 26);

        // Espera o fim do quadro (mantém 60 voltas por segundo).
        // (No SGDK 1.x, use VDP_waitVSync() no lugar desta linha)
        SYS_doVBlankProcess();
    }

    return 0;
}