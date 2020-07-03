#include "hexBoard.hpp"

class HexGame {
    public:
     HexGame(int);
     void getMove();
     SpaceStatus getUserColor() const;
     void gameLoop();
     ~HexGame();

    private:
     void makeAImove();
     SpaceStatus userColor;
     SpaceStatus aiColor;
     HexBoard board;
};