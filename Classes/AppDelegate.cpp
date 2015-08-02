#include "AppDelegate.h"
#include "MainGameScene.h"

USING_NS_CC;

AppDelegate::~AppDelegate()
{
}

AppDelegate::AppDelegate() {
}



//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	//auto size = new Size(720, 1280);
	auto sizeFrame = new Size(480, 800);
    
	auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Blocks:Revenge of Sparrow");
		glview->setFrameSize(sizeFrame->width, sizeFrame->height);
        director->setOpenGLView(glview);
    }

	glview->setDesignResolutionSize(sizeFrame->width, sizeFrame->height, ResolutionPolicy::NO_BORDER);

    director->setAnimationInterval(1.0 / 60);
    auto scene = MainGameScene::createScene();
    director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
