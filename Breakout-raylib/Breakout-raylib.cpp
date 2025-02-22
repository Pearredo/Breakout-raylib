// Breakout-raylib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// 
//Included Libraries
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "raylib.h"

//constants
const int screenWidth = 500;
const int screenHeight = 500;

struct Ball{
    Vector2 pos;
    Vector2 acceleration{ 1.0f, 1.0f };
    float velocity;
    float radius = 5.0f;
};

struct Player {
    Rectangle rect;
    float velocity = 250.0f;
    int score;
    float w = 75.0f;
    float h = 10.0f;
};

std::vector<Color> COLORS{ RED, ORANGE, PURPLE, BROWN, PINK, MAGENTA, GRAY };

struct Brick {
    Rectangle rect;
    Color color;
    float w = 50.0f;
    float h = 20.0f;
};

Texture2D texBackground;
Player player;
Ball ball;
std::vector<Brick> bricks;

Sound fxBrickHit[2];
Sound fxPaddHit;

void gameStartup() {
    //loading background into video memory
    Image imgBackground = LoadImage("assets/background.png");
    texBackground = LoadTextureFromImage(imgBackground);
    UnloadImage(imgBackground);

    //loading player paddle data
    player.rect = Rectangle{ 250.0f, 440.0f, player.w, player.h };
    player.score = 0;

    //load ball
    ball.pos = Vector2{ 250,300 };
    ball.velocity = 250.0f;

    //loading bricks
    Brick newBrick;
    for (int i = 0; i < 8;i++) {
        for (int o = 0; o < 8; o++) {
            newBrick.rect = Rectangle{
                float(40 + (i * 55)),
                float(50 + (o * 26)),
                newBrick.w,
                newBrick.h
            };
            newBrick.color = COLORS[rand() % COLORS.size()];
            bricks.push_back(newBrick);
        }
    }
    //load sounds
    fxBrickHit[0] = LoadSound("assets/brick-hit-1.wav");
    fxBrickHit[1] = LoadSound("assets/brick-hit-2.wav");
    fxPaddHit = LoadSound("assets/paddle-hit-1.wav");
}

void gameUpdate() {
    float frameTime = GetFrameTime();

    //controls
    if (IsKeyDown(KEY_LEFT)) {
        player.rect.x -= player.velocity * frameTime;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        player.rect.x += player.velocity * frameTime;
    }

    //update ball position
    ball.pos.x = ball.pos.x + ((ball.velocity * ball.acceleration.x) * frameTime);
    ball.pos.y = ball.pos.y + ((ball.velocity * ball.acceleration.y) * frameTime);

    //collision detection brick and ball
    Brick brick;
    int random;
    for (int i = 0; i < bricks.size();i++) {
        brick = bricks[i];
        if (CheckCollisionCircleRec(
            ball.pos,
            ball.radius,
            brick.rect
        )) {
            ball.acceleration.y = ball.acceleration.y * -1;
            bricks.erase(bricks.begin() + i);
            player.score = player.score + 5;
            random = rand() % 2;
            PlaySound(fxBrickHit[random]);
            break;
        }
    }

    //collision dection wall and player
    if (player.rect.x < 0) {
        player.rect.x = 0;
    }
    if (player.rect.x > (screenWidth - player.rect.width)) {
        player.rect.x = (screenWidth - player.rect.width);
    }

    //collision detection wall and ball
    if (ball.pos.x > screenWidth || ball.pos.x < ball.radius) {
        ball.acceleration.x = ball.acceleration.x * -1;
    }
    if (ball.pos.y > screenHeight || ball.pos.y < ball.radius) {
        ball.acceleration.y = ball.acceleration.y * -1;
    }

    //check collision between ball and player
    if (CheckCollisionCircleRec(ball.pos, ball.radius, player.rect)) {
        //ball.acceleration.x = ball.acceleration.x * -1;
        ball.acceleration.y = ball.acceleration.y * -1;
        PlaySound(fxPaddHit);
    }
}

void gameRender() {
    //render background
    DrawTexture(texBackground, 0, 0, RAYWHITE);

    //render bricks
    Brick brick;
    for (int i = 0; i < bricks.size(); i++) {
        brick = bricks[i];
        DrawRectangle(
            brick.rect.x,
            brick.rect.y,
            brick.rect.width,
            brick.rect.height,
            brick.color
        );
    }

    //render player
    DrawRectangle(player.rect.x, player.rect.y, player.rect.width, player.rect.height, RAYWHITE);

    //render ball
    DrawCircle(ball.pos.x, ball.pos.y, ball.radius, RAYWHITE);

    //render score
    std::string sScore = std::to_string(player.score);
    std::string displayScore1 = "Score: ";
    std::string displayScore2 = displayScore1 + sScore;
    char const* text = displayScore2.c_str();
    DrawText(text, 10, 10, 30, RAYWHITE);
}

//safely shutdowns game
void gameShutdown() {
    UnloadSound(fxBrickHit[0]);
    UnloadSound(fxBrickHit[1]);
    UnloadSound(fxPaddHit);

    UnloadTexture(texBackground);
    CloseAudioDevice();
}

//main
int main()
{
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "Breakout");
    InitAudioDevice();

    SetTargetFPS(60);

    gameStartup();

    //runs game
    while(!WindowShouldClose()) {
        
        gameUpdate();

        BeginDrawing();
        ClearBackground(BLUE);

        gameRender();

        EndDrawing();
    }
    //ends game
    gameShutdown();
    CloseWindow();
    return 0;
}

