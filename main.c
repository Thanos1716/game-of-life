#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int min(x, y) {
    if (x > y) return y;
    return x;
}

int main() {

    const int cellWidth           = 10;
    const int cellHeight          = cellWidth;
    const int cellCountVertical   = 70;
    const int cellCountHorizontal = 100;
    const int cellPadding         = 1;
    const int screenWidth         = (cellWidth  + cellPadding) * cellCountHorizontal + cellPadding;
    const int screenHeight        = (cellHeight + cellPadding) * cellCountVertical   + cellPadding;

    InitWindow(screenWidth, screenHeight, "Conway's Game of Life");
    SetTargetFPS(60);

    bool board[cellCountVertical][cellCountHorizontal];
    for (int y = 0; y < cellCountVertical; ++y) {
        for (int x = 0; x < cellCountHorizontal; ++x) {
            board[y][x] = false;
        }
    }

    bool running   = false;
    bool mode      = true;
    bool next      = false;
    bool text      = false;
    bool hideZeros = true;
    float frameTotal = 0.1f;
    float frameRemaining = frameTotal;

    bool birthRules[9] =   {0, 0, 0, 1, 0, 0, 0, 0, 0};
    bool surviveRules[9] = {0, 0, 1, 1, 0, 0, 0, 0, 0};

    while (!WindowShouldClose()) {

        // Get the coordinates of the cell the mouse is closest to
        int mouseX = (GetMouseX() - cellPadding / 2) / (cellWidth  + cellPadding);
        int mouseY = (GetMouseY() - cellPadding / 2) / (cellHeight + cellPadding);

        // On left click set the mode based on the cell the mouse is over
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (board[mouseY][mouseX]) mode = false;
            else mode = true;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) board[mouseY][mouseX] = mode;

        // Increase/decrease the frame speed
        if (IsKeyDown(KEY_UP))   frameTotal /= 1.02f;
        if (IsKeyDown(KEY_DOWN)) frameTotal *= 1.02f;

        if (running) frameRemaining -= GetFrameTime();

        // Key checks & updates
        if (IsKeyPressed(KEY_SPACE)) running = !running;
        if (IsKeyPressed(KEY_T)) text        = !text;
        if (IsKeyPressed(KEY_Z)) hideZeros   = !hideZeros;
        if (IsKeyPressed(KEY_RIGHT)) next    = true;
        if (IsKeyPressed(KEY_F)) frameTotal  = 0.1f;
        if (IsKeyPressed(KEY_C)) {
            for (int y = 0; y < cellCountVertical; ++y) {
                for (int x = 0; x < cellCountHorizontal; ++x) {
                    board[y][x] = false;
                }
            }
        }

        BeginDrawing();

        ClearBackground(DARKGRAY);

        // Draw the cells
        for (int y = 0; y < cellCountVertical; ++y) {
            for (int x = 0; x < cellCountHorizontal; ++x) {
                DrawRectangle(x * (cellWidth + cellPadding) + cellPadding, y * (cellHeight + cellPadding) + cellPadding,
                              cellWidth, cellHeight, board[y][x] ? (Color){(256 * y / cellCountVertical) % 256, (256 * (cellCountVertical - y - 1) / cellCountVertical) % 256, (256 * x / cellCountHorizontal) % 256, 255 } : BLACK);
            }
        }

        bool nextBoard[cellCountVertical][cellCountHorizontal];
        for (int y = 0; y < cellCountVertical; ++y) {
            for (int x = 0; x < cellCountHorizontal; ++x) {

                int count = 0;
                nextBoard[y][x] = board[y][x];

                // Count up surrounding live cells
                for (int i = -1; i < 2; ++i) {
                    for (int j = -1; j < 2; ++j) {
                        if (board[y + j][x + i] && !(i == 0 && j == 0) && 0 <= x + i &&
                            x + i < cellCountHorizontal && 0 <= y + j & y + j < cellCountVertical) count++;
                    }
                }

                // Update next board to next iteration based on state of current board
                if (frameRemaining <= 0 || next) {
                    if (board[y][x]) {
                        nextBoard[y][x] = surviveRules[count];
                    } else {
                        nextBoard[y][x] = birthRules[count];
                    }
                }

                // Draw number of adjacent live cells on cell
                if (text && !(hideZeros && count == 0)) DrawText(TextFormat("%i",
                    count), x * (cellWidth + cellPadding) + cellPadding + cellWidth / 4,
                    y * (cellHeight + cellPadding) + cellPadding, min(cellWidth, cellHeight), LIGHTGRAY); // cellwidth / 4 is arbitary, TODO: find better value
                }
            }

            for (int y = 0; y < cellCountVertical; ++y) {
                for (int x = 0; x < cellCountHorizontal; ++x) {
                    board[y][x] = nextBoard[y][x];
                }
            }


        if (frameRemaining <= 0) frameRemaining = frameTotal;

        next = false;

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
