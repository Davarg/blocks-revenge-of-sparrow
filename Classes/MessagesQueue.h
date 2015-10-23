#ifndef __MESSAGES_QUEUE_H__
#define __MESSAGES_QUEUE_H__

#include <cocos2d.h>

USING_NS_CC;

class MessagesQueue {
private:
	CC_DISALLOW_IMPLICIT_CONSTRUCTORS(MessagesQueue);

public:
	typedef std::function<void(void*, void*)> messageQueueCallback_2;
	typedef std::function<void(void*)> messageQueueCallback_1;

	class WrapperMessageQueueCallback_1 {
	private:
		std::string _UniqId;
		messageQueueCallback_1 _callback;
	public:
		WrapperMessageQueueCallback_1() : _UniqId("none"){}
		WrapperMessageQueueCallback_1(messageQueueCallback_1 callback, std::string uniqId) {
			_callback = callback;
			_UniqId = uniqId;
		}
		messageQueueCallback_1 getCallback() { return _callback; }
		std::string getUniqId() { return _UniqId; }
	};

	class WrapperMessageQueueCallback_2 {
	private:
		std::string _UniqId;
		messageQueueCallback_2 _callback;
	public:
		WrapperMessageQueueCallback_2() : _UniqId("none"){}
		WrapperMessageQueueCallback_2(messageQueueCallback_2 callback, std::string uniqId) {
			_callback = callback;
			_UniqId = uniqId;
		}
		messageQueueCallback_2 getCallback() { return _callback; }
		std::string getUniqId() { return _UniqId; }
	};
	
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
	static void addListener(MessageType, WrapperMessageQueueCallback_1&);
	static void removeListener(MessageType, WrapperMessageQueueCallback_1&);
	static void addListener(MessageType, void*, WrapperMessageQueueCallback_2&); //For wrappers on non-static method
	static void removeListener(MessageType, void*, WrapperMessageQueueCallback_2&); //For wrappers on non-static method
};

#endif