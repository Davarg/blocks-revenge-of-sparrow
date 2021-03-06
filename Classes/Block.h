#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <cocos2d.h>
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
	static const char* _emptyBlock;
	static const char* _blockRedPath;
	static const char* _blockGreenPath;
	static const char* _blockVioletPath;

	void setSprite(Sprite* sprite) { _sprite = sprite; }
	void setScores(int scores) { _scores = scores; }
	void setColor(Color3B color) { _color = color; }

public:
	struct bodiesStructArgs {
		b2Body *b1 = nullptr;
		b2Body *b2 = nullptr;
	};

	struct blockInfo {
		int scoresFirst;
		int scoresSecond;
		Color3B colorFirst;
		Color3B colorSecond;
		std::string spritePathFirst;
		std::string spritePathSecond;
	};

	enum blockFlags {
		ACTIVE  = 0x1001,
		PASSIVE = 0x0011,
		NEED_TO_STOP = 0x0111,
		STOPPED = 0x1111
	};
	
	static Block* createBlock(blockInfo);
	static blockInfo generateBlockInfo();
	static b2WeldJointDef getJointDef();
	static Vec2 getPosOnField(Sprite*);

	CC_PROPERTY(Size, _size, Size);
	CREATE_FUNC_CUSTOM(Block, Sprite*);
	CC_SYNTHESIZE_READONLY(int, _scores, Scores);
	CC_SYNTHESIZE_READONLY(b2Body*, _body, Body);
	CC_SYNTHESIZE(bool, _isAttached, isAttached);
	CC_SYNTHESIZE_READONLY(Color3B, _color, Color);
	CC_SYNTHESIZE_READONLY(Sprite*, _sprite, Sprite);
	CC_SYNTHESIZE_READONLY(b2Body*, _attachedBody, AttachedBody);
	CC_SYNTHESIZE_READONLY(Block*, _attachedBlock, AttachedBlock);
	
	void createJointListener(void*);
	void setPositionInPxl(Vec2);
	void setJointWith(Block*);
	Vec2 getPosOnField();
	bool init(Sprite*);
	void destroy();
};

#endif // !__BLOCK_H__
