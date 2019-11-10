#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class ScaledNoiseShad{
    
public:
    ScaledNoiseShad();
    
    void setup();
    void update(float timeSp_);
    void draw(ofImage img_);
    void gui1Setup();
    void gui1Draw();

    
    ofShader shader;
    ofShader shaderDistort;
    
    ofFbo offsetFbo[2];
    int curFboIndex;
    int prevFboIndex;
    
    float time;
        
    ofxPanel gui1;
    ofxFloatSlider level1, level2, level3, l1_ax, l1_ay, l1_bx, l1_by, l2_ax, l2_ay, l2_bx, l2_by,l3_ax, l3_ay, l3_bx, l3_by, l4_ax, l4_ay, l4_bx, l4_by;
};
