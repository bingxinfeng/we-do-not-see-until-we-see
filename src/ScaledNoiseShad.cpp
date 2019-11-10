#include "ScaledNoiseShad.h"
//-------------------------------------------------------
ScaledNoiseShad::ScaledNoiseShad(){
    
}
//--------------------------------------------------------------
void ScaledNoiseShad::setup(){
    ofDisableArbTex();
    shader.load("shader.vert", "shader.frag");
    shaderDistort.load("shader.vert", "shaderDistort.frag");
    
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
    
    // Initialize time
    time=.0001;
}
//--------------------------------------------------------------
void ScaledNoiseShad::update(float timeSp_){
    
    // make the update shder (fbo) here
    curFboIndex = prevFboIndex;
    prevFboIndex = 1 - curFboIndex;
    
    offsetFbo[curFboIndex].begin();
    shader.begin();
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());//要跟之后画的方形一样大小
    
    // time here controls how fast the "noise waterfall“ falls
    // time speed mapped with avg brightness from backgroundDiff class
    time+=timeSp_;
    shader.setUniform1f("u_time", time);//.00000001
    
    // getting values from gui1 slider, for visual testings
    shader.setUniform1f("level1", level1);
    shader.setUniform1f("level2", level2);
    shader.setUniform1f("level3", level3);
    shader.setUniform1f("l1_ax", l1_ax);
    shader.setUniform1f("l1_ay", l1_ay);
    shader.setUniform1f("l1_bx", l1_bx);
    shader.setUniform1f("l1_by", l1_by);
    shader.setUniform1f("l2_ax", l2_ax);
    shader.setUniform1f("l2_ay", l2_ay);
    shader.setUniform1f("l2_bx", l2_bx);
    shader.setUniform1f("l2_by", l2_by);
    shader.setUniform1f("l3_ax", l3_ax);
    shader.setUniform1f("l3_ay", l3_ay);
    shader.setUniform1f("l3_bx", l3_bx);
    shader.setUniform1f("l3_by", l3_by);
    shader.setUniform1f("l4_ax", l4_ax);
    shader.setUniform1f("l4_ay", l4_ay);
    shader.setUniform1f("l4_bx", l4_bx);
    shader.setUniform1f("l4_by", l4_by);
    
    // put contents in this fbo
    ofTranslate(0, 0);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
    offsetFbo[curFboIndex].end();
}
//--------------------------------------------------------------
void ScaledNoiseShad::draw(ofImage img_){
    img_.setImageType(OF_IMAGE_GRAYSCALE);
    
    shaderDistort.begin();
    shaderDistort.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shaderDistort.setUniformTexture("imageTexture", img_.getTexture(), 1);
    shaderDistort.setUniformTexture("offsetTexture", offsetFbo[curFboIndex].getTexture(), 2);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shaderDistort.end();
}
//--------------------------------------------------------------
void ScaledNoiseShad::gui1Setup(){
    ofSetVerticalSync(true);
    gui1.setup();
    
    // These were just for me to adjust the shader pattern to make a nice look visual.
    //label name, initialVal, minVal, maxVal
    gui1.add(level1.setup("level1", .25, 0., 1.0));
    gui1.add(level2.setup("level2", .5, 0., 1.0));
    gui1.add(level3.setup("level3", .75, 0., 1.0));
    
    gui1.add(l1_ax.setup("l1_ax", 655, 0, 10000));
    gui1.add(l1_ay.setup("l1_ay", 155, 0, 10000));
    gui1.add(l1_bx.setup("l1_bx", 300, 0, 10000));
    gui1.add(l1_by.setup("l1_by", 5, 0, 10000));
    
    gui1.add(l2_ax.setup("l2_ax", 755, 0, 10000));
    gui1.add(l2_ay.setup("l2_ay", 300, 0, 10000));
    gui1.add(l2_bx.setup("l2_bx", 540, 0, 10000));
    gui1.add(l2_by.setup("l2_by", 0, 0, 10000));
    
    gui1.add(l3_ax.setup("l3_ax", 1450, 0, 10000));
    gui1.add(l3_ay.setup("l3_ay", 150, 0, 10000));
    gui1.add(l3_bx.setup("l3_bx", 2000, 0, 10000));
    gui1.add(l3_by.setup("l3_by", 600, 0, 10000));
    
    gui1.add(l4_ax.setup("l4_ax", 650, 0, 10000));
    gui1.add(l4_ay.setup("l4_ay", 1900, 0, 10000));
    gui1.add(l4_bx.setup("l4_bx", 10000, 0, 10000));
    gui1.add(l4_by.setup("l4_by", 10000, 0, 10000));
}
//--------------------------------------------------------------
void ScaledNoiseShad::gui1Draw(){
    gui1.draw();
}
