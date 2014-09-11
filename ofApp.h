#pragma once
#ifndef _OF_APP
#define _OF_APP
#include "ofMain.h"
#include "ofxSecondWindow.h"
#include "ofxOpenCv.h"
//#include “ofxCv.h”

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
// otherwise, we'll use a movie file

class ofApp : public ofBaseApp{
    
public:
    
    void setup(); //to setup a font fo this project
    void update();
    void draw(); //to draw the font on screen
    void keyPressed(int);                         //On KEY Press Events will be defined here
    void keyReleased(int key);
    
    void mouseMoved(int, int);            // On Mouse Move Events will be defined here
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button); //On Mouse Press Events will be defined here
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxSecondWindow secondWindow; //multi windows
    //  ofVideoGrabber movie;                          // movie object for capturing Video from Cam
    
    //rgb object for storing color image data
    
    
    ofxCvGrayscaleImage filtered,red,green,blue;  //objects for storing grayscale image data
    int w,h;
    int                 hue, sat, val;
    
    bool				bLearnBakground;
    bool                drawEnabled;
    
    ofVideoGrabber camera,camera2,camera3,camera4;
    ofxCvColorImage hsv;
    ofxCvColorImage original;
    
    ofxCvGrayscaleImage 	Hue;
    ofxCvGrayscaleImage 	Sat;
    ofxCvGrayscaleImage 	Bri;
    
    
    unsigned char * Pixels;
    ofTexture Texture;
    
    
    
    ofxCvGrayscaleImage 	Output;
    ofxCvContourFinder contours;                  //object to store contours of the filtered image
    
    unsigned char * dPixels;
    ofTexture detectedOutput;
    
    
    
    
    int 				threshold;
    
    
    ofVec2f position;
    ofVec2f centroids[36];
    
    float distances[36];
    
    float currentD;
    
    int closestVec;
    
    void audioIn(float * input, int bufferSize, int nChannels);
    
    ofSoundStream soundStream;
    
    bool trigger;
    
    //some vals to store the mouse positions. you probably want these in a vector
    
    int posX,posY;
    
    ofImage cher;
    
    int   appFPS;
    float sequenceFPS;
    bool  bFrameIndependent;
    vector <ofImage> images;
    
    
    
    
};
#endif

