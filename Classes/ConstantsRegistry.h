#ifndef __CONSTANTS_REGISTRY_H__
#define __CONSTANTS_REGISTRY_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class ConstantsRegistry {
private:
	bool _locked;
	std::map<int, float32> _values;
	static ConstantsRegistry _registry;

public:
	enum constants {
		MOVEOFFSET,
		X_OFFSET_BLOCK,
		GRAVITY_SCALE_Y,
		JOINT_BLOCK_DIST,
		SCALE_RATIO_BOX2D,
		JOINT_BLOCK_DIST_NEGATIVE
	};

	static float32 getValueForKey(constants);
	static void setValueForKey(constants, float32);
	static void lock() { _registry._locked = true; }
	static bool isLocked() { return _registry._locked; }
};

#endif