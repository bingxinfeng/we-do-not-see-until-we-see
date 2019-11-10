#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class FractaledShad{
    
public:
    FractaledShad();
    
    void setup();
    void update(float aPlus_);
    void createFboToDistort(ofImage img_);
    void draw(ofImage img_);
    void gui2Setup();
    void gui2Draw();

    
    ofShader fractalMotionShad;
    ofShader fractaledShad;
    ofFbo offsetFbo[2];
    int curFboIndex;
    int prevFboIndex;
    ofFbo fboToDistort;
    ofImage cirMask;
    
    ofxPanel gui2;
    ofxFloatSlider stPlus, aPlus;
};
