#version 330 core
//--- in_Position: attribute index 0
//--- in_Normal: attribute index 1
//--- in_Color: attribute index 2
layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec3 in_Normal; //--- 노말 변수: attribute position 1
layout (location = 2) in vec3 in_Color; //--- 컬러 변수: attribute position 2


out vec3 out_Color; //--- 프래그먼트 셰이더에게 전달
out vec3 out_Normal; //--- 프래그먼트 셰이더에게 전달
out vec3 out_Position; //--- 프래그먼트 셰이더에게 전달

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    gl_Position = projection * view * model * vec4(in_Position, 1.0);
    out_Position = vec3(model * vec4(in_Position, 1.0));
    out_Color = in_Color;
    out_Normal = mat3(transpose(inverse(model))) * in_Normal; // 노말 변환
}