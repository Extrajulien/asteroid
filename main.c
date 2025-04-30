#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "asteroids.h"

#include "Player/player.h"

#define FRAME_PER_SEC 120

#define TWHITE (Color){255,255,255,150}

void drawGrid(int x, int y, int size, Player *player, bool hasVectorDisplay);

void drawEntitiesPos(Vector2 position, Player *player, Bullet *bullet);

void titleMenu(Rectangle *startGameBox, Rectangle *editAsteroidsMode, Rectangle *managePresets);

void renderBullets(Bullet *bullet);

void moveBullets(Bullet *bullets);

void deleteBullet(Bullet *bullet, int index);

void wrapAroundBullet(Bullet *bullets);

void shotAsteroid(Bullet *bullets, BigAsteroid **bigAsteroid, MidAsteroid **midAsteroid,
                  SmlAsteroid **smlAsteroid); //fix this

void updateGame(Player *player, Bullet *bullet, AsteroidArray *bigAstArr, AsteroidArray *midAstArr,
                AsteroidArray *smlAstArr);

void movementGame(Player *player, Bullet *bullet);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    Player player;
    Bullet bullets[MAX_BULLETS];
    AsteroidArray bigAstArr = {NULL, 0};
    AsteroidArray midAstArr = {NULL, 0};
    AsteroidArray smlAstArr = {NULL, 0};
    Vector2 mousePos = {0, 0};


    bool isTitleMenu = true;
    bool isAsteroidEditScreen = false;
    bool isGame = false;
    bool hasDebugMode = false;
    int waveNumber = 0;

    updateAsteroidsTraits();

    //InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Asteroid Julien Lamothe");//windows
    InitWindow(1920, 1080, "Asteroids"); //linux
    //ToggleBorderlessWindowed();
    Rectangle startGameBox = {(GetScreenWidth()-350) / 2, GetScreenHeight() / 2, 350, 100};
    Rectangle editAsteroidsModeBox = {(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 150, 350, 100};
    Rectangle managePresetsBox = {(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 300, 350, 100};
    generateWave(&bigAstArr, waveNumber);
    resetPlayer(&player);
    SetTargetFPS(FRAME_PER_SEC); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        //title menu
        if (isTitleMenu) {
            mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, startGameBox)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    isTitleMenu = false;
                    isGame = true;
                }
            }
            if (IsKeyPressed(KEY_SPACE)) {
                isTitleMenu = false;
                isGame = true;
            }
        }

        //game
        if (isGame) {
            movementGame(&player, bullets);
            if (IsKeyPressed(KEY_E)) hasDebugMode = !hasDebugMode;
            updateGame(&player, bullets, &bigAstArr, &midAstArr, &smlAstArr);

            if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_TAB)) {
                free(bigAstArr.asteroid);
                bigAstArr.size = 0;
                waveNumber = 0;
                generateWave(&bigAstArr, waveNumber);
                resetPlayer(&player);
                if (IsKeyPressed(KEY_TAB)) {
                    isTitleMenu = true;
                    isGame = false;
                }
            }
        }

        //updateAsteroid(&bigAsteroid, &midAsteroid, &smlAsteroid);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        char rotation[100] = "";
        sprintf(rotation, "%.2f", player.angle);
        char speedAmnt[100] = "";

        if (isGame) {
            sprintf(speedAmnt, "%.2f", sqrtf(SQUARE(player.speed.x) + SQUARE(player.speed.y)));
            ClearBackground(BLACK);

            renderBullets(bullets);
            drawPlayer(&player);
            renderAsteroids(&bigAstArr);
            renderAsteroids(&midAstArr);
            renderAsteroids(&smlAstArr);

            if (hasDebugMode) {
                drawGrid(GetScreenWidth() - 200, 200, 300, &player, true);
                drawGrid(GetScreenWidth() / 2, GetScreenHeight() / 2, 4500, &player, false);
                DrawText(rotation, 30, 30, 20, LIGHTGRAY);
                DrawText(speedAmnt, 30, 50, 20, LIGHTGRAY);
                DrawFPS(30, 80);
                drawEntitiesPos((Vector2){GetScreenWidth() - 800, 100}, &player, bullets);
            }
        }
        if (isTitleMenu) {
            titleMenu(&startGameBox, &editAsteroidsModeBox, &managePresetsBox);
            ClearBackground(BLACK);
            //DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), RED);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    free(bigAstArr.asteroid);
    free(midAstArr.asteroid);
    free(smlAstArr.asteroid);
    return 0;
}

