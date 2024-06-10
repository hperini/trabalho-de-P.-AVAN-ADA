#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

bool gameOver;
bool inMenu;
const int largura = 40;
const int altura = 20;
int* x, *y, *frutaX, *frutaY, pontuacao;
int* caldaX;
int* caldaY;
int tamanhoCalda;
int velocidade; // Velocidade da cobra
enum eDirecao { PARADO = 0, ESQUERDA, DIREITA, CIMA, BAIXO };
eDirecao direcao;

void Setup() {
    gameOver = false;
    inMenu = true;
    direcao = PARADO;
    x = new int;
    y = new int;
    frutaX = new int;
    frutaY = new int;
    caldaX = new int[100];
    caldaY = new int[100];
    *x = largura / 2;
    *y = altura / 2;
    *frutaX = rand() % largura;
    *frutaY = rand() % altura;
    pontuacao = 0;
    tamanhoCalda = 0;
    velocidade = 100; // Velocidade padrão
}

void LimparMemoria() {
    delete x;
    delete y;
    delete frutaX;
    delete frutaY;
    delete[] caldaX;
    delete[] caldaY;
}

void DesenharMenu() {
    system("cls");
    cout << "======================" << endl;
    cout << "       MENU           " << endl;
    cout << "======================" << endl;
    cout << "1. Jogar" << endl;
    cout << "2. Instrucoes" << endl;
    cout << "3. Selecionar Nivel" << endl;
    cout << "4. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

void SelecionarNivel() {
    system("cls");
    cout << "======================" << endl;
    cout << "    SELECIONAR NIVEL  " << endl;
    cout << "======================" << endl;
    cout << "Escolha o nivel de dificuldade: " << endl;
    cout << "1. Facil (Lenta)" << endl;
    cout << "2. Medio (Moderada)" << endl;
    cout << "3. Dificil (Rapida)" << endl;
    cout << "Escolha uma opcao: ";

    char escolha = _getch();
    switch (escolha) {
    case '1':
        velocidade = 150; // Velocidade lenta
        break;
    case '2':
        velocidade = 75; // Velocidade moderada
        break;
    case '3':
        velocidade = 50; // Velocidade rápida
        break;
    default:
        break;
    }

    cout << "Nivel de dificuldade definido. Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

void Desenhar() {
    HANDLE hOut;
    COORD Pos;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Pos.X = 0;
    Pos.Y = 0;
    SetConsoleCursorPosition(hOut, Pos);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false; // Oculta o cursor
    SetConsoleCursorInfo(hOut, &cursorInfo);

    for (int i = 0; i < largura + 2; i++)
        cout << "\xB2";
    cout << endl;

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (j == 0)
                cout << "\xB2";
            if (i == *y && j == *x)
                cout << "O";
            else if (i == *frutaY && j == *frutaX)
                cout << "*"; // Alterado para representar uma fruta
            else {
                bool print = false;
                for (int k = 0; k < tamanhoCalda; k++) {
                    if (caldaX[k] == j && caldaY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if (j == largura - 1)
                cout << "\xB2";
        }
        cout << endl;
    }

    for (int i = 0; i < largura + 2; i++)
        cout << "\xB2";
    cout << endl;
    cout << "Pontuacao:" << pontuacao << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            direcao = ESQUERDA;
            break;
        case 'd':
            direcao = DIREITA;
            break;
        case 'w':
            direcao = CIMA;
            break;
        case 's':
            direcao = BAIXO;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Logica() {
    int prevX = caldaX[0];
    int prevY = caldaY[0];
    int prev2X, prev2Y;
    caldaX[0] = *x;
    caldaY[0] = *y;
    for (int i = 1; i < tamanhoCalda; i++) {
        prev2X = caldaX[i];
        prev2Y = caldaY[i];
        caldaX[i] = prevX;
        caldaY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direcao) {
    case ESQUERDA:
        (*x)--;
        break;
    case DIREITA:
        (*x)++;
        break;
    case CIMA:
        (*y)--;
        break;
    case BAIXO:
        (*y)++;
        break;
    default:
        break;
    }

    if (*x >= largura || *x < 0 || *y >= altura || *y < 0)
        gameOver = true;

    for (int i = 0; i < tamanhoCalda; i++)
        if (caldaX[i] == *x && caldaY[i] == *y)
            gameOver = true;

    if (*x == *frutaX && *y == *frutaY) {
        pontuacao += 10;
        *frutaX = rand() % largura;
        *frutaY = rand() % altura;
        tamanhoCalda++;
    }
}

void AtualizarTela() {
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
}

void MostrarMensagemGameOver() {
    system("cls");
    cout << "Game Over! Pontuacao final: " << pontuacao << endl;
}

void MostrarPontuacao() {
    system("cls");
    cout << "======================" << endl;
    cout << "      PONTUACAO       " << endl;
    cout << "======================" << endl;
    cout << "Sua pontuacao final foi: " << pontuacao << endl;
    cout << "Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

void Jogar() {
    inMenu = false;
    gameOver = false; // Reinicia a variável de controle do jogo
    Setup(); // Reinicializa as variáveis do jogo
    while (!gameOver) {
        Desenhar();
        Input();
        Logica();
        Sleep(velocidade); // Utiliza a velocidade definida pelo jogador
        AtualizarTela();
    }
    MostrarMensagemGameOver();
    MostrarPontuacao(); // Mostra a pontuação após o fim da partida
}

void Instrucoes() {
    system("cls");
    cout << "======================" << endl;
    cout << "    INSTRUCOES       " << endl;
    cout << "======================" << endl;
    cout << "Use as teclas WASD para mover a cobra." << endl;
    cout << "Coma a fruta para ganhar pontos." << endl;
    cout << "Nao colida com as bordas ou com a propria calda." << endl;
    cout << "Pressione X para sair a qualquer momento." << endl;
    cout << "Pressione qualquer tecla para voltar ao menu." << endl;
    _getch();
}

int main() {
    HWND console = GetConsoleWindow();
    SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_SYSMENU & ~WS_MINIMIZEBOX);
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

    while (true) {
        DesenharMenu();
        char escolha = _getch();
        switch (escolha) {
        case '1':
            Jogar();
            break;
        case '2':
            Instrucoes();
            break;
        case '3':
            SelecionarNivel();
            break;
        case '4':
            LimparMemoria();
            return 0;
        default:
            break;
        }
    }
    LimparMemoria();
    return 0;
}


