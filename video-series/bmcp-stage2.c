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

int SearchPosition(int side, int depth)
{
    if(!depth)
    {
        return 0;
    }

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
                        
                        PrintBoard(); getchar();

                        // pawn promotion
                        if(type < 3)
                        {
                            if(dst_square + step_vector + 1 & 0x80)
                                board[dst_square]|=7;
                        }
                        
                        SearchPosition(24 - side, depth - 1);
                        //printf("move: '%s%s'\n", notation[src_square], notation[dst_square]);
                                              
                        // take back
                        board[dst_square] = 0;
                        board[src_square] = piece;
                        board[captured_square] = captured_piece;
                        
                        PrintBoard(); getchar();
                        
                        captured_piece += type < 5;
                        
                        if(type < 3 & 6*side + (dst_square & 0x70) == 0x80)captured_piece--;  
                    }
    
                    while(!captured_piece);
                }
            }
        }
    }

    return 0;   // here returns the best score
}

int main()
{
    PrintBoard();
    SearchPosition(WHITE, 3);
    return 0;
}
