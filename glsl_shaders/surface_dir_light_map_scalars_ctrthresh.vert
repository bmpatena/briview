  varying vec4 diffuse,ambientGlobal,ambient;
  varying vec3 normal,lightDir,halfVector;
  uniform vec4 r_lut, g_lut, b_lut, a_lut;
  uniform vec4 sc_lut;
  attribute float InScalar;
  attribute vec3 InNormal;
  attribute vec4 InVertex;
  uniform vec2 r_lut_last, g_lut_last, b_lut_last, a_lut_last, sc_lut_last;
  uniform vec4 lower_clamp;

  varying vec4 mat_color;

        void main()
        {
            int index;
            float sc_temp;
            //bool center=false;
            //condition if centre is the same, set to clamp value
           // if ((InScalar<=sc_lut.z)&&(sc_lut.z == sc_lut.w))
           // {
               // center=true;
           //     index=-1;
                //if lies between middle set to clamp
         //   }else 
                if ((abs(InScalar)<abs(sc_lut.z)) && (abs(sc_lut.z) == abs(sc_lut.w)))
            {
                index=-1;
                    //center=true
                //then procee to normal
            }else if (InScalar<float(sc_lut.x)){
       sc_temp=0.0;
       index=-1;
       }else if (InScalar<sc_lut.y)
       {
       sc_temp= (InScalar-sc_lut.x)/(sc_lut.y-sc_lut.x);
       index=0;
       }else if (InScalar<=sc_lut.z){
       sc_temp= (InScalar-sc_lut.y)/(sc_lut.z-sc_lut.y);
       index=1;
       }else if (InScalar<sc_lut_last.x){
       sc_temp= (InScalar-sc_lut.w)/(sc_lut_last.x-sc_lut.w);
       index=3;
       }else if (InScalar<=sc_lut_last.y){
       sc_temp= (InScalar-sc_lut_last.x)/(sc_lut_last.y-sc_lut_last.x);
       index=4;
       }else{
     //  sc_temp=1.0 ;
      // index=4;
           //clamp both sides
           index=-1;
       
       }


        if (index == -1 )
        {
        mat_color.rgba = lower_clamp;
        }else if (index<=2)
        mat_color = vec4( r_lut[index] + sc_temp * (r_lut[index+1]-r_lut[index]) , g_lut[index]+ sc_temp * (g_lut[index+1]-g_lut[index]), b_lut[index] + sc_temp * (b_lut[index+1]-b_lut[index]),(a_lut[index] + sc_temp * (a_lut[index+1]-a_lut[index])));
        else if (index == 3)
        {
        mat_color = vec4( r_lut[index] + sc_temp * (r_lut_last.x-r_lut[index]) , g_lut[index]+ sc_temp * (g_lut_last.x-g_lut[index]), b_lut[index] + sc_temp * (b_lut_last.x-b_lut[index]),(a_lut[index] + sc_temp * (a_lut_last.x-a_lut[index])));
        }else if (index==4)
        {
        mat_color = vec4( r_lut_last.x + sc_temp * (r_lut_last.y-r_lut_last.x) , g_lut_last.x + sc_temp * (g_lut_last.y-g_lut_last.x), b_lut_last.x + sc_temp * (b_lut_last.y-b_lut_last.x),(a_lut_last.x + sc_temp * (a_lut_last.y-a_lut_last.x)));
        }

            // mat_color.rgb=normalize(mat_color.rgb);
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
