LOCAL_PATH := $(call my-dir)
APP_CPPFLAGS += -std=c++11

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := 	../../Classes/AppDelegate.cpp\
					../../Classes/BackgroundElementUI.cpp\
					../../Classes/Block.cpp\
					../../Classes/BlockContactListener.cpp\
					../../Classes/BottomPanel.cpp\
					../../Classes/CommandMoveCounterClockwise.cpp\
					../../Classes/CommandMoveDown.cpp\
					../../Classes/CommandMoveLeft.cpp\
					../../Classes/CommandMoveRight.cpp\
					../../Classes/ConstantsRegistry.cpp\
					../../Classes/GameField.cpp\
					../../Classes/GLES-Render.cpp\
					../../Classes/MainGameScene.cpp\
					../../Classes/MessagesQueue.cpp\
					../../Classes/NextBlockElementUI.cpp\
					../../Classes/RandomBlockDrop.cpp\
					../../Classes/ScoresElementUI.cpp\
					../../Classes/SimpleUI.cpp\
					../../Classes/TopPanel.cpp\
					../../Classes/UserInput.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
