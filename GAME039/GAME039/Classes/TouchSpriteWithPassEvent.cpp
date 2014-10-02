//
//  TouchSpriteWithPassEvent
//

#include "TouchSpriteWithPassEvent.h"

#define AnimationSpeed 1.0f/12.0f

using namespace cocos2d;

TouchSpriteWithPassEvent* TouchSpriteWithPassEvent::createBySpriteFrameName(const char *spName){
    TouchSpriteWithPassEvent *pobSprite = new TouchSpriteWithPassEvent();
    
    if (pobSprite && pobSprite->initWithSpriteFrameName(spName))
    {
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;
}

TouchSpriteWithPassEvent* TouchSpriteWithPassEvent::createByNoImage(CCRect tempScope){
    TouchSpriteWithPassEvent *pobSprite = new TouchSpriteWithPassEvent();
    
    pobSprite->init();
    pobSprite->setTextureRect(tempScope);
    pobSprite->setOpacity(0);
    
    if (pobSprite)
    {
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;
}

TouchSpriteWithPassEvent* TouchSpriteWithPassEvent::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    TouchSpriteWithPassEvent *pobSprite = new TouchSpriteWithPassEvent();
    
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void TouchSpriteWithPassEvent::myInit(){
//    CCLOG("TouchSpriteWithPassEvent init");
    
    m_pListener = NULL;
    m_pfnSelector = NULL;
    m_touchEndCallBack = NULL;
    touchMovedCallBack= NULL;
    
    touchable = true;
    isWithAlpha = true;
}

TouchSpriteWithPassEvent::~TouchSpriteWithPassEvent(){
//    CCLOG("~TouchSpriteWithPassEvent ");
    
    m_pListener = NULL;
    m_pfnSelector = NULL;
    m_touchEndCallBack = NULL;
    touchMovedCallBack= NULL;
}

void TouchSpriteWithPassEvent::onEnter(){
    //因為CCMenu的priority是 -128
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -120, false);
    
    CCNode::onEnter();
}

void TouchSpriteWithPassEvent::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

bool TouchSpriteWithPassEvent::ccTouchBegan(CCTouch* touch, CCEvent* event){
    
    CCLog("TouchSpriteWithPassEvent::ccTouchBegan = %d", this->getTag());
    
    if (!touchable)
        return false;
    
    if (this->containsTouchLocation(touch)) {
        
//        CCLOG("觸碰到 %d",this->getTag());
        
        //Active
        if (m_pListener && m_pfnSelector)
        {
            this->setUserData(event);
            (m_pListener->*m_pfnSelector)(this);
        }
        
        return true;
    }
    
    return false;
}

void TouchSpriteWithPassEvent::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
//    CCLog("ccTouchMoved");
    
    if (m_pListener && touchMovedCallBack)
    {
        (m_pListener->*touchMovedCallBack)(this);
    }
    
}

void TouchSpriteWithPassEvent::ccTouchEnded(CCTouch* touch, CCEvent* event)
{    
    if (m_pListener && m_touchEndCallBack)
    {
        (m_pListener->*m_touchEndCallBack)(this);
    }
}
#pragma mark -


bool TouchSpriteWithPassEvent::containsTouchLocation(CCTouch *touch){
    
//    CCLog("this position = %f, %f", this->getPositionX(), this->getPositionY());
    
    CCRect thisRect = CCRectMake(getPositionX() - getContentSize().width * getAnchorPoint().x, getPositionY() - getContentSize().height * getAnchorPoint().y, getContentSize().width  * getScaleX(), getContentSize().height  * getScaleY());
    
    CCPoint position = touch->getLocation();
    
    bool isTouched = thisRect.containsPoint(position);
    

    if (isWithAlpha)
    {
        if (isTouched) {
            //取得圖片該點的RGBA 做判斷.
            CCPoint nodeLocation = this->convertToNodeSpace(position);
            
            //-----
            u_int8_t data[4];
            CCRenderTexture *renderTexture = CCRenderTexture::create(getContentSize().width, getContentSize().height);
            
            renderTexture->begin();

            this->draw();
            
            glReadPixels(nodeLocation.x, nodeLocation.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
            renderTexture->end();
            
    //        CCSprite *aaa = CCSprite::createWithTexture(renderTexture->getSprite()->getTexture());
    //        aaa->setPosition(ccp(200, 300));
    //        this->getParent()->addChild(aaa,50);
            
            renderTexture->release();
            
            int alpha = data[3];
            
            if (alpha < 85) {
                // alpha 小於 85 當做透明.
                //不處理點擊觸碰.
                
                return false;
            }
        }
    }
    
    return isTouched;
}

void TouchSpriteWithPassEvent::setAllEventCallBack(CCObject *rec, SEL_MenuHandler selector, SEL_MenuHandler touchEndCallBack, SEL_MenuHandler touchMovedCallBack){
    m_pListener = rec;
    m_pfnSelector = selector;
    m_touchEndCallBack = touchEndCallBack;
    this->touchMovedCallBack = touchMovedCallBack;
}

void TouchSpriteWithPassEvent::setTouchable(bool touchable)
{
    this->touchable = touchable;
}

void TouchSpriteWithPassEvent::setWithAlpha(bool isWithAlpha)
{
    this->isWithAlpha = isWithAlpha;
}