#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "../clay_renderer_raylib.c"

const uint32_t FONT_ID_BODY_24 = 0;
const uint32_t FONT_ID_BODY_16 = 1;

#define COLOR_ORANGE \
    (Clay_Color) { 225, 138, 50, 255 }
#define COLOR_BLUE \
    (Clay_Color) { 111, 173, 162, 255 }
#define COLOR_BLACK \
    (Clay_Color) { 0, 0, 0, 255 }

#define COLOR_WHITE \
    (Clay_Color) { 255, 255, 255, 255 }

#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) \
    (Clay_Vector2) { .x = vector.x, .y = vector.y }

Clay_String profileText = CLAY_STRING("Profile Page one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen");
Clay_TextElementConfig headerTextConfig = (Clay_TextElementConfig){.fontId = 1, .fontSize = 16, .textColor = {0, 0, 0, 255}};

Clay_CornerRadius cornerraidus = {10, 10, 10, 10};

int g_selectedListItem = 0;
int g_selectedHeader = 0;
char g_sliderValueTextBuffer[100] = {0};

/* NULL TERMINATED LIST */
Clay_String g_itemnames[] = {
    CLAY_STRING("SOmething"),
    CLAY_STRING("OtherTHings"),
    CLAY_STRING("A REALLY REALLY REALLY LONG NAME"),
    CLAY_STRING("some"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("Obj"),
    CLAY_STRING("LAST"),
    {.chars = NULL, .length = 0},
};

void HandleHeaderButtonInteraction_OnHover(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData)
{
    if (pointerData.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME)
    {
        g_selectedHeader = (int)userData;
    }
}

Clay_Color GetHeaderColor(int idx)
{
    Clay_Color c;

    if (g_selectedHeader == idx)
        c = COLOR_WHITE;
    else if (Clay_Hovered())
        c = COLOR_BLUE;
    else
        c = COLOR_ORANGE;
    return c;
}

// Examples of re-usable "Components"
void RenderHeaderButton(Clay_String text, int idx)
{
    CLAY(CLAY_LAYOUT({.padding = {16, 8}}),
         CLAY_RECTANGLE((Clay_RectangleElementConfig){
             .color = GetHeaderColor(idx),
             .cornerRadius = cornerraidus}),
         Clay_OnHover(HandleHeaderButtonInteraction_OnHover, idx))
    {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG(headerTextConfig));
    }
}

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

Clay_String ClayStringFromCString(const char *str)
{
    return (Clay_String){
        .chars = str,
        .length = strlen(str)};
}

float PercentChangeF(float old_value, float new_value)
{
    if (old_value == 0)
    {
        return (new_value == 0) ? 0.0f : INFINITY;
    }
    return ((new_value - old_value) / old_value) * 100.0f;
}

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
            .sizing = {.width = CLAY_SIZING_GROW(), .height = {.type = CLAY__SIZING_TYPE_FIXED, .sizeMinMax = {.min = 32, .max = 32}}},
            .padding.x = 0,
            .childAlignment = {.x = CLAY_ALIGN_X_LEFT}})

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
                    .x = ScailFloat(mouseRelitiveToTrack.x - thumb_internal_data->boundingBox.width * 0.5f, 0, trackSize.x, 0, 1),
                    .y = ScailFloat(mouseRelitiveToTrack.y - thumb_internal_data->boundingBox.height * 0.5f, 0, trackSize.y, 0, 1)};

                *value = Clamp(ScailFloat(click_percent.x, 0, 1, min, max), min, max);
            }
        }

        CLAY(
            Clay__AttachId(trackLeftId),
            // CLAY_ID("TRACKL"),
            CLAY_RECTANGLE((Clay_RectangleElementConfig){
                .color = COLOR_BLACK, .cornerRadius = cornerraidus}),
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
                 .cornerRadius = cornerraidus}),
             Clay_OnHover(SliderThumb_OnHover, (intptr_t)value))
        {
            /* Thumb Content */
        }

        CLAY(Clay__AttachId(trackRightId),
             // CLAY_ID("TRACKR"),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = COLOR_BLACK,
                 .cornerRadius = cornerraidus}),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}))
        {
        }
    }
}

