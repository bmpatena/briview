uniform sampler3D tex0;
//uniform float im_th_bias,im_th_scale;
uniform float opacity;

uniform vec4 r_lut, g_lut, b_lut, a_lut;
uniform vec4 sc_lut;

uniform vec2 r_lut_last, g_lut_last, b_lut_last, a_lut_last, sc_lut_last;
uniform vec4 lower_clamp;


void main()
{
vec4 color = vec4(0.0,0.0,0.0,1.0);


//float im_color=(texture3D(tex0,gl_TexCoord[0].stp).r - im_th_bias)*im_th_scale;
float im_color=texture3D(tex0,gl_TexCoord[0].stp).r ;


        int index;
        float sc_temp;
        if (im_color<float(sc_lut.x))
        {
        sc_temp=0.0;
        index=-1;
        }else if (im_color<sc_lut.y)
        {
        sc_temp= (im_color-sc_lut.x)/(sc_lut.y-sc_lut.x);
        index=0;
        }else if (im_color<sc_lut.z){
        sc_temp= (im_color-sc_lut.y)/(sc_lut.z-sc_lut.y);
        index=1;
        }else if (im_color<sc_lut.w){
        sc_temp= (im_color-sc_lut.z)/(sc_lut.w-sc_lut.z);
        index=2;
        }else if (im_color<sc_lut_last.x){
        sc_temp= (im_color-sc_lut.w)/(sc_lut_last.x-sc_lut.w);
        index=3;
        }else if (im_color<sc_lut_last.y){
        sc_temp= (im_color-sc_lut_last.x)/(sc_lut_last.y-sc_lut_last.x);
        index=4;
        }else{
        sc_temp=1.0 ;
        index=4;
        }
        if (index == -1 )
        {
        color.rgba = lower_clamp;
        }else if (index<=2)
        color = vec4( r_lut[index] + sc_temp * (r_lut[index+1]-r_lut[index]) , g_lut[index]+ sc_temp * (g_lut[index+1]-g_lut[index]), b_lut[index] + sc_temp * (b_lut[index+1]-b_lut[index]),(a_lut[index] + sc_temp * (a_lut[index+1]-a_lut[index]))*opacity);
        else if (index == 3)
        {
        color = vec4( r_lut[index] + sc_temp * (r_lut_last.x-r_lut[index]) , g_lut[index]+ sc_temp * (g_lut_last.x-g_lut[index]), b_lut[index] + sc_temp * (b_lut_last.x-b_lut[index]),(a_lut[index] + sc_temp * (a_lut_last.x-a_lut[index]))*opacity);
        }else if (index==4)
        {
        color = vec4( r_lut_last.x + sc_temp * (r_lut_last.y-r_lut_last.x) , g_lut_last.x + sc_temp * (g_lut_last.y-g_lut_last.x), b_lut_last.x + sc_temp * (b_lut_last.y-b_lut_last.x),(a_lut_last.x + sc_temp * (a_lut_last.y-a_lut_last.x))*opacity);
        }

        gl_FragColor = color;
}
