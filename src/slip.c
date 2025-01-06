#include "clay.h"
#include "slip.h"
#include "raylib.h"
#include "raymath.h"
#include <string.h>

Clay_CornerRadius g_ui_slider_corrner_raidus = {10, 10, 10, 10};
Clay_CornerRadius g_ui_checkbox_corrner_raidus = {10, 10, 10, 10};

float ScailFloat(float value, float from_min, float from_max, float to_min, float to_max)
{
    if (from_max == from_min)
    {
        return to_min; // div 0
    }

    return (value - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}

Clay_String ClayStringFromCString(const char *str) { return (Clay_String){.chars = str, .length = strlen(str)}; }

void CheckBox(bool *isChecked, Clay_String id)
{
    Clay_ElementId checkBoxId = Clay__HashString(id, 0, 0);
    Clay_ElementId checkId = Clay__HashString(id, 1, 0);

    CLAY(Clay__AttachId(checkBoxId),
         CLAY_LAYOUT((Clay_LayoutConfig){
             .padding = {5, 5},
             .sizing = {.height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW()},
             .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}),
         CLAY_RECTANGLE((Clay_RectangleElementConfig){
             .color = COLOR_GRAY, .cornerRadius = g_ui_checkbox_corrner_raidus}))
    {
        CLAY(Clay__AttachId(checkId),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = *isChecked ? COLOR_GREEN : COLOR_RED,
                 .cornerRadius = g_ui_checkbox_corrner_raidus}))
        {
            if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                *isChecked = !*isChecked;
            }
        }
    }
}

void Slider(float *value, float min, float max, Clay_String id)
{

    const float fixed_thumb_size = 64.0f;

    float zero_to_one_pos = Clamp(ScailFloat(*value, min, max, 0.0f, 1.0f), 0, 1);

    Clay_ElementId sliderId = Clay__HashString(id, 0, 0);
    Clay_ElementId thumbId = Clay__HashString(id, 1, 0);
    Clay_ElementId trackLeftId = Clay__HashString(id, 2, 0);
    Clay_ElementId trackRightId = Clay__HashString(id, 3, 0);

    CLAY(Clay__AttachId(sliderId),
         CLAY_LAYOUT((Clay_LayoutConfig){
             .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
         }))
    {

        if (Clay_Hovered() && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {

            Clay_ElementLocationData trackpos = Clay_GetElementLocationData(sliderId);
            Clay_ElementLocationData thumbpos = Clay_GetElementLocationData(thumbId);

            if (!trackpos.found || !thumbpos.found)
            {
                TraceLog(LOG_ERROR, "COULD NOT FIND SLIDER TRACK PART");
                return;
            }

            Vector2 mouse = GetMousePosition();
            Clay_BoundingBox track = trackpos.elementLocation;
            Clay_BoundingBox thumb = thumbpos.elementLocation;

            Vector2 click_relitive_to_track = Vector2Subtract(mouse, (Vector2){track.x, track.y});

            Vector2 click_zero_one = (Vector2){
                .x = ScailFloat(track.width - click_relitive_to_track.x, 0, track.width, 0, 1),
                .y = ScailFloat(track.height - click_relitive_to_track.y, 0, track.height, 0, 1)};

            // TraceLog(LOG_INFO, TextFormat("%.2f, %.2f", click_relitive_to_track.x, click_relitive_to_track.y));

            // this scailfloat call also inverts our value
            *value = Clamp(ScailFloat(click_zero_one.x, 1, 0, min, max), min, max);
        }

        CLAY(Clay__AttachId(trackLeftId),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = COLOR_BLACK, .cornerRadius = g_ui_slider_corrner_raidus}),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .sizing = {.width = CLAY_SIZING_PERCENT(zero_to_one_pos), .height = CLAY_SIZING_GROW()}}))
        {
        }
        /*  THUMB   */
        CLAY(Clay__AttachId(thumbId),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .padding = {.x = fixed_thumb_size},
                 .sizing = {.width = CLAY_SIZING_FIXED(fixed_thumb_size), .height = CLAY_SIZING_GROW()}}),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = Clay_Hovered() && IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? COLOR_BLUE : COLOR_WHITE,
                 .cornerRadius = g_ui_slider_corrner_raidus}))
        {
            /* Thumb Content */
        }

        CLAY(Clay__AttachId(trackRightId),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = COLOR_BLACK,
                 .cornerRadius = g_ui_slider_corrner_raidus}),
             CLAY_LAYOUT((Clay_LayoutConfig){.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}))
        {
        }
    }
}


void TextEditor(char *content, Clay_TextElementConfig tec)
{
    // --- STATE UPDATE ---

    // --- RENDER ---

    CLAY_TEXT(ClayStringFromCString(content), &tec);
}