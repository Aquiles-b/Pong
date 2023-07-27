#include "include/raylib.h"

class Paddle {
protected:
    void limits() {
        if (posi.y <= 0)
            posi.y = 0;
        if (posi.y >= GetScreenHeight() - posi.height)
            posi.y = GetScreenHeight() - posi.height;
    }
public:
    Rectangle posi;
    int speed, ctrl_up, ctrl_down, ctrl_hab1;
    bool hab1;
    float timeh1;

void draw() {
    DrawRectangle(posi.x, posi.y, posi.width, posi.height, WHITE);
}

void update() {
    if (IsKeyDown(ctrl_up))
        posi.y -= speed;
    if (IsKeyDown(ctrl_down))
        posi.y += speed;

    if (IsKeyDown(ctrl_hab1) && hab1) {
        posi.height = 200;
        hab1 = false;
        timeh1 = GetTime();
    }
    if (!hab1 && (GetTime() - timeh1 >= 3))
        posi.height = 80;

    limits();
}
};

class Cpu: public Paddle {

public:
void draw() {
    DrawRectangle(posi.x, posi.y, posi.width, posi.height, WHITE);
}

void update(float bola) {

    if (bola > posi.height/2 + posi.y)
        posi.y += speed;

    if (bola < posi.height/2 + posi.y)
        posi.y -= speed;

    if (GetRandomValue(0, 10000) == 3000 && hab1) {
        posi.height = 200;
        hab1 = false;
        timeh1 = GetTime();
    }
    if (!hab1 && (GetTime() - timeh1 >= 3))
        posi.height = 80;

    limits();
}
};

class Ball {
public:
    Vector2 posi;
    float speed_x, speed_y, raio;
    Color cor;
    bool is_point;
    int p1, p2;

void draw() {
    DrawCircle(posi.x, posi.y, raio, cor);
}

void update() {
    posi.x += speed_x;
    posi.y += speed_y;

    if (posi.y - raio <= 0)
        speed_y *= -1;
    if (posi.y + raio >= GetScreenHeight())
        speed_y *= -1;

    if (posi.x - raio <= 0) {
        p2 += 1;
        is_point = true;
    }
    if (posi.x + raio >= GetScreenWidth()) {
        p1 += 1;
        is_point = true;
    }
}
};

void default_configs(Paddle *player, Cpu *player2, Ball *bola) {
    player2->posi.width = player->posi.width = 15;
    player2->posi.height = player->posi.height = 80;
    player2->speed = player->speed = 7;
    player2->posi.y = player->posi.y = (float) GetScreenHeight()/2 - (float) player->posi.height/2;
    player->posi.x = 10.0f;
    player2->posi.x = GetScreenWidth() - player2->posi.width - 10;
    player->hab1 = player2->hab1 = true;
    player->ctrl_hab1 = KEY_E;
    player->timeh1 = player2->timeh1 = 100;
    player2->ctrl_hab1 = KEY_KP_0;

    player->ctrl_up = KEY_W;
    player->ctrl_down = KEY_S;
    player2->ctrl_up = KEY_UP;
    player2->ctrl_down = KEY_DOWN;

    bola->posi.x = GetScreenWidth()/2.0f;
    bola->posi.y = GetScreenHeight()/2.0f;
    bola->speed_x = 7 - 14 * GetRandomValue(0, 1);
    bola->speed_y = 7 - 14 * GetRandomValue(0, 1);
    bola->raio = 15;
    bola->is_point = false;
    bola->cor = WHITE;
}

void drawing(Paddle *player, Cpu *bot, Ball *bola, short paused)
{
    BeginDrawing();
    ClearBackground(BLACK);
    player->draw();
    bot->draw();
    bola->draw();
    DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE);
    DrawText(TextFormat("%d", bola->p1), GetScreenWidth() * 0.25f, 20, 60, WHITE);
    DrawText(TextFormat("%d", bola->p2), GetScreenWidth() * 0.75f, 20, 60, WHITE);

    if (paused == 1)
        DrawText("Paused", GetScreenWidth()/2 - 140, 50, 70, SKYBLUE);

    EndDrawing();
}

void updating(Paddle *player, Cpu *bot, Ball *bola) {

    if (CheckCollisionCircleRec(bola->posi, bola->raio, player->posi)) {
        bola->speed_x *= -1.1f;
        player->speed *= 1.1f;
        bot->speed *= 1.1f;
    }
    if (CheckCollisionCircleRec(bola->posi, bola->raio, bot->posi)) {
        bola->speed_x *= -1.1f;
        player->speed *= 1.1f;
        bot->speed *= 1.1f;
    }

    if (bola->is_point) {
        bola->cor = GREEN;
        drawing(player, bot, bola, -1);
        WaitTime(0.5f);
        default_configs(player, bot, bola);
    }
    player->update();
    bot->update(bola->posi.y);
    bola->update();
}

int main(void)
{
    int width = 1280, height = 720;
    Paddle player, player2;
    Cpu bot;
    short paused = 1;
    Ball bola;

    InitWindow(width, height, "Pong de cria");
    SetTargetFPS(60);

    bola.p1 = bola.p2 = 0;
    default_configs(&player, &bot, &bola);
    while (!WindowShouldClose())
    {
        if(paused != 1) {
            updating(&player, &bot, &bola);
        }
        drawing(&player, &bot, &bola, paused);
        if (IsKeyReleased(KEY_P))
            paused *= -1;
    }

    CloseWindow();

    return 0;
}
