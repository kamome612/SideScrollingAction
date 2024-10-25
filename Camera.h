#pragma once
#include "Engine/GameObject.h"
class Camera :
    public GameObject
{
public:
    Camera(GameObject* parent) :GameObject(parent) { valueX = 0, valueY = 0; }
    ~Camera() {}

    void SetValueXY(int x, int y) { valueX = x, valueY = y; }
    void SetValueX(int x) { valueX = x;}
    void SetValueY(int y) { valueY = y;}
    int GetValueX() { return valueX; }
    int GetValueY() { return valueY; }
private:
    int valueX;
    int valueY;
};

