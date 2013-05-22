#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofBackground(0,0,0);
    
    int winW, winH;
    
    camW = 320;
    camH = 240;
    winW = camW*2;
    winH = camH*2;

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

    
    vector<string> blendmodes;
    blendmodes.push_back("Alpha");
    blendmodes.push_back("Add");
    blendmodes.push_back("Multiply");
    blendmodes.push_back("Subtract");
    blendmodes.push_back("Screen");
    
    gui->addWidgetDown(new ofxUILabel("LiveGraffiti", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISlider(304,16,-100.0,100.0,0.0,"Dimmer"));
    // nice values for negative are 97.0/76.7
    gui->addWidgetDown(new ofxUISlider(304,16,-100.0,100.0,-1.85,"Brightness"));
    gui->addWidgetDown(new ofxUISlider(304,16,-100.0,100.0,0.0,"Contrast"));
    gui->addWidgetDown(new ofxUIDropDownList("Blendmode", blendmodes, 304));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->addWidgetDown(new ofxUILabel("Press [SPACE] to clear the canvas", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUILabel("Press [TAB] to toggle UI on/off", OFX_UI_FONT_SMALL));
    //    gui->loadSettings("GUI/guiSettings.xml");
}

//--------------------------------------------------------------

void testApp::resetDrawing(){
    fbo.begin();
        ofBackground(0,0,0);
    fbo.end();
}

ofBlendMode testApp::getSelectedBlendMode(){
//    ofxUIDropDownList *ddlist = (ofxUIDropDownList *) gui->getWidget("Blendmode");
//    vector<ofxUIWidget *> &selected = ddlist->getSelected();
//    for(int i = 0; i < selected.size(); i++)
//    {
//        cout << "SELECTED: " << selected[i]->getName() << endl;
//    }

    vector<ofxUIWidget *> &selected = ((ofxUIDropDownList*)gui->getWidget("Blendmode"))->getSelected();
    string value = selected.size() == 1 ? selected[0]->getName() : "";

    if(value == "Alpha") return OF_BLENDMODE_ALPHA;
    if(value == "Add") return OF_BLENDMODE_ADD;
    if(value == "Multiply") return OF_BLENDMODE_MULTIPLY;
    if(value == "Subtract") return OF_BLENDMODE_SUBTRACT;
//    if(value == "Screen") return OF_BLENDMODE_SCREEN;
    // default
    return OF_BLENDMODE_SCREEN;
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
        rgb2 -= ((ofxUISlider*)gui->getWidget("Dimmer"))->getScaledValue();

        // create mask image copy of darkened image (auto-converts to grayscale)
        maskImg = rgb2;
        if(brightness >= 0 || contrast >= 0) maskImg.brightnessContrast(((ofxUISlider*)gui->getWidget("Brightness"))->getScaledValue(), ((ofxUISlider*)gui->getWidget("Contrast"))->getScaledValue());

        // draw to framebuffer, not to the screen
        fbo.begin();
            ofEnableBlendMode(getSelectedBlendMode());

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

    float step = 0.05;
    if (key == OF_KEY_UP){ ((ofxUISlider*)gui->getWidget("Dimmer"))->setValue(((ofxUISlider*)gui->getWidget("Dimmer"))->getScaledValue()+step);}
    if (key == OF_KEY_DOWN){ ((ofxUISlider*)gui->getWidget("Dimmer"))->setValue(((ofxUISlider*)gui->getWidget("Dimmer"))->getScaledValue()-step);}
    if (key == '['){ ((ofxUISlider*)gui->getWidget("Brightness"))->setValue(((ofxUISlider*)gui->getWidget("Brightness"))->getScaledValue()+step);}
    if (key == '\''){ ((ofxUISlider*)gui->getWidget("Brightness"))->setValue(((ofxUISlider*)gui->getWidget("Brightness"))->getScaledValue()-step);}
    if (key == ']'){ ((ofxUISlider*)gui->getWidget("Contrast"))->setValue(((ofxUISlider*)gui->getWidget("Contrast"))->getScaledValue()+step);}
    if (key == '\\'){ ((ofxUISlider*)gui->getWidget("Contrast"))->setValue(((ofxUISlider*)gui->getWidget("Contrast"))->getScaledValue()-step);}
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
//    if(e.widget->getName() == "Dimmer")
//    {
//        ofxUISlider *slider = (ofxUISlider *) e.widget;
//        dimmer = slider->getScaledValue();
//    }
//
//    if(e.widget->getName() == "Brightness")
//    {
//        ofxUISlider *slider = (ofxUISlider *) e.widget;
//        brightness = slider->getScaledValue();
//    }
//    
//    if(e.widget->getName() == "Contrast")
//    {
//        ofxUISlider *slider = (ofxUISlider *) e.widget;
//        contrast = slider->getScaledValue();
//    }
}

