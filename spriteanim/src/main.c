// ============================================================
// SPRITE ANIMADO
// Evolução do projeto 5: o personagem agora tem duas animações.
//   - PARADO (idle): quando nenhum botão direcional é apertado
//   - ANDANDO (walk): quando a cruzeta está sendo apertada
// Além disso, ele vira para o lado em que está andando.
// ============================================================

#include <genesis.h>
#include "resources.h"

// Tamanho de um quadro do sprite, em pixels.
#define TAM 32

// Tamanho da tela em pixels.
#define LARGURA_TELA 320
#define ALTURA_TELA  224

// Pixels por quadro de jogo (2 x 60 = 120 pixels por segundo).
#define VELOCIDADE 2

// ------------------------------------------------------------
// NOMES PARA AS ANIMAÇÕES
// Cada animação é identificada pelo número da LINHA da folha de
// sprites. Dar nomes aos números deixa o código legível:
// em vez de "SPR_setAnim(p, 1)", lemos "SPR_setAnim(p, ANIM_WALK)".
// ------------------------------------------------------------
#define ANIM_IDLE 0   // linha 0 da folha: parado
#define ANIM_WALK 1   // linha 1 da folha: andando

// (No SGDK 1.x, escreva apenas: int main())
int main(bool hardReset)
{
    // Liga o sistema de sprites (uma vez só).
    SPR_init();

    // Carrega a paleta do sprite na PAL1.
    PAL_setPalette(PAL1, heroi.palette->data, DMA);

    // Posição inicial: centro da tela.
    s16 x = (LARGURA_TELA - TAM) / 2;
    s16 y = (ALTURA_TELA - TAM) / 2;

    // Cria o sprite e guarda o "controle remoto" dele.
    Sprite *personagem = SPR_addSprite(&heroi, x, y,
                                       TILE_ATTR(PAL1, FALSE, FALSE, FALSE));

    // ----------------------------------------------------------
    // MEMÓRIA DO PERSONAGEM
    // Guardamos duas informações entre um quadro e outro:
    //   animAtual -> qual animação está rodando agora
    //   olhandoEsq-> TRUE se ele está virado para a esquerda
    // ----------------------------------------------------------
    u8   animAtual  = ANIM_IDLE;
    bool olhandoEsq = FALSE;

    // Começa tocando a animação parada.
    SPR_setAnim(personagem, ANIM_IDLE);

    VDP_drawText("USE A CRUZETA", 13, 1);

    while (TRUE)
    {
        u16 estado = JOY_readJoypad(JOY_1);

        // ------------------------------------------------------
        // 1) DESCOBRIR SE ELE ESTÁ SE MOVENDO
        // Começamos supondo que está parado. Se algum botão
        // direcional estiver apertado, mudamos para "TRUE".
        // ------------------------------------------------------
        bool andando = FALSE;

        if (estado & BUTTON_LEFT)
        {
            x -= VELOCIDADE;
            andando = TRUE;
            olhandoEsq = TRUE;    // virou para a esquerda
        }
        if (estado & BUTTON_RIGHT)
        {
            x += VELOCIDADE;
            andando = TRUE;
            olhandoEsq = FALSE;   // virou para a direita
        }
        if (estado & BUTTON_UP)
        {
            y -= VELOCIDADE;
            andando = TRUE;
        }
        if (estado & BUTTON_DOWN)
        {
            y += VELOCIDADE;
            andando = TRUE;
        }

        // Limites da tela (como no projeto 5).
        if (x < 0) x = 0;
        if (x > LARGURA_TELA - TAM) x = LARGURA_TELA - TAM;
        if (y < 0) y = 0;
        if (y > ALTURA_TELA - TAM) y = ALTURA_TELA - TAM;

        // ------------------------------------------------------
        // 2) ESCOLHER A ANIMAÇÃO
        // Operador "? :" é um "if" de uma linha:
        //   condição ? valor_se_verdadeiro : valor_se_falso
        // ------------------------------------------------------
        u8 animDesejada = andando ? ANIM_WALK : ANIM_IDLE;

        // ------------------------------------------------------
        // 3) TROCAR DE ANIMAÇÃO SÓ QUANDO MUDAR
        // Se chamássemos SPR_setAnim a cada quadro, a animação
        // poderia reiniciar o tempo todo e ficaria presa no
        // primeiro quadro. Por isso comparamos antes:
        // só trocamos se a animação desejada for DIFERENTE da atual.
        // ------------------------------------------------------
        if (animDesejada != animAtual)
        {
            animAtual = animDesejada;
            SPR_setAnim(personagem, animAtual);
        }

        // ------------------------------------------------------
        // 4) ESPELHAR O SPRITE CONFORME A DIREÇÃO
        // SPR_setHFlip vira a imagem na horizontal. Assim
        // desenhamos o personagem só olhando para a direita e o
        // console faz o espelho para a esquerda, de graça.
        // ------------------------------------------------------
        SPR_setHFlip(personagem, olhandoEsq);

        // ------------------------------------------------------
        // 5) POSIÇÃO E ENVIO PARA A TELA
        // O SGDK avança os quadros da animação sozinho, usando o
        // tempo (o "6" do resources.res), quando chamamos
        // SPR_update.
        // ------------------------------------------------------
        SPR_setPosition(personagem, x, y);
        SPR_update();

        // Espera o fim do quadro (60 voltas por segundo).
        // (No SGDK 1.x, use VDP_waitVSync() no lugar desta linha)
        SYS_doVBlankProcess();
    }

    return 0;
}