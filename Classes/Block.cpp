#include "Block.h"
#include "MainGameScene.h"
#include "Constants.h"
#include "GameField.h"
#include <math.h>
#include "BackgroundElementUI.h"

const char* Block::_blockRedPath = "block_red.png";
const char* Block::_blockGreenPath = "block_green.png";
const char* Block::_blockYellowPath = "block_yellow.png";

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
#ifdef _DEBUG
		_sprite->setOpacity(60);
#endif

		b2BodyDef bodyDef;
		bodyDef.position = b2Vec2(_sprite->getPositionX() / SCALE_RATIO_BOX2D, _sprite->getPositionY() / SCALE_RATIO_BOX2D);
		bodyDef.userData = _sprite;
		bodyDef.type = b2_dynamicBody;

		b2PolygonShape shape;
		shape.SetAsBox((_sprite->getContentSize().width / SCALE_RATIO_BOX2D) / 2.2f
			, (_sprite->getContentSize().height / SCALE_RATIO_BOX2D) / 2.2f);

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

Block::blockInfo Block::generateBlockInfo() {
	blockInfo result;
	std::mt19937 generator(time(0));
	std::uniform_int_distribution<> uid(0, 2);

	const int COLOR1 = uid(generator);
	const int COLOR2 = uid(generator);

	switch (COLOR1) {
	case 0:
		result.colorFirst = Color3B::GREEN;
		result.spritePathFirst = _blockGreenPath;
		break;

	case 1:
		result.colorFirst = Color3B::RED;
		result.spritePathFirst = _blockRedPath;
		break;

	case 2:
		result.colorFirst = Color3B::YELLOW;
		result.spritePathFirst = _blockYellowPath;
		break;
	}

	switch (COLOR2) {
	case 0:
		result.colorSecond = Color3B::GREEN;
		result.spritePathSecond = _blockGreenPath;
		break;

	case 1:
		result.colorSecond = Color3B::RED;
		result.spritePathSecond = _blockRedPath;
		break;

	case 2:
		result.colorSecond = Color3B::YELLOW;
		result.spritePathSecond = _blockYellowPath;
		break;
	}

	result.scoresFirst = 100;
	result.scoresSecond = 100;
	return result;
}



Block* Block::createBlock(blockInfo info) {
	Block *result = nullptr;

	auto spriteA = Sprite::create(info.spritePathFirst);
	spriteA->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	auto a = Block::create(spriteA);
	a->setScores(info.scoresFirst);
	a->setColor(info.colorFirst);
	a->setisAttached(false);
	a->setSprite(spriteA);
	result = a;

	auto spriteB = Sprite::create(info.spritePathSecond);
	spriteB->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	auto b = Block::create(spriteB);
	b->setScores(info.scoresSecond);
	b->setColor(info.colorSecond);
	b->setisAttached(true);
	b->setSprite(spriteB);
	result->setJointWith(b);

	if (result) {
		CC_SAFE_RETAIN(result);
		CC_SAFE_RETAIN(result->getAttachedBlock());
	}

	return result;
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
	Vec2 posSprite = _sprite->getPosition();
	Size size = _sprite->getContentSize();
	_body->SetTransform({ (posSprite.x / SCALE_RATIO_BOX2D) + ((size.width / 2) / SCALE_RATIO_BOX2D)
		, (posSprite.y / SCALE_RATIO_BOX2D) + ((size.height / 2) / SCALE_RATIO_BOX2D) }, 0);
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
	Vec2 localPosAnimSprite;
	BackgroundElementUI *beui = 
		(BackgroundElementUI*)MainGameScene::getUI()->getChildrenByName(BackgroundElementUI::name());
	auto animSprite = beui->getAnimatedSprite();
	localPosAnimSprite = animSprite->convertToNodeSpace(_sprite->getPosition());
	
	
	auto a = round(ceilf(localPosAnimSprite.y / _sprite->getContentSize().height));
	auto b = round(localPosAnimSprite.x / _sprite->getContentSize().width);
	result.y = abs(round(ceilf(localPosAnimSprite.y / _sprite->getContentSize().height)));
	result.x = abs(round(localPosAnimSprite.x / _sprite->getContentSize().width));

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
	auto typeA = bodies->b1->GetType();
	auto typeB = bodies->b2->GetType();

	if ((bodies->b1->GetContactList() && bodies->b2->GetContactList())
		&& (typeA == b2BodyType::b2_dynamicBody
				|| typeA == b2BodyType::b2_staticBody)
			&& (typeB == b2BodyType::b2_dynamicBody
				|| typeB == b2BodyType::b2_staticBody)) {
		auto dst = bodies->b2->GetPosition() - bodies->b1->GetPosition();
		b2WeldJointDef jointDef;

		/*bodies->b1->SetGravityScale(GRAVITY_SCALE_Y);
		bodies->b2->SetGravityScale(GRAVITY_SCALE_Y);*/

		jointDef.collideConnected = false;
		jointDef.localAnchorA = { dst.x, dst.y };
		jointDef.localAnchorB = { 0, 0 };
		jointDef.bodyA = bodies->b1;
		jointDef.bodyB = bodies->b2;
		MainGameScene::getWorld()->CreateJoint(&jointDef);
	}

	CC_SAFE_DELETE(args);
}