#include "mine_sweeper.h"
#include "key_input.h"
#include <random>
#include <iostream>
#include <cassert>
#include <type_traits>
#include <cstdlib>


template<typename T, typename Base>
bool instance_of(Base* ptr) {
	static_assert(std::is_polymorphic<Base>::value,
		"Base must have at least one virtual function");
	return dynamic_cast<T*>(ptr) != nullptr;
}

GameBoard::GameBoard(int num_row, int num_col, int num_bomb) {
	size_y_ = num_row;
	size_x_ = num_col;
	field_size_y_ = size_y_ + 2;
	field_size_x_ = size_x_ + 2;
	num_bomb_ = num_bomb;
	cursor_row_ = 1;
	cursor_col_ = 1;

	initField();
	setBomb();
	calcFieldBombValue();
}

void GameBoard::initField() {
	//フィールド初期化
	for (int row = 0; row < field_size_y_; row++) {
		std::vector<Panel*> panel_row;
		for (int col = 0; col < field_size_x_; col++) {
			Panel* p;
			if (row == 0 || col == 0 ||
				row == (size_y_ + 1) || col == (size_x_ + 1)) {
				p = new BorderPanel();
			}
			else {
				p = new BlankPanel();
			}
			panel_row.push_back(p);
		}
		field_.push_back(panel_row);
	}
}

void GameBoard::setBomb() {
	// 乱数生成器を初期化
	std::random_device rd;  // ハドウェア乱数生成器
	std::mt19937 gen(rd()); // メルセンヌ・ツイスター乱数生成器
	// 一様分布を定義
	std::uniform_int_distribution<int> distribution_x(1, size_x_);
	std::uniform_int_distribution<int> distribution_y(1, size_y_);
	//爆弾設置
	int bomb_counter = 0;
	while (bomb_counter < num_bomb_) {
		int row = distribution_y(gen);
		int col = distribution_x(gen);
		if (!instance_of<BombPanel>(field_[row][col]))
		{
			field_[row][col] = new BombPanel();
			bomb_counter++;
		}
	}
}

void GameBoard::calcFieldBombValue() {
	for (int row = 1; row <= size_y_; row++) {
		for (int col = 1; col <= size_x_; col++) {
			if(!instance_of<BombPanel>(field_[row][col])) {
				int value = calcPanelBombValue(row, col);
				auto bp = dynamic_cast<BlankPanel*>(field_[row][col]);
				if (bp == nullptr) {
					cout << row << "/" << col << endl;
				}
				assert(bp != nullptr);
				bp->setBombValue(value);
			}
		}
	}
}

int GameBoard::calcPanelBombValue(int y, int x) {
	int counter = 0;
	for (int row = y - 1; row <= y + 1; row++) {
		for (int col = x - 1; col <= x + 1; col++) {
			if (instance_of<BombPanel>(field_[row][col])) {
				counter++;
			}
		}
	}
	return counter;
}

std::string GameBoard::to_string() {
	std::string output = "";
	for (int row = 0; row < field_size_y_; row++) {
		for (int col = 0; col < field_size_x_; col++) {
			string panel_string;
			if (col == 0 && row == cursor_row_) {
				panel_string = ">";
			}
			else if (col == (field_size_x_ - 1) && row == cursor_row_) {
				panel_string = "<";
			}
			else if (col == cursor_col_ && row == 0) {
				panel_string = "v";
			}
			else if (col == cursor_col_ && row == (field_size_y_ - 1)) {
				panel_string = "^";
			}
			else if (col == cursor_col_ && row == cursor_row_) {
				panel_string = "@";
			}
			else {
				panel_string = field_[row][col]->to_string();
			}
			output += panel_string;
			output += " ";
		}
		output += "\n";
	}
	return output;
}

bool GameBoard::is_finished() {
	for (int row = 1; row <= size_y_; row++) {
		for (int col = 1; col <= size_x_; col++) {
			auto panel = field_[row][col];
			if (instance_of<BlankPanel>(panel)) {
				if (!panel->isOpen()) {
					return false;
				}
			}
		}
	}
	return true;
}

OpenResult GameBoard::cascade_open(int y, int x) {
	auto result = OpenResult::Safe;
	if (field_[y][x]->isOpen()) {
		return result;
	}
	result = field_[y][x]->open();
	if (result == OpenResult::Explosion) {
		return result;
	}
	if (!instance_of<BlankPanel>(field_[y][x])) {
		return result;
	}
	auto p = dynamic_cast<BlankPanel*>(field_[y][x]);
	if (p->getBombValue() > 0) {
		return result;
	}
	cascade_open(y - 1, x - 1);
	cascade_open(y - 1, x);
	cascade_open(y - 1, x + 1);
	cascade_open(y, x - 1);
	cascade_open(y, x + 1);
	cascade_open(y + 1, x - 1);
	cascade_open(y + 1, x);
	cascade_open(y + 1, x + 1);
	return result;
}

int GameBoard::count_flag() {
	int counter = 0;
	for (int row = 1; row <= size_y_; row++) {
		for (int col = 1; col <= size_x_; col++) {
			if (field_[row][col]->isFlagged()) {
				counter++;
			}
		}
	}
	return counter;
}

void GameBoard::cui_game() {
	auto result = OpenResult::Safe;
	bool finished = false;
	while (!finished) {
		int num_flag = this->count_flag();
		system("cls");
		cout << this->to_string();
		cout << "\ninput < -^ v -> / O open / F flag (" << num_flag << ")" << endl;
		auto key = get_key();
		switch (key) {
		case Key::up:
			cursor_row_--;
			if (cursor_row_ < 1) {
				cursor_row_ = 1;
			}
			break;
		case Key::down:
			cursor_row_++;
			if (cursor_row_ > size_y_) {
				cursor_row_ = size_y_;
			}
			break;
		case Key::left:
			cursor_col_--;
			if (cursor_col_ < 1) {
				cursor_col_ = 1;
			}
			break;
		case Key::right:
			cursor_col_++;
			if (cursor_col_ > size_x_) {
				cursor_col_ = size_x_;
			}
			break;
		case Key::open:
			result = cascade_open(cursor_row_, cursor_col_);
			if (result == OpenResult::Safe) {
				if (this->is_finished()) {
					cursor_col_ = -1;
					cursor_row_ = -1;
					system("cls");
					cout << this->to_string() << endl;
					cout << endl << "you win!" << endl;
					finished = true;
				}
			}
			else {
				cursor_col_ = -1;
				cursor_row_ = -1;
				system("cls");
				cout << this->to_string() << endl;
				cout << endl << "Game over!" << endl;
				finished = true;
			}
			break;
		case Key::flag:
			field_[cursor_row_][cursor_col_]->flag();
			break;
		case Key::quit:
			cout << endl << "quit games" << endl;
			finished = true;
			break;
		}
	}
}