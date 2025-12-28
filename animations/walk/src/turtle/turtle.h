#ifndef __TURTLE_H_
#define __TURTLE_H_

#include <stdint.h>

typedef struct {
    float x;
    float y;
    float angle;
    uint8_t color;
    bool pen;
    bool wrap;
} Turtle;

void Turtle_Init(Turtle* t);
void Turtle_Forward(Turtle* t, float amount);
void Turtle_Right(Turtle* t, float angle);
void Turtle_Left(Turtle* t, float angle);
void Turtle_SetAngle(Turtle* t, float angle);
void Turtle_Goto(Turtle* t, float x, float y);
void Turtle_Teleport(Turtle* t, float x, float y);
void Turtle_SetPen(Turtle* t, bool pen);
void Turtle_PenUp(Turtle* t);
void Turtle_PenDown(Turtle* t);
void Turtle_SetColor(Turtle* t, uint8_t color);
void Turtle_SetWrap(Turtle* t, bool wrap);

#endif
