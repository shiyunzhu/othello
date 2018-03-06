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

    // Array for legal moves
    vector<Move*> legal;

    Move* m;
    // Finds all of the legal moves and adds them to the vector
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            m = new Move(i, j);
            if (board->checkMove(m, ourside)){
                legal.push_back(m);
            }
            
        }
    }

    int highest = -100;
    int current = -99;
    Move* best = nullptr;
    // Calculate score of moves
    for (unsigned int i = 0; i < legal.size(); i++) {
        Board *temp = board->copy();
        temp->doMove(legal[i], ourside);
        //cerr << "Current highest: " << highest << endl;
        //cerr << "Current Move: " << legal[i]->getX() << " , " << legal[i]->getY() << endl;
        current = temp->count(ourside);
        if(legal[i]->getX() == 0 || legal[i]->getX() == 7){
            if(legal[i]->getY() == 0 || legal[i]->getY() == 7)
                current += 15;
            else{
                if(legal[i]->getY() == 1 || legal[i]->getY() == 6)
                    current -= 5;
                else current += 8;
            }
        }
        else{
            if(legal[i]->getY() == 0 || legal[i]->getY() == 7){
                if(legal[i]->getX() == 0 || legal[i]->getX() == 7)
                    current += 10;
            else{
                if(legal[i]->getX() == 1 || legal[i]->getX() == 6)
                    current -= 5;
                else current += 5;
                }
            }
            else{
                if(legal[i]->getX() == 1 || legal[i]->getX() == 6)
                    if(legal[i]->getY() == 1 || legal[i]->getY() == 6)
                        current -= 15;
            }
        }
        
        //cerr << "Current score: " << current << endl;
    
        if(current > highest){
            best = legal[i];
            highest = current;
            //cerr << "New highest: " << highest << endl;
        }
    }
    //cerr << "Chosen Move: " << best->getX() << " , " << best->getY() << endl;
    board->doMove(best, ourside);
    //cerr << endl; //
    return best;

    // If no legal moves, return nullptr
    return nullptr;
}
// This is the doMove that generates moves made randomly 
Move *Player::doMove2(Move *opponentsMove, int msLeft) {
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
