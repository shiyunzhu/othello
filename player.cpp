#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    ourside = side;
    if(side == BLACK)
        gofirst = true;
    else
        gofirst = false;
    // Create board object
    board = new Board();

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {

    // Delete board
    delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

    // Updates local board based on opponent's move (if they moved)
    // if we're going first then don't do the opponent's first move (since it's nullptr anyways)
    if(!gofirst){
        Side other = (ourside == BLACK) ? WHITE : BLACK;

        board->doMove(opponentsMove, other);
    }
    gofirst = false;
    Move* m = new Move(0,0);

    // Does first legal move if any by trying every possible move and returning first one that works (is legal)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            m->setX(i);
            m->setY(j);
            if (board->doMove(m, ourside)){
                return m;
            }
        }
    }

    // If no legal moves, return nullptr
    return nullptr;
}