void updateGame(Player *player, Bullet *bullet, AsteroidArray *bigAstArr, AsteroidArray *midAstArr,
                AsteroidArray *smlAstArr) {
    glide(player);
    moveBullets(bullet);
    moveAsteroids(bigAstArr);
    moveAsteroids(midAstArr);
    moveAsteroids(smlAstArr);
    wrapAroundBullet(bullet);
    wrapAroundPlayer(player);
    wrapAroundAsteroid(bigAstArr);
    wrapAroundAsteroid(midAstArr);
    wrapAroundAsteroid(smlAstArr);
    rotateAsteroid(bigAstArr);
    rotateAsteroid(midAstArr);
    rotateAsteroid(smlAstArr);
    setPlayerBorders(player);
}

void movementGame(Player *player, Bullet *bullet) {
    const float rotationSpeed = 230.0;
    static float howLongPressed = 0; //in seconds

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        howLongPressed += GetFrameTime();
        thrust(player, howLongPressed);
    } else {
        howLongPressed = 0;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player->angle -= (rotationSpeed * GetFrameTime()); //rotate
        if (player->angle <= 0) player->angle = 360;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player->angle += (rotationSpeed * GetFrameTime());
        if (player->angle >= 360) player->angle = 0;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        shoot(player, bullet, 10);
    }

    if (IsKeyDown(KEY_R)) resetPlayer(player);
}

void drawGrid(int x, int y, int size, Player *player, bool hasVectorDisplay) {
    Vector2 topLeftCorner = {x - size / 2, y - size / 2};
    int nbHorizontalLine = 9;
    int nbVerticalLine = 9;


    float distanceVerticalLine = size / nbVerticalLine;
    float distanceHorizontalLine = size / nbHorizontalLine;
    for (int i = 0; i < nbHorizontalLine; i++) {
        DrawLine(topLeftCorner.x, (topLeftCorner.y + distanceHorizontalLine * i + distanceHorizontalLine / 2),
                 topLeftCorner.x + size, (topLeftCorner.y + distanceHorizontalLine * i + distanceHorizontalLine / 2),
                 TWHITE);
    }
    for (int i = 0; i < nbVerticalLine; i++) {
        DrawLine((topLeftCorner.x + distanceVerticalLine * i + distanceVerticalLine / 2), topLeftCorner.y,
                 (topLeftCorner.x + distanceVerticalLine * i + distanceVerticalLine / 2), topLeftCorner.y + size,
                 TWHITE);
    }
    if (hasVectorDisplay) {
        float horizontalVector = x + player->speed.x * 10;
        float verticalVector = y - player->speed.y * 10;
        Vector2 startVector = {x, y};
        Vector2 endVector = {horizontalVector, verticalVector};
        DrawLineEx(startVector, endVector, 4.0, RED);
    }
}

void drawEntitiesPos(Vector2 position, Player *player, Bullet *bullet) {
    int scale = 7;
    Vector2 topLeft = {position.x, position.y};
    DrawRectangle(position.x, position.y, GetScreenWidth() / scale, GetScreenHeight() / scale, WHITE);
    DrawRectangleLines(position.x, position.y, GetScreenWidth() / scale, GetScreenHeight() / scale, GRAY);

    DrawRectangle(player->position.x / scale + topLeft.x, player->position.y / scale + topLeft.y, 10, 10, GREEN);


    for (int i = 0; i < MAX_BULLETS; ++i) {
        Vector2 size = {bullet[i].size.x / scale, bullet[i].size.y / scale};
        if (size.x < 3) {
            size.x = 3;
        }
        if (size.y < 3) {
            size.y = 3;
        }
        DrawRectangle(bullet[i].position.x / scale + topLeft.x, bullet[i].position.y / scale + topLeft.y, size.x,
                      size.y, BLACK);
    }
}

