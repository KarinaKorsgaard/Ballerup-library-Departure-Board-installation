#version 120
uniform sampler2D tex0;

varying vec2 texCoordVarying;

void main()
{
    float tex = texture2D(tex0, texCoordVarying).r;

    // contrast
  //  tex = pow(tex,0.8);
    tex = pow(tex,1.);
    
    gl_FragColor = vec4(vec3(tex),1.);
}

