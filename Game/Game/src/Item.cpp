#include"Item.h"

Item::Item(const Rect& r, const int32& k) : 
	rect(r),
	kind(k)
{}

void Item::move() {
	rect.y += speed * Scene::DeltaTime();
}