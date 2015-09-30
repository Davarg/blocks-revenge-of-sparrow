#include "CommandMoveRight.h"
#include "Constants.h"
#include "GameField.h"
#include "UserInput.h"
#include "SimpleUI.h"
#include "MainGameScene.h"

bool CommandMoveRight::init() {
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;

	return true;
}

void CommandMoveRight::stopBlock() {
	b2Body *body1 = nullptr;
	b2Body *body2 = nullptr;

	if (_block) {
		body1 = _block->getBody();
		body2 = _block->getAttachedBody();
	}
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;
	if (body1 && body2) {
		body1->SetLinearVelocity({ 0, body1->GetLinearVelocity().y });
		body2->SetLinearVelocity({ 0, body2->GetLinearVelocity().y });
	}
	_block = nullptr;
}

void CommandMoveRight::update(float dt) {
	if (!_block)
		return;

	auto body1 = _block->getBody();
	if (!body1) {
		stopBlock();
		return;
	}
	auto body2 = _block->getAttachedBody();
	if (!body2) {
		stopBlock();
		return;
	}
	float32 offset = MOVEOFFSET;
	float32 blockSizeInMeters = _blockSize.width / SCALE_RATIO_BOX2D;

	if (body1->GetFixtureList()->GetFilterData().categoryBits == Block::blockFlags::PASSIVE
		|| body2->GetFixtureList()->GetFilterData().categoryBits == Block::blockFlags::PASSIVE)
		stopBlock();

	if (body1->GetFixtureList()->GetFilterData().categoryBits == Block::blockFlags::NEED_TO_STOP
		|| body2->GetFixtureList()->GetFilterData().categoryBits == Block::blockFlags::NEED_TO_STOP) {
		Vec2 posOnField = _block->getPosOnField();
		Sprite *sprite1 = (Sprite*)body1->GetUserData();
		Sprite *sprite2 = (Sprite*)body2->GetUserData();
		Size size = sprite1->getContentSize();
		b2Filter filter;
		
		if (GameField::getBlock({ posOnField.x + 2, posOnField.y })){
			stopBlock();

			filter = body1->GetFixtureList()->GetFilterData();
			filter.categoryBits = Block::blockFlags::ACTIVE;
			body1->GetFixtureList()->SetFilterData(filter);

			filter = body2->GetFixtureList()->GetFilterData();
			filter.categoryBits = Block::blockFlags::ACTIVE;
			body2->GetFixtureList()->SetFilterData(filter);

			body1->SetTransform(_positionOldFirst, 0);
			body2->SetTransform(_positionOldSecond, 0);

			sprite1->setPosition({ (body1->GetPosition().x * SCALE_RATIO_BOX2D) - size.width / 2
				, (body1->GetPosition().y * SCALE_RATIO_BOX2D) - size.height / 2 });
			sprite2->setPosition({ (body2->GetPosition().x * SCALE_RATIO_BOX2D) - size.width / 2
				, (body2->GetPosition().y * SCALE_RATIO_BOX2D) - size.height / 2 });

			SimpleUI *simpleUI = MainGameScene::getUI();
			UserInput *input = (UserInput*)simpleUI->getChildrenByName(UserInput::name());
			input->dropInputEvents();

			return;
		}

		if (!_isUndo) {
			body1->SetTransform({ _positionOldFirst.x + blockSizeInMeters, body1->GetPosition().y }, 0);
			body2->SetTransform({ _positionOldSecond.x + blockSizeInMeters, body2->GetPosition().y }, 0);
		}
		else {
			body1->SetTransform({ _positionOldFirst.x - blockSizeInMeters, body1->GetPosition().y }, 0);
			body2->SetTransform({ _positionOldSecond.x - blockSizeInMeters, body2->GetPosition().y }, 0);
		}
		
		sprite1->setPosition({ (body1->GetPosition().x * SCALE_RATIO_BOX2D) - size.width / 2
			, (body1->GetPosition().y * SCALE_RATIO_BOX2D) - size.height / 2 });
		sprite2->setPosition({ (body2->GetPosition().x * SCALE_RATIO_BOX2D) - size.width / 2
			, (body2->GetPosition().y * SCALE_RATIO_BOX2D) - size.height / 2 });

		stopBlock();

		filter = body1->GetFixtureList()->GetFilterData();
		filter.categoryBits = Block::blockFlags::STOPPED;
		body1->GetFixtureList()->SetFilterData(filter);

		filter = body2->GetFixtureList()->GetFilterData();
		filter.categoryBits = Block::blockFlags::STOPPED;
		body2->GetFixtureList()->SetFilterData(filter);

		body1->SetActive(false);
		body1->SetActive(true);
		body2->SetActive(false);
		body2->SetActive(true);
	}

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
	_positionOldFirst = body1->GetPosition();
	_positionOldSecond = body2->GetPosition();
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