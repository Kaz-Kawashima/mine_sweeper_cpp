#pragma once

#include "Panel.h"
#include <vector>
using namespace std;

class GameBoard
{
public:
	GameBoard(int num_row, int num_col, int num_bomb);
	void cui_game();
private:
	std::vector<std::vector<Panel*>> field_;
	int size_x_;
	int size_y_;
	int field_size_x_;
	int field_size_y_;
	int num_bomb_;
	int cursor_row_;
	int cursor_col_;
	void initField();
	void setBomb();
	void calcFieldBombValue();
	int calcPanelBombValue(int y, int x);
	std::string to_string();
	bool is_finished();
	OpenResult cascade_open(int y, int x);
	int count_flag();
};

