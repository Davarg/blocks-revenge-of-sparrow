#include "Block.h"
#include "MessagesQueue.h"

std::vector<MessagesQueue::Message> msgQueue;
std::map<MessagesQueue::MessageType, std::vector<MessagesQueue::WrapperMessageQueueCallback_1> > mapListeners;

void MessagesQueue::update(float dt) {
	for (Vector<Message>::iterator it = msgQueue.begin();
			it != msgQueue.end();) {
		auto vec = mapListeners.at(it->mt);
		if (vec.size()) {
			for (auto vecIt = vec.begin(); vecIt != vec.end(); vecIt++) {
				auto a = vecIt->getCallback();
				a(it->args);
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
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto iterator = mapListeners.find(mt);
		iterator->second.push_back(callback);
	}
	else {
		std::vector<WrapperMessageQueueCallback_1> vec;
		vec.push_back(callback);
		mapListeners.insert(std::make_pair(mt, vec));
	}
}

void MessagesQueue::removeListener(MessageType mt, WrapperMessageQueueCallback_1& callback) {
	if (mapListeners.end() != mapListeners.find(mt)) {
		auto it = mapListeners.find(mt);
		for (auto itVec = it->second.begin(); itVec != it->second.end();) {
			if (itVec->getUniqId() == callback.getUniqId()) {
				itVec = it->second.erase(itVec);
			}
		}
		it->second.shrink_to_fit();
	}
}