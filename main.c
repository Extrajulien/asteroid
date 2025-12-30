#define FRAME_PER_SEC 120

#define TWHITE (Color){255,255,255,150}

#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>//hell nah

#include "main_api.h"
#include "asteroids.h"
#include "menu.h"
#include "particles.h"
#include "files.h"

#include "Player/player.h"

Player player;

void drawGrid(int x, int y, int size, Player *player, bool hasVectorDisplay);

void drawEntitiesPos(Vector2 position, Player *player, Bullet *bullet);

void renderBullets(Bullet *bullet);

void moveBullets(Bullet *bullets);

void initBullets(Bullet *bullets);

void deleteBullet(Bullet *bullet, int index);

void wrapAroundBullet(Bullet *bullets);

void updateGame(Player *player, Bullet *bullet, AsteroidArray *bigAstArr, AsteroidArray *midAstArr,
                AsteroidArray *smlAstArr);

void movementGame(Player *player, Bullet *bullet);

void showGameoverScreen();

static bool isTitleMenu = true;
static bool isAsteroidEditScreen = false;
static bool isEditPresetsScreen = false;
static bool isGame = false;
static bool hasDebugMode = false;
static bool isGameoverScreen = false;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    pthread_t thread1;
    Bullet bullets[MAX_BULLETS];
    AsteroidArray bigAstArr = {NULL, 0, 0};
    AsteroidArray midAstArr = {NULL, 0, 0};
    AsteroidArray smlAstArr = {NULL, 0, 0};
    srand((unsigned int)time(NULL));

    int waveNumber = 0;
    int score = 0;

    readPresetFile();
    particleArrInit(10);
    //InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Asteroid Julien Lamothe");//windows
    InitWindow(1920, 1080, "Asteroids"); //linux
    initBullets(bullets);
    //ToggleBorderlessWindowed();
    initPlayer(&player);
    generateWave(&bigAstArr, waveNumber);
    SetTargetFPS(FRAME_PER_SEC); // Set our game to run at 60 frames-per-second
    loadThemes();
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
            titleMenuInput(&isTitleMenu, &isGame, &isAsteroidEditScreen, &isEditPresetsScreen);
            if (isGame) {
                initPlayer(&player);
                score = 0;
                freeAsteroidArray(&bigAstArr, BIG);
                freeAsteroidArray(&midAstArr, MEDIUM);
                freeAsteroidArray(&smlAstArr, SMALL);
                waveNumber = 0;
                generateWave(&bigAstArr, waveNumber);
                isGameoverScreen = false;
            }
        }

        if (isAsteroidEditScreen) {
            updateEditAsteroidMenu(&bigAstArr, &midAstArr, &smlAstArr);
        }

        //game
        if (isGame) {
            if (bigAstArr.nbAsteroid == 0 && midAstArr.nbAsteroid == 0 && smlAstArr.nbAsteroid == 0) {
                freeAsteroidArray(&bigAstArr, BIG);
                freeAsteroidArray(&midAstArr, MEDIUM);
                freeAsteroidArray(&smlAstArr, SMALL);
                waveNumber++;
                generateWave(&bigAstArr, waveNumber);
            }
            movementGame(&player, bullets);
            if (IsKeyPressed(KEY_E)) hasDebugMode = !hasDebugMode;
            updateGame(&player, bullets, &bigAstArr, &midAstArr, &smlAstArr);

            PackageCollisionBullet *packageBullet = malloc(sizeof(PackageCollisionBullet));
            *packageBullet = (PackageCollisionBullet){&bigAstArr, &midAstArr, &smlAstArr, bullets, &score};
            //pthread_create(&thread1, NULL, checkCollisionAstBullet, packageBullet);
            checkCollisionAstBullet(packageBullet);
            //pthread_join(thread1, NULL);
            free(packageBullet);

            PackageCollisionPlayer *packagePlayer = malloc(sizeof(PackageCollisionPlayer));
            *packagePlayer = (PackageCollisionPlayer){&player, &bigAstArr, &midAstArr, &smlAstArr};
            checkCollisionAstPlayer(packagePlayer);
            free(packagePlayer);

            if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_TAB)) {
                freeAsteroidArray(&bigAstArr, BIG);
                freeAsteroidArray(&midAstArr, MEDIUM);
                freeAsteroidArray(&smlAstArr, SMALL);
                bigAstArr.size = 0;
                midAstArr.size = 0;
                waveNumber = 0;
                bigAstArr.nbAsteroid = 0;
                midAstArr.nbAsteroid = 0;
                smlAstArr.nbAsteroid = 0;
                score = 0;
                initBullets(bullets);
                generateWave(&bigAstArr, waveNumber);
                resetPlayer(&player);
                if (IsKeyPressed(KEY_TAB)) {
                    particleArrDestroy();
                    isTitleMenu = true;
                    isGame = false;
                }
            }
        }

        if (isGameoverScreen) {
            ClearBackground(BLACK);
            updateGame(&player, bullets, &bigAstArr, &midAstArr, &smlAstArr);
        }

        //updateAsteroid(&bigAsteroid, &midAsteroid, &smlAsteroid);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        char rotation[100] = "";
        sprintf(rotation, "%.2f", player.angle);
        char speedAmnt[100] = "";

        if (isGameoverScreen) {
            drawParticles();
            renderAsteroids(&bigAstArr);
            renderAsteroids(&midAstArr);
            renderAsteroids(&smlAstArr);
            showGameoverScreen();
        }

        if (isGame) {
            sprintf(speedAmnt, "%.2f", sqrtf(SQUARE(player.speed.x) + SQUARE(player.speed.y)));
            ClearBackground(BLACK);
            drawParticles();
            renderBullets(bullets);
            drawPlayer(&player);
            renderAsteroids(&bigAstArr);
            renderAsteroids(&midAstArr);
            renderAsteroids(&smlAstArr);

            char scoreText[100] = "";
            sprintf(scoreText,"%d", score);
            DrawText(scoreText, 10, 50, 32, WHITE);
            char livesText[100] = "";
            sprintf(livesText,"pv:%d", player.lives);
            DrawText(livesText, 10, 10, 40, ORANGE);

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
            ClearBackground(BLACK);
            titleMenu();

            //DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), RED);
        }
        if (isAsteroidEditScreen) {
            ClearBackground(BLACK);
            editAsteroidMenu(&isTitleMenu, &isAsteroidEditScreen);
            renderAsteroids(&bigAstArr);
            renderAsteroids(&midAstArr);
            renderAsteroids(&smlAstArr);
            if (isTitleMenu) {
                freeAsteroidArray(&bigAstArr, BIG);
                freeAsteroidArray(&midAstArr, MEDIUM);
                freeAsteroidArray(&smlAstArr, SMALL);
                bigAstArr.nbAsteroid = 0;
                midAstArr.nbAsteroid = 0;
                smlAstArr.nbAsteroid = 0;
                generateWave(&bigAstArr, 0);
            }
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    freeAsteroidArray(&bigAstArr, ((BigAsteroid*)bigAstArr.asteroid[0])->base.type);
    freeAsteroidArray(&midAstArr, ((MidAsteroid*)midAstArr.asteroid[0])->base.type);
    freeAsteroidArray(&smlAstArr, SMALL);
    return 0;
}

