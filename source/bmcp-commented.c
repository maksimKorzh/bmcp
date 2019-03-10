/*********************************************************************************\
;                                    BMCP v1.0                                    ;
;---------------------------------------------------------------------------------;
;                    A tribute to chess programming community                     ;
;              based on the ideas taken from micro-Max by H.G.Muller              ;
;---------------------------------------------------------------------------------;
;                                by Maksim Korzh                                  ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;           THIS WORK IS DEDICATED TO HOBBY PROGRAMMERS WHO AIMS TO GET           ;
;                        THE VERY GIST OF CHESS PROGRAMMING                       ;
;---------------------------------------------------------------------------------;
;       "A vague understanding of your goals leads to unpredictable results       ;
;             and abandoning the project halfway..." - my experience              ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;        Copyright © 2018 Maksim Korzh <freesoft.for.people@gmail.com>            ;
;---------------------------------------------------------------------------------;
;     This work is free. You can redistribute it and/or modify it under the       ;
;      terms of the Do What The Fuck You Want To Public License, Version 2,       ;
;       as published by Sam Hocevar. See the COPYING file for more details.       ;
;---------------------------------------------------------------------------------;
'       THIS PROGRAM IS FREE SOFTWARE. IT COMES WITHOUT ANY WARRANTY, TO          ;
;        THE EXTENT PERMITTED BY APPLICABLE LAW. YOU CAN REDISTRIBUTE IT          ;
;       AND/OR MODIFY IT UNDER THE TERMS OF THE DO WHAT THE FUCK YOU WANT         ;
;          TO PUBLIC LICENCE, VERSION 2, AS PUBLISHED BY SAM HOCEVAR.             ;
;                SEE http://www.wtfpl.net/ FOR MORE DETAILS.                      ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;                           Version 2, December 2004                              ;
;                                                                                 ;
;        Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>                         ;
;                                                                                 ;
;        Everyone is permitted to copy and distribute verbatim or modified        ;
;        copies of this license document, and changing it is allowed as long      ;
;        as the name is changed.                                                  ;
;                                                                                 ;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;          TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION        ;
;                                                                                 ;
;         0. You just DO WHAT THE FUCK YOU WANT TO.                               ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                                Piece encoding                                 //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  emSq, P+, P-, K, N, B, R, Q                                                  //
//     0,  1,  2, 3, 4, 5, 6, 7                                                  //
//                                                                               //
//  w = 8  b = 16  virgin = 32                                                   //
//                                                                               //
//  wP : P+ | w = 9                                                              //
//  wK :  K | w = 11                                                             //
//  wN :  N | w = 12                                                             //
//  wB :  B | w = 13                                                             //
//  wR :  R | w = 14                                                             //
//  wQ :  Q | w = 15                                                             //
//                                                                               //
//  bP : P- | b = 18                                                             //
//  bK :  K | b = 19                                                             //
//  bN :  N | b = 20                                                             //
//  bB :  B | b = 21                                                             //
//  bR :  R | b = 22                                                             //
//  bQ :  Q | b = 23                                                             //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

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

enum { WHITE = 8, BLACK = 16};    // side to move

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
        if(!(sq % 16)) printf(" %d  ", 8 - (sq / 16));    // print ranks to the left of the board
        //printf(" %c", ((sq & 8) && (sq += 7)) ? '\n' : pieces[board[sq] & 15]);    // ASCII pieces
        printf(" %s", ((sq & 8) && (sq += 7)) ? "\n" : pieces[board[sq] & 15]);    // unicode pieces
    }
    
    printf("\n     a b c d e f g h\n\nYour move: \n");
}

int SearchPosition(int side, int depth, int alpha, int beta)    // returns best move's score, stores best move
{
    if(!depth)    // if leaf node is reached - evaluate position
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
        if(!(src_square & 0x88))    // check if square where piece to generate moves for stands is on board
        {
            piece = board[src_square];    // store current piece code (might be empty square if no piece)
                                        
            if(piece & side)    // if piece belongs to the moving side
            {
                type = piece & 7;   // extract piece type (e.g. wP, bP, K, N, R, B, Q)
                directions = move_offsets[type + 30];    // init current piece's move offsets list pointer
                
                while(step_vector = move_offsets[++directions])    // loop over move offsets
                {
                    dst_square = src_square;    // init destination square (to square)
                    
                    do                          // loop over destination squares within one ray (for sliders)
                    {           
                        dst_square += step_vector;    // get next destination square
                        captured_square = dst_square;    // init square where piece capture occurs
                        
                        if(dst_square & 0x88) break;    // break out of loop if destination square is off board
    
                        captured_piece = board[captured_square];    // init captured piece
    
                        if(captured_piece & side) break;    // break if captured own piece
                        if(type < 3 && !(step_vector & 7) != !captured_piece) break;    // pawns captures only diagonally
                        if((captured_piece & 7) == 3) return 10000;    /* score move on king capture,
                                                                          we are at the illegal branch now, so no
                                                                          need to keep searching any further
                                                                       */
                        // make move
                        board[captured_square] = 0;    // clear captured square
                        board[src_square] = 0;         // clear source square (from square where piece was)
                        board[dst_square] = piece;     // put piece to destination square (to square)

                        // pawn promotion
                        if(type < 3)    // if pawn
                        {
                            if(dst_square + step_vector + 1 & 0x80)    // goes to the 1th/8th rank
                                board[dst_square]|=7;    // convert it to queen
                        }
                        
                        score = -SearchPosition(24 - side, depth - 1, -beta, -alpha);    // recursive negamax search call
                                              
                        // take back
                        board[dst_square] = 0;    // clear the destination square (to square)
                        board[src_square] = piece;     // put the piece back to it's original square (from square)
                        board[captured_square] = captured_piece;    // restore captured piece on source square if any

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
                        
                        captured_piece += type < 5;    // fake capture for leapers(knights, kings, pawns)
                        
                        // unfake capture for pawns if double pawn push is on the cards
                        if(type < 3 & 6*side + (dst_square & 0x70) == 0x80)captured_piece--;
                    }
    
                    while(!captured_piece);    // terminate loop if captured something
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
    char user_move[5];    // storing user input (format: e2e4, queening is automatical)
    
    int depth = 3;    // search depth (2 - 5 recommended)
    int side = WHITE;    // side to move

    PrintBoard();

