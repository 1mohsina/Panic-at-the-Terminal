//
// Created by nusra on 11/24/2026.
//

#ifndef PANIC_AT_THE_TERMINAL_MAZE_H
#define PANIC_AT_THE_TERMINAL_MAZE_H

const int MAZE_ROWS = 19;
const int MAZE_COLS = 35;
const int ROOM_COUNT = 9;

const int ROOM_LOCKED =0, ROOM_UNLOCKED =1, ROOM_COMPLETED = 2;

const int MOVE_BLOCKED =0;
const int MOVE_OK =1;
const int MOVE_ROOM =2;
const int MOVE_LOCKED =3;

class maze{
    public:
            maze();
            void generate();
            int move(char key);
            void draw() const;
            int roomAt(int y, int x) const;
            int getRoomState(int id) const;
            void setRoomState(int id,int state);
            int getSteps() const;
            int getPlayerY() const;
            int getPlayerX() const;

    private:
           char grid[MAZE_ROWS][MAZE_COLS];
           int roomY[ROOM_COUNT], roomX[ROOM_COUNT], roomState[ROOM_COUNT];
           int playerY, playerX, steps;
           void carve(int y, int x);
           bool placeRooms();
};


#endif //PANIC_AT_THE_TERMINAL_MAZE_H
