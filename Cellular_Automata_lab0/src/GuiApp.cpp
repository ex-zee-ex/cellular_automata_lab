/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"


int fbob=60;

int guiscale=275;

void GuiApp::setup(){
    
    ofBackground(0);
    //floatValue=0.0;
    
    gui.setup();
    
}
  //----------------------------------
    void GuiApp::update() {
       
    }


//------------------------------
    void GuiApp::draw() {
        
        auto mainSettings = ofxImGui::Settings();
        
        int gui_hscaler=550;
        int gui_vscaler=680;
        ImGui::SetNextWindowPos(ImVec2(0*gui_hscaler, 0*gui_vscaler), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(gui_hscaler, gui_vscaler), ImGuiCond_FirstUseEver);
        
        
        gui.begin();
        
        
        //channel 1 select folder
        if (ofxImGui::BeginWindow("channel1", mainSettings, false))
        {
        if (ImGui::CollapsingHeader("collapsing folder"))
        {
            //ah ok so this is a const char not a string
            ImGui::Text("i'm in a collapsable folder");
            
            /*
            //so for dropdown menues set up a char array of items like so
            const char* items[] = { "cam1", "syphon" };
            static int item_current = 0;
            ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
            if(item_current==0){channel1_select=1;}
            if(item_current==1){channel1_select=3;}
            */
             
            ImGui::SliderFloat("mix", &mix, -2.0f, 2.0f);
            ImGui::SliderFloat("key value", &lumakey_value, .0f, 1.0f);
            
            ImGui::SliderFloat("trails", &trails, -5.0f, 5.0f);
            
            ImGui::SliderFloat("weight_up", &w_u, -5.0f, 5.0f);
            ImGui::SliderFloat("weight_down", &w_d, -5.0f, 5.0f);
            ImGui::SliderFloat("weight_left", &w_l, -5.0f, 5.0f);
            ImGui::SliderFloat("weight_right", &w_r, -5.0f, 5.0f);
            ImGui::SliderFloat("weight_home", &w_h, -5.0f, 5.0f);
            
            ImGui::SliderFloat("weight_moore", &w_m, -2.0f, 2.0f);
            
            ImGui::SliderFloat("distance_up", &d_u, 0.0f, 10.0f);
            ImGui::SliderFloat("distance_down", &d_d, 0.0f, 10.0f);
            ImGui::SliderFloat("distance_left", &d_l, 0.0f, 10.0f);
            ImGui::SliderFloat("distance_right", &d_r, 0.0f, 10.0f);
            
            ImGui::SliderFloat("blur", &blur_amount, 0.0f, 5.0f);
            ImGui::SliderFloat("sharpen_amount", &sharpen_amount, .0f, .2f);
            ImGui::SliderFloat("sharpen_boost", &sharpen_boost, .0f, 2.0f);
            
            ImGui::SliderFloat("saturation_boost", &saturation_boost, .0f, 2.0f);


            
            
            //ImGui::Checkbox("saturation_wrap", &ch1_saturation_wrap);
            
            
            
        }//end collapsing folder
            
        }//endImguiWindow
        ofxImGui::EndWindow(mainSettings);
      
        
        
        
        
        gui.end();
    }


//---------------------------




