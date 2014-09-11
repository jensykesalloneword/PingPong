#include "ofApp.h"

#pragma mark once

int circleX, circleY,i, red, green, blue;
//————————————————————–
//--------------------------------------------------------------
void ofApp::setup(){

    ///Multi Windows
    
    ofSetWindowTitle("first window");
    
    // the arguments for the second window are its initial x and y position,
    // and its width and height. the last argument is whether the window
    // should be undecorated, i.e. title bar is visible. setting it to
    // true removes the title bar.
    
    secondWindow.setup("second window", 50, 50, 1024, 768, false);
    
    // running the command below positions an undecorated window to display on a second
    // monitor or projector. this is a good way to set up a fullscreen display, while
    // retaining a control window in the primary monitor.
    
    //secondWindow.setup("second window", ofGetScreenWidth(), 0, 1280, 800, true);
    
    
    
    ofSetFrameRate(100);
    ofBackground(150,50,10);
    w = 320;
    h = 240;
    //single cam
    camera.initGrabber(320,240);
    camera.setVerbose(true);
	camera.setDeviceID(1);
    
    //multi cam
    
//    camera2.initGrabber(320,240);
//    camera2.setVerbose(true);
//	camera2.setDeviceID(1);

    
    hue = 0;
    sat = 0;
    val = 0;
    drawEnabled = true;
    //reserve memory for cv images
    
    hsv.allocate(w,h);
    original.allocate(w,h);
    
    Hue.allocate(w, h);
    Sat.allocate(w,h);
    Bri.allocate(w,h);
    
    
    //for the tag
    Pixels = new unsigned char [w * h];
    Texture.allocate(w,h, GL_LUMINANCE);
    
    Output.allocate(w,h);
    
    //for the collected output
    dPixels = new unsigned char[w * h];
    for (int i = 0; i < (w * h); i++){
        dPixels[i] = 0;
    }
    detectedOutput.allocate(w,h, GL_LUMINANCE);
    
    
    
    
    
	bLearnBakground = true;
	threshold = 80;
    
    
    //————————————————————–sound input setup
    
    
    ofSetVerticalSync(true);
	
    //	 0 output channels,
    //	 2 input channels
    //	44100 samples per second
    //	256 samples per buffer
    //	 4 num buffers (latency)
    
    int bufferSize = 256;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    //when this is true, that means we want the X Y positions of the mouse
    trigger=false;
    
    
    
    //————————————————————–vector info setup
    
    
    
    centroids[0].x=(0);
    
    centroids[0].y=(5);
    
    
    centroids[1].x=(50);
    
    centroids[1].y=(58);
    
    
    centroids[2].x=(129);
    
    centroids[2].y=(58);
    
    
    centroids[3].x=(195);
    
    centroids[3].y=(58);
    
    
    centroids[4].x=(267);
    
    centroids[4].y=(58);
    
    
    centroids[5].x=(320);
    
    centroids[5].y=(58);
    
    
    centroids[6].x=(14);
    
    centroids[6].y=(191);
    
    
    centroids[7].x=(50);
    
    centroids[7].y=(191);
    
    
    centroids[8].x=(129);
    
    centroids[8].y=(191);
    
    
    centroids[9].x=(195);
    
    centroids[9].y=(191);
    
    
    centroids[10].x=(267);
    
    centroids[10].y=(191);
    
    
    centroids[11].x=(320);
    
    centroids[11].y=(191);
    
    
    
    centroids[12].x=(14);
    
    centroids[12].y=(215);
    
    
    
    centroids[13].x=(50);
    
    centroids[13].y=(215);
    
    
    centroids[14].x=(129);
    
    centroids[14].y=(215);
    
    
    centroids[15].x=(195);
    
    centroids[15].y=(215);
    
    
    centroids[16].x=(267);
    
    centroids[16].y=(215);
    
    
    centroids[17].x=(320);
    
    centroids[17].y=(215);
    
    
    centroids[18].x=(14);
    
    centroids[18].y=(145);
    
    
    centroids[19].x=(50);
    
    centroids[19].y=(145);
    
    
    centroids[20].x=(129);
    
    centroids[20].y=(145);
    
    
    centroids[21].x=(295);
    
    centroids[21].y=(145);
    
    
    centroids[22].x=(267);
    
    centroids[22].y=(145);
    
    
    centroids[23].x=(320);
    
    centroids[23].y=(145);
    

    
    //————————————————————–imagesequence
    
    // read the directory for the images
    // we know that they are named in seq
    ofDirectory dir;
    
    int nFiles = dir.listDir("test");
    if(nFiles) {
        
        for(int i=0; i<dir.numFiles(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().loadImage(filePath);
            
        }
        
    }
    else printf("Could not find folder\n");
    
    // this toggle will tell the sequence
    // be be indepent of the app fps
    bFrameIndependent = true;
    
    // this will set the speed to play
    // the animation back we set the
    // default to 24fps
    sequenceFPS = 24;
    
    // set the app fps
    appFPS = 60;
    ofSetFrameRate(appFPS);
    
}





//--------------------------------------------------------------
void ofApp::update(){
   
    
    camera.update();
    if(camera.isFrameNew()){
        //copy webcam pixels to rgb image
        original.setFromPixels(camera.getPixels(),320,240);
       
       
        hsv=original;
        hsv.convertRgbToHsv();
        
        
        
        //store the three channels as grayscale images
        
        
        hsv.convertToGrayscalePlanarImages(Hue, Sat, Bri);
        
        Hue.flagImageChanged();
		Sat.flagImageChanged();
		Bri.flagImageChanged();
		
		unsigned char * huePixels = Hue.getPixels();
		unsigned char * satPixels = Sat.getPixels();
		unsigned char * briPixels = Bri.getPixels();
        
        //filter the image based on the hue we want
		for (int i = 0; i < (w * h); i++) {
			if ((huePixels[i] <= 30  && huePixels[i] <= 100) &&
				(satPixels[i] >= 140)) {
				Pixels[i] = 255;
			} else {
				Pixels[i] = 0;
			}
		}
		
		Texture.loadData(Pixels, w, h, GL_LUMINANCE);
		Output.setFromPixels(Pixels, w, h);
        
        //find orange object over a certain size
		contours.findContours(Output, 160, (w * h) / 3, 1, false, true);
        
		//if big enough orange is found draw
		if (contours.blobs.size() > 0 && drawEnabled) {

			int lineWidth = 3;
			
			for (int i = -lineWidth; i <= lineWidth; i++) {
				for (int j = -lineWidth; j <= lineWidth; j++) {
					dPixels[((int)(contours.blobs.at(0).centroid.y + i) * w) + (int)((w - lineWidth - 1) - contours.blobs.at(0).centroid.x + j)] = 255;
				}
			}
		}
        detectedOutput.loadData(Pixels, w, h, GL_LUMINANCE);
        
        
        //————————————————————–vectorsco-ordinates
	}
    
    for (int i=0;i<36;i++) {
        
        
        distances[i]=position.distance(centroids[i]);
        
    }

    //Region of interest.
    currentD=10000;
    for (int i=0;i< 36; i++) {
        
        
        if (distances[i]<currentD) {
            
            currentD=distances[i];
            //this is the winner - the closest vector. You can attach a video/image to this...
            closestVec=i;
         
        }
    }
    int frameIndex = closestVec; //was set to zero
    cout << closestVec << endl;
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    // draw to the main window
    ofSetColor(0, 255, 0);
    ofDrawBitmapString("this is the first window", 100, 100);
    ofRect(150, 190, 240, 160);
    
    // draw the second window
//    secondWindow.begin();
//    ofBackground(255);
//    ofSetColor(0, 0, 255);
//    ofDrawBitmapString("this is the second window", 100, 100);
//    ofEllipse(320, 250, 200, 200);
//    secondWindow.end();
    
    ///^^^initialise this into the areas required.
    
    
    
    ofBackground(68,158,239,0.5);
    ofSetHexColor(0xffffff);
    camera.draw(20,20);
  //  camera2.draw(50,50);
    //draw all cv images
    
    
	
    
    hsv.draw(20,280);
    filtered.draw(20,280);
    
    // then draw the contours:
    contours.draw(20,280);
    
    // Texture.draw(320,20);
    Output.draw(340,280);
    
    char tmpStr[255];
	sprintf(tmpStr, "h: %i\ns: %i\nv: %i", hue, sat, val);
	ofDrawBitmapString(tmpStr, 20, 800);
    //————————————————————–
    
    

    //    draw circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        
        contours.blobs[i].draw(340,280);
        ofSetColor(0,255,0);
		
        ofRect(
               contours.blobs[i].boundingRect.getCenter().x,
               contours.blobs[i].boundingRect.getCenter().y,20,20);
    
    }
    //    //finding the center of the blob and where it is in relation to the center of the vectors.
  
    for(int i = 0; i < contours.blobs.size(); i ++) {
     //  if (trigger==true)  {
        position.x = contours.blobs[0].centroid.x;
        position.y=  contours.blobs[0].centroid.y;
   //  trigger=false;
   //     }
    }

    
    
    ofCircle(position.x, position.y, 10);
    
    
    // finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "bg subtraction and blob detection" << endl
    << "press ' ' to capture bg" << endl
    << "threshold " << threshold << " (press: +/-)" << endl
    << "num blobs found " << contours.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 600);
    
    //imagesequencecode
    int frameIndex = closestVec; //was set to zero
    // we need some images if not return
    if((int)images.size() <= 0) {
        ofSetColor(255);
        ofDrawBitmapString("No Images...", 150, ofGetHeight()/2);
        return;
    }
    // draw the second window
    secondWindow.begin();
    ofBackground(255);
    ofSetColor(0, 0, 255);
    ofDrawBitmapString("this is the second window", 100, 100);
   // ofEllipse(320, 250, 200, 200);
   

    // draw the image sequence at the new frame count
    ofSetColor(255);
    images[frameIndex].draw(0,0,1024, 768);
    
    secondWindow.end();
    
    // draw where we are in the sequence
    float x = 0;
    for(int offset = 0; offset < 5; offset++) {
        int i = (frameIndex + offset) % images.size();
        ofSetColor(255);
        images[i].draw(720+x, 540, 240, 160);
        x += 160;
    }
    
    
    // how fast is the app running and some other info
    ofSetColor(50);
    ofRect(720, 920, 200, 200);
    ofSetColor(200);
    string info;
    info += ofToString(frameIndex)+" sequence index\n";
    info += ofToString(appFPS)+"/"+ofToString(ofGetFrameRate(), 0)+" fps\n";
    info += ofToString(sequenceFPS)+" Sequence fps\n\n";
    info += "Keys:\nup & down arrows to\nchange app fps\n\n";
    info += "left & right arrows to\nchange sequence fps";
    info += "\n\n't' to toggle\nframe independent("+ofToString(bFrameIndependent)+")";
    
    ofDrawBitmapString(info, 1420, 30);
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    //—————————————————————hue
    if (key == 'h') {
		for (int i = 0; i < (w * h); i++) {
			Pixels[i] = 0;
		}
	}
	
	
    
    //—————————————————————threshold
    if (key == OF_KEY_RETURN) { drawEnabled = false; }
    
    switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case 'p':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
    
    //—————————————————————imagesequence
    
    if(key == OF_KEY_UP)      appFPS ++;
    if(key == OF_KEY_DOWN)    appFPS --;
    if(key == OF_KEY_LEFT)    sequenceFPS --;
    if(key == OF_KEY_RIGHT)   sequenceFPS ++;
    if(key == 't')            bFrameIndependent = !bFrameIndependent;
    
    // check for less than zero...
    sequenceFPS = MAX(sequenceFPS, 1);
    appFPS      = MAX(appFPS, 1);
    
    ofSetFrameRate(appFPS);
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){

 /*
    //    //this is going to be the current volume
    float curVol = 0.0;
    
    
    //lets go through each sample and calculate the root mean square (RMS) which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        
        //this calculates the RMS volume for each input channel and makes it a number between 0-1
        curVol += input[i*2]*0.5 * input[i*2]*0.5;;
        curVol += input[i*2+1]*0.5 * input[i*2+1]*0.5;
        
    }
    
    //if the current RMS Volume is over the threshold (in this case 0,5), set trigger to true;
    
    if (curVol>0.5) {
        trigger=true;
    }
	


*/
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == OF_KEY_RETURN) { drawEnabled = true; }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    // cout << “mouseMoved: ” << x << “, ” << y << endl;
//    circleX = x;
//   circleY = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    unsigned char * huePixels = Hue.getPixels();
    unsigned char * satPixels = Sat.getPixels();
    unsigned char * briPixels = Bri.getPixels();
	
	hue = huePixels[x + (y * Hue.width)];
	sat = satPixels[x + (y * Sat.width)];
	val = briPixels[x + (y * Bri.width)];
    
    //calculate local mouse x,y in image
//   int mx = x % w;
//    int my = y % h;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
