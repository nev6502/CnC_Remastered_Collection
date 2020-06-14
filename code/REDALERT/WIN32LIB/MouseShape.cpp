/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Low level sprite drawing functions for Dune2 sprite format (used for mouse in early C&C games).
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "../FUNCTION.H"
#include <stdint.h>
#include "mouse.h"
#include "mouseshape.h"
#include "gbuffer.h"
//#include "lcw.h"
#include <cstring>
#include <algorithm>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

using std::memcpy;

uint8_t g_mouseShapeBuffer[65000]; // Original code shares the global ShapeBuffer with other sprite drawing.

int Get_Shape_Uncomp_Size(void* shape)
{
    return static_cast<MouseShapeFrameHeader*>(shape)->data_length;
}

int Extract_Shape_Count(void* shape)
{
    return static_cast<MouseShapeHeader*>(shape)->frame_count;
}

void* Extract_Shape(void* shape, int frame)
{
    char* data = static_cast<char*>(shape);
    MouseShapeHeader* header = static_cast<MouseShapeHeader*>(shape);

    if (shape && frame >= 0 && header->frame_count > frame) {
        return data + (&header->offset)[frame] + 2;
    }

    return nullptr;
}

int Get_Shape_Width(void* shape)
{
    // return static_cast<ShapeHeaderStruct*>(shape)->Width;
    return static_cast<MouseShapeFrameHeader*>(shape)->width;
}

int Get_Shape_Height(void* shape)
{
    return static_cast<MouseShapeFrameHeader*>(shape)->height;
}

//int Restore_Shape_Height(void* shape)
//{
//    int oldheight = static_cast<MouseShapeFrameHeader*>(shape)->height;
//    static_cast<MouseShapeFrameHeader*>(shape)->height = static_cast<MouseShapeFrameHeader*>(shape)->original_height;
//    return oldheight;
//}

int Get_Shape_Original_Height(void* shape)
{
    return static_cast<MouseShapeFrameHeader*>(shape)->original_height;
}

void __cdecl Mouse_Shadow_Buffer(WWMouseClass& mouse, GraphicViewPortClass& viewport, void* buffer, int x_pos, int y_pos, int hspot_x, int hspot_y, BOOL save)
{
   
}

void __cdecl Mouse_Draw(WWMouseClass& mouse, GraphicViewPortClass& viewport, int x_pos, int y_pos)
{
  
}

void* __cdecl Mouse_Set_Cursor(WWMouseClass& mouse, int hspot_x, int hspot_y, void* frame)
{
    return NULL;
}
