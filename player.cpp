#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    strat = true;
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


    // Updates local board based on opponent's move (if they moved)
    // if we're going first then don't do the opponent's first move (since it's nullptr anyways)

    Side other = (ourside == BLACK) ? WHITE : BLACK;
    if(!gofirst){

        board->doMove(opponentsMove, other);
    }
    gofirst = false;
    
    // uses testingmax2, for tournament play
    if(strat){
        Move * move = new Move(-1, -1);
        int best = minimax2(board, 5, 5, true, move);
        if (move->getX() == -1) {
            move = nullptr;
        }
        board->doMove(move, ourside);
        return move;
    }

    // Uses minimax when testingMinimax is true else goes to heuristic
    if (testingMinimax) {
        Move * move = new Move(-1, -1);
        int best = minimax(board, 2, true, move);
        if (move->getX() == -1) {
            move = nullptr;
        }
        board->doMove(move, ourside);
        return move;
    }



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
        
    

        if(current > highest){
            best = legal[i];
            highest = current;
        }
    }

    board->doMove(best, ourside);

    

    return best;

    // If no legal moves, return nullptr
    return nullptr;
}
// This is the doMove that generates moves made randomly 
Move *Player::doMove2(Move *opponentsMove, int msLeft) {


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

// This is minimax recursive function
int Player::minimax(Board *board, int depth, bool maxPlayer, Move* move) {

    // uses getValue to return the value of the board at lowest level
    if (depth == 0) {
        return getValue(board);
    }

    // sets Side other to the other side
    Side other = (ourside == BLACK) ? WHITE : BLACK;

    // if maximum player is true, then calculate the maximum value
    if (maxPlayer) {

        Move *m = new Move(0, 0);
        int best = -999999;

        // check all of the possible legal moves
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                m->setX(i);
                m->setY(j);

                // create new board
                Board *temp = new Board();
                temp = board->copy();

                // if we can do the move, do the move
                if (temp->doMove(m, ourside)) {

                    // recursively find the score
                    int score = minimax(temp, depth-1, false, move);

                    // compare score to the best score
                    if (score > best) {
                        best = score;
                        move->setX(i);
                        move->setY(j);
                    }
                }

                // clean up memory
                delete temp;
            }
        }
        delete m;
        return best;
    }

    // calculate the minimum value, since its the other player's turn
    else { 

        Move *m = new Move(0, 0);
        int best2 = 999999;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                m->setX(i);
                m->setY(j);

                Board *temp = new Board();
                temp = board->copy();

                if (temp->checkMove(m, other)) {
                    temp->doMove(m, other);
                    int score = minimax(temp, depth-1, true, move);

                    if (score < best2) {
                        best2 = score;
                    }
                }
                delete temp;
            }
        }
        delete m;
        return best2;
    }
}

// This is minimax recursive function
int Player::minimax2(Board *board2, int depth, int orig, bool maxPlayer, Move* move) {
    Side tempo = ourside;
    Side other = (ourside == BLACK) ? WHITE : BLACK;
    if(!maxPlayer)
        tempo = other;
        
    // uses getValue to return the value of the board at lowest level
    if (depth == 0) {
        if(maxPlayer)
            return getValue2(board2, move, tempo);
        return -1 * getValue2(board2, move, tempo);
    }

    // sets Side other to the other side
    //Side other = (ourside == BLACK) ? WHITE : BLACK;

    // if maximum player is true, then calculate the maximum value
    if (maxPlayer) {

        Move *m = new Move(0, 0);
        int best = -999999;

        // check all of the possible legal moves
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                m->setX(i);
                m->setY(j);

                // create new board
                Board *temp = new Board();
                temp = board2->copy();

                // if we can do the move, do the move
                if (temp->doMove(m, ourside)) {

                    // recursively find the score
                    int score = minimax2(temp, depth-1, orig, false, move);

                    // compare score to the best score
                    if (score >= best) {
                        best = score;
                        if(depth == orig){
                            move->setX(i);
                            move->setY(j);  
                        }
                    }
                }

                // clean up memory
                delete temp;
            }
        }
        delete m;
        return best;
    }

    // calculate the minimum value, since its the other player's turn
    else { 

        Move *m = new Move(0, 0);
        int best2 = 999999;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                m->setX(i);
                m->setY(j);

                Board *temp = new Board();
                temp = board2->copy();

                if (temp->checkMove(m, other)) {
                    temp->doMove(m, other);
                    int score = minimax2(temp, depth-1, orig, true, move);

                    if (score <= best2) {
                        best2 = score;
                    }
                }
                delete temp;
            }
        }
        delete m;
        return best2;
    }
}

// This is a helper function that calculates the value of position
// based on a simple heuristic
int Player::getValue(Board *board){
    Side other = (ourside == BLACK) ? WHITE : BLACK;
    return board->count(ourside) - board->count(other);
}

int Player::getValue2(Board *board2, Move* move, Side our){
    Side other = (our == BLACK) ? WHITE : BLACK;
    int current = board2->count(our) - board2->count(other);
    if(move->getX() == 0 || move->getX() == 7){
            if(move->getY() == 0 || move->getY() == 7)
                current += 30;
            else{
                if(move->getY() == 1 || move->getY() == 6)
                    current -= 15;
                else current += 15;
            }
        }
        else{
            if(move->getY() == 0 || move->getY() == 7){
                if(move->getX() == 0 || move->getX() == 7)
                    current += 30;
            else{
                if(move->getX() == 1 || move->getX() == 6)
                    current -= 15;
                else current += 15;
                }
            }
            else{
                if(move->getX() == 1 || move->getX() == 6)
                    if(move->getY() == 1 || move->getY() == 6)
                        current -= 15;
            }
        }
    return current;
}

