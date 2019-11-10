// Author: Bingxin Feng
// Title: We don't see untill we see (Interactive Installtion)
// Project: Goldsmiths Computational Arts MA 2019 Final Project

// Project Code Reference are cited in WeDntSeeTillWeSee/README.md file.

#pragma once

#include "ofMain.h"

// addons
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxGui.h"

// my classes
#include "ScaledNoiseShad.h"
#include "FractaledShad.h"
#include "BackgroundDiff.h"

using namespace cv;
using namespace ofxCv;
using namespace glm;

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    void drawMenu();
	
    
    //faceTracker
	ofVideoGrabber myCam;
    ofImage myImg, myImg2;
	ofxFaceTracker tracker;
    bool showFaceTracker;
    
    
    //arduino - https://www.youtube.com/watch?v=DOX5agKUi7M
    //https://github.com/keitasumiya/oF_arduino
    ofSerial mySerial;
    void arduinoSetup();
    void arduinoUpdate();
    void arduinoCalculation();
    int timer;
    
    bool screenshort;
    void takeScreenshort();
    
    // ScaledNoiseShad (the main background pattern)
    ScaledNoiseShad scaledNoiseShad;

    
    // fractaledShad (the masked circle mirror area)
    FractaledShad fractaledShad;
    
    
    //Background diff :
    BackgroundDiff bgDiff;
    bool showBgDiff;
    

    bool showGui;
    
   
};
