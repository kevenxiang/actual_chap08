//
//  SimpleAudioEngineTest.cpp
//  actual_chap08
//
//  Created by xiang on 2017/7/14.
//
//

#include "SimpleAudioEngineTest.hpp"

// 根据不同平台选择不同的音效格式文件
#if (CC_TARGET_PLATFORM == CC_PLATFOR_ANDROID)
#define EFFECT_FILE        "effect2.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFOR_MARMALADE)
#define EFFECT_FILE        "effect1.raw"
#else
#define EFFECT_FILE        "effect1.wav"
#endif

// 根据不同平台选择不同的背景音乐格式文件
#if (CC_TARGET_PLATFORM == CC_PLATFOR_WIN32)
#define MUSIC_FILE        "music.mid"
#elif (CC_TARGET_PLATFORM == CC_PLATFOR_BLACKBERRY || CC_TARGET_PLATFORM == CC_PLATFOR_LINUX )
#define MUSIC_FILE        "background.ogg"
#else
#define MUSIC_FILE        "background.mp3"
#endif

USING_NS_CC;

// 自定义按钮类，通过事件监听绑定响应函数
class Button : public Node {
public:
    // 使用图片路径创建精灵
    static Button *createWithSprite(const char *filePath) {
        auto b = new Button();
        if (b && !b->initSpriteButton(filePath)) {
            delete b;
            b = NULL;
        }
        
        return b;
    }
    
    //使用文本创建精灵
    static Button *createWithText(const char *text) {
        auto b = new Button();
        if (b && !b->initTextButton(text)) {
            delete b;
            b = NULL;
        }
        
        return b;
    }
    
    // 析构函数
    ~Button() {}
    
    // 绑定的响应事件函数
    void onTriggered(const std::function<void(void)> &onTriggered) {
        _onTriggered = onTriggered;
    }

    
private:
    
    //构造函数
    Button()
    : _child(NULL) {
        
        // 创建单点触摸事件监听器
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        // 触摸开始，如果触摸点在标签之内，缩放标签
        listener->onTouchBegan = [=](Touch *touch, Event *event) {
            const bool hits = touchHits(touch);
            if (hits) {
                scaleButtonTo(0.9f);
            }
            
            return hits;
        };
        
        // 触摸结束，调用响应函数，同时缩放标签
        listener->onTouchEnded = [=](Touch *touch,Event *event) {
            const bool hits = touchHits(touch);
            if (hits && _onTriggered) {
                _onTriggered();
            }
            scaleButtonTo(1);
        };
        
        // 触摸取消，缩放标签
        listener->onTouchCancelled = [=](Touch *touch,Event *event) {
            scaleButtonTo(1);
        };
        
        // 添加场景优先事件监听器
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
    
    }
    
    // 使用图片路径创建精灵
    bool initSpriteButton(const char *filePath) {
        _child = Sprite::create(filePath);
        addChild(_child);
        return true;
    }
    
    // 使用文本创建精灵
    bool initTextButton(const char *text) {
        _child = Label::createWithSystemFont(text, "", 16);
        addChild(_child);
        return true;
    }
    
    // 判断触摸位置是否在标签之内
    bool touchHits(Touch *touch) {
        const Rect area(0, 0, _child->getContentSize().width, _child->getContentSize().height);
        return area.containsPoint(_child->convertToNodeSpace(touch->getLocation()));
    }
    
    // 缩放标签动作
    void scaleButtonTo(float scale) {
        auto action = ScaleTo::create(0.1f, scale);
        action->setTag(900);
        stopActionByTag(900);
        runAction(action);
    }
    
    // 节点对象，即标签
    Node *_child;
    // 响应函数
    std::function<void(void)> _onTriggered;
    
};


// 自定义音量调至滑块类
class AudioSlider : public Node {
    
public:
    // create函数，如果init函数调用出错则回收对象
    static AudioSlider *create() {
        auto ret = new AudioSlider();
        if (ret && !ret->init()) {
            delete ret;
            ret = NULL;
        }
        
        return ret;
    }
    
    // 返回滑块当前值
    float getValue() const {
        return _slider->getValue();
    }
    
    void setValue(float minValue,float maxValue, float value) {
        
        // 设置音量滑块的范围最小值、最大值和当前值
        _slider->setMinimumValue(minValue);
        _slider->setMaximumValue(maxValue);
        _slider->setValue(value);
        
        // 创建滑块上面显示最小值和最大值的标签
        char buffer[32];
        
        sprintf(buffer, "%.2f", minValue);
        _lblMinValue = Label::createWithSystemFont(buffer, "" , 18);
        addChild(_lblMinValue);
        
        sprintf(buffer, "%.2f", maxValue);
        _lblMaxValue = Label::createWithSystemFont(buffer, "", 18);
        addChild(_lblMaxValue);
    
    }
    
    // 设置滑块、最小值标签、最大值标签位置
    void setSliderPosition(float x,float y) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        _slider->setPosition(visibleSize.width * x, visibleSize.height * y);
        _lblMinValue->setPosition(visibleSize.width * (x - 0.1), visibleSize.height * (y + 0.05));
        _lblMaxValue->setPosition(visibleSize.width * (x + 0.1), visibleSize.height * (y + 0.05));
    }
    
    //返回滑块对象
    ControlSlider *getSlider() {
        return _slider;
    }
    
