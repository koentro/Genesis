// ============================================================
// LENDO O CONTROLE
// O programa mostra na tela quais botões do controle estão
// sendo apertados naquele momento.
// Ideia: a cada quadro, o console "pergunta" ao controle
// quais botões estão pressionados, e nós mostramos a resposta.
// ============================================================

// Caixa de ferramentas do SGDK (texto, controle, vídeo...).
#include <genesis.h>

// ------------------------------------------------------------
// FUNÇÃO AUXILIAR
// Uma "função" é uma receita que podemos reutilizar.
// Esta mostra UM botão na tela. Em vez de repetir o mesmo código
// 8 vezes, escrevemos uma vez e chamamos várias.
//
//   nome   -> o texto do botão (ex.: "A")
//   estado -> a "foto" de todos os botões neste momento
//   mascara-> qual botão queremos checar (ex.: BUTTON_A)
//   x, y   -> onde escrever na tela (coluna e linha)
// ------------------------------------------------------------
void mostrarBotao(const char *nome, u16 estado, u16 mascara, u16 x, u16 y)
{
    // Escreve o nome do botão na posição indicada.
    VDP_drawText(nome, x, y);

    // "estado & mascara" é um filtro: ele isola só o botão que
    // nos interessa. Se o resultado for diferente de zero, o
    // botão está apertado.
    if (estado & mascara)
    {
        // Botão apertado: mostra [X].
        VDP_drawText("[X]", x + 8, y);
    }
    else
    {
        // Botão solto: mostra [ ].
        // Os dois textos têm o MESMO tamanho (3 caracteres), então
        // um apaga completamente o outro. Se tivessem tamanhos
        // diferentes, sobrariam pedaços do texto antigo na tela.
        VDP_drawText("[ ]", x + 8, y);
    }
}

// Porta de entrada do programa.
// (No SGDK 1.x, escreva apenas: int main())
int main(bool hardReset)
{
    // Caixinha que guarda a "foto" dos botões neste quadro.
    // "u16" é uma caixinha de tamanho médio. Cada botão do
    // controle corresponde a um "interruptor" dentro dela:
    // ligado = apertado, desligado = solto.
    u16 estado;

    // Título fixo, escrito uma vez só (fica fora do ciclo).
    VDP_drawText("LENDO O CONTROLE", 12, 3);
    VDP_drawText("Aperte os botoes", 12, 5);

    // Ciclo principal: repete para sempre, uma volta por quadro.
    while (TRUE)
    {
        // Pergunta ao controle 1 quais botões estão apertados
        // agora e guarda a resposta na caixinha "estado".
        // JOY_1 é a primeira porta de controle do console.
        estado = JOY_readJoypad(JOY_1);

        // Mostra cada botão, usando a função que criamos acima.
        // BUTTON_UP, BUTTON_A etc. são "máscaras" já prontas
        // no SGDK, uma para cada botão.

        // Direcional (cruzeta)
        mostrarBotao("CIMA",     estado, BUTTON_UP,    10, 9);
        mostrarBotao("BAIXO",    estado, BUTTON_DOWN,  10, 10);
        mostrarBotao("ESQUERDA", estado, BUTTON_LEFT,  10, 11);
        mostrarBotao("DIREITA",  estado, BUTTON_RIGHT, 10, 12);

        // Botões de ação (segunda coluna, mais à direita)
        mostrarBotao("A",     estado, BUTTON_A,     24, 9);
        mostrarBotao("B",     estado, BUTTON_B,     24, 10);
        mostrarBotao("C",     estado, BUTTON_C,     24, 11);
        mostrarBotao("START", estado, BUTTON_START, 24, 12);

        // Espera o fim do quadro (mantém 60 voltas por segundo).
        // (No SGDK 1.x, use VDP_waitVSync() no lugar desta linha)
        SYS_doVBlankProcess();
    }

    return 0;
}