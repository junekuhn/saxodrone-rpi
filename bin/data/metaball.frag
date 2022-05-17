
#version 150
//varying vec2 vTexCoord;
//uniform num_balls
//uniform const int num_balls;
uniform vec3 metaballs[7];
const int numBalls = 7;

uniform float WIDTH;
uniform float HEIGHT;
const float RADIUS = 500.0;

out vec4 outputColor;
vec3 hsv2rgb(vec3 c) {

        vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
        vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
        return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec2 pos = gl_FragCoord.xy;
    float x = pos.x / WIDTH;
    float y = pos.y / HEIGHT;
    float v = 0.0;

    for (int i = 0; i < numBalls; i++) {
        vec3 ball = vec3(metaballs[i].x/WIDTH, metaballs[i].y/HEIGHT, metaballs[i].z/RADIUS);
        float dx = ball.x - x;
        float dy = ball.y - y;
        float r = ball.z;
        v += r * r / (dx * dx + dy * dy);
    }

    if (0.1 < v && v < 1.1) {
        float a = (v - 0.9) * 4.;
        outputColor = vec4(hsv2rgb(vec3(a, 1., 1.)), 1.0);
    } else {
        outputColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
