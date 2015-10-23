#include "Block.h"
#include "MessagesQueue.h"

struct funcAndObj {
	void* obj = nullptr;
	MessagesQueue::WrapperMessageQueueCallback_1 callback_1;
	MessagesQueue::WrapperMessageQueueCallback_2 callback_2;
};

std::vector<MessagesQueue::Message> msgQueue;
std::map<MessagesQueue::MessageType, std::vector<funcAndObj> > mapListeners;

void MessagesQueue::update(float dt) {
	for (Vector<Message>::iterator it = msgQueue.begin();
			it != msgQueue.end();) {
		auto vec = mapListeners.at(it->mt);
		if (vec.size()) {
			for (auto vecIt = vec.begin(); vecIt != vec.end(); vecIt++) {
				if (vecIt->obj != nullptr) {
					auto a = vecIt->callback_2.getCallback();
					a(vecIt->obj, it->args);
				}
				else {
					auto b = vecIt->callback_1.getCallback();
					b(it->args);
				}
			}
		}
		it = msgQueue.erase(it);
	}
	msgQueue.shrink_to_fit();
}

void MessagesQueue::addMessageToQueue(Message msg) {
	msgQueue.push_back(msg);
}

void MessagesQueue::addListener(MessageType mt, WrapperMessageQueueCallback_1& callback) {
	funcAndObj fao;
	fao.callback_1 = callback;
	
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto iterator = mapListeners.find(mt);
		iterator->second.push_back(fao);
	}
	else {
		std::vector<funcAndObj> vec;
		vec.push_back(fao);
		mapListeners.insert(std::make_pair(mt, vec));
	}
}

void MessagesQueue::addListener(MessageType mt, void* ptrObj, WrapperMessageQueueCallback_2& callback) {
	funcAndObj fao;
	fao.callback_2 = callback;
	fao.obj = ptrObj;

	if (mapListeners.end() != mapListeners.find(mt)) {
		auto iterator = mapListeners.find(mt);
		iterator->second.push_back(fao);
	}
	else {
		std::vector<funcAndObj> vec;
		vec.push_back(fao);
		mapListeners.insert(std::make_pair(mt, vec));
	}
}

void MessagesQueue::removeListener(MessageType mt, WrapperMessageQueueCallback_1& callback) {
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto it = mapListeners.find(mt);
		for (auto itVec = it->second.begin(); itVec != it->second.end();) {
			if (itVec->callback_1.getUniqId() == callback.getUniqId()) {
				itVec = it->second.erase(itVec);
			}
		}
		it->second.shrink_to_fit();
	}
}

void MessagesQueue::removeListener(MessageType mt, void* ptrObj, WrapperMessageQueueCallback_2& callback) {
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto it = mapListeners.find(mt);
		for (auto itVec = it->second.begin(); itVec != it->second.end();) {
			if (itVec->callback_2.getUniqId() == callback.getUniqId() && itVec->obj == ptrObj) {
				itVec = it->second.erase(itVec);
			}
		}
		it->second.shrink_to_fit();
	}
}