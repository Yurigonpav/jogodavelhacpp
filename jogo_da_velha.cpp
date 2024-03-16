#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define WIDTH 300
#define HEIGHT 300
#define SQUARE_SIZE 100

using namespace sf;

// Verifica se há vitória em uma linha, coluna ou diagonal
bool checkWin(const std::string& player, const std::string board[3][3]) {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true; // Linha
        }
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            return true; // Coluna
        }
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true; // Diagonal principal
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true; // Diagonal secundária
    }
    return false;
}

// Verifica se o tabuleiro está cheio (empate)
bool checkDraw(const std::string board[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == "") {
                return false;
            }
        }
    }
    return true;
}

// Algoritmo Minimax para encontrar a melhor jogada possível
int minimax(std::string player, std::string board[3][3], int depth, bool isMaximizing) {
    if (checkWin("X", board)) {
        return -1;
    }
    if (checkWin("O", board)) {
        return 1;
    }
    if (checkDraw(board)) {
        return 0;
    }

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == "") {
                    board[i][j] = player;
                    bestScore = std::max(bestScore, minimax(player == "X" ? "O" : "X", board, depth + 1, false));
                    board[i][j] = "";
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == "") {
                    board[i][j] = player;
                    bestScore = std::min(bestScore, minimax(player == "X" ? "O" : "X", board, depth + 1, true));
                    board[i][j] = "";
                }
            }
        }
        return bestScore;
    }
}

// Escolhe a melhor jogada usando o algoritmo Minimax
void makeBotMove(std::string player, std::string board[3][3]) {
    int bestScore = -1000;
    int bestMoveX = -1;
    int bestMoveY = -1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == "") {
                board[i][j] = player;
                int score = minimax(player == "X" ? "O" : "X", board, 0, false);
                board[i][j] = "";
                if (score > bestScore) {
                    bestScore = score;
                    bestMoveX = i;
                    bestMoveY = j;
                }
            }
        }
    }

    board[bestMoveX][bestMoveY] = player;
}

// Limpa o tabuleiro
void resetBoard(std::string board[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = "";
        }
    }
}

int main() {
    while (true) {
        RenderWindow window(VideoMode(WIDTH, HEIGHT), "Jogo da Velha");
        window.setFramerateLimit(60);

        std::string board[3][3];
        int turn = 0; // 0 para X, 1 para O

        Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Erro ao carregar a fonte." << std::endl;
            return 1;
        }

        srand(time(0)); // Inicializa a semente para geração de números aleatórios

        std::string difficulty;
        bool difficultySelected = false;
        bool playAgain = true;
        bool backToDifficulty = false;

        while (window.isOpen()) {
            if (playAgain && !backToDifficulty) {
                resetBoard(board);
                while (!difficultySelected && window.isOpen()) {
                    window.clear(Color::Black);

                    Text text;
                    text.setFont(font);
                    text.setCharacterSize(24);
                    text.setFillColor(Color::White);
                    text.setString("Selecione a dificuldade:\nF - Facil\nD - Dificil");
                    text.setPosition(WIDTH / 2 - 100, HEIGHT / 2 - 50);
                    window.draw(text);

                    window.display();

                    Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed) {
                            window.close();
                        }

                        if (event.type == Event::KeyPressed) {
                            if (event.key.code == Keyboard::F) {
                                difficulty = "Facil";
                                difficultySelected = true;
                            }
                            if (event.key.code == Keyboard::D) {
                                difficulty = "Dificil";
                                difficultySelected = true;
                            }
                        }
                    }
                }
            }

            while (window.isOpen()) {
                Event event;
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return 0; // Termina o programa quando a janela é fechada
                    }

                    if (event.type == Event::MouseButtonPressed && difficultySelected) {
                        if (event.mouseButton.button == Mouse::Left) {
                            int x = event.mouseButton.x / SQUARE_SIZE;
                            int y = event.mouseButton.y / SQUARE_SIZE;

                            if (x < 3 && y < 3 && board[x][y] == "") {
                                if (turn == 0) {
                                    board[x][y] = "X";
                                    if (checkWin("X", board)) {
                                        playAgain = false;
                                        break;
                                    } else if (checkDraw(board)) {
                                        playAgain = false;
                                        break;
                                    } else {
                                        turn = 1;
                                    }
                                } else {
                                    if (difficulty == "Facil") {
                                        int botX, botY;
                                        do {
                                            botX = rand() % 3;
                                            botY = rand() % 3;
                                        } while (board[botX][botY] != "");

                                        board[botX][botY] = "O";
                                        if (checkWin("O", board)) {
                                            playAgain = false;
                                            break;
                                        } else if (checkDraw(board)) {
                                            playAgain = false;
                                            break;
                                        } else {
                                            turn = 0;
                                        }
                                    } else if (difficulty == "Dificil") {
                                        makeBotMove("O", board);
                                        if (checkWin("O", board)) {
                                            playAgain = false;
                                            break;
                                        } else if (checkDraw(board)) {
                                            playAgain = false;
                                            break;
                                        } else {
                                            turn = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (event.type == Event::KeyPressed && event.key.code == Keyboard::S && !playAgain) {
                        window.close(); // Fecha a janela para iniciar uma nova instância do jogo
                    } 
                }

                window.clear(Color::Black);

                // Desenha as linhas do tabuleiro
                for (int i = 0; i < 2; ++i) {
                    Vertex line[] = {
                        Vertex(Vector2f(SQUARE_SIZE * (i + 1), 0)),
                        Vertex(Vector2f(SQUARE_SIZE * (i + 1), HEIGHT))
                    };
                    window.draw(line, 2, Lines);

                    Vertex line2[] = {
                        Vertex(Vector2f(0, SQUARE_SIZE * (i + 1))),
                        Vertex(Vector2f(WIDTH, SQUARE_SIZE * (i + 1)))
                    };
                    window.draw(line2, 2, Lines);
                }

                // Desenha as peças no tabuleiro
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        if (board[i][j] == "X") {
                            Text X("X", font, 50);
                            X.setPosition(i * SQUARE_SIZE + 30, j * SQUARE_SIZE + 30);
                            X.setFillColor(Color::White);
                            window.draw(X);
                        } else if (board[i][j] == "O") {
                            Text O("O", font, 50);
                            O.setPosition(i * SQUARE_SIZE + 30, j * SQUARE_SIZE + 30);
                            O.setFillColor(Color::White);
                            window.draw(O);
                        }
                    }
                }

                window.display();

                if (!playAgain) {
                    window.clear(Color::Black);

                    Text resultText;
                    resultText.setFont(font);
                    resultText.setCharacterSize(20);
                    resultText.setFillColor(Color::White);
                    if (checkWin("X", board)) {
                        resultText.setString("venceu!\nPressione S para jogar novamente.");
                    } else if (checkWin("O", board)) {
                        resultText.setString("perdeu!\nPressione S para jogar novamente.");
                    } else {
                        resultText.setString("Empate!\nPressione S para jogar novamente.");
                    }
                    resultText.setPosition(WIDTH / 2 - 150, HEIGHT / 2 - 50);
                    window.draw(resultText);

                    window.display();
                }
            }

            if (backToDifficulty) {
                difficultySelected = false;
                playAgain = true;
                backToDifficulty = false;
            }
        }
    }

    return 0;
}
