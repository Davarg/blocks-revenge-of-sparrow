#include "MainGameScene.h"
#include "Constants.h"
#include "BackgroundElementUI.h"
#include <math.h>
#include "GLES-Render.h"

static b2World *world = nullptr;
static MainGameScene *gameScene = nullptr;
static GLESDebugDraw *debugDraw = nullptr;

MainGameScene::~MainGameScene() {
	/*if (_spriteGlass)
		CC_SAFE_RELEASE_NULL(_spriteGlass);*/ //Throw error somewhere in GL 
	CC_SAFE_RELEASE_NULL(_moveLeft);
	CC_SAFE_RELEASE_NULL(_moveRight);
	CC_SAFE_RELEASE_NULL(_moveCounterClockwise);
	CC_SAFE_RELEASE_NULL(_moveClockwise);
	CC_SAFE_RELEASE_NULL(_moveDown);
	CC_SAFE_RELEASE_NULL(_currentBlock);
	CC_SAFE_DELETE(world);
	CC_SAFE_DELETE(_simpleUI);
}

b2World* MainGameScene::getWorld() {
	if (!world) {
		b2Vec2 gravityVec{ 0, -1.8f };
		world = new b2World(gravityVec);

#ifdef _DEBUG
		debugDraw = new GLESDebugDraw(SCALE_RATIO_BOX2D);
		uint32  flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		// flags + = b2Draw :: e_aabbBit;
		// flags + = b2Draw :: e_pairBit;
		flags += b2Draw::e_centerOfMassBit;
		debugDraw->SetFlags(flags);
		world->SetDebugDraw(debugDraw);
#endif
	}

	return world;
}

#ifdef _DEBUG
void MainGameScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
	glEnableVertexAttribArray(0);
	world->DrawDebugData();
}
#endif

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

	_moveLeft = nullptr;
	_moveRight = nullptr;
	_currentBlock = nullptr;

	_simpleUI = new SimpleUI(this);
	_currentBlock = Block::generateBlock();

	BackgroundElementUI *beui = (BackgroundElementUI*)_simpleUI->getChildrenByName(BackgroundElementUI::name());
	
	if (_currentBlock) {
		Vec2 pos = { 0, 0 };
		_currentBlock->setPositionInPxl(pos);
		beui->getLayer()->addChild(_currentBlock->getSprite());

		/*_currentBlock->getAttachedBlock()->setPositionInPxl(Vec2(_currentBlock->getSprite()->getContentSize().width
			+ _currentBlock->getSprite()->getPositionX()
			, _currentBlock->getSprite()->getPositionY()));
		beui->getLayer()->addChild(_currentBlock->getAttachedBlock()->getSprite()); */
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

	/*const Size gameFieldSizePxl = beui->getUserSize();
	const Size sizeBlock = _currentBlock->getSprite()->getContentSize();
	GameField::init(gameFieldSizePxl.width / sizeBlock.width, gameFieldSizePxl.height / sizeBlock.height);*/
	GameField::init();

	MainGameScene::getWorld()->SetContactListener(&_blockContactListener);
	MessagesQueue::addListener(MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, static_cast<void*>(this)
				, &MainGameScene::wrapperToAddBlockListener);
	MessagesQueue::addListener(MessagesQueue::MessageType::CREATE_JOINT, &Block::createJointListener);

	_simpleUI->show();

	return true;
}

void MainGameScene::update(float dt) {
	int velocityIterations = 10;
	int positionIterations = 10;	//MainGameScene::getWorld()->Step(dt, velocityIterations, positionIterations);
	/*for (b2Body *body = MainGameScene::getWorld()->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData()) {
			Sprite *spr = static_cast<Sprite*>(body->GetUserData());
			if (spr != nullptr) {
				spr->setPosition(Vec2(body->GetPosition().x * SCALE_RATIO_BOX2D, body->GetPosition().y * SCALE_RATIO_BOX2D));
				spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
			}
		}
	}

	MainGameScene::getWorld()->ClearForces();
	MessagesQueue::update(dt);*/
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
	pauseSchedulerAndActions();

	GameField::setBlock(_currentBlock);
	GameField::checkField();

	_currentBlock = Block::generateBlock();
	BackgroundElementUI *beui = (BackgroundElementUI*)_simpleUI->getChildrenByName(BackgroundElementUI::name());
	const Size gameFieldSizePxl = beui->getUserSize();
	auto spriteBack = beui->getLayer()->getChildByTag(0);

	if (_currentBlock) {
		Vec2 pos = { spriteBack->getPositionX() + 1, spriteBack->getContentSize().height - 2 };
		_currentBlock->setPositionInPxl(pos);
		beui->getLayer()->addChild(_currentBlock->getSprite());

		_currentBlock->getAttachedBlock()->setPositionInPxl(Vec2(_currentBlock->getSprite()->getContentSize().width
			+ _currentBlock->getSprite()->getPositionX()
			, _currentBlock->getSprite()->getPositionY()));
		beui->getLayer()->addChild(_currentBlock->getAttachedBlock()->getSprite());
	}

	resumeSchedulerAndActions();
}