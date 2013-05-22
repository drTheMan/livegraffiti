#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofBackground(0,0,0);
    
    int winW, winH;
    
    camW = 320;
    camH = 240;
    winW = camW*2;
    winH = camH*2;
    dimmer = 0.0;
    brightness = 97.0;
    contrast = 76.7;

    ofSetWindowShape(winW, winH);

    // this guy grabs our video (webcam) feed
    grabber.initGrabber(camW, camH, true);
    
	//need this for alpha to come through
	ofEnableAlphaBlending();

    // allocate our image buffers
    rgb.allocate(camW, camH);
    rgb2.allocate(camW,camH);
    maskImg.allocate(camW,camH);
    fbo.allocate(rgb2.getWidth()+10, rgb2.getHeight()+10);
    
    resetDrawing();
    
    // ui-stuff
    bDrawUI = true;
    gui = new ofxUICanvas(0,0,winW,winH);		//ofxUICanvas(float x, float y, float width, float height)

    gui->addWidgetDown(new ofxUILabel("LiveGraffiti", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISlider(304,16,-100.0,100.0,dimmer,"Dimmer"));
    gui->addWidgetDown(new ofxUISlider(304,16,-100.0,100.0,brightness,"Brightness"));
    gui->addWidgetDown(new ofxUISlider(304,16,-100.0,100.0,contrast,"Contrast"));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
//    gui->loadSettings("GUI/guiSettings.xml");
}

//--------------------------------------------------------------

void testApp::resetDrawing(){
    fbo.begin();
        ofBackground(0,0,0);
    fbo.end();
}

void testApp::update(){
    // update video grabber (get the latest frame)
    grabber.update();

    if (grabber.isFrameNew()) {
        // copy webcam pixels to rgb image
        rgb.setFromPixels(grabber.getPixels(), camW, camH);

        // mirror horizontally
        rgb.mirror(false, true);

        // copy to second buffer image
        rgb2 = rgb;
        
        // apply current brightness setting (darken image in second buffer)
        rgb2 -= dimmer;

        // create mask image copy of darkened image (auto-converts to grayscale)
        maskImg = rgb2;
        if(brightness >= 0 || contrast >= 0) maskImg.brightnessContrast(brightness, contrast);

        // draw to framebuffer, not to the screen
        fbo.begin();
            ofEnableBlendMode(OF_BLENDMODE_ADD);
//            maskImg.invert();
            maskImg.draw(0,0);
        fbo.end();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofSetColor(255,255,255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    rgb.draw(0,0);
    rgb2.draw(camW,0);
    maskImg.draw(0, camH);
    fbo.draw(camW, camH);
}

//void testApp::drawInfo(){
//    ofPushStyle();
//    ofSetHexColor(0xFF0000);
//    ofDrawBitmapString("dimmer: " + ofToString(dimmer), 10, 10);
//    ofDrawBitmapString("brightness: " + ofToString(brightness), 10, 30);
//    ofDrawBitmapString("contrast: " + ofToString(contrast), 10, 50);
//    ofPopStyle();
//}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	// in fullscreen mode, on a pc at least, the 
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...
	
	if (key == 's' || key == 'S'){
		grabber.videoSettings();
	}
    
    if (key == ' '){
        resetDrawing();
    }

    if (key == OF_KEY_UP){
        if(dimmer >= 5)
            dimmer -= 5;
    }

    float step = 0.1;
    if (key == OF_KEY_UP && dimmer >= step){ dimmer -= step; ((ofxUISlider*)gui->getWidget("Dimmer"))->setValue(dimmer); }
    if (key == OF_KEY_DOWN && dimmer <= 255-step){ dimmer += step; ((ofxUISlider*)gui->getWidget("Dimmer"))->setValue(dimmer); }
    if(key == '\\' && brightness >= step) {brightness -= step;     ((ofxUISlider*)gui->getWidget("Brightness"))->setValue(brightness); }
    if(key == ']' && brightness <= 255-step){ brightness += step;      ((ofxUISlider*)gui->getWidget("Brightness"))->setValue(brightness); }
    if(key == '\'' && contrast >= step) {contrast -= step;      ((ofxUISlider*)gui->getWidget("Contrast"))->setValue(contrast); }
    if(key == '[' && contrast <= 255-step){ contrast += step;     ((ofxUISlider*)gui->getWidget("Contrast"))->setValue(contrast); }
	
    if(key == '\t') gui->toggleVisible();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "Dimmer")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        dimmer = slider->getScaledValue();
    }

    if(e.widget->getName() == "Brightness")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        brightness = slider->getScaledValue();
    }
    
    if(e.widget->getName() == "Contrast")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        contrast = slider->getScaledValue();
    }
}

