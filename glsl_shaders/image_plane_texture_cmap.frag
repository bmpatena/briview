uniform sampler3D tex0;
uniform bool imageOnOff;
uniform float opacity,im_th_bias,im_th_scale;

uniform vec4 r_lut, g_lut, b_lut, a_lut;
uniform vec4 sc_lut;

uniform vec2 r_lut_last, g_lut_last, b_lut_last, a_lut_last, sc_lut_last;
uniform vec4 lower_clamp;

varying vec4 mat_color;



void main()
{

vec4 color = vec4(1.0,1.0,1.0,1.0);
vec3 im_color;


gl_FragColor = color;
}
