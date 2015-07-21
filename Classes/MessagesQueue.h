#ifndef __MESSAGES_QUEUE_H__
#define __MESSAGES_QUEUE_H__

#include "cocos2d.h"

USING_NS_CC;

class MessagesQueue {
private:
	CC_DISALLOW_IMPLICIT_CONSTRUCTORS(MessagesQueue);

public:
	enum MessageType {
		ADD_BLOCK_ON_SCENE,
		UPDATE_GAME_FIELD,
		CREATE_JOINT
	};

	struct Message {
		MessageType mt;
		void* args;
	};

	static void update(float dt);
	static void addMessageToQueue(Message msg);
	static void addListener(MessageType mt, void (*ptrFunc)(void*));
	static void removeListener(MessageType mt, void(*ptrFunc)(void*));
	static void addListener(MessageType mt, void* ptrObj, void(*ptrFunc)(void*, void*)); //For wrappers on non-static method
	static void removeListener(MessageType mt, void* ptrObj, void(*ptrFunc)(void*, void*)); //For wrappers on non-static method
};

#endif