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

void titleMenu();

void renderBullets(Bullet *bullet);

void moveBullets(Bullet *bullets);

void deleteBullet(Bullet *bullet, int index);

void wrapAroundBullet(Bullet *bullets);

void shotAsteroid(Bullet *bullets, BigAsteroid **bigAsteroid, MidAsteroid **midAsteroid, SmlAsteroid **smlAsteroid);



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

    const float rotationSpeed = 230.0;
    bool isTitleMenu = true;
    bool hasDebugMode = false;
    float howLongPressed = 0;//in seconds
    //InitWindow(GetMonitorWidth(), GetMonitorHeight(), "Asteroid Julien Lamothe");//windows
    InitWindow(1600, 900, "Asteroid Julien Lamothe");//linux
    //ToggleBorderlessWindowed();

    generateWave(&bigAstArr, 0);
    resetPlayer(&player);
    SetTargetFPS(FRAME_PER_SEC);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            howLongPressed += GetFrameTime();
            thrust(&player, howLongPressed);
        } else {
            howLongPressed = 0;
        }
        glide(&player);
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            player.angle -= (rotationSpeed * GetFrameTime());//rotate
            if (player.angle <= 0) player.angle = 360;
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            player.angle += (rotationSpeed * GetFrameTime());
            if (player.angle >= 360) player.angle = 0;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (isTitleMenu) {
                isTitleMenu = false;
            } else {
                shoot(&player, bullets, 10);
            }
        }

        if (IsKeyDown(KEY_R)) resetPlayer(&player);

        if (IsKeyPressed(KEY_E)) hasDebugMode = !hasDebugMode;

        //updateAsteroid(&bigAsteroid, &midAsteroid, &smlAsteroid);

        moveBullets(bullets);
        moveAsteroids(&bigAstArr);
        moveAsteroids(&midAstArr);
        moveAsteroids(&smlAstArr);
        wrapAroundBullet(bullets);
        wrapAroundPlayer(&player);
        wrapAroundAsteroid(&bigAstArr);
        wrapAroundAsteroid(&midAstArr);
        wrapAroundAsteroid(&smlAstArr);
        rotateAsteroid(&bigAstArr);
        rotateAsteroid(&midAstArr);
        rotateAsteroid(&smlAstArr);
        setPlayerBorders(&player);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        char rotation[100] = "";
        sprintf(rotation, "%.2f", player.angle);
        char timePressed[100] = "";
        sprintf(timePressed, "%.2f", howLongPressed);
        char speedAmnt[100] = "";

        if (!isTitleMenu) {
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
                DrawText(timePressed, 30, 50, 20, LIGHTGRAY);
                DrawText(speedAmnt, 30, 80, 20, LIGHTGRAY);
                DrawFPS(30, 100);
                drawEntitiesPos((Vector2) {GetScreenWidth() - 800, 100}, &player, bullets);
            }
        } else {
            titleMenu();
            ClearBackground(BLACK);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    free(bigAstArr.asteroid);
    free(midAstArr.asteroid);
    free(smlAstArr.asteroid);
    return 0;

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

void titleMenu() {
    char *title = "Roches Spatiales";
    int middleX = (GetScreenWidth() / 2);
    int middleY = (GetScreenHeight() / 2);
    DrawText(title, middleX, middleY, 24, WHITE);
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
    bullet[index].size = (Vector2) {0, 0};
    bullet[index].position = (Vector2) {0, 0};
    bullet[index].speed = (Vector2) {0, 0};
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