void updateGame(Player *player, Bullet *bullet, AsteroidArray *bigAstArr, AsteroidArray *midAstArr,
                AsteroidArray *smlAstArr) {
    glide(player);
    moveBullets(bullet);
    moveAsteroids(bigAstArr);
    moveAsteroids(midAstArr);
    moveAsteroids(smlAstArr);
    moveParticles();
    wrapAroundBullet(bullet);
    wrapAroundPlayer(player);
    wrapAroundAsteroid(bigAstArr);
    wrapAroundAsteroid(midAstArr);
    wrapAroundAsteroid(smlAstArr);
    rotateAsteroid(bigAstArr, BIG);
    rotateAsteroid(midAstArr, MEDIUM);
    rotateAsteroid(smlAstArr, SMALL);
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
        if (bullets[i].distance == 0) continue;
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
    bullet[index].speed = (Vector2){0, 0};
    bullet[index].distance = 0;
}

void wrapAroundBullet(Bullet *bullets) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bullets[i].distance == 0) continue;
        if (bullets[i].position.x > GetScreenWidth() + bullets[i].size.x) bullets[i].position.x = -bullets[i].size.x;
        if (bullets[i].position.y > GetScreenHeight() + bullets[i].size.y) bullets[i].position.y = -bullets[i].size.y;
        if (bullets[i].position.x < -bullets[i].size.x) bullets[i].position.x = GetScreenWidth() + bullets[i].size.x;
        if (bullets[i].position.y < -bullets[i].size.y) bullets[i].position.y = GetScreenHeight() + bullets[i].size.y;
    }
}

void initBullets(Bullet *bullets) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].distance = 0;
        bullets[i].position = (Vector2){50,50};
        bullets[i].speed = (Vector2){0,0};
        bullets[i].size = (Vector2){5,5};
    }
}

void gameoverPlayer(Player *player) {
    isGameoverScreen = true;
    isGame = false;

}

void showGameoverScreen() {
    static float timer = 0;
    const int fontSize = 256;
    const char* gameover = "Game Over";
    const Vector2 textSize = MeasureTextEx(GetFontDefault(), gameover, fontSize, 25);
    DrawTextEx(GetFontDefault(), gameover,
        (Vector2){GetScreenWidth()/2-textSize.x/2, GetScreenHeight()/2-textSize.y/2}, fontSize, 25, ORANGE);
    timer += GetFrameTime();
    if (timer > 3) {
    isGameoverScreen = false;
        isTitleMenu = true;
        timer = 0;
        particleArrDestroy();
    }
}