private:
    
    // init函数，创建一个滑块对象
    bool init() {
        _slider = ControlSlider::create("sliderTrack.png", "sliderProgress.png", "sliderThumb.png");
        addChild(_slider);
        return true;
    }
    
    ControlSlider *_slider;  //调整滑块
    Label *_lblMinValue;  //最小音量
    Label *_lblMaxValue;  //最大音量

};



Scene *SimpleAudioEngineTest::createScene() {
    auto scene = Scene::create();
    auto layer = SimpleAudioEngineTest::create();
    scene->addChild(layer);
    return scene;
}

bool SimpleAudioEngineTest::init() {
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 关闭菜单
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(SimpleAudioEngineTest::menuCloseCallback, this));
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2, origin.y + closeItem->getContentSize().height/2));
    // 创建菜单
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    // 预加载背景音乐和音效
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE);
    
    // 背景音乐控制标签
    auto lblMusic = Label::createWithSystemFont("背景音乐控制", "", 18);
    lblMusic->setPosition(Vec2(visibleSize.width * 0.25, visibleSize.height * 0.9));
    this->addChild(lblMusic);
    
    // 播放背景音乐
    auto playMusic = Button::createWithText("播放");
    playMusic->onTriggered([]{
        SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE);
    });
    
    playMusic->setPosition(Vec2(visibleSize.width * 0.10, visibleSize.height * 0.8));
    this->addChild(playMusic);
    
    // 停止背景音乐
    auto stopMusic = Button::createWithText("停止");
    stopMusic->onTriggered([]{
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    });
    stopMusic->setPosition(Vec2(visibleSize.width * 0.25, visibleSize.height * 0.8));
    this->addChild(stopMusic);
    
    // 回放背景音乐
    auto rewindMusic = Button::createWithText("回放");
    rewindMusic->onTriggered([]{
        SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
    });
    rewindMusic->setPosition(Vec2(visibleSize.width * 0.40, visibleSize.height * 0.8));
    this->addChild(rewindMusic);
    
    // 暂停背景音乐
    auto pauseMusic = Button::createWithText("暂停");
    pauseMusic->onTriggered([]{
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    });
    pauseMusic->setPosition(Vec2(visibleSize.width * 0.10, visibleSize.height * 0.65));
    this->addChild(pauseMusic);
    
    // 继续背景音乐
    auto resumeMusic = Button::createWithText("恢复");
    resumeMusic->onTriggered([]{
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    });
    resumeMusic->setPosition(Vec2(visibleSize.width * 0.25, visibleSize.height * 0.65));
    this->addChild(resumeMusic);
    
    // 音效控制标签
    auto lblSound = Label::createWithSystemFont("音效控制", "", 20);
    lblSound->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.9));
    this->addChild(lblSound);
    
    // 播放音效
    auto playEffect = Button::createWithText("播放");
    playEffect->onTriggered([this]{
        const float pitch = _sliderPitch->getValue();
        const float pan = _sliderPan->getValue();
        const float gain = _sliderGain->getValue();
        _soundId = SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE, false, pitch, pan, gain);
    });
    playEffect->setPosition(Vec2(visibleSize.width * 0.60, visibleSize.height * 0.8));
    this->addChild(playEffect);
    
    // 暂停音效
    auto pauseEffect = Button::createWithText("暂停");
    pauseEffect->onTriggered([this]{
        SimpleAudioEngine::getInstance()->pauseEffect(_soundId);
    });
    pauseEffect->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.8));
    this->addChild(pauseEffect);
    
    // 继续音效
    auto resumeEffect = Button::createWithText("继续");
    resumeEffect->onTriggered([this]{
        SimpleAudioEngine::getInstance()->resumeEffect(_soundId);
    });
    resumeEffect->setPosition(Vec2(visibleSize.width * 0.90, visibleSize.height * 0.8));
    this->addChild(resumeEffect);
    
    // 停止音效
    auto stopEffect = Button::createWithText("停止");
    stopEffect->onTriggered([this]{
        SimpleAudioEngine::getInstance()->stopEffect(_soundId);
    });
    stopEffect->setPosition(Vec2(visibleSize.width * 0.60, visibleSize.height * 0.7));
    this->addChild(stopEffect);
    
    // 循环播放音效
    auto playEffectInLoop = Button::createWithText("循环播放");
    playEffectInLoop->onTriggered([this]{
        const float pitch = _sliderPitch->getValue();
        const float pan = _sliderPan->getValue();
        const float gain = _sliderGain->getValue();
        SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE, true, pitch, pan, gain);
    });
    playEffectInLoop->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.70));
    this->addChild(playEffectInLoop);
    
    // 暂停所有音效
    auto pauseAllEffect = Button::createWithText("暂停所有");
    pauseEffect->onTriggered([this]{
        SimpleAudioEngine::getInstance()->pauseAllEffects();
    });
    pauseAllEffect->setPosition(Vec2(visibleSize.width * 0.55, visibleSize.height * 0.6));
    this->addChild(pauseAllEffect);
    
    // 继续所有音效
    auto resumeAllEffect = Button::createWithText("继续所有");
    resumeAllEffect->onTriggered([this]{
        SimpleAudioEngine::getInstance()->resumeAllEffects();
    });
    resumeAllEffect->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height * 0.6));
    this->addChild(resumeAllEffect);
    
    // 停止所有音效
    auto stopAllEffect = Button::createWithText("停止所有");
    stopAllEffect->onTriggered([this]{
        SimpleAudioEngine::getInstance()->stopAllEffects();
    });
    stopAllEffect->setPosition(Vec2(visibleSize.width * 0.92, visibleSize.height * 0.6));
    this->addChild(stopAllEffect);
    
    /***********************音量控件**************************/
    // 音量调整标签
    auto lblMusicVolume = Label::createWithSystemFont("音量", "Arial", 24);
    lblMusicVolume->setPosition(visibleSize.width*0.08, visibleSize.height*0.45);
    this->addChild(lblMusicVolume);
    // 音量大小调整滑块
    _sliderMusicVolume = AudioSlider::create();
    _sliderMusicVolume->setValue(0.00f, 1.00f, 1.0f);
    _sliderMusicVolume->setSliderPosition(0.28, 0.45);
    // 绑定音量事件处理函数，当滑块被拖动时被调用
    _sliderMusicVolume->getSlider()->addTargetWithActionForControlEvents(this, cccontrol_selector(SimpleAudioEngineTest::musicValueChanged), Control::EventType::VALUE_CHANGED);
    this->addChild(_sliderMusicVolume);
    /***********************音效控件**************************/
    // 音效调整标签
    auto lblEffectsVolume = Label::createWithSystemFont("音效", "Arial", 24);
    lblEffectsVolume->setPosition(visibleSize.width*0.60, visibleSize.height*0.45);
    this->addChild(lblEffectsVolume);
    // 音效大小调整滑块
    _sliderEffectsVolume = AudioSlider::create();
    _sliderEffectsVolume->setValue(0.00f, 1.00f, 1.0f);
    _sliderEffectsVolume->setSliderPosition(0.80, 0.45);
    // 绑定音量事件处理函数，当滑块被拖动时被调用
    _sliderEffectsVolume->getSlider()->addTargetWithActionForControlEvents(this, cccontrol_selector(SimpleAudioEngineTest::effectsValueChanged), Control::EventType::VALUE_CHANGED);
    this->addChild(_sliderEffectsVolume);
    /***********************音效高音控件**************************/
    // 高音调整标签
    auto lblPltich = Label::createWithSystemFont("高音", "Arial", 24);
    lblPltich->setPosition(visibleSize.width*0.60, visibleSize.height*0.35);
    this->addChild(lblPltich);
    // 高音大小调整滑块
    _sliderPitch = AudioSlider::create();
    _sliderPitch->setValue(-0.50f, 2.00f, 1.0f);
    _sliderPitch->setSliderPosition(0.80, 0.35);
    this->addChild(_sliderPitch);
    /***********************音效立体效果控件**************************/
    // 立体效果调整标签
    auto lblPan = Label::createWithSystemFont("立体效果", "Arial", 24);
    lblPan->setPosition(visibleSize.width*0.60, visibleSize.height*0.25);
    this->addChild(lblPan);
    // 立体效果调整滑块
    _sliderPan = AudioSlider::create();
    _sliderPan->setValue(-1.00f, 1.00f, 0.0f);
    _sliderPan->setSliderPosition(0.80, 0.25);
    this->addChild(_sliderPan);
    /***********************音效音量控件**************************/
    // 音量调整标签
    auto lblGain = Label::createWithSystemFont("音量", "Arial", 24);
    lblGain->setPosition(visibleSize.width*0.60, visibleSize.height*0.15);
    this->addChild(lblGain);
    // 音量调整滑块
    _sliderGain = AudioSlider::create();
    _sliderGain->setValue(0.00f, 1.00f, 1.0f);
    _sliderGain->setSliderPosition(0.80, 0.15);
    this->addChild(_sliderGain);
    
    return true;

}

// 背景音乐音量调整事件处理函数，当滑块被拖动时被调用
void SimpleAudioEngineTest::musicValueChanged(Ref *pSender, Control::EventType controlEvent){
    // 将pSender类型转成拖动滑块ControlSlider
    ControlSlider* pSlider = dynamic_cast<ControlSlider*>(pSender);
    // 获取滑块的当前值
    const float musicVolume = pSlider->getValue();
    // 设置背景音乐值
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(musicVolume);
}

// 音效音量调整事件处理函数，当滑块被拖动时被调用
void SimpleAudioEngineTest::effectsValueChanged(Ref *pSender, Control::EventType controlEvent){
    // 将pSender类型转成拖动滑块ControlSlider
    ControlSlider* pSlider = dynamic_cast<ControlSlider*>(pSender);
    // 获取滑块的当前值
    const float effectVolume = pSlider->getValue();
    // 设置音效值
    SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume);
}


void SimpleAudioEngineTest::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}



























