#include <math.h>
#include "UserInput.h"
#include "MainGameScene.h"
#include "RandomBlockDrop.h"
#include "ConstantsRegistry.h"
#include "NextBlockElementUI.h"
#include "BackgroundElementUI.h"

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
		b2Vec2 gravityVec{ 0, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y) };
		world = new b2World(gravityVec);

#ifdef _DEBUG
		debugDraw = new GLESDebugDraw(ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D));
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
	layer->_currentBlock = RandomBlockDrop::dropBlock();

	return scene;
}

bool MainGameScene::init() {
	if (!Layer::init())
		return false;

	_simpleUI = new SimpleUI(this);
	
	GameField::init(8, 13);
	MainGameScene::getWorld()->SetContactListener(&_blockContactListener);

	MessagesQueue::messageQueueCallback_2 c2Scene = MainGameScene::wrapperToAddBlockListener;
	MessagesQueue::messageQueueCallback_1 c1Block = Block::createJointListener;
	MessagesQueue::WrapperMessageQueueCallback_2 callback2(c2Scene, "MainGameSceneAddBlock");
	MessagesQueue::WrapperMessageQueueCallback_1 callback1(c1Block, "BlockCreateJoint");
	MessagesQueue::addListener(MessagesQueue::MessageType::ADD_BLOCK_ON_SCENE, static_cast<void*>(this), callback2);
	MessagesQueue::addListener(MessagesQueue::MessageType::CREATE_JOINT, callback1);

	_simpleUI->show();

	return true;
}

void MainGameScene::update(float dt) {
	int velocityIterations = 10;
	int positionIterations = 10;	MainGameScene::getWorld()->Step(dt, velocityIterations, positionIterations);
	for (b2Body *body = MainGameScene::getWorld()->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData()) {
			Sprite *spr = static_cast<Sprite*>(body->GetUserData());
			if (spr != nullptr) {
				Size size = spr->getContentSize();
				if (spr->getAnchorPoint() == Vec2::ANCHOR_BOTTOM_LEFT) {
					spr->setPosition({ (body->GetPosition().x 
								* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)) - size.width / 2
						, (body->GetPosition().y 
								* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)) - size.height / 2 });
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
	pause();

	GameField::setBlock(_currentBlock);
	GameField::checkField();
	_currentBlock = RandomBlockDrop::dropBlock();

	resume();
}