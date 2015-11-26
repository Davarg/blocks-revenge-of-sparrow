#include "MainGameScene.h"
#include "ConstantsRegistry.h"
#include "BackgroundElementUI.h"

const char* BackgroundElementUI::_name = "BACKGROUND_HANDMADE";

BackgroundElementUI::~BackgroundElementUI() {
	if (_bodyGlass)
		MainGameScene::getWorld()->DestroyBody(_bodyGlass);
	if (!_bodyGlass && _spriteGlass)
		CC_SAFE_RELEASE_NULL(_spriteGlass);
	CC_SAFE_RELEASE_NULL(_layerBack);
}

BackgroundElementUI::BackgroundElementUI(Layer *layer, Size winSize) {
	Vector<SpriteFrame*> animFrames(_numAnimFiles);
	char str[256] = { 0 };
	_animatedSprite = Sprite::create();

	for (int i = 1; i <= animFrames.capacity(); i++) {
		sprintf(str, "%s%d.png", _animPathFolder, i);

		auto frame = SpriteFrame::create(str, Rect(0, 0, 416, 680));
		animFrames.pushBack(frame);
	}

	_layerBack = Layer::create();
	_layerParent = layer;
	_layerBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_layerBack->setPosition(Vec2::ZERO);

	_spriteGlass = Sprite::create(_glassPath);
	_spriteGlass->setPosition(Vec2::ZERO);
	_spriteGlass->setTag(_tagBackground);
	_spriteGlass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spriteGlass->setPosition(Vec2::ZERO);

	_animatedSprite->setPosition(32, 65);
	_animatedSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	animation->setLoops(-1);
	auto animate = Animate::create(animation);
	_animatedSprite->runAction(animate);

#ifdef _DEBUG
	_spriteGlass->setOpacity(60);
	_animatedSprite->setOpacity(60);
#endif
	_layerBack->addChild(_spriteGlass);   
	_layerBack->addChild(_animatedSprite);
	createGlass();
}

void BackgroundElementUI::show() {
	_layerParent->addChild(_layerBack);
}

void BackgroundElementUI::disable(bool flag) {
	_disable = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}

Size BackgroundElementUI::getUserSize() const {
	return _animatedSprite->getContentSize();
}

Vec2 BackgroundElementUI::getAnimatedPosition() const {
	return _animatedSprite->getPosition();
}

void BackgroundElementUI::createGlass() {
	b2BodyDef glassDef;
	glassDef.type = b2_staticBody;
	glassDef.position = b2Vec2(_spriteGlass->getPositionX() 
			/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
		, _spriteGlass->getPositionY() 
			/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D));
	_bodyGlass = MainGameScene::getWorld()->CreateBody(&glassDef);

	b2PolygonShape shapeBottom;
	shapeBottom.SetAsBox(15, 1.5f);
	
	b2FixtureDef fixtureDefBottom;
	fixtureDefBottom.density = 1;
	fixtureDefBottom.friction = 1000;
	fixtureDefBottom.restitution = -0.3f;
	fixtureDefBottom.shape = &shapeBottom;
	fixtureDefBottom.filter.categoryBits = Block::blockFlags::ACTIVE;
	_bodyGlass->CreateFixture(&fixtureDefBottom);

	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(1, 25, { 0, 0 }, 0);

	b2FixtureDef fixtureDefLeft;
	fixtureDefLeft.density = 1;
	fixtureDefLeft.friction = 0;
	fixtureDefLeft.restitution = 0;
	fixtureDefLeft.shape = &shapeLeft;
	_bodyGlass->CreateFixture(&fixtureDefLeft);

	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(1.11f, 25, { 15.1f, 0 }, 0);

	b2FixtureDef fixtureDefRight;
	fixtureDefRight.density = 1;
	fixtureDefRight.friction = 0;
	fixtureDefRight.restitution = 0;
	fixtureDefRight.shape = &shapeRight;
	_bodyGlass->CreateFixture(&fixtureDefRight);
}