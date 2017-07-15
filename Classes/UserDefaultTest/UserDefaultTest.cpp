//
//  UserDefaultTest.cpp
//  actual_chap08
//
//  Created by xiang on 2017/7/15.
//
//

#include "UserDefaultTest.hpp"

USING_NS_CC;

Scene *UserDefaultTest::createScene() {
    auto scene = Scene::create();
    auto layer = UserDefaultTest::create();
    scene->addChild(layer);
    return scene;
}

bool UserDefaultTest::init() {
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto label = Label::createWithSystemFont("用户数据存储测试(请查看控制台)", "", 16);
    this->addChild(label, 0);
    label->setPosition(Vec2(visibleSize.width/2, visibleSize.height-50));
    
    // ①设置用户数据
    UserDefault::getInstance()->setStringForKey("string", "value1");
    UserDefault::getInstance()->setIntegerForKey("integer", 10);
    UserDefault::getInstance()->setFloatForKey("float", 2.3f);
    UserDefault::getInstance()->setDoubleForKey("double", 2.4);
    UserDefault::getInstance()->setBoolForKey("bool", true);
    
     log("********************** 第一次设置的用户数据 ***********************");
    
    // ②打印之前设置的数据值
    std::string ret = UserDefault::getInstance()->getStringForKey("string");
    log("string is %s", ret.c_str());
    double d = UserDefault::getInstance()->getDoubleForKey("double");
    log("double is %f", d);
    int i = UserDefault::getInstance()->getIntegerForKey("integer");
    log("integer is %d", i);
    float f = UserDefault::getInstance()->getFloatForKey("float");
    log("float is %f", f);
    bool b = UserDefault::getInstance()->getBoolForKey("bool");
    if (b)
    {
        log("bool is true");
    }
    else
    {
        log("bool is false");
    }
    
    // ③通过Key修改用户数据
    UserDefault::getInstance()->setStringForKey("string", "value2");
    UserDefault::getInstance()->setIntegerForKey("integer", 11);
    UserDefault::getInstance()->setFloatForKey("float", 2.5f);
    UserDefault::getInstance()->setDoubleForKey("double", 2.6);
    UserDefault::getInstance()->setBoolForKey("bool", false);
    // ④将XML数据保存到文件中。
    UserDefault::getInstance()->flush();
    
    log("********************** 修改之后的用户数据 ***********************");
    // ⑤再次获取用户数据并打印
    ret = UserDefault::getInstance()->getStringForKey("string");
    log("string is %s", ret.c_str());
    d = UserDefault::getInstance()->getDoubleForKey("double");
    log("double is %f", d);
    i = UserDefault::getInstance()->getIntegerForKey("integer");
    log("integer is %d", i);
    f = UserDefault::getInstance()->getFloatForKey("float");
    log("float is %f", f);
    b = UserDefault::getInstance()->getBoolForKey("bool");
    if (b)
    {
        log("bool is true");
    }
    else
    {
        log("bool is false");
    }
    
    // ⑥获取XML文件的默认地址
    std::string filePath = UserDefault::getInstance()->getXMLFilePath();
    log("XML文件保存路径：%s",filePath.c_str());


    
    
    return true;
}































