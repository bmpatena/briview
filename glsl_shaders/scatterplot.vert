attribute vec4 InVertex;
attribute vec3 InNormal;
attribute float InScalar;

varying vec4 diffuse,ambientGlobal,ambient;
varying vec3 normal,lightDir,halfVector;
uniform vec4 r_lut, g_lut, b_lut, a_lut;
uniform vec4 sc_lut;

//uniform float r_lut_last, g_lut_last, b_lut_last, a_lut_last, sc_lut_last;
//uniform vec4 lower_clamp;
uniform mat4 MVP;

varying vec4 mat_color;

void main()
{
    
    if (InScalar==float(3))
       mat_color = vec4(1.0,0.0,0.0,1.0);
    else if (InScalar==float(2))
       mat_color = vec4(0.0,0.0,1.0,1.0);	 
    else if (InScalar==float(1))
       mat_color = vec4(1.0,1.0,1.0,1.0);
    else
       mat_color = vec4(0.0,0.0,0.0,1.0);
    
    
    
    
    
    
    //  mat_color.rgb=normalize(mat_color.rgb);
    // normal = normalize(gl_NormalMatrix * inNormal);
    normal = normalize(gl_NormalMatrix * InNormal);
    
    lightDir = normalize(vec3(gl_LightSource[0].position));
    halfVector = normalize(gl_LightSource[0].halfVector.xyz);
    
    /* Compute the diffuse, ambient and globalAmbient terms */
    diffuse =  mat_color * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    //   diffuse+= gl_BackMaterial.diffuse * gl_LightSource[0].diffuse;
    /* The ambient terms have been separated since one of them */
    /* suffers attenuation */
    ambient =  mat_color * gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    //    ambient =  gl_BackMaterial.ambient * gl_LightSource[0].ambient;
    
    ambientGlobal =  mat_color *  gl_FrontMaterial.ambient * gl_LightModel.ambient;
    
    gl_Position = MVP * InVertex ; //ftransform();
}
