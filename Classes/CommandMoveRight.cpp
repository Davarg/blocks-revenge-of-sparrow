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
	_isRedo = false;
	_isUndo = false;
	_block->getBody()->SetLinearVelocity({ 0, _block->getBody()->GetLinearVelocity().y });
	_block->getAttachedBody()->SetLinearVelocity({ 0, _block->getAttachedBody()->GetLinearVelocity().y });
}

void CommandMoveRight::update(float dt) {
	auto body1 = _block->getBody();
	auto body2 = _block->getAttachedBody();
	float32 offset = MOVEOFFSET;
	float32 blockSizeInMeters = _blockSize.width / SCALE_RATIO_BOX2D;

	if ((body1->GetPosition().x - _positionOldFirst.x) < blockSizeInMeters
			&& !_isUndo
			&& _isExecute) {
		if (((body1->GetPosition().x + offset) - _positionOldFirst.x) >= blockSizeInMeters) {
			while (offset > 0) {
				offset -= MOVEOFFSET / 100;
				if (((body1->GetPosition().x + offset) - _positionOldFirst.x) < blockSizeInMeters) {
					stopBlock();
					body1->SetTransform({ body1->GetPosition().x + offset, body1->GetPosition().y }, 0);
					body2->SetTransform({ body2->GetPosition().x + offset, body2->GetPosition().y }, 0);
					return;
				}
			}
		}
		assert(offset);
		if (body1->GetLinearVelocity().x < 0
			&& (_positionOldFirst.x - body1->GetPosition().x) < (blockSizeInMeters / 7)) {
			stopBlock();
			body1->SetTransform(_positionOldFirst, 0);
			body2->SetTransform(_positionOldSecond, 0);
			return;
		}
		body1->SetLinearVelocity({ (float32)(body1->GetLinearVelocity().x + offset), body1->GetLinearVelocity().y });
		body2->SetLinearVelocity({ (float32)(body2->GetLinearVelocity().x + offset), body2->GetLinearVelocity().y });
		_positionOldFirst.y = body1->GetPosition().y;
		_positionOldSecond.y = body2->GetPosition().y;
	}
	else if ((_positionOldFirst.x - body1->GetPosition().x) < blockSizeInMeters
				&& _isUndo
				&& _isExecute) {
		if ((_positionOldFirst.x - (body1->GetPosition().x - offset)) >= blockSizeInMeters) {
			while (offset > 0) {
				offset -= MOVEOFFSET / 100;
				if ((_positionOldFirst.x - (body1->GetPosition().x - offset) < blockSizeInMeters)) {
					stopBlock();
					body1->SetTransform({ body1->GetPosition().x - offset, body1->GetPosition().y }, 0);
					body2->SetTransform({ body2->GetPosition().x - offset, body2->GetPosition().y }, 0);
					return;
				}
			}
		}
		assert(offset);
		if (body1->GetLinearVelocity().x > 0
			&& (_positionOldFirst.x - body1->GetPosition().x) < (blockSizeInMeters / 7)) {
			stopBlock();
			body1->SetTransform(_positionOldFirst, 0);
			body2->SetTransform(_positionOldSecond, 0);
			return;
		}
		body1->SetLinearVelocity({ (float32)(body1->GetLinearVelocity().x - offset), body1->GetLinearVelocity().y });
		body2->SetLinearVelocity({ (float32)(body2->GetLinearVelocity().x - offset), body2->GetLinearVelocity().y });
		_positionOldFirst.y = body1->GetPosition().y;
		_positionOldSecond.y = body2->GetPosition().y;
	}
	else
		stopBlock();
}

void CommandMoveRight::execute(Block *block) {
	_block = block;
	_positionOldFirst = _block->getBody()->GetPosition();
	_positionOldSecond = _block->getAttachedBody()->GetPosition();
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