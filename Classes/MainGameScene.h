#ifndef __MAIN_GAME_SCENE_H__
#define __MAIN_GAME_SCENE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Block.h"
#include "Command.h"
#include "CommandMoveLeft.h"
#include "CommandMoveRight.h"
#include "CommandMoveCounterClockwise.h"
#include "CommandMoveClockwise.h"
#include "CommandMoveDown.h"
#include "BlockContactListener.h"
#include "GameField.h"
#include "MessagesQueue.h"
#include "SimpleUI.h"

USING_NS_CC;

class MainGameScene : public Layer {
private:
	Command *_moveLeft;
	Command *_moveDown;
	SimpleUI *_simpleUI;
	Command *_moveRight;
	Block *_currentBlock;
	Command *_moveClockwise;
	Command *_moveCounterClockwise;
	BlockContactListener _blockContactListener;
	
public: 
	~MainGameScene();
	static b2World* getWorld();
	static Scene* createScene();
	static void wrapperToAddBlockListener(void* ptrObj, void* args);
	
	bool init() override;
	void addBlockListener(void* args);
	void update(float dt) override;
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);	

	CREATE_FUNC(MainGameScene);
};

#endif //__MAIN_GAME_SCENE_H__