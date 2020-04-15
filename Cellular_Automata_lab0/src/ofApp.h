/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#include "ofMain.h"
//#include "ofxMidi.h"
#include "GuiApp.h"
#include "ofxSyphon.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
    
 
    
    //-----------guibiz
    shared_ptr<GuiApp> gui;
    
    
    
    
    ofShader shader_CA;
    ofShader shader_blur;
    ofShader shader_sharpen;
    
   
    

 
    ofFbo fbo_draw;
    ofFbo fbo_feedback;
    ofFbo fbo_blur;
    ofFbo fbo_sharpen;
  
    
    
    ofFbo syphonTexture;
  
  

   
    
    ofVideoGrabber cam1;
    
    

    

    
    
   


    //sypphon some shit in yo
    
    ofxSyphonClient mClient;
    
    //and send it back out too
    ofxSyphonServer mainOutputSyphonServer;
    
    ofImage pnt_img;
    
};
