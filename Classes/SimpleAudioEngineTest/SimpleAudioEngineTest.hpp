//
//  SimpleAudioEngineTest.hpp
//  actual_chap08
//
//  Created by xiang on 2017/7/14.
//
//

#ifndef SimpleAudioEngineTest_hpp
#define SimpleAudioEngineTest_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "extensions/GUI/CCControlExtension/CCControlSlider.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::extension;

class AudioSlider;


class SimpleAudioEngineTest : public cocos2d::Layer {
private:
    //音效的id
    unsigned int _soundId;
    // 调整背景音乐值和音效值的滑块
    AudioSlider *_sliderMusicVolume;
    AudioSlider *_sliderEffectsVolume;
    AudioSlider *_sliderPitch;
    AudioSlider *_sliderPan;
    AudioSlider *_sliderGain;
    
public:
    
    static cocos2d::Scene *createScene();
    
    virtual bool init();
    
    // 背景音乐滑块响应函数
    void musicValueChanged(Ref *pSender, Control::EventType controlEvent);
    // 音效滑块响应函数
    void effectsValueChanged(Ref *pSender, Control::EventType controlEvent);
    
    // 选择器回调函数
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(SimpleAudioEngineTest);
};


#endif /* SimpleAudioEngineTest_hpp */































