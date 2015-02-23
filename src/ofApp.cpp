#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

vector<ofxMacamPs3Eye*> cameras;
int camCounter;
ofPlanePrimitive plane;

//------------------------------------------------------------
// TO DO
//
// - Key Combo to switch cameras around
// - OpenCV examples working
// - Key Combo to set eye space
// - Key Combo to turn off cameras
// - Key Combo to turn off tracking caps
//
//-------------------------------------------------------------



//--------------------------------------------------------------
void ofApp::setup()
{
    //crazy contour code
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    //
    
    //ofBackground(0);
    //ofSetLogLevel( OF_LOG_VERBOSE );
    ofSetVerticalSync( true );
    
    
    //ofSetWindowPosition(1920, 0);
    //ofToggleFullscreen();
    showOverlay = false;
    // predictive = true;
    
    ofHideCursor();
    
    oculusRift.baseCamera = &cam;
    
    oculusRift.setup();
    
    oculusRift.dismissSafetyWarning();
    
    vector<ofxMacamPs3EyeDeviceInfo*> deviceList = ofxMacamPs3Eye::getDeviceInfoList();
    for (int i = 0; i < deviceList.size(); i++)
    {
        ofxMacamPs3Eye * newcamera = new ofxMacamPs3Eye();
        ofTexture * texture = new ofTexture();
        newcamera->setDeviceID(deviceList[i]->id);
        newcamera->initGrabber(640, 480);
        cameras.push_back(newcamera);
        cameras[i]->setAutoGainAndShutter(true); // otherwise we can'   t set gain or shutter
        
        
        camCounter = deviceList.size();
        
    }
    
    if (camCounter == 0)
    {
        macbook.initGrabber(640, 480);
    }
    
    
    for(int i = 0; i < 20; i++){
        DemoSphere d;
        d.color = ofColor(ofRandom(255),
                          ofRandom(255),
                          ofRandom(255));
        
        d.pos = ofVec3f(ofRandom(-500, 500),0,ofRandom(-500,500));
        
        d.floatPos.x = d.pos.x;
        d.floatPos.z = d.pos.z;
        
        d.radius = ofRandom(2, 50);
        
        d.bMouseOver = false;
        d.bGazeOver  = false;
        
        demos.push_back(d);
    }
    
    //enable mouse;
    cam.begin();
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (camCounter>0)
    {
        for (int i = 0; i < cameras.size(); i++)
        {
            
            cameras[i]->update();
            
        }
    }
    else
    {
        if(cameras.size() == 0)
        {
            ofDrawBitmapString("No PS3Eye found.", 20, 20);
            
            macbook.update();
        }
    }
    
    
    if(oculusRift.isSetup())
    {
        ofRectangle viewport = oculusRift.getOculusViewport();
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if(oculusRift.isSetup())
    {
        if(showOverlay)
        {
            oculusRift.beginOverlay(-230, 320,240);
            ofRectangle overlayRect = oculusRift.getOverlayRectangle();
            
            ofPushStyle();
            ofEnableAlphaBlending();
            ofFill();
            ofSetColor(255, 40, 10, 200);
            
            ofRect(overlayRect);
            
            ofSetColor(255,255);
            ofFill();
            
            
            if(cameras.size() == 0)
            {
                ofDrawBitmapString("No PS3EYE Found! \nFPS:"+ofToString(ofGetFrameRate()), 40, 40);
            }
            else
            {
                ofDrawBitmapString("FPS:"+ofToString(ofGetFrameRate())+"\nPredictive Tracking " + (oculusRift.getUsePredictiveOrientation() ? "YES" : "NO"), 40, 40);
            }
            
            ofSetColor(0, 255, 0);
            ofNoFill();
            //ofCircle(overlayRect.getCenter(), 20);
            
            ofPopStyle();
            oculusRift.endOverlay();
        }
        
        ofSetColor(255);
        glEnable(GL_DEPTH_TEST);
        
        oculusRift.beginLeftEye();
        drawRightEyeScene(); //rename the methods for these
        oculusRift.endLeftEye();
        
        oculusRift.beginRightEye();
        drawLeftEyeScene(); //rename the methods for these
        oculusRift.endRightEye();
        
        oculusRift.draw();
        
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        cam.begin();
        cam.end();
    }
    
}

//--------------------------------------------------------------
void ofApp::drawLeftEyeScene()
{
    if (camCounter == 2)
    {
        ofPushMatrix();
        ofDrawPlane(-600, -400, 1200, 800);
        ofScale(-1,1,1);
        ofRotate(90, 0, 0, -1);
        
        for (int i = 0; i < cameras.size(); i++)
        {
            ofTranslate(1, -35, 100);
            
            cameras[0]->draw(-600, -400, 1200, 1000);
            ofTranslate(1, 1, 1);
        }
    }
    else if (camCounter == 1)
    {
        ofPushMatrix();
        ofDrawPlane(-600, -400, 1200, 800);
        ofScale(-1,1,1);
        ofRotate(90, 0, 0, -1);
        
        for (int i = 0; i < cameras.size(); i++)
        {
            ofTranslate(1, -1, 100);
        
            cameras[0]->draw(-725, -543.75, 1450, 1087.5);
            ofTranslate(1, 1, 1);
        }
    }
    else if (camCounter == 0)
    {
        ofPushMatrix();
        ofDrawPlane(-600, -400, 1200, 800);
        ofScale(1,1,1);
        ofRotate(180, 0, 0, -1);
        
        ofTranslate(1, 1, 100);
        macbook.draw(-600, -400, 1200, 1000);
        ofTranslate(1, 1, 1);
    }
    
    ofPopMatrix();
    ofPushStyle();
    ofNoFill();
    
    //billboard and draw the mouse
    if(oculusRift.isSetup()){
        
        ofPushMatrix();
        oculusRift.multBillboardMatrix();
        ofSetColor(255, 0, 0);
        ofCircle(0,0,.5);
        ofPopMatrix();
        
    }
    
    ofPopStyle();
    
    contourFinder.draw();
    
}

//--------------------------------------------------------------
void ofApp::drawRightEyeScene()
{
    
    if (camCounter == 2)
    {
        ofPushMatrix();
        ofDrawPlane(-600, -500, 1200, 1000);
        ofScale(-1,1,1);
        ofRotate(90, 0, 0, -1);
        
        for (int i = 0; i < cameras.size(); i++)
        {
            ofTranslate(1, 1, 100);
            cameras[1]->draw(-600, -500, 1200, 1000);
            ofTranslate(1, 1, 1);
        }
    }
    else if (camCounter == 1)
    {
        ofPushMatrix();
        //ofDrawPlane(-600, -500, 1200, 1000);
        ofScale(-1,1,1);
        ofRotate(90, 0, 0, -1);
        
        for (int i = 0; i < cameras.size(); i++)
        {
            ofTranslate(1, 1, 100);
            cameras[0]->draw(-600, -500, 1200, 900);
            ofTranslate(1, 1, 1);
        }
    }
    else if (camCounter == 0)
    {
        ofPushMatrix();
        ofDrawPlane(-600, -400, 1200, 800);
        ofScale(1,1,1);
        ofRotate(180, 0, 0, -1);
        
        ofTranslate(1, 1, 100);
        macbook.draw(-600, -500, 1200, 1000);
        ofTranslate(1, 1, 1);
    }

    
    ofPopMatrix();
    ofPushStyle();
    ofNoFill();
    
    //billboard and draw the mouse
    if(oculusRift.isSetup()){
        
        ofPushMatrix();
        oculusRift.multBillboardMatrix();
        ofSetColor(255, 0, 0);
        ofCircle(0,0,.5);
        ofPopMatrix();
        
    }
    
    ofPopStyle();
    
    contourFinder.draw();
    
}

//-------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if( key == 'f' )
    {
        //gotta toggle full screen for it to be right
        ofToggleFullscreen();
    }
    
    if(key == 's')
    {
        oculusRift.reloadShader();
    }
    
    if(key == 'l')
    {
        oculusRift.lockView = !oculusRift.lockView;
    }
    
    if(key == 'o'){
        showOverlay = !showOverlay;
    }
    if(key == 'r'){
        oculusRift.reset();
        
    }
    if(key == 'h'){
        ofHideCursor();
    }
    if(key == 'H'){
        ofShowCursor();
    }
    
    if(key == 'p'){
        predictive = !predictive;
        oculusRift.setUsePredictedOrientation(predictive);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
//void ofApp::mouseMoved(int x, int y)
//{
//   cursor2D.set(x, y, cursor2D.z);
//}

//--------------------------------------------------------------
//void ofApp::mouseDragged(int x, int y, int button)
//{
//   cursor2D.set(x, y, cursor2D.z);
//}

//--------------------------------------------------------------
//void ofApp::mousePressed(int x, int y, int button)
//{
//
//}

//--------------------------------------------------------------
//void ofApp::mouseReleased(int x, int y, int button)
//{

//}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}



//--------------------------------------------------------------
void ofApp::onAutoGainAndShutterChange(bool & value)
{
    ps3eye.setAutoGainAndShutter(value);
}

//--------------------------------------------------------------
void ofApp::onGainChange(float & value)
{
    // Only set if auto gain & shutter is off
}

//--------------------------------------------------------------
void ofApp::onShutterChange(float & value)
{
    // Only set if auto gain & shutter is off
}

//--------------------------------------------------------------
void ofApp::onGammaChange(float & value)
{
    ps3eye.setGamma(value);
}

//--------------------------------------------------------------
void ofApp::onBrightnessChange(float & value)
{
    ps3eye.setBrightness(value);
}

//--------------------------------------------------------------
void ofApp::onContrastChange(float & value)
{
    ps3eye.setContrast(value);
}

//--------------------------------------------------------------
void ofApp::onHueChange(float & value)
{
    ps3eye.setHue(value);
}

//--------------------------------------------------------------
void ofApp::onLedChange(bool & value)
{
    ps3eye.setLed(value);
}

//--------------------------------------------------------------
void ofApp::onFlickerChange(int & value)
{
    ps3eye.setFlicker(value);
}

//--------------------------------------------------------------
void ofApp::onWhiteBalanceChange(int & value)
{
    ps3eye.setWhiteBalance(value);
}
