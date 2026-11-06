// ============================================================
// CONTADOR DE FRAMES
// O programa conta quantas "imagens" (quadros) o console já
// desenhou e mostra esse número na tela.
// Pense num flipbook: cada página virada é um quadro.
// ============================================================

// Pega emprestada a "caixa de ferramentas" do SGDK.
// Nela já vêm prontas as funções de escrever na tela,
// ler o controle, tocar som, etc.
#include <genesis.h>

// "main" é a porta de entrada do programa.
// Quando o Mega Drive liga, ele começa a executar daqui,
// de cima para baixo, linha por linha.
// (No SGDK 1.x, escreva apenas: int main())
int main(bool hardReset)
{
    // ----------------------------------------------------------
    // 1) PREPARANDO AS "CAIXINHAS" (variáveis)
    // ----------------------------------------------------------

    // Caixinha chamada "frames", começando com o número 0.
    // Vai guardar quantos quadros já foram desenhados.
    // "u32" é o tamanho da caixinha: cabe um número bem grande.
    u32 frames = 0;

    // Caixinha chamada "segundos", também começando em 0.
    u32 segundos = 0;

    // Caixinha especial para guardar TEXTO (até 16 letras/números).
    // O computador trata o número 42 (para fazer contas) de forma
    // diferente do texto "42" (dois desenhos para mostrar na tela).
    // Esta caixinha guarda a versão "desenhável".
    char texto[16];

    // ----------------------------------------------------------
    // 2) ESCREVENDO OS TÍTULOS FIXOS (só uma vez)
    // ----------------------------------------------------------

    // VDP_drawText escreve um texto na tela.
    // Os dois números depois do texto são o endereço, como numa
    // folha quadriculada: (coluna, linha).
    // A tela tem 40 colunas e 28 linhas.
    VDP_drawText("CONTADOR DE FRAMES", 11, 8);

    // Estes títulos nunca mudam, por isso ficam FORA do loop.
    // Escrever uma vez é suficiente.
    VDP_drawText("Frames:", 10, 12);
    VDP_drawText("Segundos:", 10, 14);

    // ----------------------------------------------------------
    // 3) O CICLO PRINCIPAL (repete para sempre)
    // ----------------------------------------------------------

    // "while (TRUE)" significa "repita para sempre".
    // Tudo entre as chaves { } roda de novo e de novo, até
    // desligar o console. Todo jogo funciona assim.
    // Cada volta do ciclo = um quadro na tela.
    while (TRUE)
    {
        // Soma 1 na caixinha "frames".
        // É como anotar: "mais uma página virada".
        frames++;

        // Transforma o número da caixinha "frames" em texto
        // e guarda na caixinha "texto".
        // O 7 quer dizer: use pelo menos 7 posições, completando
        // com zeros à esquerda (o 42 vira "0000042").
        // Isso evita sobras do número anterior na tela.
        uintToStr(frames, texto, 7);

        // Escreve o texto na tela, na coluna 20, linha 12,
        // logo ao lado do título "Frames:".
        VDP_drawText(texto, 20, 12);

        // O console desenha 60 quadros por segundo.
        // Então: segundos = frames dividido por 60.
        // Ex.: 180 quadros = 3 segundos.
        // Em contas com números inteiros o resto é descartado:
        // 150 quadros dão 2 segundos (e não 2,5).
        segundos = frames / 60;

        // Mesmo processo: converte em texto (mínimo de 4
        // posições) e escreve na tela ao lado de "Segundos:".
        uintToStr(segundos, texto, 4);
        VDP_drawText(texto, 20, 14);

        // "ESPERE A PRÓXIMA PÁGINA".
        // Faz o programa aguardar a tela terminar de ser
        // desenhada antes de voltar ao início do ciclo.
        // É isso que mantém o ritmo de 60 voltas por segundo.
        // Sem esta linha, o contador subiria muito mais rápido.
        // (No SGDK 1.x, use VDP_waitVSync() no lugar desta linha)
        SYS_doVBlankProcess();
    }

    // Formalidade da linguagem C: a função "main" precisa
    // devolver um valor. Esta linha nunca é alcançada, porque
    // o ciclo acima não termina.
    return 0;
}