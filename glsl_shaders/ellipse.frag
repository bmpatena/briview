uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

void main(void)
{
gl_FragColor = vec4(1,0,0,1);
   // gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
    vec2 midpoint = vec2(0,0);
//screenDim_.xy * 0.5;
  //  float radius = min(screenDim_.x, screenDim_.y) * circleRadius_;
float radius = 1.0;    
float dist = length(gl_FragCoord.xy - midpoint);
//    circle = 
smoothstep(radius-1.0, radius+1.0, dist);
}
