#include "MainGameScene.h"
#include "CommandMoveDown.h"
#include "ConstantsRegistry.h"

bool CommandMoveDown::init() {
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;

	return true;
}

void CommandMoveDown::update(float dt) {
}

void CommandMoveDown::stopBlock(bool withBody) {
	if (withBody) {
		auto body1 = _block->getBody();
		auto body2 = _block->getAttachedBody();
		if (body1 && body2) {
			body1->SetLinearVelocity({ 0, 0 });
			body2->SetLinearVelocity({ 0, 0 });
		}
	}
	_isExecute = false;
	_block = nullptr;
}

void CommandMoveDown::execute(Block *block) {
	_isExecute = true;

	auto body1 = block->getBody();
	if (!body1) {
		stopBlock();
		return;
	}
	auto body2 = block->getAttachedBody();
	if (!body2) {
		stopBlock();
		return;
	}
	_block = block;
	body1->SetLinearVelocity({ 0, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y) * 4 });
	body2->SetLinearVelocity({ 0, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y) * 4 });
}

void CommandMoveDown::undo() {
	_isUndo = false;
	_isRedo = false;
}

void CommandMoveDown::redo() {
	_isUndo = false;
	_isRedo = false;
}