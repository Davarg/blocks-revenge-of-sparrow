#include "BackgroundElementUI.h"
#include "MainGameScene.h"
#include "Constants.h"

const char* BackgroundElementUI::_name = "BACKGROUND_UGLY";

BackgroundElementUI::~BackgroundElementUI() {
	if (_bodyGlass)
		MainGameScene::getWorld()->DestroyBody(_bodyGlass);
	if (!_bodyGlass && _spriteGlass)
		CC_SAFE_RELEASE_NULL(_spriteGlass);
}

BackgroundElementUI::BackgroundElementUI(Layer *layer, Vec2 pos) {
	_spriteGlass = Sprite::create(_glassPath);
	_spriteGlass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spriteGlass->setPosition(pos); //_spriteGlass->setPosition(Vec2(5, 3));
	_layer = layer;
	createGlass();
}

void BackgroundElementUI::show() {
	_layer->addChild(_spriteGlass);
}

void BackgroundElementUI::disable(bool flag) {
	_disable = flag;
	if (!flag)
		_layer->removeChild(_spriteGlass);
	else
		_layer->addChild(_spriteGlass);
}

void BackgroundElementUI::createGlass() {
	const int DENSITY = 10;
	const float POS_Y = -0.6;

	b2BodyDef glassDef;
	glassDef.type = b2_staticBody;
	glassDef.position = b2Vec2(_spriteGlass->getPositionX() / SCALE_RATIO
		, _spriteGlass->getPositionY() / SCALE_RATIO);
	_bodyGlass = MainGameScene::getWorld()->CreateBody(&glassDef);

	b2PolygonShape shapeBottom;
	shapeBottom.SetAsBox(_spriteGlass->getContentSize().width / SCALE_RATIO, POS_Y);
	b2FixtureDef fixtureDefBottom;
	fixtureDefBottom.density = 1;
	fixtureDefBottom.friction = 1000;
	fixtureDefBottom.restitution = -0.3f;
	fixtureDefBottom.shape = &shapeBottom;
	fixtureDefBottom.filter.categoryBits = Block::getActiveCategoryBits();
	_bodyGlass->CreateFixture(&fixtureDefBottom);

	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(0.1, _spriteGlass->getContentSize().height / SCALE_RATIO
		, b2Vec2(-0.7, _spriteGlass->getContentSize().height / SCALE_RATIO), 0);
	b2FixtureDef fixtureDefLeft;
	fixtureDefLeft.density = 1;
	fixtureDefLeft.friction = 0;
	fixtureDefLeft.restitution = 0;
	fixtureDefLeft.shape = &shapeLeft;
	_bodyGlass->CreateFixture(&fixtureDefLeft);

	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(0.1, _spriteGlass->getContentSize().height / SCALE_RATIO
		, b2Vec2(_spriteGlass->getContentSize().width / SCALE_RATIO - 0.8
		, _spriteGlass->getContentSize().height / SCALE_RATIO), 0);
	b2FixtureDef fixtureDefRight;
	fixtureDefRight.density = 1;
	fixtureDefRight.friction = 0;
	fixtureDefRight.restitution = 0;
	fixtureDefRight.shape = &shapeRight;
	_bodyGlass->CreateFixture(&fixtureDefRight);
}