varying vec4 diffuse,ambientGlobal, ambient;
varying vec3 normal,lightDir,halfVector;
varying vec4 mat_color;
void main()
{
       vec3 n,halfV;
        float NdotL,NdotHV;
       vec4 color =  ambient+ambientGlobal;

        /* a fragment shader can't write a varying variable, hence we need
        a new variable to store the normalized interpolated normal */
        n = normalize(normal);

        /* compute the dot product between normal and normalized lightdir */

      NdotL = dot(n,normalize(lightDir));
        //2-sided lighting
        if (NdotL < 0.0)
        {
            n=-n;
            NdotL = dot(n,normalize(lightDir));
        }
        NdotL = max(NdotL,0.0);

        if (NdotL > 0.0) {

                color += diffuse * NdotL ;

                halfV = normalize(halfVector);
                NdotHV = max(dot(n,halfV),0.0);
              color += mat_color * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
        }
//        color=vec4(1.0,0.0,0.0,1.0);
//        color=vec4(1.0,0.0,0.0,1.0);
//color=mat_color;
        gl_FragColor = color;
}
