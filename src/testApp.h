#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
    
        void resetDrawing();
        void drawShade();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

    
        ofVideoGrabber grabber;
        
        ofxCvColorImage rgb,hsb;
        ofxCvGrayscaleImage hue,sat,bri,filtered;
        ofxCvContourFinder contours;
    
        ofxCvColorImage rgb2;
        ofxCvGrayscaleImage maskImg;

        ofImage mask;
        ofImage topLayer;
        ofImage bottomLayer;
        ofShader maskShader;
    
        ofFbo fbo;
    
        int w,h;
        int brightness;
    
};