Clay_LayoutConfig dropdownTextItemLayout = (Clay_LayoutConfig){.padding = {8, 4}};
Clay_RectangleElementConfig dropdownRectangleConfig = (Clay_RectangleElementConfig){.color = {180, 180, 180, 255}};
Clay_TextElementConfig dropdownTextElementConfig = (Clay_TextElementConfig){.fontSize = 24, .textColor = {255, 255, 255, 255}};

void RenderDropdownTextItem(int index)
{
    CLAY(CLAY_IDI("ScrollContainerItem", index), CLAY_LAYOUT(dropdownTextItemLayout), CLAY_RECTANGLE(dropdownRectangleConfig))
    {
        CLAY_TEXT(CLAY_STRING("I'm a text field in a scroll container."), &dropdownTextElementConfig);
    }
}

void LeftSideBarItemSelected_OnHover(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData)
{
    if (pointerInfo.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME)
    {
        TraceLog(LOG_INFO, TextFormat("Pressed %d", (int)userData));
        g_selectedListItem = (int)userData;
    }
}

Clay_Color GetListBoxItemBackgroundColor(int idx)
{
    if (idx == g_selectedListItem)
        return (Clay_Color){0, 255, 0, 255};
    if (Clay_Hovered())
        return (Clay_Color){0, 128, 0, 255};
    return (Clay_Color){150, 150, 255, 255};
}

