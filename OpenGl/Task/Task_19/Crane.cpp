#include "Model.h"
#include "Crane.h"


Crane::Crane()
    : body(Cube())
    , frontLeg1(Cube())
    , frontLeg2(Cube())
    , head(Cube())
    , cannon1(Cube())
    , cannon2(Cube())
{
}

Crane::~Crane()
{
}

extern float b;
extern float m_theta;
extern float leg_theta;
extern float leg_distance;
extern float cannon_theta;

void Crane::Draw(GLuint shaderProgramID)
{
    float base_width = 1.f;
    float base_height = 1.f;
    float base_depth = 1.f;
    vec4 base_center = vec4(0.f, 0.f, 0.f,1.f);

    mat4 identity = mat4(1.0f);
    mat4 model = mat4(1.0f);
    mat4 trs = mat4(1.0f);
    mat4 trs1 = mat4(1.0f);
    mat4 trs2 = mat4(1.0f);
    mat4 rot = mat4(1.0f);
    mat4 rot2 = mat4(1.0f);
    mat4 scale = mat4(1.0f);

    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    
    // ======================= body =======================
    scale = glm::scale(identity, vec3(1.5f, 0.5f, 1.5f));
    float body_width = base_width * 1.5f;
    float body_height = base_height * 0.5f;
    float body_depth = base_depth * 1.5f;
    trs = glm::translate(identity, vec3(0.f+b, body_height / 2.f, 0.f));
    rot = glm::rotate(identity, radians(0.f), vec3(0.f, 1.f, 0.f));
    model = trs * rot * scale;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    body.Draw(shaderProgramID);

    // ======================= frontLeg1 =======================
    model = mat4(1.0f);
    scale = glm::scale(identity, vec3(0.2f, 0.2f, 0.5f));
    float frontLeg1_width = base_width * 0.2f;
    float frontLeg1_height = base_height * 0.2f;
    float frontLeg1_depth = base_depth * 0.5f;
    trs = glm::translate(identity, vec3(-body_width/2.f + frontLeg1_width/2.f + leg_distance, frontLeg1_height / 2.f, body_depth / 2.f + frontLeg1_depth / 2.f));
    rot = glm::rotate(identity, radians(-leg_theta), vec3(0.f, 1.f, 0.f));
    model = trs  * scale;
    model = rot * model;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    frontLeg1.Draw(shaderProgramID);
    
    // ======================= frontLeg2 =======================
    model = mat4(1.0f);
    scale = glm::scale(identity, vec3(0.2f, 0.2f, 0.5f));
    float frontLeg2_width = base_width * 0.2f;
    float frontLeg2_height = base_height * 0.2f;
    float frontLeg2_depth = base_depth * 0.5f;
    trs = glm::translate(identity, vec3(body_width / 2.f - frontLeg1_width / 2.f - leg_distance, frontLeg1_height / 2.f, body_depth / 2.f + frontLeg1_depth / 2.f));
    rot = glm::rotate(identity, radians(leg_theta), vec3(0.f, 1.f, 0.f));
    model = trs * scale;
    model = rot * model;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    frontLeg2.Draw(shaderProgramID);
    
    // ======================= head =======================
    model = mat4(1.0f);
    scale = glm::scale(identity, vec3(0.75f, 0.25f, 0.75f));
    float head_width = base_width * 0.75f;
    float head_height = base_height * 0.25f;
    float head_depth = base_depth * 0.75f;
    trs = glm::translate(identity, vec3(0.f+b, head_height / 2.f + body_height, 0.f));
    rot = glm::rotate(identity, radians(m_theta), vec3(0.f, 1.f, 0.f));
    model = trs * rot * scale;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    head.Draw(shaderProgramID);
   
    // ======================= cannon1 =======================
    model = mat4(1.0f);
    scale = glm::scale(identity, vec3(0.2f, 0.5f, 0.2f));
    float cannon1_width = base_width * 0.2f;
    float cannon1_height = base_height * 0.5f;
    float cannon1_depth = base_depth * 0.2f;
    trs = glm::translate(identity, vec3(-head_width / 2.f + cannon1_width / 2 + b, body_height+ head_height + cannon1_height / 2.f, 0.f));
    rot = glm::rotate(identity, radians(m_theta), vec3(0.f, 1.f, 0.f));
    rot2 = glm::rotate(identity, radians(cannon_theta), vec3(0.f, 0.f, 1.f));
    trs1 = glm::translate(identity, vec3(head_width/2 - cannon1_width/2, -(body_height+head_height), 0.f));
    trs2 = glm::translate(identity, vec3(-head_width / 2 + cannon1_width / 2, (body_height + head_height), 0.f));
    model = trs *rot * scale;
    model = trs2 *rot2 *trs1 * model;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    cannon1.Draw(shaderProgramID);
    
    // ======================= cannon2 =======================
    model = mat4(1.0f);
    scale = glm::scale(identity, vec3(0.2f, 0.5f, 0.2f));
    float cannon2_width = base_width * 0.2f;
    float cannon2_height = base_height * 0.5f;
    float cannon2_depth = base_depth * 0.2f;
    trs = glm::translate(identity, vec3(head_width / 2.f - cannon1_width / 2 + b, body_height + head_height + cannon1_height / 2.f, 0.f));
    rot = glm::rotate(identity, radians(m_theta), vec3(0.f, 1.f, 0.f));
    rot2 = glm::rotate(identity, radians(-cannon_theta), vec3(0.f, 0.f, 1.f));
    trs1 = glm::translate(identity, vec3(-head_width / 2 + cannon1_width / 2, -(body_height + head_height), 0.f));
    trs2 = glm::translate(identity, vec3(+head_width / 2 - cannon1_width / 2, (body_height + head_height), 0.f));
    model = trs * rot * scale;
    model = trs2 * rot2 * trs1 * model;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    cannon2.Draw(shaderProgramID);
}

void Crane::init_buffer()
{
    body.model_init_buffer();
    frontLeg1.model_init_buffer();
    frontLeg2.model_init_buffer();
    head.model_init_buffer();
    cannon1.model_init_buffer();
    cannon2.model_init_buffer();
}


