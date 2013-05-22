#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{
	
	public:
		
        // OF-basics
		void setup();
		void update();
		void draw();

        // helper methods
        void resetDrawing();
        ofBlendMode getSelectedBlendMode();

        // event-handlers
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


        // camera video grabber
        ofVideoGrabber grabber;

        // image objects; "normal" webcam image, manipulated webcam image and grayscale (mask) image
        ofxCvColorImage rgb, rgb2;
        ofxCvGrayscaleImage maskImg;
        ofxCvImage* source;

        // screenbuffer
        ofFbo fbo;

        // webcam image dimensions
        int camW,camH;
        
        // UI-stuff
        bool bDrawUI;
        ofxUICanvas *gui;

        void exit();
        void guiEvent(ofxUIEventArgs &e);
};
