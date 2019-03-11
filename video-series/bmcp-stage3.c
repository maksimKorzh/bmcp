#include <stdio.h>
#include <string.h>

int board[128] = {                 // 0x88 board + positional scores

    22, 20, 21, 23, 19, 21, 20, 22,    0,  0,  5,  5,  0,  0,  5,  0, 
    18, 18, 18, 18, 18, 18, 18, 18,    5,  5,  0,  0,  0,  0,  5,  5,
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 15, 20, 20, 15, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 20, 30, 30, 20, 10,  5,    
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 20, 30, 30, 20, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 15, 20, 20, 15, 10,  5,
     9,  9,  9,  9,  9,  9,  9,  9,    5,  5,  0,  0,  0,  0,  5,  5,
    14, 12, 13, 15, 11, 13, 12, 14,    0,  0,  5,  5,  0,  0,  5,  0

};

char *notation[] = {           // convert square id to board notation

    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",     "i8","j8","k8","l8","m8","n8","o8", "p8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",     "i7","j7","k7","l7","m7","n7","o7", "p7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",     "i6","j6","k6","l6","m6","n6","o6", "p6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",     "i5","j5","k5","l5","m5","n5","o5", "p5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",     "i4","j4","k4","l4","m4","n4","o4", "p4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",     "i3","j3","k3","l3","m3","n3","o3", "p3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",     "i2","j2","k2","l2","m2","n2","o2", "p2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",     "i1","j1","k1","l1","m1","n1","o1", "p1",

};

enum { WHITE = 8, BLACK = 16 };    // side to move

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                                Piece encoding                                 //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  emSq, P+, P-, K, N, B, R, Q                      0  0  0    0  0  0  0       //
//     0,  1,  2, 3, 4, 5, 6, 7                     64 32 16    8  4  2  1       //
//                                                                               //
//  w = 8  b = 16                                                                //
//                                                                               //
//  wP : P+ | w = 9     0001 | 1000 = 1001           1 & 1 = 1   bitwise AND     //
//  wK :  K | w = 11    0011 | 1000 = 1011           1 & 0 = 0                   //
//  wN :  N | w = 12    0100 | 1000 = 1100           0 & 1 = 0                   //
//  wB :  B | w = 13    0101 | 1000 = 1101           0 & 0 = 0                   //
//  wR :  R | w = 14    0110 | 1000 = 1110                                       //
//  wQ :  Q | w = 15    0111 | 1000 = 1111           1 | 1 = 1   bitwise OR      //
//                                                   1 | 0 = 1                   //
//  bP : P- | b = 18    00010 | 10000 = 10010        0 | 1 = 1                   //
//  bK :  K | b = 19    00011 | 10000 = 10011        0 | 0 = 0                   //
//  bN :  N | b = 20    00100 | 10000 = 10100                                    //
//  bB :  B | b = 21    00101 | 10000 = 10101                                    //
//  bR :  R | b = 22    00110 | 10000 = 10110                                    //
//  bQ :  Q | b = 23    00111 | 10000 = 10111                                    //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////


/* 
    Pieces array map:
    
        empty square,
        skip 2nd,
        black pawn,
        black king,
        black knight,
        black bishop,
        black rook,
        black queen,
        skip 9th,
        white pawn,
        skip 11th,
        white king,
        white knight,
        white bishop,
        white rook,
        white queen
*/

//char pieces[] = ".-pknbrq-P-KNBRQ";     // print ASCII characters to represent pieces on board

char *pieces[] = {                      // print unicode characters to represent pieces on board

	".", "-", "\u265F", "\u265A", "\u265E", "\u265D", "\u265C", "\u265B", 
	"-", "\u2659", "-", "\u2654", "\u2658", "\u2657", "\u2656", "\u2655",  

};

static int move_offsets[] = {

   15,  16,  17,   0,                           // white pawns
  -15, -16, -17,   0,                           // black pawns
    1,  16,  -1, -16,   0,                      // rooks
    1,  16,  -1, -16,  15, -15, 17, -17,  0,    // queens, kings and bishops
   14, -14,  18, -18,  31, -31, 33, -33,  0,    // knights
    3,  -1,  12,  21,  16,   7, 12              /* starting indexes for each piece type in order:
                                                   black pawns, white pawns, kings, knights, bishops, rooks, queens
                                                   
                                                   e.g. piece type is 4 - knight
                                                   move_offset[30] == 0
                                                   move_offset[from 31 to 37] == 3, -1, 12, 21, 16, 7, 12
                                                   so move_offset[30 + type(which is 4 in case of knight)] is 34,
                                                   so at move_offset[34] value is 21 - this is the index where knight
                                                   offsets are starting, so we can loop over knight move offests to
                                                   generate knight moves.
                                                   
                                                */ 

};

// Relative values to evaluate the material score of the position.

