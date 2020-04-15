/*
 re the midi biz
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */






/*thanks to ed tannenbaum for help in fixing up the framebuffer code!*/










//so what is going to be added in here
//try out the new hsb controls for the framebuffers and also test with the other channels
//how many shader runs can we do?  curious if it will be feasible to do seperate shader runs for all of the vertex displacements but i think that it would be a good goal to have in terms of making a universial function for that to reuse as much as possible!  plus a goal is to test how many shader passes i can run before noticing slowdown!





#include "ofApp.h"


#include <iostream>

ofPolyline tetrahedron;






float scale1=1;
float scale2=1;

//vidmixer variables
float scale=.5;

float tt=0;



int framedelayoffset=0;  // this is used as an index to the circular framebuffers eeettt
unsigned int framecount=0; // framecounter used to calc offset eeettt


//i got this up to 240 frames with steady on my computer, is a bit skewed extreme for real time video
//executions tho
const int fbob=60; // number of "framedelay" buffers eeettt
//int dd=fbob;
//this buffers the framebuffers from the final screen draws

//this buffers the framebuffers from the final screen draws
ofFbo pastFrames[fbob];

ofFbo pastFrames_comp[fbob];
//ofFbo is the openframeworks framebuffer object
//this line is declaring an array of framebuffer objects
//many c++ afficionados prefer a c++ vector object instead of arrays every time
//because c++ vectors are like arrays but with some handier ways to access and add and delete contents
//but on the other hand vectors can have dynamic lengths and one can really end up in some
//trouble with dynamically working with framebuffers bc its generally best pratice to allocate
//memory on the gpu for all of yr framebuffers before yr code runs, trying to mess with that on
//the fly could have some interesting appeal to a glitch art style but is not going to please
//others

void incIndex()  // call this every frame to calc the offset eeettt
{
    
    framecount++;
    framedelayoffset=framecount % fbob;
}

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetFrameRate(30);
   // ofDisableAlphaBlending();
    //ofSetVerticalSync(FALSE);
    
    //syphon input
    
    //mClient.set("","Black Syphon");
    mClient.setup();
    //mClient.set("","Syphoner");
    //mClient.set("","Black Syphon");
    
    mClient.set("","Syphoner");

    
    
    //syphonoutput
    mainOutputSyphonServer.setName("LIL_WAAAVES");
    
    
    
    
   
    
	ofSetVerticalSync(true);
	ofBackground(0);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
    

    
    
    /**shaderobiz***/
  //  ofSetDataPathRoot("../Resources/data/");
#ifdef TARGET_OPENGLES

#else
    if(ofIsGLProgrammableRenderer()){
    
    }else{
   
        shader_CA.load("shadersGL2/shader_CA");
        shader_blur.load("shadersGL2/shader_blur");
        shader_sharpen.load("shadersGL2/shader_sharpen");
     
    }
