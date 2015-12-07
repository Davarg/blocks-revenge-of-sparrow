#include "Block.h"
#include "MessagesQueue.h"

std::queue<MessagesQueue::Message> msgQueue;
std::map<MessagesQueue::MessageType, std::vector<MessagesQueue::WrapperMessageQueueCallback_1> > mapListeners;

void MessagesQueue::update(float dt) {
	while (!msgQueue.empty()) {
		auto vectorListeners = mapListeners.at(msgQueue.front().mt);
		if (vectorListeners.size()) {
			for (auto listenerIter = vectorListeners.begin(); listenerIter != vectorListeners.end(); listenerIter++) {
				auto a = listenerIter->getCallback();
				a(msgQueue.front().args);
			}
		}
		msgQueue.pop();
	}
}

void MessagesQueue::addMessageToQueue(Message msg) {
	msgQueue.push(msg);
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