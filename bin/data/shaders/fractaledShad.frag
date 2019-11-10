//fragment shader
#version 150

out vec4 fragColor;

uniform vec2 u_resolution;
uniform sampler2D imageTexture;
uniform sampler2D offsetTexture; //画的颜色图层


void main() {
    // Get the texture co-ordinate of the current pixel
    vec2 texCoord = gl_FragCoord.xy / textureSize(offsetTexture, 0);
    
    // Lookup the offset value for this pixel from the offsetTexture
    vec4 offsetVal = texture(offsetTexture, texCoord);
    texCoord.y = 1.0 - texCoord.y;//??好像没差？
    
    // Lookup the colour value for this pixel applying the offset to the texture coordinate
    vec4 textureCol = texture(imageTexture, texCoord + offsetVal.xy*.1);
    
    //alpha mask:
    float mask = texture(offsetTexture, texCoord).g + texture(offsetTexture, texCoord).b*1.5;
    
    // Set output colour for this pixel
    fragColor = vec4(textureCol.rgb, mask);//这里的rgba都是指最后画出imageTexture的rgba
}
