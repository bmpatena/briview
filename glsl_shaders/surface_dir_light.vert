  varying vec4 diffuse,ambientGlobal,ambient;
  varying vec3 normal,lightDir,halfVector;

  attribute vec3 InNormal;
  attribute vec4 InVertex;

        void main()
        {

               // normal = normalize(gl_NormalMatrix * inNormal);
               normal = normalize(gl_NormalMatrix * InNormal);

                lightDir = normalize(vec3(gl_LightSource[0].position));

          //      if ( dot(normal,lightDir) <0.0 ){
           //         normal= -normal;
            //    }


                halfVector = normalize(gl_LightSource[0].halfVector.xyz);

                /* Compute the diffuse, ambient and globalAmbient terms */
                diffuse =   gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
                //   diffuse+= gl_BackMaterial.diffuse * gl_LightSource[0].diffuse;
                /* The ambient terms have been separated since one of them */
                /* suffers attenuation */
                ambient =  gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
                //   ambient =  gl_BackMaterial.ambient * gl_LightSource[0].ambient;

                ambientGlobal =   gl_FrontMaterial.ambient * gl_LightModel.ambient;

                gl_Position = ftransform();

             //   gl_Position = gl_ModelViewProjectionMatrix * InVertex ; //ftransform();
        }

