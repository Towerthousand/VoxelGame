#include "Arrow.hpp"

Arrow::Arrow(World &world) : Entity(world) {
}

Arrow::~Arrow() {

}

void Arrow::update(float deltaTime) {
	movePos(deltaTime);
}

void Arrow::draw() {

}
