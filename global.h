#ifndef GLOBAL_H
#define GLOBAL_H

//兵 100 士 250 象 250 马 350 炮 350
#define KING 10000
#define CAR 500
#define HORSE 350
#define BISHOP 250
#define CANON 350
#define PAWN 100
#define ELEPHANT 250

//这一组宏定义了各种棋子的灵活性,每多走一个位置应加上的分值
//兵 15，士 1，象 1，车 6 ，马 12，炮 6，王 0
#define KING_EXRA 0
#define CAR_EXRA  6
#define HORSE_EXRA 12
#define BISHOP_EXRA 1
#define CANON_EXRA 6
#define PAWN_EXRA 15
#define ELEPHANT_EXRA 1

#endif // GLOBAL_H
