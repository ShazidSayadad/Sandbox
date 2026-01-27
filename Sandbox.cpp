#include "raylib.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define BLOCK_SIZE 4
#define WORLD_HEIGHT HEIGHT/BLOCK_SIZE
#define WORLD_WIDTH WIDTH/BLOCK_SIZE
#define DELAY 0.02


typedef enum blocks {
    EMPTY,
    SAND,
    WATER,
    ROCK,
    LAVA,
    AIR
};

blocks current_block = ROCK;
blocks world[WORLD_WIDTH][WORLD_HEIGHT] = { EMPTY };
vector <pair<pair<int, int>, blocks>> states;

int pixels = 0;

void place_block(int x, int y, blocks type) {

    if (x < 0 || y< 0 || x>WORLD_WIDTH || y>WORLD_HEIGHT)
        return;

    if (world[x][y] == EMPTY) {
        world[x][y] = type;
        pixels++;
    }
        
}

void draw_pixel(int x,int y,Color color) {
    int temp = y;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            DrawRectangle(x + j, temp, 1, 1, color);
        }
        temp++;
    }
}

void draw_world() {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            switch (world[i][j]) {

                case SAND:
                    draw_pixel(i * BLOCK_SIZE, j * BLOCK_SIZE, BROWN);
                    break;
                case WATER:
                    draw_pixel(i * BLOCK_SIZE, j * BLOCK_SIZE, BLUE);
                    break;
                case ROCK:
                    draw_pixel(i * BLOCK_SIZE, j * BLOCK_SIZE, GRAY);
                    break;
                case LAVA:
                    draw_pixel(i * BLOCK_SIZE, j * BLOCK_SIZE, ORANGE);
                    break;
                case AIR:
                    draw_pixel(i * BLOCK_SIZE, j * BLOCK_SIZE, WHITE);
                    break;
                default: 
                    break;
            }
                
        }
    }
}

void update_sand(int i, int j) {
    if (j + 1 < WORLD_HEIGHT) {
        if (world[i][j + 1] == EMPTY) {
            world[i][j] = EMPTY;
            world[i][j + 1] = SAND;
        }
        else if (world[i][j + 1] == WATER) {
            world[i][j] = WATER;
            world[i][j + 1] = SAND;
        }
    }
}

void update_water(int i, int j) {
    if (j + 1 < WORLD_HEIGHT) {
        if (world[i][j + 1] == EMPTY) {
            world[i][j] = EMPTY;
            world[i][j + 1] = WATER;
        }
        else if (world[i][j + 1] == LAVA) {
            world[i][j] = EMPTY;
            world[i][j + 1] = AIR;
        }
        else {
            if (i - 1 > 0 && i + 1 < WORLD_WIDTH) {
                if (world[i + 1][j] == EMPTY && world[i - 1][j] == EMPTY) {
                    int side = (rand() % 2 == 0) ? -1 : 1;
                    world[i + side][j] = WATER;
                    world[i][j] = EMPTY;
                }
                else if (world[i + 1][j] == EMPTY) {
                    world[i + 1][j] = WATER;
                    world[i][j] = EMPTY;
                }
                else if (world[i - 1][j] == EMPTY) {
                    world[i - 1][j] = WATER;
                    world[i][j] = EMPTY;
                }
            }
        }
    }
}

void update_lava(int i, int j) {
    if (j + 1 < WORLD_HEIGHT) {
        if (world[i][j + 1] == EMPTY) {
            world[i][j] = EMPTY;
            world[i][j + 1] = LAVA;
        }
        else if (world[i][j + 1] == WATER) {
            world[i][j] = EMPTY;
            world[i][j + 1] = AIR;
        }
    }
}

void update_air() {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            if (j - 1 == 0)
                world[i][j] = EMPTY;
            if (world[i][j] == AIR) {
                if (j - 1 > 0) {
                    if (world[i][j - 1] == ROCK) {
                        
                        world[i][j] = EMPTY;
                    }
                    else {
                        world[i][j - 1] = AIR;
                        world[i][j] = EMPTY;
                    }
                }
            }
        }
    }
}

void update_world() {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = WORLD_HEIGHT-1; j >= 0; j--) {
            if (world[i][j] == SAND) {
                update_sand(i, j);
            }
            else if (world[i][j] == WATER) {
                update_water(i, j);
            }
            else if (world[i][j] == LAVA) {
                update_lava(i, j);
            }
        }
    }
}


int main(void)
{
    InitWindow(WIDTH, HEIGHT, "raylib works!");
    SetTargetFPS(60);
    double time = 0;
    string currentBlockText="Rock";

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (IsKeyPressed(KEY_ONE)){
            current_block = ROCK;
            currentBlockText = "Rock";
        }
            
        if (IsKeyPressed(KEY_TWO)) {
            current_block = SAND;
            currentBlockText = "Sand";
        }
            
        if (IsKeyPressed(KEY_THREE)) {
            current_block = WATER;
            currentBlockText = "Water";
        }

        if (IsKeyPressed(KEY_FOUR)) {
            current_block = LAVA;
            currentBlockText = "Lava";
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int x = (int)ceil((float)GetMouseX() / 4);
            int y = (int)ceil((float)GetMouseY() / 4);
            place_block(x, y, current_block);        
        }

        draw_world();
        DrawText(("Current element: " + currentBlockText).c_str(), 0, 0, 20, WHITE);

        if (GetTime() - time >= DELAY) {
            update_world();
            update_air();
            time = GetTime();
        }
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
