#include "CommandMoveDown.h"
#include "Constants.h"

bool CommandMoveDown::init() {
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;

	return true;
}

void CommandMoveDown::update(float dt) {
}

void CommandMoveDown::execute(Block *block) {
}

void CommandMoveDown::undo() {
}

void CommandMoveDown::redo() {
}