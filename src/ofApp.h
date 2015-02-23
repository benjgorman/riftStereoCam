#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOculusDK2.h"
#include "ofxMacamPs3Eye.h"

typedef struct{
    ofColor color;
    ofVec3f pos;
    ofVec3f floatPos;
    float radius;
    bool bMouseOver;
    bool bGazeOver;
} DemoSphere;


class ofApp : public ofBaseApp{
    
    void setup();
    void update();
    void draw();
    
    void drawLeftEyeScene();
    void drawRightEyeScene();
    
    void keyPressed(int key);
    void keyReleased(int key);
    //void mouseMoved(int x, int y);
    //void mouseDragged(int x, int y, int button);
    //void mousePressed(int x, int y, int button);
    //void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    //void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxOculusDK2		oculusRift;
    
    
    ofLight				light;
    ofEasyCam			cam;
    bool showOverlay;
    bool predictive;
    vector<DemoSphere> demos;
    
    ofVec3f cursor2D;
    ofVec3f cursor3D;
    
    ofVec3f cursorRift;
    ofVec3f demoRift;
    
    ofVec3f cursorGaze;
    
    void onAutoGainAndShutterChange(bool & value);
    void onGainChange(float & value);
    void onShutterChange(float & value);
    void onGammaChange(float & value);
    void onBrightnessChange(float & value);
    void onContrastChange(float & value);
    void onHueChange(float & value);
    void onLedChange(bool & value);
    void onFlickerChange(int & value);
    void onWhiteBalanceChange(int & value);
    
    ofxMacamPs3Eye ps3eye;
    ofxMacamPs3Eye ps3eye2;
    
    ofxCv::ContourFinder contourFinder;
    
};