Clay_RenderCommandArray CreateLayout()
{

    Clay_BeginLayout();
    CLAY(CLAY_ID("OuterContainer"),
         CLAY_LAYOUT((Clay_LayoutConfig){
             .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
             .padding = {16, 16},
             .childGap = 16}),
         CLAY_RECTANGLE((Clay_RectangleElementConfig){
             .color = {200, 200, 200, 255}}))
    {
        CLAY(CLAY_ID("LeftSideBar"),
             CLAY_LAYOUT((Clay_LayoutConfig){.layoutDirection = CLAY_TOP_TO_BOTTOM,
                                             .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()},
                                             .padding = {16, 16},
                                             .childGap = 16}),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){
                 .color = {150, 150, 255, 255},
                 .cornerRadius = cornerraidus}))
        {
            CLAY(CLAY_ID("ListOfObjects"),
                 CLAY_SCROLL((Clay_ScrollElementConfig){.vertical = true}),
                 CLAY_LAYOUT((Clay_LayoutConfig){
                     .layoutDirection = CLAY_TOP_TO_BOTTOM,
                     .childGap = 5,
                     .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}))
            {

                Clay_String c = g_itemnames[0];
                int i = 0;
                while (c.chars != NULL)
                {

                    CLAY(CLAY_LAYOUT((Clay_LayoutConfig){
                             .sizing = {.width = CLAY_SIZING_GROW()},
                             .childAlignment = {.x = CLAY_ALIGN_X_LEFT}}),
                         CLAY_RECTANGLE((Clay_RectangleElementConfig){
                             .color = GetListBoxItemBackgroundColor(i),
                             .cornerRadius = cornerraidus}),
                         Clay_OnHover(LeftSideBarItemSelected_OnHover, i))
                    {
                        Clay_TextElementConfig cfg = {
                            .fontId = FONT_ID_BODY_16,
                            .fontSize = 16,
                            .textColor = COLOR_BLACK};

                        CLAY_TEXT(g_itemnames[i], CLAY_TEXT_CONFIG(cfg));
                    }

                    i += 1;
                    c = g_itemnames[i];
                }
            }
        }

        CLAY(CLAY_ID("RightPanel"),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .layoutDirection = CLAY_TOP_TO_BOTTOM,
                 .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                 .childGap = 16}))
        {
            CLAY(CLAY_ID("HeaderBar"),
                 CLAY_LAYOUT((Clay_LayoutConfig){
                     .sizing = {.width = CLAY_SIZING_GROW()},
                     .childAlignment = {.x = CLAY_ALIGN_X_LEFT},
                     .padding = {8, 8},
                     .childGap = 8}),
                 CLAY_RECTANGLE((Clay_RectangleElementConfig){
                     .color = {180, 180, 180, 255},
                     .cornerRadius = cornerraidus}))
            {
                RenderHeaderButton(CLAY_STRING("Header Item 1"), 0);
                RenderHeaderButton(CLAY_STRING("Header Item 2"), 1);
                RenderHeaderButton(CLAY_STRING("Header Item 3"), 2);
            }
            CLAY(CLAY_ID("MainContent"),
                 //  CLAY_SCROLL({.vertical = true}),
                 CLAY_LAYOUT((Clay_LayoutConfig){
                     .layoutDirection = CLAY_TOP_TO_BOTTOM,
                     .padding = {16, 16},
                     .childGap = 16,
                     .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}}),
                 CLAY_RECTANGLE((Clay_RectangleElementConfig){
                     .color = {200, 200, 255, 255},
                     .cornerRadius = cornerraidus}))
            {
                static float v = 0;
                const float min = 0, max = 100;
                Clay_TextElementConfig te = {.fontId = FONT_ID_BODY_16, .fontSize = 24, .textColor = COLOR_ORANGE};

                CLAY(CLAY_ID("SliderDemo"),
                     CLAY_LAYOUT((Clay_LayoutConfig){
                         .layoutDirection = CLAY_TOP_TO_BOTTOM,
                         .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}}))
                {

                    float d = (-1 * v) + 100;
                    snprintf(g_sliderValueTextBuffer, sizeof(g_sliderValueTextBuffer),
                             "%.2f, %.2f", v, d);

                    CLAY_TEXT(ClayStringFromCString(g_sliderValueTextBuffer), CLAY_TEXT_CONFIG(te));

                    CLAY(CLAY_LAYOUT({.padding = {5, 5}, .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}}))
                    {
                        Slider(&v, min, max, CLAY_STRING("SLIDER1"));
                    }
                    Slider(&d, min, max, CLAY_STRING("SLIDER2"));
                }
            }
        }

        Clay_ScrollContainerData scrollData = Clay_GetScrollContainerData(Clay_GetElementId(CLAY_STRING("MainContent")));
        if (scrollData.found)
        {
            CLAY(CLAY_ID("ScrollBar"),
                 CLAY_FLOATING((Clay_FloatingElementConfig){
                     .offset = {.y = -(scrollData.scrollPosition->y / scrollData.contentDimensions.height) * scrollData.scrollContainerDimensions.height},
                     .zIndex = 1,
                     .parentId = Clay_GetElementId(CLAY_STRING("MainContent")).id,
                     .attachment = {.element = CLAY_ATTACH_POINT_RIGHT_TOP, .parent = CLAY_ATTACH_POINT_RIGHT_TOP}}))
            {
                CLAY(CLAY_ID("ScrollBarButton"),
                     CLAY_LAYOUT((Clay_LayoutConfig){.sizing = {CLAY_SIZING_FIXED(12), CLAY_SIZING_FIXED((scrollData.scrollContainerDimensions.height / scrollData.contentDimensions.height) * scrollData.scrollContainerDimensions.height)}}),
                     CLAY_RECTANGLE((Clay_RectangleElementConfig){
                         .cornerRadius = {6},
                         .color = Clay_PointerOver(Clay__HashString(CLAY_STRING("ScrollBar"), 0, 0))
                                      ? (Clay_Color){100, 100, 140, 150}
                                      : (Clay_Color){120, 120, 160, 150}}))
                {
                    /*scroll bar button content*/
                }
            }
        }
    }
    return Clay_EndLayout();
}

typedef struct
{
    Clay_Vector2 clickOrigin;
    Clay_Vector2 positionOrigin;
    bool mouseDown;
} ScrollbarData;

