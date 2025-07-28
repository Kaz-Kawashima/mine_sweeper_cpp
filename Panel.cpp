#include "Panel.h"

void Panel::flag() {
	if (is_flagged_) {
		is_flagged_ = false;
	}
	else {
		is_flagged_ = true;
	}
};

bool Panel::isOpen() {
	return is_open_;
};

bool Panel::isFlagged() {
	return is_flagged_;
}

bool Panel::isBomb() {
	return false;
}


BlankPanel::BlankPanel()
{
	is_open_ = false;
	is_flagged_ = false;
	bomb_value_ = -1;
};

OpenResult BlankPanel::open() {
	is_open_ = true;
	return OpenResult::Safe;
}

int BlankPanel::getBombValue() {
	return bomb_value_;
}
	
void BlankPanel::setBombValue(int value) {
	if (bomb_value_ < 0 && value >=  0) {
		bomb_value_ = value;
	}
}

std::string BlankPanel::to_string() {
	if (is_flagged_) {
		return "F";
	}
	if (is_open_) {
		if (bomb_value_ == 0) {
			return " ";
		}
		else {
			return std::to_string(bomb_value_);
		}
	}
	else {
		return "#";
	}
}

BombPanel::BombPanel() {
	is_open_ = false;
	is_flagged_ = false;
}

OpenResult BombPanel::open() {
	is_open_ = true;
	return OpenResult::Explosion;
}

bool BombPanel::isBomb() {
	return true;
}

std::string BombPanel::to_string() {
	if (is_open_) {
		return "B";
	}
	if (is_flagged_) {
		return "F";
	}
	else {
		return "#";
	}
}

BorderPanel::BorderPanel() {
	is_open_ = true;
	is_flagged_ = false;
}

OpenResult BorderPanel::open(){
	return OpenResult::Safe;
}


std::string BorderPanel::to_string() {
	return "=";
}