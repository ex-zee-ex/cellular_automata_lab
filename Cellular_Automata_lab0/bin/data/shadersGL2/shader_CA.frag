#version 120



uniform sampler2DRect cam1;


uniform sampler2DRect tex0;



//vidmixervariables

uniform float cam1_scale;

uniform float width;
uniform float height;

uniform float mixxx;
uniform float lumakey_value;


varying vec2 texCoordVarying;





uniform float ps;

uniform vec2 cam1dimensions;


uniform float pp=1.0;


uniform float weight_u;
uniform float weight_d;
uniform float weight_l;
uniform float weight_r;
uniform float weight_h;

uniform float weight_m;


uniform float distance_u;
uniform float distance_d;
uniform float distance_l;
uniform float distance_r;

uniform float trails;

uniform float saturation_boost;

float cheap_bright(vec4 c){

    return .33*c.r+.5*c.g+.16*c.b;
}


vec3 rgb2hsb(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsb2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}





vec4 mix_rgb(vec4 ch1, vec4 ch2, int mixswitch,float blend, float lumavalue, float lumathresh, float bright1,float bright2){
    vec4 mixout=vec4(0.0,0.0,0.0,0.0);
 
    
    //pass ch1 it thru
    
    if(mixswitch==0){
        mixout=ch1;
        
    }//endpass
    
    
    //blendit
    if(mixswitch==1){
        mixout=mix(ch1,ch2,blend);
            }//endblend
    
    //keyit
    if(mixswitch==2){
    
        if((bright1<lumavalue+lumathresh)&&(bright1>lumavalue-lumathresh)){
            mixout=mix(ch1,ch2,blend);
        
        }//endifbright1
     
     
        
        if(bright1<lumavalue){
            mixout=ch2;
        }
        else{
            mixout=mix(ch1,ch2,blend);
            
        }
         
    
    }//keyit
    
    //mixout=vec3(mixhue,mixsat,mixbright);
    return mixout;
}//endmixfunction






vec2 rotate(vec2 coord,float theta){
    vec2 center_coord=vec2(coord.x-width/2,coord.y-height/2);
    vec2 rotate_coord=vec2(0,0);
    float spiral=abs(coord.x+coord.y)/2*width;
    coord.x=spiral+coord.x;
    coord.y=spiral+coord.y;
    rotate_coord.x=center_coord.x*cos(theta)-center_coord.y*sin(theta);
    rotate_coord.y=center_coord.x*sin(theta)+center_coord.y*cos(theta);
    
    
    
    
    
    rotate_coord=rotate_coord+vec2(width/2,height/2);

    
    return rotate_coord;
    
    
}//endrotate


