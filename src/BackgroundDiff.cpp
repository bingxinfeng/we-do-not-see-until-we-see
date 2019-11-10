#include "BackgroundDiff.h"
//-------------------------------------------------------
BackgroundDiff::BackgroundDiff(){
    
}
//--------------------------------------------------------------
void BackgroundDiff::imgAllocate(ofImage img_){
    
    currColorImg.allocate(img_.getWidth(), img_.getHeight());
    
    // need to allocate cv grayImg to the vertical size first (for imgRotate90)
    grayImage.allocate(img_.getHeight(), img_.getWidth());
    blurred.allocate(img_.getHeight(), img_.getWidth());
    diff.allocate(img_.getHeight(), img_.getWidth());
    mask.allocate(img_.getHeight(), img_.getWidth());
}
//--------------------------------------------------------------
void BackgroundDiff::update(ofImage img_){ // call this line inside camera update
    
    currColorImg.setFromPixels(img_.getPixels());

    //Convert to grayscale image (left top)
    grayImage = currColorImg;
    
    //Smoothing image (right top)
    blurred = grayImage;
    blurred.blurGaussian( blurGaussian ); //bigger, more blur
    
    //Store first frame to background image
    if ( !background.bAllocated ) background = blurred;
    
    //Find difference of the frame and background
    diff = blurred;
    diff.absDiff(blurred, background); // get pixel difference
    
    //Thresholding for obtaining binary image
    mask = diff;
    mask.threshold( maskThreshold );
}
//--------------------------------------------------------------
float BackgroundDiff::getAvgBrightness(){
    
    //calculate brightness in 1/3 mask image (the upper part of the upper ripples)
    float rSum = 0;
    float gSum = 0;
    float bSum = 0;
    for(int y=0; y<mask.getHeight()/3; y++){
        for(int x=0; x<mask.getWidth(); x++){
            ofColor scanImgColor = mask.getPixels().getColor(x,y);
            rSum += scanImgColor.r;
            gSum += scanImgColor.g;
            bSum += scanImgColor.b;
        }
    }
    // The number of pixels we are averaging
    int samples = mask.getHeight()*(mask.getWidth()/3);
    // Update the average color
    averageColor.r = rSum / samples;
    averageColor.b = bSum / samples;
    averageColor.g = gSum / samples;
    avgBrightness = (averageColor.r / 255.0) * 0.3 + (averageColor.g / 255.0) * 0.59 + (averageColor.b / 255.0) * 0.11;
   
    return avgBrightness;
}
//--------------------------------------------------------------
void BackgroundDiff::draw(){
    // draw mask image at the top left corner
    float posX = ofGetWidth()/2;
    float posY = 0;
    ofSetColor(255);
    mask.draw(posX, posY, ofGetWidth()/2, ofGetHeight()/2 );
    
    // show the text of avgBrightness value
    ofPushStyle();
    ofSetColor(255,0,0);
    ofDrawBitmapString("avgBrightness", posX+20, posY+20);
    ofDrawBitmapString(avgBrightness, posX+20, posY+35);
    ofPopStyle();
}
//--------------------------------------------------------------
void BackgroundDiff::resetBackground(){
    background = blurred;
}
//--------------------------------------------------------------
void BackgroundDiff::gui3Setup(){
    ofSetVerticalSync(true);
    gui3.setup();
    
    gui3.add(blurGaussian.setup("blurGaussian", 15, 0, 30));
    gui3.add(maskThreshold.setup("maskThreshold", 50, 0, 150));
}
//--------------------------------------------------------------
void BackgroundDiff::gui3Draw(){
    gui3.draw();
}
