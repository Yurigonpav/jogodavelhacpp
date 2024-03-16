# Jogo da Velha em C++ com SFML

Este é um simples jogo da velha implementado em C++ utilizando a biblioteca SFML para renderização gráfica. O jogo permite que o usuário jogue contra um bot que utiliza o algoritmo Minimax para tomar suas decisões.

## Funcionalidades

- Interface gráfica usando SFML
- Modo de um jogador contra o bot (dificuldades Fácil e Difícil)
- Verificação automática de vitória, derrota ou empate
- Opção de jogar novamente após o término do jogo

## Como Jogar

1. Execute o programa.
2. Selecione a dificuldade pressionando F para Fácil ou D para Difícil.
3. Clique nas células vazias para fazer sua jogada.
4. O jogo continuará até que haja um vencedor ou empate.
5. Após o término do jogo, pressione S para jogar novamente.

## Pré-requisitos

- SFML (Simple and Fast Multimedia Library)
- Compilador C++ compatível

## Como Compilar

Certifique-se de ter a biblioteca SFML instalada e configurada corretamente. Depois, utilize um compilador C++ para compilar o código-fonte.

Por exemplo, no Linux com g++:

```bash
g++ jogo_da_velha.cpp -o jogo_da_velha -lsfml-graphics -lsfml-window -lsfml-system
