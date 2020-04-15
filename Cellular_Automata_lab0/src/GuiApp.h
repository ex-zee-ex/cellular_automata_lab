#pragma once

#include "ofMain.h"
//#include "ofxDatGui.h"
#include "ofxImGui.h"
class GuiApp : public ofBaseApp{
    
public:
public:
    void setup();
    void update();
    void draw();
    
    ofxImGui::Gui gui;
    float floatValue;
    
    float lumakey_value=0.0;
    float mix=0;
   
    float w_u=0.0;
    float w_d=0.0;
    float w_l=0.0;
    float w_r=0.0;
    float w_h=1.0;
    
    float w_m=1.0;
    
    float d_u=0.0;
    float d_d=0.0;
    float d_l=0.0;
    float d_r=0.0;
    
    float trails=1.0;
    float blur_amount=1.0;
    float sharpen_amount=0.0;
    float sharpen_boost=0.0;
    float saturation_boost=0.0;

   
   };
