#ifndef __GAME_FIELD_H__
#define __GAME_FIELD_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Block.h"

USING_NS_CC;

class GameField {
private:
	CC_DISALLOW_IMPLICIT_CONSTRUCTORS(GameField);

public:
	~GameField();
	
	static b2Vec2 getSize();
	static void checkField();
	static void recountGameField();
	static Block* getBlock(Vec2 pos);
	static void setBlock(Block* block);
	static Block* getBlock(int y, int x);
	static void updateGameField(void* args);
	static void setBlock(int y, int x, Block* block);
	static void init(const int height, const int width);
};

#endif