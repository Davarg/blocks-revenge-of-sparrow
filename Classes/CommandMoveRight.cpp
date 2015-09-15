#include "CommandMoveRight.h"
#include "Constants.h"

bool CommandMoveRight::init() {
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;

	return true;
}

void CommandMoveRight::stopBlock() {
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	_isExecute = false;
	_block->getBody()->SetLinearVelocity({ 0, _block->getBody()->GetLinearVelocity().y });
	_block->getAttachedBody()->SetLinearVelocity({ 0, _block->getAttachedBody()->GetLinearVelocity().y });
}

void CommandMoveRight::update(float dt) {
	static float32 lastPosX = 0;
	auto body1 = _block->getBody();
	auto body2 = _block->getAttachedBody();
	float32 offset = MOVEOFFSET;
	float32 blockSizeInMeters = _blockSize.width / SCALE_RATIO_BOX2D;

	if ((body1->GetPosition().x - _positionOld.x) < blockSizeInMeters
			&& lastPosX != body1->GetPosition().x
			&& !_isUndo
			&& _isExecute) {
		if (((body1->GetPosition().x + offset) - _positionOld.x) >= blockSizeInMeters) {
			while (offset > 0) {
				offset -= MOVEOFFSET / 100;
				if (((body1->GetPosition().x + offset) - _positionOld.x) < blockSizeInMeters) {
					stopBlock();
					body1->SetTransform({ body1->GetPosition().x + offset, body1->GetPosition().y }, 0);
					body2->SetTransform({ body2->GetPosition().x + offset, body2->GetPosition().y }, 0);
					lastPosX = 0;
					return;
				}
			}
		}
		assert(offset);
		body1->SetLinearVelocity({ (float32)(body1->GetLinearVelocity().x + offset), body1->GetLinearVelocity().y });
		body2->SetLinearVelocity({ (float32)(body2->GetLinearVelocity().x + offset), body2->GetLinearVelocity().y });
		lastPosX = body1->GetPosition().x;
	}
	else if ((_positionOld.x - body1->GetPosition().x) < blockSizeInMeters
				&& lastPosX != body1->GetPosition().x
				&& _isUndo
				&& _isExecute) {
		if ((_positionOld.x - (body1->GetPosition().x - offset)) >= blockSizeInMeters) {
			while (offset > 0) {
				offset -= MOVEOFFSET / 100;
				if ((_positionOld.x - (body1->GetPosition().x - offset) < blockSizeInMeters)) {
					stopBlock();
					body1->SetTransform({ body1->GetPosition().x - offset, body1->GetPosition().y }, 0);
					body2->SetTransform({ body2->GetPosition().x - offset, body2->GetPosition().y }, 0);
					lastPosX = 0;
					return;
				}
			}
		}
		assert(offset);
		body1->SetLinearVelocity({ (float32)(body1->GetLinearVelocity().x - offset), body1->GetLinearVelocity().y });
		body2->SetLinearVelocity({ (float32)(body2->GetLinearVelocity().x - offset), body2->GetLinearVelocity().y });
		lastPosX = body1->GetPosition().x;
	}
	else {
		stopBlock();
		lastPosX = 0;
	}
}

void CommandMoveRight::execute(Block *block) {
	_block = block;
	_positionOld = _block->getBody()->GetPosition();
	_blockSize = block->getSprite()->getContentSize();
	_isUndo = false;
	_isRedo = false;
	_isExecute = true;
}

void CommandMoveRight::undo() {
	_isUndo = true;
	_isRedo = false;
}

void CommandMoveRight::redo() {
	_isUndo = false;
	_isRedo = true;
}