#include "BackgroundElementUI.h"
#include "MainGameScene.h"
#include "Constants.h"

const char* BackgroundElementUI::_name = "BACKGROUND_HANDMADE";

BackgroundElementUI::~BackgroundElementUI() {
	if (_bodyGlass)
		MainGameScene::getWorld()->DestroyBody(_bodyGlass);
	if (!_bodyGlass && _spriteGlass)
		CC_SAFE_RELEASE_NULL(_spriteGlass);
	CC_SAFE_RELEASE_NULL(_layerBack);
}

BackgroundElementUI::BackgroundElementUI(Layer *layer, Size winSize) {
	auto path = new std::string(_backAnimFolderPath);
	auto vec = new std::vector<std::string>();
	vec->push_back(*path);

	FileUtils::getInstance()->setSearchPaths(*vec);
	const float scaleX = winSize.width / _realSize.width;
	const float scaleY = (winSize.height - 55) / _realSize.height;
	Vector<SpriteFrame*> animFrames(_numAnimFiles);
	char str[10] = { 0 };
	_animatedSprite = Sprite::create();

	for (int i = 1; i <= animFrames.capacity(); i++) {
		sprintf(str, "%d.png", i);
		auto frame = SpriteFrame::create(str, Rect(0, 0, 502, 712));
		animFrames.pushBack(frame);
	}

	_layerBack = Layer::create();
	_layerParent = layer;
	_layerBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_layerBack->setPosition(0, (_realBottomMargin * scaleY) + (_realSize.height * scaleY));
	_layerBack->setContentSize(_realSize);
	_layerBack->setScaleX(scaleX);
	_layerBack->setScaleY(scaleY);

	_spriteGlass = Sprite::create(_glassPath);
	_spriteGlass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spriteGlass->setPosition(0, 0);
	_spriteGlass->setScaleX(_layerBack->getContentSize().width / _spriteGlass->getContentSize().width);
	_spriteGlass->setScaleY(_layerBack->getContentSize().height / _spriteGlass->getContentSize().height);
	_spriteGlass->setTag(_tagBackground);

	_animatedSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_animatedSprite->setScaleX(0.0275f);
	_animatedSprite->setScaleY(0.0267f);
	_animatedSprite->setPosition(1.1f, 0.51f);
	_animatedSprite->setContentSize(animFrames.at(0)->getOriginalSizeInPixels());

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	animation->setLoops(-1);
	auto animate = Animate::create(animation);
	_animatedSprite->runAction(animate);

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