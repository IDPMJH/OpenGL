// SceneManager.h
#pragma once
#include <vector>
#include "GLU_Object.h"

class SceneManager {
public:
    void AddObject(GLU_Object* object);
    void RemoveObject(GLU_Object* object);
    void DrawObjects(GLuint shaderProgramID);

private:
    std::vector<GLU_Object*> objects;
};

// SceneManager.cpp
#include "SceneManager.h"

void SceneManager::AddObject(GLU_Object* object) {
    objects.push_back(object);
}

void SceneManager::RemoveObject(GLU_Object* object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

void SceneManager::DrawObjects(GLuint shaderProgramID) {
    for (auto& object : objects) {
        object->draw(GLU_FILL);
    }
}
