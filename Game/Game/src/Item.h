# pragma once
# include <Siv3D.hpp>

// ƒAƒCƒeƒ€
class Item {
public:
	Rect rect;

	int32 kind;

	static constexpr int32 speed = 200;

	Item(const Rect& r, const int32& k);

	void move();
};