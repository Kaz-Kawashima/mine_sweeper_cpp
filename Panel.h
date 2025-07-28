#pragma once
#include <string>

constexpr int NOT_INITIALIZED = -1;

enum class OpenResult
{
	Safe,
	Explosion
};

class Panel
{
public:
	virtual OpenResult open() = 0;
	void flag();
	bool isOpen();
	bool isFlagged();
	bool isBomb();
	virtual std::string to_string() = 0;
protected:
	bool is_open_;
	bool is_flagged_;
};

class BlankPanel : public Panel
{
public:
	BlankPanel();
	OpenResult open();
	int getBombValue();
	void setBombValue(int);
	std::string to_string();
private:
	int bomb_value_;
};

class BombPanel : public Panel 
{
public:
	BombPanel();
	OpenResult open();
	std::string to_string();
	bool isBomb();
};

class BorderPanel : public Panel
{
public:
	BorderPanel();
	OpenResult open();
	std::string to_string();
};


