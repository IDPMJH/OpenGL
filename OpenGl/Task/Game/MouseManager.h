#ifndef MOUSEMANAGER_H
#define MOUSEMANAGER_H

#include <unordered_map>

class MouseManager {
public:
    MouseManager() : lastX(0), lastY(0), firstMouse(true) {}

    void MouseDown(int button) {
        mouseStates[button] = true;
    }

    void MouseUp(int button) {
        mouseStates[button] = false;
    }

    bool IsButtonPressed(int button) const {
        auto it = mouseStates.find(button);
        if (it != mouseStates.end()) {
            return it->second;
        }
        return false;
    }

    void MouseMove(float xpos, float ypos, int windowWidth, int windowHeight) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        xoffset = xpos - lastX;
        yoffset = lastY - ypos; // y-coordinates are inverted
        lastX = xpos;
        lastY = ypos;

        // Convert to OpenGL coordinates
        xpos = (2.0f * xpos) / windowWidth - 1.0f;
        ypos = 1.0f - (2.0f * ypos) / windowHeight;

        currentX = xpos;
        currentY = ypos;
    }

    float GetXOffset() const {
        return xoffset;
    }

    float GetYOffset() const {
        return yoffset;
    }

    float GetCurrentX() const {
        return currentX;
    }

    float GetCurrentY() const {
        return currentY;
    }

private:
    std::unordered_map<int, bool> mouseStates;
    float lastX, lastY;
    float xoffset, yoffset;
    float currentX, currentY;
    bool firstMouse;
};

#endif // MOUSEMANAGER_H