//#define DEBUG    // uncoment to see computer playing itself
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

    while(1)  // play vs computer
    {
        memset(&user_move[0], 0, sizeof(user_move));    // clear user input
        
        if(!fgets(user_move, 5, stdin)) continue;    // store user move
        if(user_move[0] == '\n') continue;    // repeat loop if no move entered

        int user_src, user_dst;    // variables to store user from/to squares

        for(int sq = 0; sq < 128; sq++)    // loop over board squares
        {
            if(!(sq & 0x88))    // if square is onboard
            {
                if(!strncmp(user_move, notation[sq], 2))    // if source square exists on board
                    user_src = sq;                          // convert string square to decimal

                if(!strncmp(user_move + 2, notation[sq], 2))    // if destination square exists on board
                    user_dst = sq;                              // convert string square to decimal
            }
        }
        
        // make user move
        board[user_dst] = board[user_src];
        board[user_src] = 0;

        // if pawn promotion
        if(((board[user_dst] == 9) && (user_dst >= 0 && user_dst <= 7)) ||
           ((board[user_dst] == 18) && (user_dst >= 112 && user_dst <= 119)))
            board[user_dst] |= 7;    // convert pawn to corresponding side's queen

        PrintBoard();

        side = 24 - side;   // change side
              
        int score = SearchPosition(side, depth, -10000, 10000);    // search best AI response

        // make AI move
        board[best_dst] = board[best_src];
        board[best_src] = 0;

        // if pawn promotion
        if(((board[best_dst] == 9) && (best_dst >= 0 && best_dst <= 7)) ||
           ((board[best_dst] == 18) && (best_dst >= 112 && best_dst <= 119)))
            board[best_dst] |= 7;    // convert pawn to corresponding side's queen

        side = 24 - side;    // change side

        PrintBoard();
        printf("\nscore: '%d'\n", score);

        // Checkmate detection
        if(score == 10000 || score == -10000) {printf("Checkmate!\n"); break;}

        printf("best move: '%s%s'\n", notation[best_src], notation[best_dst]);
        //getchar();
    }

    return 0;
}
