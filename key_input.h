#pragma once

enum class Key
{
	up,
	down,
	left,
	right,
	open,
	flag,
	quit
};


Key get_key();
void hit_any_key();


