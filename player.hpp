#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *doMove2(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side ourside;
    bool strat; // true if using best strategy for competition

    bool gofirst;
    Board *board;

    int minimax(Board *board, int depth, bool maxPlayer, Move* move);
    int minimax2(Board *board, int depth, int orig, bool maxPlayer, Move* move);
    int getValue(Board *board);
    int getValue2(Board *board, Move* move, Side our);
};

#endif
