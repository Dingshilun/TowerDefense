#include "StartScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("StartScene//StartScene.csb");

    addChild(rootNode);

    return true;
}

bool StartScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	log("StartScene touched");
	/*create Vector<Sprite*>*/
	auto sp = Sprite::create("StartScene//Tony.png");
	sp->setPosition(Point(touch->getLocation().x, touch->getLocation().y));
	this->addChild(sp);
	return true;
}

void StartScene::onTouchEnded(Touch *touch, Event *unused_event)
{

}
