#pragma once

extern void InitLogFile(const char* fileName);
extern void CloseLogFile();

extern bool IsEngineRun;

class InputSystem;
extern InputSystem* gInputSystem;
class WindowSystem;
extern WindowSystem* gWindowSystem;

extern float GetWindowAspect();
extern int GetWindowWidth();
extern int GetWindowHeight();