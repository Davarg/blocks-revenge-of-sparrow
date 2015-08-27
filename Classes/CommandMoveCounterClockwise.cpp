#include "CommandMoveCounterClockwise.h"
#include "Constants.h"
#include "GameField.h"
#include "MainGameScene.h"

bool CommandMoveCounterClockwise::init() {
	_isExecute = false;
	_isRedo = false;
	_isUndo = false;

	return true;
}

void moveBlock(int mainBlockY, int mainBlockX, int attachedBlockY, int attachedBlockX, Block* block) {
	b2WeldJointDef jointDef = Block::getJointDef(); //Need to change anchors
	jointDef.bodyA = block->getBody();
	jointDef.bodyB = block->getAttachedBody();

	//Anchor point of Sprite must be at left down corner!
	if (attachedBlockX > mainBlockX){
		if (mainBlockY + 1 == GameField::getSize().y)
			return;

		if (!GameField::getBlock(mainBlockY + 1, mainBlockX)) {
			MainGameScene::getWorld()->DestroyJoint(block->getBody()->GetJointList()->joint);
			jointDef.localAnchorA = { 0.5, JOINT_BLOCK_DIST };
			jointDef.localAnchorB = { 0.5, 0 };
			MainGameScene::getWorld()->CreateJoint(&jointDef);

			Size size = block->getSprite()->getContentSize();
			b2Vec2 position;

			position.x = (block->getSprite()->getPositionX() / SCALE_RATIO_BOX2D) + (size.width / 2) / SCALE_RATIO_BOX2D;
			position.y = ((block->getSprite()->getPositionY() + size.height) / SCALE_RATIO_BOX2D) 
							+ (size.height / 2) / SCALE_RATIO_BOX2D;

			block->getAttachedBody()->SetTransform(position, 0);
		}
	}
	else if (attachedBlockX < mainBlockX) {
		if (mainBlockY - 1 < 0)
			return;

		if (!GameField::getBlock(mainBlockY - 1, mainBlockX)) {
			MainGameScene::getWorld()->DestroyJoint(block->getBody()->GetJointList()->joint);
			jointDef.localAnchorA = { 0.5, JOINT_BLOCK_DIST_NEGATIVE };
			jointDef.localAnchorB = { 0.5, 1 };
			MainGameScene::getWorld()->CreateJoint(&jointDef);

			Size size = block->getSprite()->getContentSize();
			b2Vec2 position;

			position.x = (block->getSprite()->getPositionX() / SCALE_RATIO_BOX2D) + (size.width / 2) / SCALE_RATIO_BOX2D;
			position.y = ((block->getSprite()->getPositionY() - size.height) / SCALE_RATIO_BOX2D)
							+ (size.height / 2) / SCALE_RATIO_BOX2D;

			block->getAttachedBody()->SetTransform(position, 0);
		}
	}
	else if (attachedBlockY > mainBlockY) {
		if (mainBlockX - 1 < 0)
			return;

		if (!GameField::getBlock(mainBlockY, mainBlockX - 1)) {
			MainGameScene::getWorld()->DestroyJoint(block->getBody()->GetJointList()->joint);
			jointDef.localAnchorA = { JOINT_BLOCK_DIST_NEGATIVE, 0 };
			jointDef.localAnchorB = { 1, 0 };
			MainGameScene::getWorld()->CreateJoint(&jointDef);

			Size size = block->getSprite()->getContentSize();
			b2Vec2 position;

			position.x = ((block->getSprite()->getPositionX() - size.width) / SCALE_RATIO_BOX2D)
							+ (size.width / 2) / SCALE_RATIO_BOX2D;
			position.y = (block->getSprite()->getPositionY() / SCALE_RATIO_BOX2D) + (size.height / 2) / SCALE_RATIO_BOX2D;

			block->getAttachedBody()->SetTransform(position, 0);
		}
	}
	else if (attachedBlockY < mainBlockY) {
		if (mainBlockX + 1 == GameField::getSize().x)
			return;

		if (!GameField::getBlock(mainBlockY, mainBlockX + 1)) {
			MainGameScene::getWorld()->DestroyJoint(block->getBody()->GetJointList()->joint);
			jointDef.localAnchorA = { JOINT_BLOCK_DIST, 0 };
			jointDef.localAnchorB = { 0, 0 };
			MainGameScene::getWorld()->CreateJoint(&jointDef);

			Size size = block->getSprite()->getContentSize();
			b2Vec2 position;

			position.x = ((block->getSprite()->getPositionX() + size.width) / SCALE_RATIO_BOX2D)
							+ (size.width / 2) / SCALE_RATIO_BOX2D;
			position.y = (block->getSprite()->getPositionY() / SCALE_RATIO_BOX2D) + (size.height / 2) / SCALE_RATIO_BOX2D;

			block->getAttachedBody()->SetTransform(position, 0);
		}
	}
}

void CommandMoveCounterClockwise::update(float dt) {
}

void CommandMoveCounterClockwise::execute(Block *block) {
	const int mainBlockY = block->getPosOnField().y;
	const int mainBlockX = block->getPosOnField().x;
	const int attachedBlockY = block->getAttachedBlock()->getPosOnField().y;
	const int attachedBlockX = block->getAttachedBlock()->getPosOnField().x;

	_isUndo = false;
	_isRedo = false;
	_isExecute = true;
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	moveBlock(mainBlockY, mainBlockX, attachedBlockY, attachedBlockX, block);
	_isExecute = false;
}

void CommandMoveCounterClockwise::undo() {
}

void CommandMoveCounterClockwise::redo() {
}
