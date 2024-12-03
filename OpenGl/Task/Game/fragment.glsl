#version 330 core
//--- out_Color: 버텍스 셰이더에서 입력받는 색상 값
//--- out_Normal: 버텍스 셰이더에서 입력받는 노말 벡터
//--- FragColor: 출력할 색상의 값으로 프레임 버퍼로 전달 됨.
in vec3 out_Color; //--- 버텍스 셰이더에게서 전달 받음
in vec3 out_Normal; //--- 버텍스 셰이더에게서 전달 받음
in vec3 out_Position; //--- 버텍스 셰이더에게서 전달 받음

out vec4 FragColor; //--- 색상 출력

//--- 조명의 위치, 색상, 객체의 색상, 카메라의 위치를 받음
uniform vec3 lightPos; //--- 조명의 위치
uniform vec3 lightColor; //--- 조명의 색
uniform vec3 objectColor; //--- 객체의 색
uniform vec3 viewPos; //--- 카메라의 위치


void main(void)
{
    // Ambient lighting
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(out_Normal);
    vec3 lightDir = normalize(lightPos - out_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    int shininess = 128;
    vec3 viewDir = normalize(viewPos - out_Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * out_Color;
    FragColor = vec4(result, 1.0);
}