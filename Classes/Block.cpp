#include "Block.h"
#include "MainGameScene.h"
#include "Constants.h"
#include "GameField.h"
#include <math.h>

void Block::destroy() {
	_sprite->removeFromParentAndCleanup(true);
	auto world = _body->GetWorld();
	/*b2Joint *joint = _body->GetJointList()->joint;

	if (joint != nullptr)
	world->DestroyJoint(joint);*/
	world->DestroyBody(_body);
}

bool Block::init(Sprite* _sprite) {
	try {
		_sprite->setScaleX(0.037f);
		_sprite->setScaleY(0.035f);
#ifdef _DEBUG
		_sprite->setOpacity(60);
#endif

		b2BodyDef bodyDef;
		bodyDef.position = b2Vec2(_sprite->getPositionX() / SCALE_RATIO, _sprite->getPositionY() / SCALE_RATIO);
		bodyDef.userData = _sprite;
		bodyDef.type = b2_dynamicBody;

		b2PolygonShape shape;
		shape.SetAsBox((_sprite->getContentSize().width / SCALE_RATIO) / 2.2f
			, (_sprite->getContentSize().height / SCALE_RATIO) / 2.2f);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1;
		fixtureDef.friction = 0;
		fixtureDef.restitution = 0;
		fixtureDef.shape = &shape;
		fixtureDef.filter.categoryBits = Block::getActiveCategoryBits();

		_body = MainGameScene::getWorld()->CreateBody(&bodyDef);
		_body->CreateFixture(&fixtureDef);

		_body->SetLinearDamping(2);
		_body->SetAngularDamping(10);
	}
	catch (...) {
		cocos2d::log("Error while init Block");
		return false;
	}
	return true;
}

Block* Block::generateBlock() {
	Block* block = nullptr;
	std::mt19937 generator(time(0));
	std::uniform_int_distribution<> uid(0, 2);

	const int COLOR1 = uid(generator);
	const int COLOR2 = uid(generator);
	Sprite *blockSprite2 = nullptr;
	Sprite *blockSprite = nullptr;

	switch (COLOR1) {
	case 0:
		blockSprite = Sprite::create("../Resources/block_green.png");
		blockSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		block = Block::createBlock(blockSprite, 100, Color3B::GREEN);
		break;

	case 1:
		blockSprite = Sprite::create("../Resources/block_red.png");
		blockSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		block = Block::createBlock(blockSprite, 100, Color3B::RED);
		break;

	case 2:
		blockSprite = Sprite::create("../Resources/block_yellow.png");
		blockSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		block = Block::createBlock(blockSprite, 100, Color3B::YELLOW);
		break;
	}

	switch (COLOR2) {
	case 0:
		blockSprite2 = Sprite::create("../Resources/block_green.png");
		blockSprite2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		block->setJointWith(Block::createBlock(blockSprite2, 100, Color3B::GREEN, true));
		break;

	case 1:
		blockSprite2 = Sprite::create("../Resources/block_red.png");
		blockSprite2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		block->setJointWith(Block::createBlock(blockSprite2, 100, Color3B::RED, true));
		break;

	case 2:
		blockSprite2 = Sprite::create("../Resources/block_yellow.png");
		blockSprite2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		block->setJointWith(Block::createBlock(blockSprite2, 100, Color3B::YELLOW, true));
		break;
	}
	if (block) {
		CC_SAFE_RETAIN(block);
		CC_SAFE_RETAIN(block->getAttachedBlock());
	}

	return block;
}

Block* Block::createBlock(Sprite *sprite, int scores, Color3B color, bool isAttached) {
	Block* block = Block::create(sprite);
	block->setSprite(sprite);
	block->setScores(scores);
	block->setColor(color);
	block->setisAttached(isAttached);

	if (block)
		return block;
	else
		return nullptr;
}

b2WeldJointDef Block::getJointDef() {
	b2WeldJointDef jointDef;
	jointDef.collideConnected = false;
	jointDef.localAnchorA = b2Vec2(JOINT_BLOCK_DIST, 0);
	jointDef.localAnchorB = b2Vec2(0, 0);

	return jointDef;
}

void Block::setJointWith(Block *block) {
	_attachedBody = block->getBody();
	_attachedBlock = block;

	b2WeldJointDef jointDef = Block::getJointDef();
	jointDef.bodyA = _body;
	jointDef.bodyB = _attachedBody;
	
	MainGameScene::getWorld()->CreateJoint(&jointDef);
}

void Block::setSize(Size _size) {
	_size = _size;
}

void Block::setPositionInPxl(Vec2 pos) {
	_sprite->setPosition(pos);
	_body->SetTransform(b2Vec2(pos.x / SCALE_RATIO, pos.y / SCALE_RATIO), 0);
}

Size Block::getSize() {
	int a = _sprite->getContentSize().width * 2;
	int b = ((Sprite*)_attachedBody->GetUserData())->getPositionX();
	int c = ((Sprite*)_attachedBody->GetUserData())->getContentSize().width;
	int d = _sprite->getPositionX();
	int e = _sprite->getContentSize().width;
	
	_size.width = a + (b - d - e);
	_size.height = _sprite->getContentSize().height;

	return _size;
}

Vec2 Block::getPosOnField() {
	Vec2 result;

	result.y = round(_sprite->getPositionY() / _sprite->getContentSize().height);
	result.x = round(_sprite->getPositionX() / _sprite->getContentSize().width);

	return result;
}

Vec2 Block::getPosOnField(Sprite *spr) {
	Vec2 result;

	result.y = round(spr->getPositionY() / spr->getContentSize().height);
	result.x = round(spr->getPositionX() / spr->getContentSize().width);

	return result;
}

void Block::createJointListener(void* args) {
	auto bodies = static_cast<bodiesStructArgs*>(args);
	b2WeldJointDef jointDef;
	jointDef.collideConnected = false;
	jointDef.localAnchorA = b2Vec2(bodies->b1->GetLocalPoint(b2Vec2(bodies->pos.x / SCALE_RATIO, 0)).x
									, bodies->b1->GetLocalPoint(b2Vec2(0, bodies->pos.y / SCALE_RATIO)).y + 0.1f);
	jointDef.localAnchorB = b2Vec2(0, 0);
	jointDef.bodyA = bodies->b1;
	jointDef.bodyB = bodies->b2;
	MainGameScene::getWorld()->CreateJoint(&jointDef);

	CC_SAFE_DELETE(args);
}