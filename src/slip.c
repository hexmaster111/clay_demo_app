#include "clay.h"
#include "slip.h"
#include "raylib.h"
#include "raymath.h"
#include <string.h>

Clay_CornerRadius g_ui_slider_corrner_raidus = {10, 10, 10, 10};


void SliderThumb_OnHover(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData)
{
    float *value = (float *)userData;
}

float ScailFloat(float value, float from_min, float from_max, float to_min, float to_max)
{
    if (from_max == from_min)
    {
        return to_min; // div 0
    }

    return (value - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}

 Clay_String ClayStringFromCString(const char *str) { return (Clay_String){.chars = str, .length = strlen(str)}; }


extern Clay__ScrollContainerDataInternalArray Clay__scrollContainerDatas;

Clay__ScrollContainerDataInternal *FindScrollContainerDataInternal(Clay_ElementId id)
{
    Clay__ScrollContainerDataInternal *find = NULL;
    for (int sidx = 0; sidx < Clay__scrollContainerDatas.length; sidx++)
    {
        find = Clay__ScrollContainerDataInternalArray_Get(
            &Clay__scrollContainerDatas,
            sidx);

        if (find->elementId == id.id)
        {
            break;
        }

        find = NULL;
    }

    return find;
}


void Slider(float *value, float min, float max, Clay_String label)
{

    float zero_to_one_pos = Clamp(ScailFloat(*value, min, max, 0.0f, 1.0f), 0, 1);

    Clay_ElementId sliderId = Clay__HashString(label, 0, 0);
    Clay_ElementId thumbId = Clay__HashString(label, 1, 0);
    Clay_ElementId trackLeftId = Clay__HashString(label, 2, 0);
    Clay_ElementId trackRightId = Clay__HashString(label, 3, 0);

    CLAY(
        // CLAY_ID("SLIDER"),
        Clay__AttachId(sliderId),
        CLAY_SCROLL((Clay_ScrollElementConfig){.horizontal = true}),
        CLAY_LAYOUT((Clay_LayoutConfig){
            .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
        })

        // CLAY_RECTANGLE((Clay_RectangleElementConfig){.color = COLOR_WHITE})
    )
    {

        if (Clay_Hovered() && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Clay_ScrollContainerData track = Clay_GetScrollContainerData(sliderId);
            Clay_ScrollContainerData thumb = Clay_GetScrollContainerData(thumbId);
            Clay__ScrollContainerDataInternal *track_internal_data = FindScrollContainerDataInternal(sliderId);
            Clay__ScrollContainerDataInternal *thumb_internal_data = FindScrollContainerDataInternal(thumbId);

            if (track.found && thumb.found && track_internal_data && thumb_internal_data)
            {
                // put the track where the users currsor is clicking on the track

                Vector2 mousePos = GetMousePosition();
                Vector2 trackPos = (Vector2){track_internal_data->boundingBox.x, track_internal_data->boundingBox.y};

                Vector2 mouseRelitiveToTrack = Vector2Subtract(mousePos, trackPos);
                Vector2 trackSize = (Vector2){track_internal_data->boundingBox.width, track_internal_data->boundingBox.height};

                Vector2 click_percent = (Vector2){
                    .x = ScailFloat(mouseRelitiveToTrack.x - thumb_internal_data->boundingBox.width * 0.5f, 0, trackSize.x - thumb_internal_data->boundingBox.width * 0.5f, 0, 1),
                    .y = ScailFloat(mouseRelitiveToTrack.y - thumb_internal_data->boundingBox.height * 0.5f, 0, trackSize.y - thumb_internal_data->boundingBox.height * 0.5f, 0, 1)};

                *value = Clamp(ScailFloat(click_percent.x, 0, 1, min, max), min, max);
            }
        }

        CLAY(Clay__AttachId(trackLeftId),
             // CLAY_ID("TRACKL"),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = COLOR_BLACK, .cornerRadius = g_ui_slider_corrner_raidus}),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .sizing = {.width = CLAY_SIZING_PERCENT(zero_to_one_pos), .height = CLAY_SIZING_GROW()}}))
        {
        }
        /*  THUMB   */
        CLAY(Clay__AttachId(thumbId),
             CLAY_SCROLL((Clay_ScrollElementConfig){.horizontal = true}),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .sizing = {.width = CLAY_SIZING_FIXED(64), .height = CLAY_SIZING_GROW()}}),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = Clay_Hovered() && IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? COLOR_BLUE : COLOR_WHITE,
                 .cornerRadius = g_ui_slider_corrner_raidus}),
             Clay_OnHover(SliderThumb_OnHover, (intptr_t)value))
        {
            /* Thumb Content */
        }

        CLAY(Clay__AttachId(trackRightId),
             // CLAY_ID("TRACKR"),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = COLOR_BLACK,
                 .cornerRadius = g_ui_slider_corrner_raidus}),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}))
        {
        }
    }
}
