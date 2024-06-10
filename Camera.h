#pragma once
#include "Engine/GameObject.h"
class Camera :
    public GameObject
{
public:
    Camera(GameObject* parent) :GameObject(parent) { value = 0; }
    ~Camera() {}

    void SetValue(int v) { value = v; }
    int GetValue() { return value; }
private:
    int value;
};

