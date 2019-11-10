#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"

class BackgroundDiff{
    
public:
    BackgroundDiff();
    
    void imgAllocate(ofImage img_);
    void update(ofImage img_);
    void draw();
    float getAvgBrightness();
    void resetBackground();
    void gui3Setup();
    void gui3Draw();

    
    ofxCvColorImage currColorImg;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage blurred;
    ofxCvGrayscaleImage background;
    ofxCvGrayscaleImage diff;
    ofxCvGrayscaleImage mask;
    ofColor averageColor;
    float avgBrightness;
    
    
    ofxPanel gui3;
    ofxFloatSlider blurGaussian, maskThreshold;
};
