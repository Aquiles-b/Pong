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
    bool hab1, is_bot;
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

void update_bot(float bola) {

    if (bola > posi.height/2 + posi.y)
        posi.y += speed;

    if (bola < posi.height/2 + posi.y)
        posi.y -= speed;

    if (GetRandomValue(0, 3333) == 3000 && hab1) {
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

void default_configs(Paddle *player, Paddle *player2, Ball *bola) {
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

void draw_controls(char ctr[], float posi)
{
    float h = GetScreenHeight();
    float a = 70;
    DrawRectangleLinesEx(Rectangle{posi, h * 0.60f, a, a}, 0.9f, WHITE);
    DrawRectangleLinesEx(Rectangle{posi, h * 0.60f + a, a, a}, 0.9f, WHITE);
    DrawRectangleLinesEx(Rectangle{posi + 2*a, h * 0.60f + a, a, a}, 0.9f, WHITE);

    float alt = h * 0.615f;
    DrawText(TextFormat("%c%c", ctr[0], ctr[1]), posi + 14, alt, 50, WHITE);
    DrawText(TextFormat("%c%c", ctr[2], ctr[3]), posi + 14,  alt + a, 50, WHITE);

    DrawText(TextFormat("%c", ctr[4]), posi + 14 + 2*a, alt + a, 50, WHITE);
    
}

void drawing(Paddle *player, Paddle *player2, Ball *bola, short paused)
{
    BeginDrawing();
    ClearBackground(BLACK);
    player->draw();
    player2->draw();
    bola->draw();
    DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE);
    DrawText(TextFormat("%d", bola->p1), GetScreenWidth() * 0.25f, 20, 60, WHITE);
    DrawText(TextFormat("%d", bola->p2), GetScreenWidth() * 0.75f, 20, 60, WHITE);

    if (paused == 1) {
        char ctr1[] = "W S E";
        char ctr2[] = "UpDw0";
        DrawRectangle(0, 0, GetScreenWidth() * 0.40f, GetScreenHeight() * 0.3f, BLACK);
        DrawText("Poooong", 20, 10, 80, WHITE);
        DrawText("P -> pause\nR -> Restart", GetScreenWidth() * 0.05f, GetScreenHeight() * 0.6f, 30, WHITE);
        if (player2->is_bot) {
            DrawText("-> Player vs CPU", 40, 90, 50, SKYBLUE);
            DrawText("   Player vs Player", 40, 140, 50, WHITE);
            draw_controls(ctr1, GetScreenWidth() * 0.25f);
        }
        else {
            DrawText("   Player vs CPU", 40, 90, 50, WHITE);
            DrawText("-> Player vs Player", 40, 140, 50, SKYBLUE);
            draw_controls(ctr1, GetScreenWidth() * 0.25f);
            draw_controls(ctr2, GetScreenWidth() * 0.75f);
        }
    }
    EndDrawing();
}

void updating(Paddle *player, Paddle *player2, Ball *bola) {

    if (CheckCollisionCircleRec(bola->posi, bola->raio, player->posi)) {
        bola->speed_x *= -1.1f;
        player->speed *= 1.1f;
        player2->speed *= 1.1f;
    }
    if (CheckCollisionCircleRec(bola->posi, bola->raio, player2->posi)) {
        bola->speed_x *= -1.1f;
        player->speed *= 1.1f;
        player2->speed *= 1.1f;
    }
    if (bola->is_point) {
        bola->cor = GREEN;
        drawing(player, player2, bola, -1);
        WaitTime(0.5f);
        default_configs(player, player2, bola);
    }

    player->update();
    if (player2->is_bot)
        player2->update_bot(bola->posi.y);
    else
        player2->update();

    bola->update();
}

void updating_pause(Paddle *player1, Paddle *player2, Ball *bola)
{
    if (IsKeyPressed(KEY_W))
        player2->is_bot = true;
    if (IsKeyPressed(KEY_S))
        player2->is_bot = false;
    if (IsKeyPressed(KEY_R)) {
        default_configs(player1, player2, bola);
        bola->p1 = bola->p2 = 0;
    }
}

int main(void)
{
    int width = 1280, height = 720;
    Paddle player, player2;
    short paused = 1;
    Ball bola;

    InitWindow(width, height, "Pong de cria");
    bola.p1 = bola.p2 = 0;
    SetTargetFPS(60);
    

    default_configs(&player, &player2, &bola);
    player2.is_bot = false;
    while (!WindowShouldClose())
    {
        if(paused != 1)
            updating(&player, &player2, &bola);
        else
            updating_pause(&player, &player2, &bola);
        drawing(&player, &player2, &bola, paused);

        if (IsKeyReleased(KEY_P))
            paused *= -1;
    }

    CloseWindow();

    return 0;
}
