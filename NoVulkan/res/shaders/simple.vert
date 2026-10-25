#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColorInstance;
layout(location = 4) in vec3 inTranslation;
layout(location = 5) in vec3 inScale;
layout(location = 6) in vec3 inRotation;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

mat4 translateModel(mat4 modelMat){
    modelMat[3][0] = inTranslation.x;
	modelMat[3][1] = inTranslation.y;
	modelMat[3][2] = inTranslation.z;
    return modelMat;
}

mat4 rotateXModel(mat4 modelMat){
    float s = sin(inRotation.x);
    float c = cos(inRotation.x);

    mat4 r = mat4(1,0,0,0,  0,c,-s,0,    0,s,c,0,   0,0,0,1);

    modelMat *= r;
    return modelMat;
}

mat4 rotateYModel(mat4 modelMat){
    float s = sin(inRotation.y);
    float c = cos(inRotation.y);

    mat4 r = mat4(c,0,s,0,  0,1,0,0,    -s,0,c,0,   0,0,0,1);

    modelMat *= r;
    return modelMat;
}

mat4 rotateZModel(mat4 modelMat){
    float s = sin(inRotation.z);
    float c = cos(inRotation.z);

    mat4 r = mat4(c,-s,0,0,  s,c,0,0,    0,0,1,0,   0,0,0,1);

    modelMat *= r;
    return modelMat;
}

mat4 scaleModel(mat4 modelMat){
    float x = inScale.x;
    float y = inScale.y;
    float z = inScale.z;

    mat4 s = mat4(x,0,0,0,  0,y,0,0,    0,0,z,0,    0,0,0,1);
    modelMat *= s;
    return modelMat;
}

void main() {
    mat4 newModelMatrix = ubo.model;
    newModelMatrix = translateModel(newModelMatrix);
    newModelMatrix = rotateXModel(newModelMatrix);
    newModelMatrix = rotateYModel(newModelMatrix);
    newModelMatrix = rotateZModel(newModelMatrix);
    newModelMatrix = scaleModel(newModelMatrix);
    
    gl_Position = ubo.proj * ubo.view * newModelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor * inColorInstance;
    fragTexCoord = inTexCoord;
}