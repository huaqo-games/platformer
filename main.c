#include "raylib.h"
#include "utils.h"
#include "raymath.h"

#define MAX_BUILDINGS 100


int main(void){
    
    const int screenWidth = 800;
    const int screenHeight = 450;
    int gameplayFramesCounter = 0;
    int logoFramesCounter = 0;

    InitWindow(screenWidth, screenHeight, "My Game");
    
    GameScreen currentScreen = LOGO;

    Rectangle buildings[MAX_BUILDINGS] = { 0 };
    Color buildColors[MAX_BUILDINGS] = { 0 };

    int spacing = 0;

    for (int i = 0; i < MAX_BUILDINGS; i++){
        buildings[i].width = (float)GetRandomValue(50, 200);
        buildings[i].height = (float)GetRandomValue(100, 800);
        buildings[i].x = -6000.0f + spacing;
        buildings[i].y = 400.0f - buildings[i].height;

        spacing += (int)buildings[i].width;

        buildColors[i] = (Color){ GetRandomValue(200, 240),GetRandomValue(200,240),GetRandomValue(200,250), 255};
    }

    Player player = { 0 };
    player.position = (Vector2){ 400, 280};
    player.speed = 0;
    player.canJump = false;

    EnvItem envItems[] = {
        {{ 0 , 400, 6000, 10 }, 1, GRAY },   
        {{ 300, 100, 400, 10 }, 1, GRAY },  
        {{ 250, 150, 100, 10 }, 1, GRAY },  
        {{ 650, 200, 100, 10 }, 1, GRAY },  
        {{ 329, 250, 96, 10 }, 1, GRAY },   
        {{ 523, 300, 71, 10 }, 1, GRAY },   
        {{ 496, 350, 58, 10 }, 1, GRAY },   
        {{ 177, 400, 118, 10 }, 1, GRAY },  
        {{ 166, 450, 114, 10 }, 1, GRAY },  
        {{ 500, 500, 118, 10 }, 1, GRAY }   
    };



    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenter,
        UpdateCameraCenterInsideMap,
        UpdateCameraCenterSmoothFollow,
        UpdateCameraEvenOutOnLanding,
        UpdateCameraPlayerBoundsPush
    };

    int cameraOption = 0;
    int cameraUpdatersLength = sizeof(cameraUpdaters)/sizeof(cameraUpdaters[0]);

    char *cameraDescriptions[] = {
        "Follow player center",
        "Follow player center, but clamp to map edges",
        "Follow player center; smoothed",
        "Follow player center horizontally; update player center vertically after landing",
        "Player push camera on getting too close to screen edge"
    };

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        switch(currentScreen){
            
            case LOGO:
            {
                logoFramesCounter++;
                if(logoFramesCounter > 120){
                    currentScreen = TITLE;
                }
            } break;

            case TITLE:
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)){
                    currentScreen = GAMEPLAY;
                }
            } break;

            case GAMEPLAY:
            {
                float deltaTime = GetFrameTime();

                UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

                gameplayFramesCounter++;
                if (IsKeyPressed(KEY_Q)){
                    currentScreen = ENDING;
                }
                
                camera.target = (Vector2){ player.position.x + 20, player.position.y + 20};

                if(IsKeyDown(KEY_A)) camera.rotation--;
                else if(IsKeyDown(KEY_S)) camera.rotation++;

                camera.zoom += ((float)GetMouseWheelMove()*0.05f);
                if (camera.zoom > 3.0f) camera.zoom = 3.0f;
                else if(camera.zoom < 0.25f) camera.zoom = 0.25f;

                if (IsKeyPressed(KEY_R)){
                    camera.zoom = 1.0f;
                    camera.rotation = 0.0f;
                    player.position = (Vector2){ 400, 280 };
                }

                if (IsKeyPressed(KEY_C)) cameraOption = (cameraOption + 1)%cameraUpdatersLength;

                cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);

            }break;

            case ENDING:
            {
                gameplayFramesCounter = 0;
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)){
                    currentScreen = TITLE;
                }          
            } break;
            default: break;

        }

        BeginDrawing();
            ClearBackground(WHITE);
            switch(currentScreen){
                
                case LOGO:
                {
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
                } break;

                case TITLE:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
                } break;

                case GAMEPLAY:
                {
                    BeginMode2D(camera);
                        DrawRectangle(-6000,400,13000,8000, DARKGRAY);
                        for (int i = 0; i < MAX_BUILDINGS; i++) DrawRectangleRec(buildings[i], buildColors[i]);
                        for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
                        Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40.0f, 40.0f};
                        DrawRectangleRec(playerRect, RED);
                        DrawCircleV(player.position, 5.0f, GOLD);
                    EndMode2D();

                    // Info
                    const char* data_labels[] = {"FPS: %d", "MouseX: %d", "MouseY: %d", "ScreenWidth: %d","ScreenHeight: %d"};
                    int (*values[])() = {GetFPS, GetMouseX, GetMouseY, GetScreenWidth, GetScreenHeight};
                    for (int i = 0; i < sizeof(data_labels)/sizeof(char*); i++) {
                        int value = values[i]();
                        DrawText(TextFormat(data_labels[i], value), 20, 20 + i * 20, 20, BLACK);
                    }

                    int fontSize = 20;
                    const char* controls_labels[] = {
                        "Free 2d camera controls:",
                        "- Right/Left to move",
                        "- Space to jump",
                        "- Mouse Wheel to Zoom in-out",
                        "- C to change camera mode",
                        "- A / S to Rotate","- R to reset Zoom and Rotation"};
                    for (int i = 0; i<sizeof(controls_labels)/sizeof(const char*);i++){
                        int textWidth = MeasureText(controls_labels[i], fontSize);
                        DrawText(controls_labels[i], screenWidth - textWidth - 20, 20 + i * 20, 20, BLACK);
                    }
                }break;

                case ENDING:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
                } break;
                default: break;

            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}