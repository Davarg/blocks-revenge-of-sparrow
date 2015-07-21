#include "MainGameScene.h"
#include "Constants.h"
#include <math.h>

static b2World *world = nullptr;
static MainGameScene *gameScene = nullptr;

MainGameScene::~MainGameScene() {
	/*if (_spriteGlass)
		CC_SAFE_RELEASE_NULL(_spriteGlass);*/ //Throw error somewhere in GL 
	if (_bodyGlass)
		world->DestroyBody(_bodyGlass);
	if (_moveLeft)
		CC_SAFE_RELEASE_NULL(_moveLeft);
	if (_moveRight)
		CC_SAFE_RELEASE_NULL(_moveRight);
	if (_moveCounterClockwise)
		CC_SAFE_RELEASE_NULL(_moveCounterClockwise);
	if (_moveClockwise)
		CC_SAFE_RELEASE_NULL(_moveClockwise);
	if (_moveDown)
		CC_SAFE_RELEASE_NULL(_moveDown);
	if (_currentBlock)
		CC_SAFE_RELEASE_NULL(_currentBlock);
	if (world)
		CC_SAFE_DELETE(world);
}

b2World* MainGameScene::getWorld() {
	if (!world) {
		b2Vec2 gravityVec{ 0, -1.8f };
		world = new b2World(gravityVec);
	}

	return world;
}

Scene* MainGameScene::createScene() {
	auto scene = Scene::create();
	auto layer = MainGameScene::create();
	gameScene = layer;

	scene->addChild(layer);
	layer->scheduleUpdate();

	return scene;
}

bool MainGameScene::init() {
	if (!Layer::init())
		return false;

	_spriteGlass = nullptr;
	_bodyGlass = nullptr;
	_moveLeft = nullptr;
	_moveRight = nullptr;
	_currentBlock = nullptr;

	auto size = Director::getInstance()->getVisibleSize();
	_spriteGlass = Sprite::create("glass.png");

	_spriteGlass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_spriteGlass->setPosition(Vec2(5, 3));
	
	this->addChild(_spriteGlass);

	auto listenerKey = EventListenerKeyboard::create();
	listenerKey->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKey, this);

	this->createGlass();
	this->_currentBlock = Block::generateBlock();
	if (this->_currentBlock) {
		this->_currentBlock->setPositionInPxl(Vec2(_spriteGlass->getPositionX() + 4
			, _spriteGlass->getContentSize().height - 2));
		this->addChild(this->_currentBlock->getSprite());

		this->_currentBlock->getAttachedBlock()->setPositionInPxl(Vec2(this->_currentBlock->getSprite()->getContentSize().width
			+ this->_currentBlock->getSprite()->getPositionX()
			, this->_currentBlock->getSprite()->getPositionY()));
		this->addChild(this->_currentBlock->getAttachedBlock()->getSprite());
	}

	_moveLeft = CommandMoveLeft::create();
	_moveRight = CommandMoveRight::create();
	_moveCounterClockwise = CommandMoveCounterClockwise::create();

	if (_moveLeft && _moveRight && _moveCounterClockwise) {
		CC_SAFE_RETAIN(_moveLeft);
		CC_SAFE_RETAIN(_moveRight);
		CC_SAFE_RETAIN(_moveCounterClockwise);
	}
	_moveClockwise = nullptr;//new CommandMoveClockwise(); //Memory Leak
	_moveDown = nullptr;//CommandMoveDown::create();

	this->initArrayOfBlocks();

	_glassHeight = _spriteGlass->getContentSize().height / _currentBlock->getSprite()->getContentSize().height;
	_glassWidt = _spriteGlass->getContentSize().width / _currentBlock->getSprite()->getContentSize().width;

	MainGameScene::getWorld()->SetContactListener(&this->blockContactListener);
	MessagesQueue::addListener(MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, static_cast<void*>(this)
				, &MainGameScene::wrapperToAddBlockListener);
	MessagesQueue::addListener(MessagesQueue::MessageType::CREATE_JOINT, &Block::createJointListener);

	return true;
}

void MainGameScene::createGlass() {
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

void MainGameScene::update(float dt) {
	int velocityIterations = 10;
	int positionIterations = 10;	MainGameScene::getWorld()->Step(dt, velocityIterations, positionIterations);
	for (b2Body *body = MainGameScene::getWorld()->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData()) {
			Sprite *spr = static_cast<Sprite*>(body->GetUserData());
			if (spr != nullptr) {
				spr->setPosition(Vec2(body->GetPosition().x * SCALE_RATIO, body->GetPosition().y * SCALE_RATIO));
				spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
			}
		}
	}

	MainGameScene::getWorld()->ClearForces();
	MessagesQueue::update(dt);
}

void MainGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (!_moveRight->isExecute()) {
			if (!_moveLeft->isExecute()) {
				_moveLeft->execute(_currentBlock);
				Director::getInstance()->getScheduler()->scheduleUpdate(_moveLeft, 3, false);
			}
		}
		else
			_moveRight->undo();
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (!_moveLeft->isExecute()) {
			if (!_moveRight->isExecute()) {
				_moveRight->execute(_currentBlock);
				Director::getInstance()->getScheduler()->scheduleUpdate(_moveRight, 3, false);
			}
		}
		else
			_moveLeft->undo();
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (!_moveCounterClockwise->isExecute()) {
			_moveCounterClockwise->execute(_currentBlock);
			Director::getInstance()->getScheduler()->scheduleUpdate(_moveCounterClockwise, 3, false);
		}
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		/*if (!_moveDown->isExecute()) {
			_moveDown->execute(_currentBlock);
			Director::getInstance()->getScheduler()->scheduleUpdate(_moveDown, 3, false);
		}*/
		break;
	}
}

void MainGameScene::wrapperToAddBlockListener(void* ptrObj, void* args) {
	MainGameScene *ptrScene = static_cast<MainGameScene*>(ptrObj);
	ptrScene->addBlockListener(args);
}

void MainGameScene::addBlockListener(void* args) {
	this->pauseSchedulerAndActions();

	GameField::setBlock(this->_currentBlock);
	GameField::checkField();

	this->_currentBlock = Block::generateBlock();
	if (this->_currentBlock) {
		this->_currentBlock->setPositionInPxl(Vec2(_spriteGlass->getPositionX() + 4
			, _spriteGlass->getContentSize().height - 2));
		this->addChild(this->_currentBlock->getSprite());

		this->_currentBlock->getAttachedBlock()->setPositionInPxl(Vec2(this->_currentBlock->getSprite()->getContentSize().width
			+ this->_currentBlock->getSprite()->getPositionX()
			, this->_currentBlock->getSprite()->getPositionY()));
		this->addChild(this->_currentBlock->getAttachedBlock()->getSprite());
	}

	this->resumeSchedulerAndActions();
}

void MainGameScene::initArrayOfBlocks() {
	if (_currentBlock != nullptr) {
		const int HEIGHT = _spriteGlass->getContentSize().height / _currentBlock->getSprite()->getContentSize().height;
		const int WIDTH = _spriteGlass->getContentSize().width / _currentBlock->getSprite()->getContentSize().width;

		GameField::init(HEIGHT, WIDTH);
	}
	else {
		cocos2d::log("Block doesn't have size");
		throw;
	}
}