// ============================================================
// MUDANDO AS CORES
// Com o controle, você troca a cor do fundo e a cor do texto.
//   A -> próxima cor de FUNDO
//   B -> próxima cor do TEXTO
//   C -> volta às cores originais
//
// Ideia: o Mega Drive não pinta pixels direto com "vermelho"
// ou "azul". Ele usa PALETAS: tabelas de cores numeradas.
// Para mudar uma cor na tela, basta mudar a entrada da tabela.
// ============================================================

#include <genesis.h>

// ------------------------------------------------------------
// A LISTA DE CORES
// O Mega Drive tem 3 bits por canal (vermelho, verde, azul),
// ou seja, 512 cores possíveis. Cada cor é escrita assim:
//
//        0x0BGR    (B = azul, G = verde, R = vermelho)
//
// Cada letra vai de 0 a E (só números pares: 0, 2, 4... E).
// Exemplos: 0x000E = vermelho puro, 0x0E00 = azul puro.
// "const" quer dizer que a lista não muda durante o programa.
// ------------------------------------------------------------
const u16 CORES[8] =
{
    0x0000,  // preto
    0x0EEE,  // branco
    0x000E,  // vermelho
    0x00E0,  // verde
    0x0E00,  // azul
    0x00EE,  // amarelo (vermelho + verde)
    0x0EE0,  // ciano   (verde + azul)
    0x0E0E   // magenta (vermelho + azul)
};

// Os nomes, na mesma ordem das cores acima.
// Todos têm 8 caracteres (completados com espaços) para que um
// nome apague o anterior por completo ao ser desenhado.
const char *NOMES[8] =
{
    "PRETO   ", "BRANCO  ", "VERMELHO", "VERDE   ",
    "AZUL    ", "AMARELO ", "CIANO   ", "MAGENTA "
};

// (No SGDK 1.x, escreva apenas: int main())
int main(bool hardReset)
{
    // Foto dos botões neste quadro e no quadro anterior.
    u16 estado;
    u16 anterior = 0;

    // Botões que acabaram de ser apertados (explicação mais abaixo).
    u16 apertou;

    // Qual cor da lista está em uso no fundo e no texto.
    // Começamos com fundo PRETO (posição 0) e texto BRANCO (posição 1).
    u8 corFundo = 0;
    u8 corTexto = 1;

    // ----------------------------------------------------------
    // APLICANDO AS CORES INICIAIS
    // PAL_setColor(posição, cor) troca UMA entrada da paleta.
    // Na configuração padrão do SGDK:
    //   posição 0  -> cor do FUNDO
    //   posição 15 -> cor do TEXTO
    // ----------------------------------------------------------
    PAL_setColor(0, CORES[corFundo]);
    PAL_setColor(15, CORES[corTexto]);

    // Títulos fixos, escritos uma vez.
    VDP_drawText("MUDANDO AS CORES", 12, 4);
    VDP_drawText("A: muda o fundo", 10, 8);
    VDP_drawText("B: muda o texto", 10, 9);
    VDP_drawText("C: volta ao inicio", 10, 10);
    VDP_drawText("Fundo:", 10, 14);
    VDP_drawText("Texto:", 10, 16);

    while (TRUE)
    {
        // Lê os botões neste quadro.
        estado = JOY_readJoypad(JOY_1);

        // ------------------------------------------------------
        // DETECTAR O "APERTAR" (e não o "segurar")
        // Sem isto, segurar o botão A trocaria a cor 60 vezes por
        // segundo. Queremos UMA troca por aperto.
        //
        // "~anterior" inverte a foto antiga: o que estava solto
        // vira "ligado". Ao cruzar com a foto atual (&), sobram
        // só os botões que estão apertados AGORA e NÃO estavam
        // apertados no quadro passado: ou seja, acabaram de ser
        // apertados.
        // ------------------------------------------------------
        apertou = estado & ~anterior;

        // Guarda a foto atual para comparar no próximo quadro.
        anterior = estado;

        // Botão A: avança para a próxima cor de fundo.
        if (apertou & BUTTON_A)
        {
            // "% 8" dá a volta: depois da posição 7 volta ao 0.
            corFundo = (corFundo + 1) % 8;
            PAL_setColor(0, CORES[corFundo]);
        }

        // Botão B: avança para a próxima cor do texto.
        if (apertou & BUTTON_B)
        {
            corTexto = (corTexto + 1) % 8;
            PAL_setColor(15, CORES[corTexto]);
        }

        // Botão C: restaura o fundo preto e o texto branco.
        if (apertou & BUTTON_C)
        {
            corFundo = 0;
            corTexto = 1;
            PAL_setColor(0, CORES[corFundo]);
            PAL_setColor(15, CORES[corTexto]);
        }

        // Mostra o nome da cor atual ao lado de cada título.
        VDP_drawText(NOMES[corFundo], 18, 14);
        VDP_drawText(NOMES[corTexto], 18, 16);

        // Espera o fim do quadro (mantém 60 voltas por segundo).
        // (No SGDK 1.x, use VDP_waitVSync() no lugar desta linha)
        SYS_doVBlankProcess();
    }

    return 0;
}