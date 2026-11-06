// ============================================================
// IMAGEM DE FUNDO
// Mostra uma imagem PNG na tela inteira.
//
// Ideia: na hora de compilar, uma ferramenta do SGDK chamada
// "rescomp" lê o seu PNG e o converte para o formato que o
// Mega Drive entende (pedacinhos de 8x8 pixels, chamados TILES,
// mais uma PALETA de cores). Nós só pedimos para desenhar.
// ============================================================

#include <genesis.h>

// Este arquivo é CRIADO AUTOMATICAMENTE na compilação, a partir
// do resources.res. Ele "apresenta" ao código C as imagens
// listadas lá (no nosso caso, a "fundo").
#include "resources.h"

// (No SGDK 1.x, escreva apenas: int main())
int main(bool hardReset)
{
    // ----------------------------------------------------------
    // 1) CARREGAR AS CORES DA IMAGEM
    // Cada imagem traz junto a sua paleta (até 16 cores).
    // Aqui copiamos essa paleta para a PAL0, a primeira das 4
    // paletas do console.
    //   PAL0              -> qual paleta receberá as cores
    //   fundo.palette->data -> as cores da imagem
    //   DMA               -> método rápido de transferência
    // Sem este passo, a imagem apareceria com cores erradas.
    // ----------------------------------------------------------
    PAL_setPalette(PAL0, fundo.palette->data, DMA);

    // ----------------------------------------------------------
    // 2) DESENHAR A IMAGEM
    // O Mega Drive tem dois "planos" de fundo, como duas folhas
    // transparentes empilhadas: BG_A (na frente) e BG_B (atrás).
    // Desenhamos a imagem no BG_B.
    //   &fundo -> o endereço da imagem (o "&" é obrigatório)
    //   0, 0   -> posição do canto superior esquerdo, em TILES
    //             (coluna 0, linha 0 = canto da tela)
    // ----------------------------------------------------------
    VDP_drawImage(BG_B, &fundo, 0, 0);

    // ----------------------------------------------------------
    // 3) CICLO PRINCIPAL
    // A imagem já está na tela e fica lá sozinha. Só precisamos
    // manter o programa vivo, esperando cada quadro.
    // ----------------------------------------------------------
    while (TRUE)
    {
        // (No SGDK 1.x, use VDP_waitVSync() no lugar desta linha)
        SYS_doVBlankProcess();
    }

    return 0;
}