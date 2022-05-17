#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;

void main(){
    // Simply apply the modelViewProjectionMatrix to all vertex positions
    // to calculate the position of the vertex in the display
    gl_Position = modelViewProjectionMatrix * position;
}

