#include "CommandMoveDown.h"
#include "Constants.h"

bool CommandMoveDown::init() {
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;

	return true;
}

void CommandMoveDown::update(float dt) {
	b2Vec2 oldVelocity = _block->getBody()->GetLinearVelocity();

	if (!_isUndo && !_isRedo) {
		if (_positionOld.y - _block->getBody()->GetPosition().y >=
			_block->getSprite()->getContentSize().height / SCALE_RATIO) {
			_block->getBody()->SetLinearVelocity((b2Vec2(oldVelocity.x, 0)));
			_block->getAttachedBody()->SetLinearVelocity((b2Vec2(oldVelocity.x, 0)));
			Director::getInstance()->getScheduler()->unscheduleUpdate(this);
			_isExecute = false;
		}
	}
	else if (_isUndo){
		if (_block->getBody()->GetPosition().y >= _positionOld.y) {
			_isUndo = false;
			Director::getInstance()->getScheduler()->unscheduleUpdate(this);
			_block->getBody()->SetLinearVelocity((b2Vec2(oldVelocity.x, 0)));
			_block->getAttachedBody()->SetLinearVelocity((b2Vec2(oldVelocity.x, 0)));
			_isExecute = false;
		}
	}
	else if (_isRedo) {
		if (_block->getBody()->GetPosition().y <= _positionNew.y) {
			_isRedo = false;
			Director::getInstance()->getScheduler()->unscheduleUpdate(this);
			_block->getBody()->SetLinearVelocity((b2Vec2(oldVelocity.x, 0)));
			_block->getAttachedBody()->SetLinearVelocity((b2Vec2(oldVelocity.x, 0)));
			_isExecute = false;
		}
	}
}

void CommandMoveDown::execute(Block *block) {
	int nHeight = block->getSprite()->getContentSize().height;
	b2Vec2 oldVelocity = block->getBody()->GetLinearVelocity();
	_block = block;
	_positionOld = _block->getBody()->GetPosition();
	_positionNew = { _block->getBody()->GetPosition().x
		, _block->getBody()->GetPosition().y - _block->getSprite()->getContentSize().height / SCALE_RATIO };
	_isUndo = false;
	_isRedo = false;
	_isExecute = true;

	block->getBody()->SetLinearVelocity(b2Vec2(oldVelocity.x, (-nHeight / MOVETIME) / SCALE_RATIO));
	block->getAttachedBody()->SetLinearVelocity(b2Vec2(oldVelocity.x, (-nHeight / MOVETIME) / SCALE_RATIO));
}

void CommandMoveDown::undo() {
	int nHeight = _block->getSprite()->getContentSize().height;
	b2Vec2 oldVelocity = _block->getBody()->GetLinearVelocity();

	_isUndo = true;
	_isRedo = false;

	_block->getBody()->SetLinearVelocity(b2Vec2(oldVelocity.x, (nHeight / MOVETIME) / SCALE_RATIO));
	_block->getAttachedBody()->SetLinearVelocity(b2Vec2(oldVelocity.x, (nHeight / MOVETIME) / SCALE_RATIO));
}

void CommandMoveDown::redo() {
	int nHeight = _block->getSprite()->getContentSize().height;
	b2Vec2 oldVelocity = _block->getBody()->GetLinearVelocity();

	_isUndo = false;
	_isRedo = true;

	_block->getBody()->SetLinearVelocity(b2Vec2(oldVelocity.x, (-nHeight / MOVETIME) / SCALE_RATIO));
	_block->getAttachedBody()->SetLinearVelocity(b2Vec2(oldVelocity.x, (-nHeight / MOVETIME) / SCALE_RATIO));
}