void titleMenu(Rectangle *startGameBox, Rectangle *editAsteroidsMode, Rectangle *managePresets) {
    Vector2 mousePos = GetMousePosition();
    int boxBorder = 10;
    Rectangle *pBox;
    char *title = "Asteroids";
    int titleSizeX = MeasureText(title, 256);
    DrawText(title, (GetScreenWidth()-titleSizeX) / 2, (GetScreenHeight() / 4)-100, 256, WHITE);
    DrawText("By Julien Lamothe", (GetScreenWidth()-titleSizeX)/2 + titleSizeX-MeasureText("By Julien Lamothe", 20),
        (GetScreenHeight() / 4)+112, 20, GRAY);

    pBox = startGameBox;
    if (CheckCollisionPointRec(mousePos, *startGameBox)) {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, GRAY);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Start", pBox->x + (pBox->width-50) / 2, pBox->y + pBox->height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, WHITE);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Start", pBox->x + (pBox->width-50) / 2,
                 pBox->y + pBox->height / 2 - 10, 20, WHITE);
    }

    pBox = editAsteroidsMode;
    if (CheckCollisionPointRec(mousePos, *editAsteroidsMode)) {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, GRAY);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Mode", pBox->x + (pBox->width - 96.0) / 2, pBox->y + pBox->height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, WHITE);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Mode", pBox->x + (pBox->width - 96.0) / 2,
                 pBox->y + pBox->height / 2 - 10, 20, WHITE);
    }

    pBox = managePresets;
    if (CheckCollisionPointRec(mousePos, *managePresets)) {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, GRAY);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Presets", pBox->x + (pBox->width-120) / 2, pBox->y + pBox->height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, WHITE);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Presets", pBox->x + (pBox->width-120) / 2,
                 pBox->y + pBox->height / 2 - 10, 20, WHITE);
    }

}

void renderBullets(Bullet *bullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet[i].distance > 0) {
            DrawRectangle((int) bullet[i].position.x, (int) bullet[i].position.y, (int) bullet[i].size.x,
                          (int) bullet[i].size.y, WHITE);
        }
    }
}

void moveBullets(Bullet *bullets) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        float speedX = bullets[i].speed.x * GetFrameTime() * 120;
        float speedY = bullets[i].speed.y * GetFrameTime() * 120;

        bullets[i].position.x += speedX;
        bullets[i].position.y += speedY;
        bullets[i].distance -= sqrtf(SQUARE(speedX) + SQUARE(speedY));

        if (bullets[i].distance <= 0) {
            deleteBullet(bullets, i);
        }
    }
}

void deleteBullet(Bullet *bullet, int index) {
    bullet[index].size = (Vector2){0, 0};
    bullet[index].position = (Vector2){0, 0};
    bullet[index].speed = (Vector2){0, 0};
    bullet[index].distance = 0;
}

void wrapAroundBullet(Bullet *bullets) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bullets[i].position.x > GetScreenWidth() + bullets[i].size.x) bullets[i].position.x = -bullets[i].size.x;
        if (bullets[i].position.y > GetScreenHeight() + bullets[i].size.y) bullets[i].position.y = -bullets[i].size.y;
        if (bullets[i].position.x < -bullets[i].size.x) bullets[i].position.x = GetScreenWidth();
        if (bullets[i].position.y < -bullets[i].size.y) bullets[i].position.y = GetScreenHeight();
    }
}

void shotAsteroid(Bullet *bullets, BigAsteroid **bigAsteroid, MidAsteroid **midAsteroid, SmlAsteroid **smlAsteroid) {
    //condition de hit box genre s'il se situe entre les points des asteroides
    //if ()
}

void createPresetAsteroidTraits() {
}