vec2 wrapCoord(vec2 coord){
    
    
    if(abs(coord.x)>width){coord.x=abs(width-coord.x);}
    if(abs(coord.y)>height){coord.y=abs(height-coord.y);}
    
    coord.x=mod(coord.x,width);
    coord.y=mod(coord.y,height);
    
        return coord;


}//endwrapcoord
void main()
{
    //dummy variable for output. probably don't need this but hey
    vec4 color=vec4(0.0, 0.0, 0.0, 0.0);
    
    vec4 cam1_color=texture2DRect(cam1, texCoordVarying);
    vec4 fbo_color=texture2DRect(tex0,texCoordVarying);
    
    // lets get a cellular automata neighborhood!
    
    //von neumman

    /*
     
     | |U| |
     |L|H|R|
     | |D| |
     
     */
    vec4 ca_u=texture2DRect(tex0, texCoordVarying+vec2(0,distance_u));
    vec4 ca_d=texture2DRect(tex0, texCoordVarying+vec2(0,-distance_d));
    vec4 ca_l=texture2DRect(tex0, texCoordVarying+vec2(distance_l,0));
    vec4 ca_r=texture2DRect(tex0, texCoordVarying+vec2(-distance_r,0));
    
    vec4 ca_h=texture2DRect(tex0, texCoordVarying);
    
    
    //moore
    vec4 ca_u_l=texture2DRect(tex0, texCoordVarying+vec2(distance_l,distance_u));
    vec4 ca_u_r=texture2DRect(tex0, texCoordVarying+vec2(-distance_r,distance_d));
    vec4 ca_d_l=texture2DRect(tex0, texCoordVarying+vec2(distance_l,-distance_d));
    vec4 ca_d_r=texture2DRect(tex0, texCoordVarying+vec2(-distance_r,-distance_d));
    
    //individual weights for each cell
    float w_u=weight_u*cheap_bright(ca_u);
    float w_d=weight_d*cheap_bright(ca_d);
    float w_l=weight_l*cheap_bright(ca_l);
    float w_r=weight_r*cheap_bright(ca_r);
    float w_h=weight_h*cheap_bright(ca_h);
    //then do a sum
   // vec4 ca_sum=w_u*ca_u+w_d*ca_d+w_l*ca_l+w_r*ca_r+w_h*ca_h;
    
    
    vec4 ca_sum=ca_h;
    
    
    if(cheap_bright(ca_u)>cheap_bright(ca_h)){
        ca_sum=(w_u)*ca_u+(1.0-w_u)*ca_sum;
    }
    
    if(cheap_bright(ca_d)>cheap_bright(ca_h)){
        ca_sum=(w_d)*ca_d+(1.0-w_d)*ca_sum;
    }
    
    if(cheap_bright(ca_l)>cheap_bright(ca_h)){
        ca_sum=(w_l)*ca_l+(1.0-w_l)*ca_sum;
    }
    
    if(cheap_bright(ca_r)>cheap_bright(ca_h)){
        ca_sum=(w_r)*ca_r+(1.0-w_r)*ca_sum;
    }
    /*
    ca_sum+=(w_l)*ca_d+(1.0-w_l)*ca_sum;
    ca_sum+=(w_u)*ca_u+(1.0-w_u)*ca_sum;
    ca_sum+=(w_d)*ca_d+(1.0-w_d)*ca_sum;
    ca_sum+=(w_r)*ca_d+(1.0-w_r)*ca_sum;
    */
    
    vec4 moore_ca_sum=(cheap_bright(ca_u_l)*ca_u_l+
                        cheap_bright(ca_u_r)*ca_u_r+
                        cheap_bright(ca_d_l)*ca_d_l+
                        cheap_bright(ca_d_r)*ca_d_r);
    
    ca_sum=(1.0-weight_m)*ca_sum+weight_m*moore_ca_sum;
    //now try instead extracting brightness
    /*
    vec3 ca_u_hsb=rgb2hsb(vec3(ca_u.r,ca_u.g,ca_u.g));
    vec3 ca_d_hsb=rgb2hsb(vec3(ca_d.r,ca_d.g,ca_d.g));
    vec3 ca_l_hsb=rgb2hsb(vec3(ca_l.r,ca_l.g,ca_l.g));
    vec3 ca_r_hsb=rgb2hsb(vec3(ca_r.r,ca_r.g,ca_r.g));
    vec3 ca_h_hsb=rgb2hsb(vec3(ca_h.r,ca_h.g,ca_h.g));
    
    vec3 ca_sum_hsb=vec3(0.0);
    ca_sum_hsb.x=w_u*ca_u.x+w_d*ca_d.x+w_l*ca_l.x+w_r*ca_r.x+w_h*ca_h.x;
    
    ca_sum_hsb.x=fract(ca_sum_hsb.x);
    ca_sum_hsb.x=abs(ca_sum_hsb.x);
    
    ca_sum=vec4(hsb2rgb(vec3(ca_sum_hsb.r,ca_h_hsb.g,ca_h_hsb.b)),1.0);
    //and normalize it
    //ca_sum=ca_sum/5.0;
    */
    
    
    //then add some trails
    if(ca_sum.r>=ca_h.r){
        ca_sum=trails*ca_sum;
    }

    
    ca_sum=fract(ca_sum);
    ca_sum=abs(ca_sum);
 
    vec3 ca_sum_hsb=rgb2hsb(vec3(ca_sum.r,ca_sum.g,ca_sum.b));
    
    ca_sum_hsb.y=(saturation_boost+1.0)*ca_sum_hsb.y;
    
    ca_sum=vec4(hsb2rgb(vec3(ca_sum_hsb.r,ca_sum_hsb.g,ca_sum_hsb.b)),ca_sum.a);
    //try black and white
   // ca_sum=vec4(ca_sum.r,ca_sum.r,ca_sum.r,ca_sum.r);
    //vec3 ca_sum_hsb=rbg2hsb(vec3(ca_sum.r,ca_sum.g,ca_sum.b));

    //cheap brightness
    float cam1_bright=.33*cam1_color.r+.5*cam1_color.g+.16*cam1_color.b;
    
    
    //black and white more
    
    
    if(cam1_bright<lumakey_value){
        //color=fbo_color;
        color=ca_sum;

    }
    else{
        //color=mix(cam1_color,fbo_color,mixxx);
        color=mix(cam1_color,ca_sum,mixxx);
    }

    
    gl_FragColor = color;
    
   
    

}