#endif
    
   
    
 
    fbo_draw.allocate(ofGetWidth(), ofGetHeight());
    fbo_feedback.allocate(ofGetWidth(), ofGetHeight());
    syphonTexture.allocate(ofGetWidth(), ofGetHeight());
    fbo_blur.allocate(ofGetWidth(), ofGetHeight());
    fbo_sharpen.allocate(ofGetWidth(), ofGetHeight());
  
    /*
    fbo_draw.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    fbo_feedback.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    syphonTexture.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    fbo_blur.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    */
    fbo_feedback.begin();

    ofClear(0,0,0,255);
 
    fbo_feedback.end();
    
    fbo_draw.begin();
    ofClear(0,0,0,255);
    fbo_draw.end();
    
   
  
    
    
    //allocate and clear the variable delay final draw buffers
    //full resolution version
    
    /*
    
    for(int i=0;i<fbob;i++){
        
        pastFrames[i].allocate(ofGetWidth(), ofGetHeight());
        
     //   pastFrames[i].allocate(ofGetScreenWidth(), ofGetScreenHeight());
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
        
    
    }//endifor
    
    */
    
    
    
    
    
    
    
    
   
  
    
    //camerabiz
    //if u are having low framerates u can try lower resolution grabs
    //or if its all going smooth and u want to try higher resolutions here is where to fuck with that
    
    //add some optional UI interactivity with these
    //like a incrementer that cycles thru integers mod the lenght of the device list for selecting inputs cameras
    cam1.listDevices();
    cam1.setVerbose(true);
    cam1.setDeviceID(0);
     cam1.initGrabber(1280, 960);
   //cam1.initGrabber(640, 480);
   // cam1.initGrabber(320, 240);
    
  
    
    

    //setting up a tetrahedron
    
    /*
    ofVec3f tri1;
    ofVec3f tri2;
    ofVec3f tri3;
    ofVec3f tri4;
    tri1.set(1,1,1);
    tri2.set(-1,-1,1);
    tri3.set(-1,1,-1);
    tri4.set(1,-1,-1);
    float shapeScale=ofGetWidth()/8;
    
    tri1=tri1*shapeScale;
    tri2=tri2*shapeScale;
    tri3=tri3*shapeScale;
    tri4=tri4*shapeScale;
    
    
    tetrahedron.lineTo(tri1);
    tetrahedron.lineTo(tri2);
    tetrahedron.lineTo(tri4);
    tetrahedron.lineTo(tri1);
    tetrahedron.lineTo(tri3);
    tetrahedron.lineTo(tri2);
    tetrahedron.lineTo(tri3);
    tetrahedron.lineTo(tri4);
    //tetrahedron.lineTo(tri4);
    */
    
    
    
   

    
}


