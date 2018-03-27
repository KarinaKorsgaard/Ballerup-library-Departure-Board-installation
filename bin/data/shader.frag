#version 120

varying vec4 globalColor;
uniform sampler2D tex0;

varying vec2 texCoordVarying;
void main()
{
    float tex = texture2D(tex0, texCoordVarying).r;
    // globalColor is being set inside the vertex shader.
    // the fragment color is then set to the globalColor.
    float grayscale = 0.2126 * tex.r + 0.7152 * tex.g + 0.0722 * tex.b;
    if(grayscale < .5){
        // set black pixel in buffer
        tex = 0.;
    }
    gl_FragColor = vec4(vec3(tex),1.);
}
