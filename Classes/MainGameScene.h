#ifndef __MAIN_GAME_SCENE_H__
#define __MAIN_GAME_SCENE_H__

#include "Block.h"
#include <cocos2d.h>
#include "SimpleUI.h"
#include "GameField.h"
#include <Box2D\Box2D.h>
#include "GLES-Render.h"
#include "MessagesQueue.h"
#include "BlockContactListener.h"

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
	virtual void draw(Renderer*, const Mat4&, uint32_t) override;
#endif
	
public: 
	~MainGameScene();
	static b2World* getWorld();
	static Scene* createScene();
	static SimpleUI* getUI() { return MainGameScene::gameScene->_simpleUI; }
	static Block* getCurrentBlock() { return MainGameScene::gameScene->_currentBlock; }
	
	bool init() override;
	void update(float) override;
	void addBlockListener(void*);

	CREATE_FUNC(MainGameScene);
};

#endif //__MAIN_GAME_SCENE_H__