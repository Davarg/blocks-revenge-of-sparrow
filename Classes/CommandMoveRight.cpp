#include "CommandMoveRight.h"
#include "Constants.h"

bool CommandMoveRight::init() {
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;

	return true;
}

void CommandMoveRight::update(float dt) {
	b2Vec2 oldVelocity = _block->getBody()->GetLinearVelocity();

	if (!_isUndo && !_isRedo) {
		if (_block->getBody()->GetPosition().x - _positionOld.x
			>= (_block->getSprite()->getContentSize().width - JOINT_BLOCK_DIST) / SCALE_RATIO_BOX2D) {
			_block->getBody()->SetLinearVelocity((b2Vec2(0, oldVelocity.y)));
			_block->getAttachedBody()->SetLinearVelocity((b2Vec2(0, oldVelocity.y)));
			Director::getInstance()->getScheduler()->unscheduleUpdate(this);
			_isExecute = false;
		}
	}
	else if (_isUndo) {
		if (_block->getBody()->GetPosition().x <= _positionOld.x) {
			_isUndo = false;
			Director::getInstance()->getScheduler()->unscheduleUpdate(this);
			_block->getBody()->SetLinearVelocity((b2Vec2(0, oldVelocity.y)));
			_block->getAttachedBody()->SetLinearVelocity((b2Vec2(0, oldVelocity.y)));
			_isExecute = false;
		}
	} 
	else if (_isRedo) {
		if (_block->getBody()->GetPosition().x >= _positionNew.x) {
			_isRedo = false;
			Director::getInstance()->getScheduler()->unscheduleUpdate(this);
			_block->getBody()->SetLinearVelocity((b2Vec2(0, oldVelocity.y)));
			_block->getAttachedBody()->SetLinearVelocity((b2Vec2(0, oldVelocity.y)));
			_isExecute = false;
		}
	}
}

void CommandMoveRight::execute(Block *block) {
	int nWidth = block->getSprite()->getContentSize().width;
	b2Vec2 oldVelocity = block->getBody()->GetLinearVelocity();
	_block = block;
	_positionOld = _block->getBody()->GetPosition();
	_positionNew = { _block->getBody()->GetPosition().x + _block->getSprite()->getContentSize().width / SCALE_RATIO_BOX2D
		, _block->getBody()->GetPosition().y };
	_isUndo = false;
	_isRedo = false;
	_isExecute = true;

	block->getBody()->SetLinearVelocity(b2Vec2((nWidth / MOVETIME) / SCALE_RATIO_BOX2D, oldVelocity.y));
	block->getAttachedBody()->SetLinearVelocity(b2Vec2((nWidth / MOVETIME) / SCALE_RATIO_BOX2D, oldVelocity.y));
}

void CommandMoveRight::undo() {
	int nWidth = _block->getSprite()->getContentSize().width;
	b2Vec2 oldVelocity = _block->getBody()->GetLinearVelocity();

	_isUndo = true;
	_isRedo = false;

	_block->getBody()->SetLinearVelocity(b2Vec2((-nWidth / MOVETIME) / SCALE_RATIO_BOX2D, oldVelocity.y));
	_block->getAttachedBody()->SetLinearVelocity(b2Vec2((-nWidth / MOVETIME) / SCALE_RATIO_BOX2D, oldVelocity.y));
}

void CommandMoveRight::redo() {
	int nWidth = _block->getSprite()->getContentSize().width;
	b2Vec2 oldVelocity = _block->getBody()->GetLinearVelocity();

	_isUndo = false;
	_isRedo = true;

	_block->getBody()->SetLinearVelocity(b2Vec2((nWidth / MOVETIME) / SCALE_RATIO_BOX2D, oldVelocity.y));
	_block->getAttachedBody()->SetLinearVelocity(b2Vec2((nWidth / MOVETIME) / SCALE_RATIO_BOX2D, oldVelocity.y));
}