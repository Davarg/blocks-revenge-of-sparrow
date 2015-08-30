#ifndef __MAIN_GAME_SCENE_H__
#define __MAIN_GAME_SCENE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Block.h"
#include "BlockContactListener.h"
#include "GameField.h"
#include "MessagesQueue.h"
#include "SimpleUI.h"
#include "GLES-Render.h"

USING_NS_CC;

class MainGameScene : public Layer {
private:
	static b2World* world;
	static MainGameScene* gameScene;
	static GLESDebugDraw* debugDraw;

	SimpleUI *_simpleUI;
	Block *_currentBlock;
	BlockContactListener _blockContactListener;
#ifdef _DEBUG
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
#endif
	
public: 
	~MainGameScene();
	static b2World* getWorld();
	static Scene* createScene();
	static SimpleUI* getUI() { return MainGameScene::gameScene->_simpleUI; }
	static void wrapperToAddBlockListener(void* ptrObj, void* args);
	
	bool init() override;
	void update(float dt) override;
	void addBlockListener(void* args);

	CREATE_FUNC(MainGameScene);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
#endif
};

#endif //__MAIN_GAME_SCENE_H__