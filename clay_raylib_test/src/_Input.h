#pragma once
#include "raylib.h"

class Input {
public:
    Input() = default;
    ~Input() = default;

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(Input&&) = delete;

    bool IsKeyPressed(int key);       
bool IsKeyPressedRepeat(int key); 
bool IsKeyDown(int key);          
bool IsKeyReleased(int key);      
bool IsKeyUp(int key);            
int GetKeyPressed(void);          
int GetCharPressed(void);         
const char *GetKeyName(int key);  
void SetExitKey(int key);         

};