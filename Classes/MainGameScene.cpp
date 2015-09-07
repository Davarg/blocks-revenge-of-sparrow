#include "MainGameScene.h"
#include "Constants.h"
#include "BackgroundElementUI.h"
#include <math.h>
#include "UserInput.h"

b2World* MainGameScene::world = nullptr;
MainGameScene* MainGameScene::gameScene = nullptr;
GLESDebugDraw* MainGameScene::debugDraw = nullptr;

MainGameScene::~MainGameScene() {
	/*if (_spriteGlass)
		CC_SAFE_RELEASE_NULL(_spriteGlass);*/ //Throw error somewhere in GL 
	CC_SAFE_RELEASE_NULL(_currentBlock);
	CC_SAFE_DELETE(world);
	CC_SAFE_DELETE(_simpleUI);
}

b2World* MainGameScene::getWorld() {
	if (!world) {
		b2Vec2 gravityVec{ 0, GRAVITY_SCALE_Y };
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

	_simpleUI = new SimpleUI(this);
	_currentBlock = Block::generateBlock();

	BackgroundElementUI *beui = (BackgroundElementUI*)_simpleUI->getChildrenByName(BackgroundElementUI::name());
	
	if (_currentBlock) {
#ifdef _DEBUG
		Vec2 pos = { beui->getAnimatedPosition().x + 52, 740 };
#endif
		_currentBlock->setPositionInPxl(pos);
		beui->getLayer()->addChild(_currentBlock->getSprite());

		_currentBlock->getAttachedBlock()->setPositionInPxl({ _currentBlock->getSprite()->getContentSize().width
				+ _currentBlock->getSprite()->getPositionX()
			, _currentBlock->getSprite()->getPositionY() });
		beui->getLayer()->addChild(_currentBlock->getAttachedBlock()->getSprite());
	}
	
	/*const Size gameFieldSizePxl = beui->getUserSize();
	const Size sizeBlock = _currentBlock->getSprite()->getContentSize();
	GameField::init(gameFieldSizePxl.width / sizeBlock.width, gameFieldSizePxl.height / sizeBlock.height);*/
	GameField::init();

	MainGameScene::getWorld()->SetContactListener(&_blockContactListener);
	MessagesQueue::addListener(MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, static_cast<void*>(this)
				, &MainGameScene::wrapperToAddBlockListener);
	MessagesQueue::addListener(MessagesQueue::MessageType::CREATE_JOINT, &Block::createJointListener);

	_simpleUI->show();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressed, this);
	UserInput *ui = (UserInput*)_simpleUI->getChildrenByName(UserInput::name());
	listener->onKeyReleased = CC_CALLBACK_2(UserInput::onKeyReleased, ui);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	return true;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void MainGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
		UserInput *ui = (UserInput*)_simpleUI->getChildrenByName(UserInput::name());
		ui->onKeyPressed(keyCode, event, _currentBlock);
	}
#endif

void MainGameScene::update(float dt) {
	int velocityIterations = 10;
	int positionIterations = 10;	MainGameScene::getWorld()->Step(dt, velocityIterations, positionIterations);
	for (b2Body *body = MainGameScene::getWorld()->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData()) {
			Sprite *spr = static_cast<Sprite*>(body->GetUserData());
			if (spr != nullptr) {
				Size size = spr->getContentSize();
				if (spr->getAnchorPoint() == Vec2::ANCHOR_BOTTOM_LEFT) {
					spr->setPosition({ (body->GetPosition().x * SCALE_RATIO_BOX2D) - size.width / 2
						, (body->GetPosition().y * SCALE_RATIO_BOX2D) - size.height / 2 });
				}
				spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
			}
		}
	}

	MainGameScene::getWorld()->ClearForces();
	MessagesQueue::update(dt);
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
#ifdef _DEBUG
		Vec2 pos = { beui->getAnimatedPosition().x + 52, 740 };
#endif
		_currentBlock->setPositionInPxl(pos);
		beui->getLayer()->addChild(_currentBlock->getSprite());

		_currentBlock->getAttachedBlock()->setPositionInPxl({ _currentBlock->getSprite()->getContentSize().width
			+ _currentBlock->getSprite()->getPositionX()
			, _currentBlock->getSprite()->getPositionY() });
		beui->getLayer()->addChild(_currentBlock->getAttachedBlock()->getSprite());
	}

	resumeSchedulerAndActions();
}