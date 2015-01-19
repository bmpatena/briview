attribute float InScalar;
attribute vec3 InNormal;
attribute vec4 InVertex;

void main()
{
        gl_TexCoord[0] = gl_MultiTexCoord0;
        gl_Position = ftransform();

        //gl_Position = gl_ModelViewProjectionMatrix * InVertex ; //ftransform();

}
