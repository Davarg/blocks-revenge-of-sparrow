#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>

USING_NS_CC;

#define CREATE_FUNC_CUSTOM(__TYPE__, __VAR_TYPE__) \
static __TYPE__* create(__VAR_TYPE__ var) \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(var)) \
	    { \
        pRet->autorelease(); \
        return pRet; \
	    } \
	    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

class Block : public Ref {
private:
	static const char* _blockRedPath;
	static const char* _blockGreenPath;
	static const char* _blockYellowPath;

	void setSprite(Sprite* sprite) { _sprite = sprite; }
	void setScores(int scores) { _scores = scores; }
	void setColor(Color3B color) { _color = color; }

public:
	struct bodiesStructArgs {
		b2Body *b1 = nullptr;
		b2Body *b2 = nullptr;
	};

	static Block* createBlock(Sprite *sprite, int scores, Color3B color, bool isAttached = false);
	static uint16 getPassiveCategoryBits() { return 0x0999; }
	static uint16 getActiveCategoryBits() { return 0x0666; }
	static void createJointListener(void* args);
	static Vec2 getPosOnField(Sprite *spr);
	static b2WeldJointDef getJointDef();
	static Block* generateBlock();

	CC_PROPERTY(Size, _size, Size);
	CREATE_FUNC_CUSTOM(Block, Sprite*);
	CC_SYNTHESIZE_READONLY(int, _scores, Scores);
	CC_SYNTHESIZE(bool, _isAttached, isAttached);
	CC_SYNTHESIZE_READONLY(b2Body*, _body, Body);
	CC_SYNTHESIZE_READONLY(Color3B, _color, Color);
	CC_SYNTHESIZE_READONLY(Sprite*, _sprite, Sprite);
	CC_SYNTHESIZE_READONLY(b2Body*, _attachedBody, AttachedBody);
	CC_SYNTHESIZE_READONLY(Block*, _attachedBlock, AttachedBlock);
	
	void setJointWith(Block *block);
	void setPositionInPxl(Vec2 pos);
	bool init(Sprite* _sprite);
	Vec2 getPosOnField();
	void destroy();
};

#endif // !__BLOCK_H__
