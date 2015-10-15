#ifndef __CONSTANTS_REGISTRY_H__
#define __CONSTANTS_REGISTRY_H__

#include <Box2D\Box2D.h>
#include <cocos2d.h>

USING_NS_CC;

class ConstantsRegistry {
private:
	static bool isLocked;
	static ConstantsRegistry constantsRegistry;

	std::map<int, float32> _values;
	
	CC_DISALLOW_IMPLICIT_CONSTRUCTORS(ConstantsRegistry);

public:
	enum constants {
		MOVEOFFSET,
		X_OFFSET_BLOCK,
		GRAVITY_SCALE_Y,
		JOINT_BLOCK_DIST,
		SCALE_RATIO_BOX2D,
		JOINT_BLOCK_DIST_NEGATIVE
	};

	static void lock() { isLocked = true; }
	static float32 getValueForKey(constants);
	static bool isLocked() { return isLocked; }
	static void setValueForKey(constants, float32);
};

#endif