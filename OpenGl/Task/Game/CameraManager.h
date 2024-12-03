


class KeyManger;

class CameraManager {
private:
   

public:
    CameraManager()
        : position(0.0f, 0.0f, 5.0f), 
          target(0.0f, 0.0f, 0.0f), 
          up(0.0f, 1.0f, 0.0f), 
          v(0.0f, 1.0f, 0.0f),
          speed(0.5f), 
          rotationSpeed(5.f), 
          perspective(true), 
          fov(45.0f), 
          aspectRatio(4.0f / 3.0f), 
          nearPlane(0.1f), 
          farPlane(100.0f) ,
          yaw(-90.0f), // 초기 yaw 값
          pitch(0.0f), // 초기 pitch 값
          sensitivity(0.1f) 
    {
    } 

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 getProjectionMatrix() const {
        if (perspective) {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        } else {
            return glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, nearPlane, farPlane);
        }
    }

    void update(KeyManager& keyManager, float deltaTime) {

        if (keyManager.IsKeyPressed('w')) {
            moveForward(deltaTime);
        }
        if (keyManager.IsKeyPressed('s')) {
            moveBackward(deltaTime);
        }
        if (keyManager.IsKeyPressed('a')) {
            moveLeft(deltaTime);
        }
        if (keyManager.IsKeyPressed('d')) {
            moveRight(deltaTime);
        }
        if (keyManager.IsKeyPressed('q')) {
            rotateLeft(deltaTime);
        }
        if (keyManager.IsKeyPressed('e')) {
            rotateRight(deltaTime);
        }
        if (keyManager.IsKeyPressed('p')) {
            setPerspective(true);
        }
        if (keyManager.IsKeyPressed('o')) {
            setPerspective(false);
        }
    }
    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // 제한 각도 설정
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        direction = glm::normalize(direction);

        u = glm::normalize(glm::cross(direction, up));
        v = glm::normalize(glm::cross(u, direction));
        target = position + direction;
    }

    void moveForward(float deltaTime) {
        glm::vec3 direction = glm::normalize(target - position);
        position += direction * speed * deltaTime;
        target += direction * speed * deltaTime;
    }

    void moveBackward(float deltaTime) {
        glm::vec3 direction = glm::normalize(target - position);
        position -= direction * speed * deltaTime;
        target -= direction * speed * deltaTime;
    }

    void moveLeft(float deltaTime) {
        glm::vec3 direction = glm::normalize(glm::cross(up, target - position));
        position += direction * speed * deltaTime;
        target += direction * speed * deltaTime;
    }

    void moveRight(float deltaTime) {
        glm::vec3 direction = glm::normalize(glm::cross(up, target - position));
        position -= direction * speed * deltaTime;
        target -= direction * speed * deltaTime;
    }

    void rotateLeft(float deltaTime) {
        float angle = rotationSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), up);
        position = glm::vec3(rotation * glm::vec4(position - target, 1.0f)) + target;
    }

    void rotateRight(float deltaTime) {
        float angle = -rotationSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), up);
        position = glm::vec3(rotation * glm::vec4(position - target, 1.0f)) + target;
    }

    void setPerspective(bool isPerspective) {
        perspective = isPerspective;
    }

    void setAspectRatio(float ratio) {
        aspectRatio = ratio;
    }

    void setFOV(float fieldOfView) {
        fov = fieldOfView;
    }

    void setNearPlane(float nnear) {
        nearPlane = nnear;
    }

    void setFarPlane(float ffar) {
        farPlane = ffar;
    }

private:
  
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 u; // Right vector
    glm::vec3 v; // Up vector
    glm::vec3 n; // Front vector
    float speed;
    float rotationSpeed;
    bool perspective;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float yaw;
    float pitch;
    float sensitivity;
};
