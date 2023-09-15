#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Enemy.h"
#include "Prop.h"
#include <string>

int main()
{
    const int windowHeight{384};
    const int windowWidth{384};
    InitWindow(windowWidth, windowHeight, "Classy Clash");

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Prop props[2]{
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")},
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")}};

    Character knight{windowWidth, windowHeight};
    Enemy goblin{Vector2{800.f, 300.f},
                 LoadTexture("characters/goblin_idle_spritesheet.png"),
                 LoadTexture("characters/goblin_run_spritesheet.png")};
    Enemy slime{
        Vector2{500.f, 700.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);
        // draw props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }
        knight.tick(GetFrameTime());
        // check map bounds
        if (knight.getWorldPos().x < 0.f || knight.getWorldPos().y < 0.f || knight.getWorldPos().x + windowWidth > map.width * mapScale || knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }
        // check prop collisions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        if (!knight.getAlive())
        { // character is not alive
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        { // character is alive
            std::string knightHealth = "Health: ";
            knightHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {

                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}