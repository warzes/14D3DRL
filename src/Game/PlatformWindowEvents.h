#pragma once

void WindowEventIconified();
void WindowEventRestored();
void WindowEventFocused();
void WindowEventUnFocused();
void WindowEventSize(int width, int height);

void MouseEventEnter();
void MouseEventLeave();
void MouseEventMove(int posX, int posY);