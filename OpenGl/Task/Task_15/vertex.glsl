#version 330 core
//--- in_Position: attribute index 0
//--- in_Color: attribute index 1
layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec3 in_Color; //--- 컬러 변수: attribute position 1
out vec3 out_Color; //--- 프래그먼트 세이더에게 전달


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
gl_Position = projection* view * model * vec4 (in_Position, 1.0);
out_Color = in_Color;
}
