#version 450

// Vertex Attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

layout(location = 0) out vec3 fragColour;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject
{
    mat4 projectionViewMatrix;
} uniformBufferObject;

layout(push_constant) uniform Push
{
    mat4 modelMatrix;
    vec3 colour;
} push;

void main()
{
    gl_Position = uniformBufferObject.projectionViewMatrix * push.modelMatrix * vec4(position, 1.0);
    fragColour = colour;
}