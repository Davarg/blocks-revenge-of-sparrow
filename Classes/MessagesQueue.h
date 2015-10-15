#ifndef __MESSAGES_QUEUE_H__
#define __MESSAGES_QUEUE_H__

#include <cocos2d.h>

USING_NS_CC;

class MessagesQueue {
private:
	CC_DISALLOW_IMPLICIT_CONSTRUCTORS(MessagesQueue);

public:
	enum MessageType {
		ADD_BLOCK_ON_SCENE,
		UPDATE_GAME_FIELD,
		UPDATE_SCORES,
		CREATE_JOINT
		
	};

	struct Message {
		MessageType mt;
		void* args;
	};

	static void update(float dt);
	static void addMessageToQueue(Message);
	static void addListener(MessageType, void (*)(void*));
	static void removeListener(MessageType, void(*)(void*));
	static void addListener(MessageType, void*, void(*)(void*, void*)); //For wrappers on non-static method
	static void removeListener(MessageType, void*, void(*)(void*, void*)); //For wrappers on non-static method
};

#endif