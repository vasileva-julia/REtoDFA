//#include "utils.h"


//bool implicitConcatenation(char prevSymb, char curSymb) {
//    return (prevSymb == CLOSE_B && (curSymb == OPEN_B || isSymb(curSymb)))
//            || ((isSymb(prevSymb) || prevSymb == STAR)
//                && (isSymb(curSymb) || curSymb == OPEN_B));
//}

//bool implicitEpsilon(char prevSymb, char curSymb) {
//    return (((prevSymb == CAT || prevSymb == OR)
//              && (curSymb == CLOSE_B || curSymb == OR || curSymb == CAT || curSymb == STAR))
//            || (prevSymb == OPEN_B && (curSymb == OR || curSymb == CAT || curSymb == STAR || curSymb == CLOSE_B)));
//}

//bool isSymb(char c) {
//    return c != OPEN_B && c != CLOSE_B && c != OR && c != CAT && c != STAR;
//}

//bool inOrder(char a, char b) { // a >= b
//  switch (a) {
//    case STAR:
//        return true;
//    case CAT:
//        return b != STAR;
//    case OR:
//        return b == OR;
//    default:
//        return false;
//  }
//}
