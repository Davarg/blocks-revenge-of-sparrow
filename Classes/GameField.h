#ifndef __GAME_FIELD_H__
#define __GAME_FIELD_H__

#include "Block.h"
#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class GameField {
private:
	static int _fieldWidth;
	static int _fieldHeight;
	CC_DISALLOW_IMPLICIT_CONSTRUCTORS(GameField);

public:
	~GameField();
	
	static b2Vec2 getSize();
	static void checkField();
	static Block* getBlock(Vec2);
	static void setBlock(Block*);
	static void recountGameField();
	static Block* getBlock(int, int);
	static void updateGameField(void*);
	static void setBlock(int, int, Block*);
	static void init(const int, const int);
};

#endif