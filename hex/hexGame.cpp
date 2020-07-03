#include "hexGame.hpp"
#include <iostream>
#include <string>
#include <regex>

using std::cout, std::cin, std::endl;

HexGame::HexGame(int boardSize): board(HexBoard(boardSize)) {
 
}

void HexGame::getMove() {

}

SpaceStatus HexGame::getUserColor() const {
    return userColor;
}

std::pair<int, int> getCoords(std::string rawInput) {
    size_t pos = rawInput.find(" ");
    if (pos == std::string::npos) {
        pos = rawInput.find(",");
    }
    int first = std::stoi(rawInput.substr(0, pos));
    int second = std::stoi(rawInput.substr(pos + 1));
    return std::pair(first, second);
}

void HexGame::gameLoop() {
    cout << "Started game with " << board.getSize() << "x" << board.getSize() << " board." << endl;
    cout << "Choose a color: 1 for Blue, 2 for Red: ";
    std::string rawUserColor;
    cin >> rawUserColor;
    while (rawUserColor != "1" && rawUserColor != "2") {
        cout << "Please enter a valid option: 1 for Blue, 2 for Red: ";
        cin >> rawUserColor;
    }
    userColor = static_cast<SpaceStatus>(std::stoi(rawUserColor));
    aiColor = userColor == SpaceStatus::RED ? SpaceStatus::BLUE : SpaceStatus::RED;
    cin.ignore(); // ignore remaining \n from color input

    std::regex validCoords("\\d+(\\s+|\\s*,\\s*)\\d+");
    std::string rawUserMove;

    while (board.getWinner() == SpaceStatus::EMPTY) {
        cout << "=======================" << endl << endl;
        cout << "Enter coordinates of your next move, for example \"1 2\" or \"1, 5\": ";
        
        std::getline(cin, rawUserMove);

        while (!std::regex_match(rawUserMove, validCoords)) {
            cout << "Please enter valid coordinates (two numbers separated by a space or comma): ";
            std::getline(cin, rawUserMove);
        }

        auto [i, j] = getCoords(rawUserMove);

        if (board.isLegalMove(i - 1, j - 1, userColor)) {
            board.setValue(i - 1, j - 1, userColor);
            makeAImove();
            board.printBoard();
        }
        else {
            cout << "ERROR: Please enter a valid move." << endl;
        }
        cout << endl;
    }

    SpaceStatus winner = board.getWinner();
    cout << "WINNER! - " << (winner == SpaceStatus::RED ? "Red" : "Blue") << endl;
}

void HexGame::makeAImove() {
    // Placeholder: make random move
    static int moveRow = 0;
    static int moveCol = 0;

    while (!board.isLegalMove(moveRow, moveCol, aiColor)) {
        if (moveCol + 1 < board.getSize()) {
            ++moveCol;
        }
        else {
            ++moveRow;
            moveCol = 0;
        }
        if (moveRow >= board.getSize()) {
            moveRow = 0;
            moveCol = 0;
        }
    }
    board.setValue(moveRow, moveCol, aiColor);
}

HexGame::~HexGame() {

}

int main(int argc, char **argv) {
    HexGame game(7);
    game.gameLoop();
}