ScrollbarData scrollbarData = (ScrollbarData){};

bool debugEnabled = false;

int main(void)
{
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena){.label = CLAY_STRING("Clay Memory Arena"), .memory = malloc(totalMemorySize), .capacity = totalMemorySize};
    Clay_SetMeasureTextFunction(Raylib_MeasureText);
    Clay_Initialize(clayMemory, (Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()});
    Clay_Raylib_Initialize(1024, 768, "Clay - Raylib Renderer Example", FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    Raylib_fonts[FONT_ID_BODY_24] = (Raylib_Font){
        .font = LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400),
        .fontId = FONT_ID_BODY_24,
    };
    SetTextureFilter(Raylib_fonts[FONT_ID_BODY_24].font.texture, TEXTURE_FILTER_BILINEAR);

    Raylib_fonts[FONT_ID_BODY_16] = (Raylib_Font){
        .font = LoadFontEx("resources/Roboto-Regular.ttf", 32, 0, 400),
        .fontId = FONT_ID_BODY_16,
    };
    SetTextureFilter(Raylib_fonts[FONT_ID_BODY_16].font.texture, TEXTURE_FILTER_BILINEAR);

    //--------------------------------------------------------------------------------------
    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        Vector2 mouseWheelDelta = GetMouseWheelMoveV();
        float mouseWheelX = mouseWheelDelta.x;
        float mouseWheelY = mouseWheelDelta.y;

        if (IsKeyPressed(KEY_D))
        {
            debugEnabled = !debugEnabled;
            Clay_SetDebugModeEnabled(debugEnabled);
        }
        //----------------------------------------------------------------------------------
        // Handle scroll containers
        Clay_Vector2 mousePosition = RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition());
        Clay_SetPointerState(mousePosition, IsMouseButtonDown(0) && !scrollbarData.mouseDown);
        Clay_SetLayoutDimensions((Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()});
        if (!IsMouseButtonDown(0))
        {
            scrollbarData.mouseDown = false;
        }

        if (IsMouseButtonDown(0) && !scrollbarData.mouseDown && Clay_PointerOver(Clay__HashString(CLAY_STRING("ScrollBar"), 0, 0)))
        {
            Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0, 0));
            scrollbarData.clickOrigin = mousePosition;
            scrollbarData.positionOrigin = *scrollContainerData.scrollPosition;
            scrollbarData.mouseDown = true;
        }
        else if (scrollbarData.mouseDown)
        {
            Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0, 0));
            if (scrollContainerData.contentDimensions.height > 0)
            {
                Clay_Vector2 ratio = (Clay_Vector2){
                    scrollContainerData.contentDimensions.width / scrollContainerData.scrollContainerDimensions.width,
                    scrollContainerData.contentDimensions.height / scrollContainerData.scrollContainerDimensions.height,
                };
                if (scrollContainerData.config.vertical)
                {
                    scrollContainerData.scrollPosition->y = scrollbarData.positionOrigin.y + (scrollbarData.clickOrigin.y - mousePosition.y) * ratio.y;
                }
                if (scrollContainerData.config.horizontal)
                {
                    scrollContainerData.scrollPosition->x = scrollbarData.positionOrigin.x + (scrollbarData.clickOrigin.x - mousePosition.x) * ratio.x;
                }
            }
        }

        Clay_UpdateScrollContainers(true, (Clay_Vector2){mouseWheelX, mouseWheelY}, GetFrameTime());
        // Generate the auto layout for rendering
        // double currentTime = GetTime();
        Clay_RenderCommandArray renderCommands = CreateLayout();
        // printf("layout time: %f microseconds\n", (GetTime() - currentTime) * 1000 * 1000);
        // RENDERING ---------------------------------
        //    currentTime = GetTime();
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands);
        EndDrawing();
        //    printf("render time: %f ms\n", (GetTime() - currentTime) * 1000);

        //----------------------------------------------------------------------------------
    }
    return 0;
}
