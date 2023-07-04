/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "ReadyScene.h"
#include "GameScene.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

Scene* ReadyScene::createScene()
{
	auto scene = Scene::create();
	
	auto layer = ReadyScene::create();
	
	scene->addChild(layer);
    return scene; 
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ReadyScene::init()
{
	
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto play = MenuItemImage::create("start_btn.png", "start_btn_pressed.png", CC_CALLBACK_1(ReadyScene::PlayScene, this));
	
	auto menu = Menu::create(play, NULL);
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(menu, 1);



	auto logo = Sprite::create("bird_logo.png");
	logo->setPosition(Vec2(visibleSize.width / 2, 450));
	this->addChild(logo , 4);

	auto bird = Sprite::create("bird.png");
	bird->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bird);

	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background);





	/*auto logo = Label::createWithTTF("VTC Academy", "fonts/arial.ttf", 20);
	logo->setColor(Color3B(0, 255, 0));
	logo->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50);
	this->addChild(logo);*/

	//auto play = MenuItemLabel::create(Label::createWithTTF("PLAY", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_1(ReadyScene::PlayScene, this));

	//scheduleOnce(CC_SCHEDULE_SELECTOR(ReadyScene::PlayScene), 1.5f);

	//auto start_btn = Sprite::createWithSpriteFrameName("start_btn.png");
	//auto start_btn_pressed = Sprite::createWithSpriteFrameName("start_btn_pressed.png");
	

    return true;
}
void ReadyScene::PlayScene(Ref* pSender)
{
	auto gameScene = GameScene::createScene();
	TransitionScene *transition = TransitionFade::create(1.0f, gameScene);
	Director::getInstance()->replaceScene(transition);

}



