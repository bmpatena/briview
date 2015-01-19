attribute vec4 InVertex;
attribute vec3 InNormal;
attribute float InScalar;

varying vec4 diffuse,ambientGlobal,ambient;
varying vec3 normal,lightDir,halfVector;
uniform vec4 r_lut, g_lut, b_lut, a_lut;
uniform vec4 sc_lut;

//uniform float r_lut_last, g_lut_last, b_lut_last, a_lut_last, sc_lut_last;
//uniform vec4 lower_clamp;

varying vec4 mat_color;

      void main()
      {


      int index;
      float sc_temp;
//      if (InScalar<float(sc_lut.x))
//      {
//      sc_temp=0.0;
//      index=-1;
//      }else if (InScalar<sc_lut.y)
//      {
//      sc_temp= (InScalar-sc_lut.x)/(sc_lut.y-sc_lut.x);
//      index=0;
//      }else if (InScalar<sc_lut.z){
//      sc_temp= (InScalar-sc_lut.y)/(sc_lut.z-sc_lut.y);
//      index=1;
//      }else if (InScalar<sc_lut.w){
//      sc_temp= (InScalar-sc_lut.y)/(sc_lut.w-sc_lut.z);
//      index=2;
//      }else{
//         sc_temp=1.0 ;
     // index=3;
    //  }

      if (InScalar<=float(sc_lut.x))
      {
      sc_temp=0.0;
      index=-1;
      }else if (InScalar<=sc_lut.y)
      {
      sc_temp= (InScalar-sc_lut.x)/(sc_lut.y-sc_lut.x);
      index=0;
      }else if (InScalar<=sc_lut.z){
      sc_temp= (InScalar-sc_lut.y)/(sc_lut.z-sc_lut.y);
      index=1;
      }else if (InScalar<=sc_lut.w){
      sc_temp= (InScalar-sc_lut.y)/(sc_lut.w-sc_lut.z);
      index=2;
      }else{
         sc_temp=1.0 ;
      index=3;
      }



          if (index == -1 )
          {
              mat_color.rgba = vec4( r_lut[0],g_lut[0],b_lut[0],a_lut[0]);
          }else if (index<=2)
             mat_color = vec4( r_lut[index] + sc_temp * (r_lut[index+1]-r_lut[index]) , g_lut[index]+ sc_temp * (g_lut[index+1]-g_lut[index]), b_lut[index] + sc_temp * (b_lut[index+1]-b_lut[index]),a_lut[index] + sc_temp * (a_lut[index+1]-a_lut[index]));
          else
             mat_color = vec4( r_lut[index] , g_lut[index], b_lut[index], a_lut[index] );




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

              gl_Position = gl_ModelViewProjectionMatrix * InVertex ; //ftransform();
      }
