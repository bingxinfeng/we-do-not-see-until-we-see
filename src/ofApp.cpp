#include "ofApp.h"

//arduino declarations
const int val_size = 2;
//values += coes*steps
int values[val_size] = {350, 350}; //initial value example(0,100,200)
int steps[val_size] = {1,1};
int coes[val_size] = {1,1}; // +1 -> increase, -1 -> decrease
bool isValids[val_size] = {false,false};
int minVal = 350;//390
int maxVal = 511;//512 (1023/2=511.5)

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofBackground(0);
    ofSetFrameRate(60);
    
    myCam.setDeviceID(1);
    int rescaleLevel = 3;
	myCam.setup(ofGetHeight()/rescaleLevel, ofGetWidth()/rescaleLevel);
    // already rescaled 0.2 in ofxfaceTracker.cpp
    // so as long as resolution big enough, cam setup as resolution/3 should be fine
    myCam.getTexture().setTextureWrap(GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
    
    
    // FACETRACKER
	tracker.setup();
    showFaceTracker = false;
    
    
    // Allocate all images to aviod issues.
    myImg.allocate(myCam.getWidth(), myCam.getHeight(), OF_IMAGE_COLOR);
    // BACKGROUND DIFF
    bgDiff.imgAllocate(myImg);
    showBgDiff = false;
    
    
    // SHADERS
    scaledNoiseShad.setup();
    fractaledShad.setup();
    
    
    // setup all gui:
    scaledNoiseShad.gui1Setup();
    fractaledShad.gui2Setup();
    bgDiff.gui3Setup();
    showGui = true;
    
    
    // ARDUINO
    arduinoSetup();
}
//--------------------------------------------------------------
void ofApp::update() {
    
	myCam.update();
	if(myCam.isFrameNew()) {
        myImg.setFromPixels(myCam.getPixels());
        myImg.rotate90(0);
        // FACETRACKER
		tracker.update(toCv(myImg));
        // BACKGROUND DIFF
        bgDiff.update(myImg);
	}
    
    // sclaedNoiseShad
    float timeSp_ = ofMap(bgDiff.getAvgBrightness(), 0.03 , 0.8, .000000005, .00001, true);
    scaledNoiseShad.update(timeSp_);
    // fractaledShad
    // make movement to see the central circle!
    float aPlus_ = ofMap(bgDiff.getAvgBrightness(), 0.03, 0.7, -2., 0.8, true);
    fractaledShad.update(aPlus_); //0.03, 0.08, -1.2, 0.1
    
    
    // ARDUINO
    arduinoUpdate();
    arduinoCalculation();
}
//--------------------------------------------------------------
void ofApp::draw() {
     ofClear(0);
   
    // scaledNoiseShad
    scaledNoiseShad.draw(myImg);
    

    // fractaledShad
    fractaledShad.draw(myImg);
    
    
    // BACKGROUND DIFF
    if(showBgDiff == true){
        bgDiff.getAvgBrightness();
        bgDiff.draw();
    }
    
    // FACETRACKER
    if(showFaceTracker == true){
        ofPushMatrix();
        // draw faceTracker img at the right bottom corner, under bgDiff img:
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        // scale to make it look easier and nicer in high resolution fullscreen:
        ofScale(ofGetWidth()/2/myImg.getWidth(), ofGetHeight()/2/myImg.getHeight());
        ofPushStyle();
        // draw base video:
        ofSetColor(255);
        myImg.draw(0,0); //don't change myImg size here, the faceMesh would not follow
        // draw face mesh:
        ofSetColor(255,0,0);
        ofSetLineWidth(1);
        tracker.getImageMesh().drawWireframe();
        tracker.draw(false);
        ofPopStyle();
        ofPopMatrix();
    }
    
    
    // ALL GUI
    if(showGui == true){
        scaledNoiseShad.gui1Draw();
        fractaledShad.gui2Draw();
        bgDiff.gui3Draw();
        
        drawMenu();
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == 'f') {
        showFaceTracker =! showFaceTracker;
    }
	if(key == 'r') {
		tracker.reset();
	}
    if(key == 'b') {
        showBgDiff =! showBgDiff;
    }
    if(key == ' ') {
        bgDiff.resetBackground();
    }
    if(key == 'g'){
        showGui =! showGui;
    }
}
//--------------------------------------------------------------
void ofApp::drawMenu() {
    //menu info
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(0,0,0,180);
    float menuX = 10;
    float menuY = ofGetHeight()-100;
    ofDrawRectangle(menuX, menuY, 280, 100);
    ofSetColor(255);
    ofDrawBitmapString("Curr FrameRate:", menuX+5, menuY+15);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), menuX+5, menuY+30);
    ofDrawBitmapString("Press 'f' for faceTracker image", menuX+5, menuY+45);
    ofDrawBitmapString("Press 'b' for backgroundDiff image", menuX+5, menuY+60);
    ofDrawBitmapString("Press 'g' for gui & menu", menuX+5, menuY+75);
    ofDisableAlphaBlending();
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::arduinoSetup(){
    //arduino setup, find the right device:
    mySerial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
    int baud = 9600;
    mySerial.setup(0, baud); //open the first device
}
//--------------------------------------------------------------
void ofApp::arduinoUpdate(){
    
    bool isValids[val_size] = {false,false};
    for (int i=0; i<val_size; i++){
        //bool ofSerial::writeByte(char singleByte)
        //Writes a single byte to the connected serial device. Check the return value to be sure the data was written.
        char head = (127*1+1)+i;
        char high = (values[i] >> (7*1)) & (127*1); //what does value>>7 mean?
        char low  = values[i] & (127*1);

        bool byteWasWritten1 = mySerial.writeByte(head);
        bool byteWasWritten2 = mySerial.writeByte(high);
        bool byteWasWritten3 = mySerial.writeByte(low);
        if ( byteWasWritten1 && byteWasWritten2 && byteWasWritten3 ) {
            printf("value is %d \n", values[i]);
            isValids[i] = true;
        } else {
            printf("an error occurred \n");
        }
    }
}
//--------------------------------------------------------------
void ofApp::arduinoCalculation(){
    // FACETRACKER -> ARDUINO
    // person look into mirror -> face found -> pumps keep running back&forth min-max ->
    // person's reflection got distorted by the ripples generated by pumps ->
    // face not found -> to avoid the situation that ripples rise&disappear too fast ->
    // so when face not found, pumps still run for a few seconds, then stop (i.e. back to the min value, and stay min until face found again)
    
    bool faceFound = tracker.getFound(); //0: not found, 1:found
    if (faceFound == 1){
        takeScreenshort();
        //reset timer (right before face not found)
        timer = 0;
        // when someone look into the mirror
        // increase arduino value from initial min to max
        for (int i=0; i<val_size; i++){
            values[i] += coes[i]*steps[i];
            // boundary check
            if (values[i] <= minVal){
                values[i] = minVal;
                coes[i] = +1;
            }else if (values[i] >= maxVal){
                values[i] = maxVal;
                coes[i] = -1;
            }
        } //-for loop
    }//-face:1
    
    else if (faceFound == 0){
        // timer starts counting when face not found
        timer ++;
        int duration = ofGetFrameRate()*2;
        // pumps keep running for 2 seconds
        if (timer < duration){
            takeScreenshort();
            for (int i=0; i<val_size; i++){
                // calculating values
                // inverse two pumps movement
                values[0] = minVal;
                coes[0] = 1;
                values[1] = maxVal;
                coes[1] = -1;
                
                values[i] += coes[i]*steps[i];
                // boundary check
                if (values[i] < minVal){
                    values[i] = minVal;
                    coes[i] = +1;
                }else if (values[i] > maxVal){
                    values[i] = maxVal;
                    coes[i] = -1;
                }
            } //-for loop
        }//-timer
        
        // pumps stop after 2 seconds
        else if (timer >= duration){
            for (int i=0; i<val_size; i++){
                if (values[i] > 350){
                    coes[i] = -1;
                    values[i] += coes[i]*steps[i];
                }
                if (values[i] == 350 || values[i]< 350){
                    values[i] = 350;
                }
            }
        }
    }
    cout<<timer<<endl;
}
//--------------------------------------------------------------
void ofApp::takeScreenshort(){
    int num = ofGetFrameRate();
    if(ofGetFrameNum()% num== 1 ){ // do it in every 1 sec?
        ofSaveScreen(ofToString(ofGetFrameNum())+".png");
    }
}
