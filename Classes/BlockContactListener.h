#ifndef __BLOCK_CONTACT_LISTENER__
#define __BLOCK_CONTACT_LISTENER__

#include <cocos2d.h>
#include <Box2D/Box2D.h>

USING_NS_CC;

class BlockContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact*) override;
	static void BeginContactStatic(b2Contact*);
};

#endif