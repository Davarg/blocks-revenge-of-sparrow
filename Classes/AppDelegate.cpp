#include "AppDelegate.h"
#include "MainGameScene.h"
#include "ConstantsRegistry.h"

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
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::MOVEOFFSET, 0.8f);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::X_OFFSET_BLOCK, 6);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D, 32);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y, -6.8f);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::JOINT_BLOCK_DIST, 1.6f);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::JOINT_BLOCK_DIST_NEGATIVE, -0.6f);
	ConstantsRegistry::lock();

	//auto size = new Size(720, 1280);
	auto sizeFrame = new Size(480, 800);
	
	auto vecSearchPaths = FileUtils::getInstance()->getSearchPaths();
	vecSearchPaths.push_back("../Resources/wvga");
	vecSearchPaths.push_back("../Resources/wvga/ui/back_anim");
	FileUtils::getInstance()->setSearchPaths(vecSearchPaths);
    
	auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Blocks:Revenge of Sparrow");
		glview->setFrameSize(sizeFrame->width, sizeFrame->height);
        director->setOpenGLView(glview);
    }

	glview->setDesignResolutionSize(sizeFrame->width, sizeFrame->height, ResolutionPolicy::EXACT_FIT);

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
