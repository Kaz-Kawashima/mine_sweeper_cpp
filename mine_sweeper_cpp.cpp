// mine_sweeper_cpp.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "mine_sweeper.h"

int main()
{
    std::cout << "Hello World!\n";
    auto gb = GameBoard(9, 9, 10);
    gb.cui_game();
}