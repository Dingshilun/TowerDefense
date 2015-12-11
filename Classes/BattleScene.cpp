#include "BattleScene.h"
#include "EndScene.h"
#include "GestureLayer.h"
#include "cocostudio/CocoStudio.h"
#include "Monster.h"
using namespace cocostudio::timeline;

Scene* BattleScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = BattleScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool BattleScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("BattleScene/BattleScene.csb");
	//Default scheduler
	scheduleUpdate();
	schedule(schedule_selector(BattleScene::ifwin), 2.0f);

	//Add touch listener to the scene
	//auto listenerTouch = EventListenerTouchOneByOne::create();
	//listenerTouch->setSwallowTouches(false);
	//listenerTouch->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerTouch, this);

	//Obtain map layer from cocos studio design file
	mapLayer = new MapLayer();
	mapLayer->create(static_cast<ui::ScrollView *>(rootNode->getChildByName("ScrollView_1")), 100, 100);

	//Obtain pause layer from cocos studio design file and hide it
	pauseLayer = static_cast<Layer *>(rootNode->getChildByName("Layer_Pause"));
	pauseLayer->setVisible(false);

	//create the gesture Layer
	gestureLayer = new GestureLayer();
	gestureLayer->init(mapLayer);

	//Obtain function layer from cocos studio design file
	functionLayer = static_cast<Layer *>(rootNode->getChildByName("Layer_Funct"));
	auto deleteNode = functionLayer->getChildByName("Panel_5");
	functionLayer->removeChild(deleteNode, true);
	//Obtain buttons in pause layer
	auto buttonResume = static_cast<ui::Button *>(pauseLayer->getChildByName("Panel_1")->getChildByName("Button_Resume"));
	buttonResume->addClickEventListener(CC_CALLBACK_1(BattleScene::buttonResumeClickCallBack, this));

	auto buttonBack = static_cast<ui::Button *>(pauseLayer->getChildByName("Panel_1")->getChildByName("Button_Back"));
	buttonBack->addClickEventListener(CC_CALLBACK_1(BattleScene::buttonBackClickCallBack, this));

	auto buttonSound = static_cast<ui::Button *>(pauseLayer->getChildByName("Panel_1")->getChildByName("Button_Sound"));
	buttonSound->addClickEventListener(CC_CALLBACK_1(BattleScene::buttonSoundClickCallBack, this));

	//Obtain pause button from cocos studio design file
	buttonPause = static_cast<ui::Button*>(functionLayer->getChildByName("Button_Pause"));
	buttonPause->addClickEventListener(CC_CALLBACK_1(BattleScene::buttonPauseClickCallBack, this));

	//Obtain left scroll button in function layer
	buttonLeft = static_cast<ui::Button *>(functionLayer->getChildByName("Button_Left"));
	buttonLeft->setVisible(false);
	buttonLeft->addClickEventListener(CC_CALLBACK_1(BattleScene::buttonLeftClickCallBack, this));

	//Obtain right scroll button in function layer
	buttonRight = static_cast<ui::Button *>(functionLayer->getChildByName("Button_Right"));
	buttonRight->addClickEventListener(CC_CALLBACK_1(BattleScene::buttonRightClickCallBack, this));

	randomEnemy();
	rootNode->addChild(gestureLayer);
	addChild(rootNode);

	return true;
}

void BattleScene::buttonResumeClickCallBack(cocos2d::Ref *pSender)
{
	log("Battle Scene Resume Button Clicked");
	Director::getInstance()->resume();
	pauseLayer->setVisible(false);
}

void BattleScene::buttonBackClickCallBack(cocos2d::Ref *pSender)
{
	log("Battle Scene Back Button Clicked");
	Director::getInstance()->resume();
	Director::getInstance()->popSceneWithTransition<TransitionFade>(1.0f);
}

void BattleScene::buttonSoundClickCallBack(cocos2d::Ref *pSender)
{
	log("Battle Scene Sound Button Clicked");
	auto buttonSound = static_cast<ui::Button *>(pauseLayer->getChildByName("Panel_1")->getChildByName("Button_Sound"));
	pauseLayer->setVisible(false);
}

void BattleScene::buttonPauseClickCallBack(cocos2d::Ref *pSender)
{
	log("Battle Scene Pause Button Clicked");
	Director::getInstance()->pause();
	pauseLayer->setVisible(true);
}

void BattleScene::buttonRightClickCallBack(cocos2d::Ref *pSender)
{
	log("Battle Scene Right Button Clicked");
	mapLayer->scrollMapRight();
	buttonLeft->setVisible(true);
	buttonRight->setVisible(false);
}

void BattleScene::buttonLeftClickCallBack(cocos2d::Ref *pSender)
{
	log("Battle Scene Left Button Clicked");
	mapLayer->scrollMapLeft();
	buttonRight->setVisible(true);
	buttonLeft->setVisible(false);
}

bool BattleScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	//log("Battle Scene Touch Began");
	/*Monster *sprite1 = new RectMonster(10, 5, 45, 0);
	int pointY = touch->getLocation().y;
	if (pointY >= DOWNROWY&&pointY <= MIDDLEROWY)
		mapLayer->addMonster(sprite1, DOWNROW);
	else if (pointY > MIDDLEROWY&&pointY <= UPROWY)
		mapLayer->addMonster(sprite1, MIDDLEROW);
	else if (pointY > UPROWY&&pointY <= LIMITY)
		mapLayer->addMonster(sprite1, UPROW);*/
	return true;
}

void BattleScene::update(float dt)
{
	//log("Battle Scene update");
	mapLayer->checkCollision();
}

void BattleScene::ifwin(float dt)
{
	//log("ifwin");
	int flag = mapLayer->Castle_damage();
	auto battleScene = EndScene::createScene();
	auto transition = TransitionFade::create(1.0f, battleScene);
	switch (flag)
	{
	case Win:
		Director::getInstance()->pushScene(Director::getInstance()->getRunningScene());
		Director::getInstance()->replaceScene(transition);
		break;
	case Lose:
		Director::getInstance()->pushScene(Director::getInstance()->getRunningScene());
		Director::getInstance()->replaceScene(transition);
		break;
	}
}
void BattleScene::randomEnemy()
{
	Monster* sprite1 = new RectMonster(10, 5, 30, true);
	mapLayer->addEnemy(sprite1, DOWNROW);
	Monster* sprite2 = new TriMonster(20, 5, 30, true);
	mapLayer->addEnemy(sprite2, MIDDLEROW);
	Monster* sprite3 = new CircleMonster(10, 5, 30, true);
	mapLayer->addEnemy(sprite3, UPROW);
}