//--------------------------------------------------------------
void ofApp::update() {
    
    //only update if active set a test for that too
    
    
   
    
    cam1.update();
 
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    

   
 
    
    /***shaderbix**/
    
    
    //first draw the syphon input to a framebuffer to have it available as a texture to send to the mixer
    /*
    syphonTexture.begin();
  
    
    mClient.draw(0,0,(gui->syphon_scale)*mClient.getWidth(),(gui->syphon_scale)*mClient.getHeight());
    
    syphonTexture.end();
    */
 
    
    ///draw to the buffers
    
 
    fbo_draw.begin();
    
    
    //try putting graphics up here instead and see if then we can key into opaqueness
    
    
    
    
    
    
    shader_CA.begin();
    
    
    
    //global things
    shader_CA.setUniform1f("width", ofGetWidth());
    shader_CA.setUniform1f("height", ofGetHeight());
    
    shader_CA.setUniform2f("cam1dimensions",ofVec2f(cam1.getWidth(),cam1.getHeight()));
    
    
    shader_CA.setUniform1f("mixxx",gui->mix);
    shader_CA.setUniform1f("lumakey_value",gui->lumakey_value);
    
    shader_CA.setUniform1f("weight_u",gui->w_u);
    shader_CA.setUniform1f("weight_d",gui->w_d);
    shader_CA.setUniform1f("weight_l",gui->w_l);
    shader_CA.setUniform1f("weight_r",gui->w_r);
    shader_CA.setUniform1f("weight_h",gui->w_h);
    
    shader_CA.setUniform1f("weight_m",gui->w_m);
    
    shader_CA.setUniform1f("distance_u",gui->d_u);
    shader_CA.setUniform1f("distance_d",gui->d_d);
    shader_CA.setUniform1f("distance_l",gui->d_l);
    shader_CA.setUniform1f("distance_r",gui->d_r);
    
    shader_CA.setUniform1f("trails",gui->trails);
    shader_CA.setUniform1f("saturation_boost",gui->saturation_boost);

    
    //send variables from gui
    /*
    shader_CA.setUniform1i("channel1", gui->channel1_select);
    
    shader_CA.setUniform1f("fb0blend", gui->fb0_mix);
    shader_CA.setUniform1f("fb0lumakeyvalue", gui->fb0_key_value);
    shader_CA.setUniform1f("fb0lumakeythresh", 1);//gui->fb0_key_threshold);
    shader_CA.setUniform1i("fb0mix", gui->FBmix);
    shader_CA.setUniform1i("fb0_hflip_switch", gui->fb0_hflip_switch);
    shader_CA.setUniform1i("fb0_vflip_switch", gui->fb0_vflip_switch);
    shader_CA.setUniform1i("fb0_toroid_switch", gui->fb0_toroid_switch);
    
    shader_CA.setUniform1f("fb1blend", gui->fb1_mix);
    shader_CA.setUniform1f("fb1lumakeyvalue", gui->fb1_key_value);
    shader_CA.setUniform1f("fb1lumakeythresh", gui->fb1_key_threshold);
    shader_CA.setUniform1i("fb1mix", gui->FB1mix);
    shader_CA.setUniform1i("fb1_hflip_switch", gui->fb1_hflip_switch);
    shader_CA.setUniform1i("fb1_vflip_switch", gui->fb1_vflip_switch);
    shader_CA.setUniform1i("fb1_toroid_switch", gui->fb1_toroid_switch);
    
    shader_CA.setUniform1i("cam1_hflip_switch", gui->cam1_hflip_switch);
    shader_CA.setUniform1i("cam1_vflip_switch", gui->cam1_vflip_switch);
    
    */
    
    

    //shader_CA.setUniform1f("cam1_scale", gui->cam1_scale);
   
   
    
  
    //operations folder
    
   
    
   
    
    
    
    //--------------------------send the textures
    
    //so feedback buffer is tex0
    fbo_feedback.draw(0,0);
  
    
   // shader_CA.setUniformTexture("syphon",syphonTexture.getTexture(),1);
    shader_CA.setUniformTexture("cam1",cam1.getTexture(),1);
   

    shader_CA.end();
    
	
   
    
    

    //just drawing a little rotating tetrahedron
    
    /*
    
    if(gui->tetrahedron_switch==1){
    
    ofSetColor(127+127*(sin(ofGetElapsedTimef())),127+127*(cos(ofGetElapsedTimef()/7)),127-127*(sin(ofGetElapsedTimef()/19)),255);
    ofNoFill();
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
    ofRotateZRad(ofGetElapsedTimef()/5);
    ofRotateYRad(ofGetElapsedTimef()/13);
    ofRotateXRad(ofGetElapsedTimef()/11);
    tetrahedron.draw();
    
    
   
    ofPopMatrix();
    }
    */

    
    //ofSetColor(255);
    //ofDrawRectangle(ofGetWidth()/2,ofGetHeight()/2,100,100);
    
 
    
    fbo_draw.end();
    
    //----------------------------------------------------------
    
    
    //blur it
    fbo_blur.begin();
    shader_blur.begin();
    fbo_draw.draw(0,0);
    shader_blur.setUniform1f("blurAmnt",gui->blur_amount);
    shader_blur.end();
    fbo_blur.end();
    
    //---------------------------------
    
    //sharpen it
    fbo_sharpen.begin();
    shader_sharpen.begin();
    fbo_blur.draw(0,0);
    shader_sharpen.setUniform1f("sharpen_amount",gui->sharpen_amount);
    shader_sharpen.setUniform1f("sharpen_boost",gui->sharpen_boost);
    shader_sharpen.end();
    fbo_sharpen.end();
    
    //---------------------------------
    
    
   
    
    //fbo_draw.draw(0,0);
    fbo_sharpen.draw(0,0);
    
  
    
    
    
    //feed the previous frame into position 0 (index0)
    
    fbo_feedback.begin();
    
    
   
    //fbo_draw.draw(0,0);
    fbo_blur.draw(0,0);
   
    
    
    fbo_feedback.end();
    //-----____---____---__-__---____-----_--_-
    

   
 
    
    
    
    //add a switch for this
    /*
    if(gui->syphonOutput==1){
    mainOutputSyphonServer.publishScreen();
    }
    */
    incIndex(); // increment framecount and framedelayoffset eeettt
}


 
 /****************************************************/



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
  
   
    
  
 
    
    
   
    
    if(key==';'){scale1+=0.01;}
    if(key=='/'){scale1-=0.01;}
    
    if(key=='['){scale2+=0.01;}
    if(key==']'){scale2-=0.01;}
    
    
  
   


}

/************************/
//--------------------------------------------------------------
void ofApp::exit() {
	
	
}



//--------------------------------------------------------------



//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
   
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
   
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  
}

//--------------------------------------------------------------
void ofApp::mouseReleased() {
}
