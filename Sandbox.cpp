#include "raylib.h"
#include <string>
#include <iostream>
using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define BLOCK_SIZE 4
#define WORLD_HEIGHT HEIGHT/BLOCK_SIZE
#define WORLD_WIDTH WIDTH/BLOCK_SIZE
#define DELAY 0.05


typedef enum blocks {
    EMPTY,
    SAND,
    WATER,
    ROCK
};

blocks current_block = ROCK;
blocks world[WORLD_WIDTH][WORLD_HEIGHT] = { EMPTY };

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
                default: 
                    break;
            }
                
        }
    }
}

void update_world() {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = WORLD_HEIGHT-1; j >= 0; j--) {
            if (world[i][j] == SAND) {
                if (j + 1 < WORLD_HEIGHT ) {
                    if (world[i][j + 1]==EMPTY) {
                        world[i][j] = EMPTY;
                        world[i][j+1] = SAND;
                    }
                    else if (world[i][j + 1] == WATER) {
                        world[i][j] = WATER;
                        world[i][j + 1] = SAND;
                    }
                }   
            }
            else if (world[i][j] == WATER) {
                if (j + 1 < WORLD_HEIGHT) {
                    if (world[i][j + 1] == EMPTY) {
                        world[i][j] = EMPTY;
                        world[i][j + 1] = WATER;
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

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int x = (int)ceil((float)GetMouseX() / 4);
            int y = (int)ceil((float)GetMouseY() / 4);
            place_block(x, y, current_block);        
        }

        draw_world();
        DrawText(("Current element: " + currentBlockText).c_str(), 0, 0, 20, WHITE);

        if (GetTime() - time >= DELAY) {
            update_world();
            time = GetTime();
        }
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
