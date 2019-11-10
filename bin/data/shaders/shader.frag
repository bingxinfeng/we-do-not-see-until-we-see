// https://thebookofshaders.com/edit.php?log=161127200501

//fragment shader
#version 150

out vec4 outputColor;

uniform vec2 u_resolution;
uniform float u_time;

uniform float level1; //0-1
uniform float level2; //0-1
uniform float level3; //0-1

uniform float l1_ax; //-5000,5000 L1:(10,1)(20,2)
uniform float l1_ay;
uniform float l1_bx;
uniform float l1_by;

uniform float l2_ax; //-5000,5000 L2:(45.0, 2.0)(60, 16.0)
uniform float l2_ay;
uniform float l2_bx;
uniform float l2_by;

uniform float l3_ax; //-5000,5000 L3:(90.0, 3.0)(120, 32.0)
uniform float l3_ay;
uniform float l3_bx;
uniform float l3_by;

uniform float l4_ax; //-5000,5000 L4:(200.0, 14.0)(1000, 64.0)
uniform float l4_ay;
uniform float l4_bx;
uniform float l4_by;


float random (in vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,u_time)))*43758.5453123+u_time);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

void main() {
    vec2 st = gl_FragCoord.xy / u_resolution.xy;
    st.x *= u_resolution.x / u_resolution.y;
    
    float v;
    if (st.y < level1) {
        v = noise(st * vec2(l1_ax, l1_ay)) - noise(st * vec2(l1_bx, l1_by));
    } else if (st.y < level2) {
        v = noise(st * vec2(l2_ax, l2_ay)) - noise(st * vec2(l2_bx, l2_by));
    } else if (st.y < level3) {
        v = noise(st * vec2(l3_ax, l3_ay)) - noise(st * vec2(l3_bx, l3_by));
    } else {
        v = noise(st * vec2(l4_ax, l4_ay)) - noise(st * vec2(l4_bx, l4_by));
    }
    
    outputColor = vec4(vec3(v), 1.0);
}
