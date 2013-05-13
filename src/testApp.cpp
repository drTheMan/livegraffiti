#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofBackground(0,0,0);
    
    w = 320;
    h = 240;
    brightness = 200;

    ofSetWindowShape(w*3, h*2);

    // this guy grabs our video (webcam) feed
    grabber.initGrabber(w, h, true);
    
	//need this for alpha to come through
	ofEnableAlphaBlending();
	
    // for testing purpose
	topLayer.loadImage("topLayer.png");
	mask.loadImage("mask.png");
	bottomLayer.loadImage("bottomLayer.png");
    
    // allocate our image buffers
    rgb.allocate(w, h);
    rgb2.allocate(w,h);
    maskImg.allocate(w,h);
    fbo.allocate(rgb2.getWidth()+10, rgb2.getHeight()+10);
    
    resetDrawing();

    
	//set the texture parameters for the maks shader. just do this at the beginning
	maskShader.load("composite");
	maskShader.begin();
	maskShader.setUniformTexture("Tex0", topLayer.getTextureReference(), 0);
	maskShader.setUniformTexture("Tex1", mask.getTextureReference(), 1);
	maskShader.end();
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
        //copy webcam pixels to rgb image
        rgb.setFromPixels(grabber.getPixels(), w, h);

        //mirror horizontally
        rgb.mirror(false, true);

        // copy to second buffer image
        rgb2 = rgb;
        
        // apply current brightness setting (darken image in second buffer)
        rgb2 -= brightness;

        // create mask image copy of darkened image (auto-converts to grayscale)
        maskImg = rgb2;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofSetColor(255,255,255);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    //draw all cv images
    rgb.draw(0,0);
    
    rgb2.draw(w,0);
    
    maskImg.draw(w, h);
    
    // draw to framebuffer, not to the screen
    fbo.begin();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
//        drawShade();
        rgb2.draw(0,0);
    fbo.end();
    
    fbo.draw(w*2, 0);
}

void testApp::drawShade(){
    int x,y;
    ofShader* shader = &maskShader;
    //ofImage* img = &topLayer;
    ofxCvColorImage* img = &rgb2;
    //ofImage* masker = &mask;
    ofxCvGrayscaleImage* masker = &maskImg;

    x=0;
    y=0;

    //first draw the bottom layer
    //	bottomLayer.draw(x, y);

	// make sure our new image blends with the existing content of the buffer
//	shader->begin();
        //our shader uses two textures, the top layer and the alpha
        //we can load two textures into a shader using the multi texture coordinate extensions
        glActiveTexture(GL_TEXTURE0_ARB);
        img->getTextureReference().bind();
        
        glActiveTexture(GL_TEXTURE1_ARB);
        masker->getTextureReference().bind();
        
        //draw a quad the size of the frame
        glBegin(GL_QUADS);

            //move the mask around with the mouse by modifying the texture coordinates
            float maskOffset = 0; //15 - mouseY + y ;
            glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskOffset);
            glVertex2f(x,y);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, img->getWidth(), 0);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, masker->getWidth(), maskOffset);
            glVertex2f(x+img->getWidth(), y);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, img->getWidth(), img->getHeight());
            glMultiTexCoord2d(GL_TEXTURE1_ARB, masker->getWidth(), masker->getHeight() + maskOffset);
            glVertex2f(x+img->getWidth(), y+bottomLayer.getHeight());
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, img->getHeight());
            glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, masker->getHeight() + maskOffset);
            glVertex2f(x, y+img->getHeight());
    
        glEnd();
        
        //deactive and clean up
        glActiveTexture(GL_TEXTURE1_ARB);
        masker->getTextureReference().unbind();
        
        glActiveTexture(GL_TEXTURE0_ARB);
        img->getTextureReference().unbind();
//	shader->end();
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

    if (key == OF_KEY_UP){
        if(brightness >= 5)
            brightness -= 5;
    }
    
    if (key == OF_KEY_DOWN){
        if(brightness <= 250)
            brightness += 5;
    }
	
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
