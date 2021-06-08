#include <raylib.h>
#include <stdio.h>

int min(x, y) {
    if (x > y) {
        return y;
    }
    return x;
}

int main() {

    const int cellWidth           = 10;
    const int cellHeight          = cellWidth;
    const int cellCountVertical   = 70;
    const int cellCountHorizontal = 110;
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

    bool running = true;
    bool mode    = true;
    bool next    = false;
    bool text    = false;
    float totalTimeOfFrame = 0.1f;
    float timeUntilNextFrame = totalTimeOfFrame;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) running = !running;

        if (IsKeyDown(KEY_UP))   totalTimeOfFrame /= 1.02;
        if (IsKeyDown(KEY_DOWN)) totalTimeOfFrame *= 1.02;

        timeUntilNextFrame -= GetFrameTime();

        for (int y = 0; y < cellCountVertical; ++y) {
            for (int x = 0; x < cellCountHorizontal; ++x) {
                if (board[y][x] == true) {
                    DrawRectangle(x * (cellWidth + cellPadding) + cellPadding, y * (cellHeight + cellPadding) + cellPadding, cellWidth, cellHeight, WHITE);
                } else {
                    DrawRectangle(x * (cellWidth + cellPadding) + cellPadding, y * (cellHeight + cellPadding) + cellPadding, cellWidth, cellHeight, DARKGRAY);
                }
            }
        }

        if (IsKeyPressed(KEY_RIGHT)) next = true;

        if (timeUntilNextFrame < 0 || next/*running || next*/) { /// HJHLJKHKJHLHJHLJLHLHKJHJLHJH***********
            bool nextBoard[cellCountVertical][cellCountHorizontal];
            for (int y = 0; y < cellCountVertical; ++y) {
                for (int x = 0; x < cellCountHorizontal; ++x) {
                    nextBoard[y][x] = board[y][x];
                }
            }

            for (int y = 0; y < cellCountVertical; ++y) {
                for (int x = 0; x < cellCountHorizontal; ++x) {
                    int count = 0;
                    for (int i = -1; i < 2; ++i) {
                        for (int j = -1; j < 2; ++j) {
                            if (board[y + j][x + i] && !(i == 0 && j == 0) && 0 <= x + i && x + i < cellCountHorizontal && 0 <= y + j && y + j < cellCountVertical) {
                                count++;
                            }
                        }
                    }

                    if (board[y][x]) { // if cell is alive
                        if ((count > 3 || count < 2) && (running || next)) {
                            nextBoard[y][x] = false; // die
                        }

                    }
                    else { // if cell is dead
                        if (count == 3 && (running || next)) {
                            nextBoard[y][x] = true; // be born
                        }
                    }


                if (text) DrawText(TextFormat("%i", count), x * (cellWidth + cellPadding) + cellPadding + cellWidth/4, y * (cellHeight + cellPadding) + cellPadding, min(cellWidth, cellHeight), BLACK);
                }
            }

            for (int y = 0; y < cellCountVertical; ++y) {
                for (int x = 0; x < cellCountHorizontal; ++x) {
                    board[y][x] = nextBoard[y][x];
                }
            }
            timeUntilNextFrame = totalTimeOfFrame;
        }
        next = false;

        BeginDrawing();
        ClearBackground(BLACK);

        int mouseX = (GetMouseX() - cellPadding / 2) / (cellWidth  + cellPadding);
        int mouseY = (GetMouseY() - cellPadding / 2) / (cellHeight + cellPadding);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (board[mouseY][mouseX]) {
                mode = false;
            } else {
                mode = true;
            }
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            board[mouseY][mouseX] = mode;
        }

        if (IsKeyPressed(KEY_C)) {
            for (int y = 0; y < cellCountVertical; ++y) {
                for (int x = 0; x < cellCountHorizontal; ++x) {
                    board[y][x] = false;
                }
            }
        }

        if (IsKeyPressed(KEY_T)) text = !text;

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
