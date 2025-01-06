#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "slip.h"
#include "../clay_renderer_raylib.c"

const uint32_t FONT_ID_BODY_24 = 0;
const uint32_t FONT_ID_BODY_16 = 1;

#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) \
    (Clay_Vector2) { .x = vector.x, .y = vector.y }

Clay_String profileText = CLAY_STRING("Profile Page one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen");
Clay_TextElementConfig headerTextConfig = (Clay_TextElementConfig){.fontId = 1, .fontSize = 16, .textColor = {0, 0, 0, 255}};

Clay_CornerRadius cornerraidus = {10, 10, 10, 10};

int g_selectedListItem = -1;
int g_selectedHeader = 2;
char g_sliderValueTextBuffer[100] = {0};

typedef struct
{
    Clay_String partname;
    Rectangle visuals[15];
    int visualparts;

    bool isplaced;
    Vector2 position;
} InvItem;

InvItem g_player_inv[] = {
    {
        .partname = CLAY_STRING("CORE"),
        .isplaced = true,
        .position = {0, 0},
        .visualparts = 2,
        .visuals = {
            {0.0, 0.0, 5.0, 5.0},
            {2.5, 2.5, 2.5, 2.5},
        },
    },

    {
        .partname = CLAY_STRING("BOOST"),
        .isplaced = false,
        .position = {0, 0},
        .visualparts = 2,
        .visuals = {
            {0.0, 0.0, 10.0, 5.0},
            {2.5, 2.5, 2.5, 2.5},
        },
    },

    {
        .partname = CLAY_STRING("HULL HEALTH"),
        .isplaced = false,
        .position = {0, 0},
        .visualparts = 1,
        .visuals = {
            {0.0, 0.0, 10.0, 10.0},
        },
    },

    // NULL used to find end of list
    {.isplaced = false, .partname = {.chars = NULL, .length = 0}},
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

void RenderMenuButton(Clay_String text,
                      void (*onHover)(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData),
                      intptr_t userdata,
                      Clay_Color (*colorGet)(intptr_t userdata))
{
    CLAY(CLAY_LAYOUT({.padding = {16, 8}}),
         CLAY_RECTANGLE((Clay_RectangleElementConfig){
             .color = colorGet(userdata),
             .cornerRadius = cornerraidus}),
         Clay_OnHover(onHover, userdata))
    {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG(headerTextConfig));
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
        if (g_selectedListItem == (int)userData)
        {
            g_selectedListItem = -1;
            return;
        }

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

Clay_Color GetTextEditorMenuBarButtonColor(intptr_t ud)
{
    if (Clay_Hovered() && !IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        return COLOR_BLUE;
    }
    else if (Clay_Hovered())
    {
        return COLOR_SKYBLUE;
    }
    else
    {
        return COLOR_ORANGE;
    }
}

void PartBuilderDemo()
{
}

void File_OnHover(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData)
{
}

void Edit_OnHover(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData)
{
}

void TextEditorDemo()
{
    static char *text = NULL;

    if (text == NULL)
    {
        text = calloc(1, 1024); // this IS a mem leak
        TraceLog(LOG_INFO, "ALLOC TEXT DEMO");

        memcpy(text, "hello world!", 13);
    }

    CLAY(CLAY_ID("TextEditorDemo"),
         CLAY_LAYOUT((Clay_LayoutConfig){
             .childAlignment = {.x = CLAY_ALIGN_X_LEFT},
             .layoutDirection = CLAY_TOP_TO_BOTTOM,
             .sizing = {.height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW()}}))
    {
        CLAY(CLAY_ID("TextEditorDemoToolBar"),
             CLAY_LAYOUT((Clay_LayoutConfig){.sizing = {.height = CLAY_SIZING_FIXED(64), .width = CLAY_SIZING_GROW()}, .childGap = 8}),
             CLAY_RECTANGLE((Clay_RectangleElementConfig){.color = {180, 180, 180, 255}, .cornerRadius = cornerraidus}))
        {
            RenderMenuButton(CLAY_STRING("File"), File_OnHover, 0, GetTextEditorMenuBarButtonColor);
            RenderMenuButton(CLAY_STRING("Edit"), Edit_OnHover, 0, GetTextEditorMenuBarButtonColor);
        }
        
        Clay_TextElementConfig te = {.fontId = FONT_ID_BODY_16, .fontSize = 24, .textColor = COLOR_BLACK};

        CLAY(CLAY_ID("TextEditorHolder"), CLAY_LAYOUT((Clay_LayoutConfig){.sizing = {.height = CLAY_SIZING_GROW(), .width = CLAY_SIZING_GROW()}}))
        {
            TextEditor(text, te);
        }
    }
}

void LayoutDemo()
{
    static float fv = 0;
    static bool bc = false;

    const float min = 0, max = 100;
    Clay_TextElementConfig te = {.fontId = FONT_ID_BODY_16, .fontSize = 24, .textColor = COLOR_BLACK};

    CLAY(CLAY_ID("CheckBox Demo"),
         CLAY_LAYOUT((Clay_LayoutConfig){
             .sizing = {
                 .width = CLAY_SIZING_GROW(),
                 .height = CLAY_SIZING_FIXED(64)}}))
    {
        CLAY(CLAY_ID("Check Box Label"),
             CLAY_LAYOUT((Clay_LayoutConfig){
                 .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT()}}))
        {

            CLAY(CLAY_ID("Cb Label Expander"),
                 CLAY_LAYOUT((Clay_LayoutConfig){
                     .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT()}}))
            {
                CLAY_TEXT(CLAY_STRING("Demo Checkbox"), CLAY_TEXT_CONFIG(te));
            }

            CheckBox(&bc, CLAY_STRING("CHECKBOX1"));
        }
    }

    CLAY(CLAY_ID("SliderDemo"),
         CLAY_LAYOUT((Clay_LayoutConfig){
             .layoutDirection = CLAY_TOP_TO_BOTTOM,
             .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}}))
    {

        float d = (-1 * fv) + 100;
        snprintf(g_sliderValueTextBuffer, sizeof(g_sliderValueTextBuffer),
                 "%.2f, %.2f", fv, d);

        CLAY_TEXT(ClayStringFromCString(g_sliderValueTextBuffer), CLAY_TEXT_CONFIG(te));
        CLAY(CLAY_LAYOUT((Clay_LayoutConfig){
            .padding = {5, 5},
            .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(32)}}))
        {
            Slider(&fv, min, max, CLAY_STRING("SLIDER1"));
        }

        CLAY(CLAY_LAYOUT((Clay_LayoutConfig){
            .padding = {5, 5},
            .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(32)}}))
        {
            Slider(&d, min, max, CLAY_STRING("SLIDER2"));
        }
    }
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
                InvItem c = g_player_inv[0];
                int i = 0;
                while (c.partname.chars != NULL)
                {
                    if (c.isplaced)
                        goto next;

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

                        CLAY_TEXT(c.partname, CLAY_TEXT_CONFIG(cfg));
                    }

                next:
                    i += 1;
                    c = g_player_inv[i];
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
                RenderHeaderButton(CLAY_STRING("Ui Element Demo"), 0);
                RenderHeaderButton(CLAY_STRING("Builder Ui Demo"), 1);
                RenderHeaderButton(CLAY_STRING("Text Editor Demo"), 2);
            }
            CLAY(CLAY_ID("MainContent"),
                 CLAY_LAYOUT((Clay_LayoutConfig){
                     .layoutDirection = CLAY_TOP_TO_BOTTOM,
                     .padding = {16, 16},
                     .childGap = 16,
                     .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}}),
                 CLAY_RECTANGLE((Clay_RectangleElementConfig){
                     .color = {200, 200, 255, 255},
                     .cornerRadius = cornerraidus}))
            {

                switch (g_selectedHeader)
                {
                case 0:
                    LayoutDemo();
                    break;

                case 1:
                    PartBuilderDemo();
                    break;

                case 2:
                    TextEditorDemo();
                    break;
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

void clay_error(Clay_ErrorData ed) { TraceLog(LOG_ERROR, ed.errorText.chars); }

int main(void)
{
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena){.memory = malloc(totalMemorySize), .capacity = totalMemorySize};
    Clay_SetMeasureTextFunction(Raylib_MeasureText);

    Clay_ErrorHandler cerror = {.errorHandlerFunction = clay_error, .userData = 0};

    Clay_Initialize(clayMemory, (Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()}, cerror);
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
