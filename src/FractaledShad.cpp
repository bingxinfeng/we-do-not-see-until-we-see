#include "FractaledShad.h"
//-------------------------------------------------------
FractaledShad::FractaledShad(){
    
}
//--------------------------------------------------------------
void FractaledShad::setup(){
    fractalMotionShad.load("fractalMotionShad.vert", "fractalMotionShad.frag");
    fractaledShad.load("fractalMotionShad.vert", "fractaledShad.frag");
    
    // Allocate floating point framebuffers to hold accumulated offset values
    for (int i = 0; i < 2; i++) {
        offsetFbo[i].allocate(ofGetWidth(), ofGetHeight(), GL_RGBA16F);
        offsetFbo[i].begin();
        ofClear(0);
        offsetFbo[i].end();
    }
    // Initialize indexes indicating which is the offsetFBO for the current frame and which is the one for the previous frame
    curFboIndex = 0;
    prevFboIndex = 1;
    
    fboToDistort.allocate(ofGetWidth(), ofGetHeight());
    fboToDistort.begin();
    ofClear(0);
    fboToDistort.end();
    
    cirMask.load("circleMask.png");
}

//--------------------------------------------------------------
void FractaledShad::update(float aPlus_){
    
    // Swap over the current and previous framebuffers for the accumulated offset
    curFboIndex = prevFboIndex;
    prevFboIndex = 1 - curFboIndex;
    
    // Run the fragment shader to accumulate more offset using Curl Noise
    offsetFbo[curFboIndex].begin();
    fractalMotionShad.begin();
    fractalMotionShad.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    fractalMotionShad.setUniform1f("u_time", ofGetElapsedTimef());
    fractalMotionShad.setUniform1f("u_stPlus", 1.85);
    fractalMotionShad.setUniform1f("u_aPlus", aPlus_);
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    fractalMotionShad.end();
    offsetFbo[curFboIndex].end();
}
//--------------------------------------------------------------
void FractaledShad::createFboToDistort(ofImage img_){
    fboToDistort.begin();
    img_.setImageType(OF_IMAGE_GRAYSCALE);
    img_.draw(0,0, ofGetWidth(), ofGetHeight());
    //cirMask.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    ofPushMatrix();
    ofScale(ofGetWidth()/cirMask.getWidth(),ofGetHeight()/cirMask.getHeight());
    for (int x=0; x<cirMask.getWidth()+5; x+=5){
        for (int y=0; y<cirMask.getHeight()+5; y+=5){
            ofColor brightness = cirMask.getColor(x,y);
            if (brightness==0){
                ofSetColor(0);
                ofDrawCircle(x, y, 5);
            }
        }
    }
    ofPopMatrix();
    fboToDistort.end();
}
//--------------------------------------------------------------
void FractaledShad::draw(ofImage img_){
    
    createFboToDistort(img_);
    //fboToDistort.draw(0,0);
    
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    // Draw the image file using values from the current offsetFbo to distort the image
    fractaledShad.begin();
    fractaledShad.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    fractaledShad.setUniformTexture("imageTexture", fboToDistort.getTexture(), 1);
    fractaledShad.setUniformTexture("offsetTexture", offsetFbo[curFboIndex].getTexture(), 2);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    fractaledShad.end();
    
    ofDisableBlendMode();
}
//--------------------------------------------------------------
void FractaledShad::gui2Setup(){
    ofSetVerticalSync(true);
    gui2.setup();
    gui2.add(stPlus.setup("stPlus", 1.85, 0, 3));
    gui2.add(aPlus.setup("aPlus", -1.2, -1.5, 1.5));
}
//--------------------------------------------------------------
void FractaledShad::gui2Draw(){
    gui2.draw();
}