/* 
    Piece weights array map:
    
        empty square,
        skip 2nd,
        black pawn,
        black king = 0,    for kings are not evaluated, only king captures
        black knight,
        black bishop,
        black rook,
        black queen,
        skip 9th,
        white pawn,
        skip 11th,
        white king = 0,    for kings are not evaluated, only king captures
        white knight,
        white bishop,
        white rook,
        white queen
*/

int piece_weights[] = { 0, 0, -100, 0, -300, -350, -500, -900, 0, 100, 0, 0, 300, 350, 500, 900 };
int best_src, best_dst;    // to store the best move found in search

void PrintBoard()
{
    for(int sq = 0; sq < 128; sq++)
    {
        if(!(sq % 16)) printf(" %d  ", 8 - (sq / 16));
        //printf(" %c", ((sq & 8) && (sq += 7)) ? '\n' : pieces[board[sq] & 15]);    // ASCII pieces
        printf(" %s", ((sq & 8) && (sq += 7)) ? "\n" : pieces[board[sq] & 15]);    // unicode pieces
    }
    
    printf("\n     a b c d e f g h\n\nYour move: \n");
}

int SearchPosition(int side, int depth, int alpha, int beta)
{
    if(!depth)
    {
        // Evaluate position        
        int mat_score = 0, pos_score = 0, pce, eval = 0;
    
        for(int sq = 0; sq < 128; sq++)    // loop over board squares
        {
            if(!(sq & 0x88))    // evaluate only those squares that are on board
            {
                if(pce = board[sq])    // store piece code value
                {
                    mat_score += piece_weights[pce & 15]; // material score
                    (pce & 8) ? (pos_score += board[sq + 8]) : (pos_score -= board[sq + 8]); // positional score
                }
            }
        }
    
        eval = mat_score + pos_score;

        return (side == 8) ? eval : -eval;   // here returns current position's score
    }

    int old_alpha = alpha;    // needed to check whether to store best move or not
    int temp_src;             // temorary best from square
    int temp_dst;             // temporary best to square
    int score = -10000;       // minus infinity

    // Generate moves
    int piece, type, directions, dst_square, captured_square, captured_piece, step_vector;
    
    for(int src_square = 0; src_square < 128; src_square++)    // loop over board squares
    {
        if(!(src_square & 0x88))
        {
            piece = board[src_square];
                                        
            if(piece & side)    // 0??? & 1000 = 1???  // 1??? & 1000
            {
                type = piece & 7;    // ?101 & 0111 = 101
                directions = move_offsets[type + 30];
                
                while(step_vector = move_offsets[++directions])    // loop over piece directions/move offsets
                {
                    dst_square = src_square;
                    
                    do    // loop over destination squares within one ray (for sliders)
                    {
                        dst_square += step_vector;
                        captured_square = dst_square;
                        
                        if(dst_square & 0x88) break;
    
                        captured_piece = board[captured_square];                        
    
                        if(captured_piece & side) break;
                        if(type < 3 && !(step_vector & 7) != !captured_piece) break;
                        if((captured_piece & 7) == 3) return 10000;    // on king capture
                        
                        // make move
                        board[captured_square] = 0;
                        board[src_square] = 0;
                        board[dst_square] = piece;
                        
                        // pawn promotion
                        if(type < 3)
                        {
                            if(dst_square + step_vector + 1 & 0x80)
                                board[dst_square]|=7;
                        }
                        
                        score = -SearchPosition(24 - side, depth - 1, -beta, -alpha);    // recursive negamax search call
                                              
                        // take back
                        board[dst_square] = 0;
                        board[src_square] = piece;
                        board[captured_square] = captured_piece;
                        
                        //Needed to detect checkmate
                        best_src = src_square;
                        best_dst = dst_square;

                        // alpha-beta stuff
                        if(score > alpha)
                        {
                            if(score >= beta)
                                return beta;    // beta cutoff
                            
                            alpha = score;     // alpha acts like max in minimax, best move score so far
        
                            // save best move in given branch
                            temp_src = src_square;
                            temp_dst = dst_square;
                        }              
                                                
                        captured_piece += type < 5;
                        
                        if(type < 3 & 6*side + (dst_square & 0x70) == 0x80)captured_piece--;  
                    }
    
                    while(!captured_piece);
                }
            }
        }
    }

    // store the best move
    if(alpha != old_alpha)
    {
        best_src = temp_src;
        best_dst = temp_dst;
    }

    return alpha;   // here returns the best score
}

int main()
{
    int depth = 3;    // search depth (2 - 5 recommended)
    int side = WHITE;    // side to move

    PrintBoard();

#define DEBUG    // uncoment to see computer playing itself
#ifdef DEBUG

    while(1)    // computer plays against itself
    {
        int score = SearchPosition(side, depth, -10000, 10000);    // search best move
        
        // make AI move
        board[best_dst] = board[best_src];
        board[best_src] = 0;

        side = 24 - side;    // change side

        PrintBoard();
        printf("\nscore: '%d'\n", score);
        printf("best move: '%s%s'\n", notation[best_src], notation[best_dst]);
        
        getchar();
    }

#endif

    return 0;
}
