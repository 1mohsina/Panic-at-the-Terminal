#ifndef MAZE_H
#define MAZE_H

const int MAZE_ROWS = 31;
const int MAZE_COLS = 35;
const int ROOM_COUNT = 9;

const int BOX_H =7;
const int BOX_W =11;

const int ROOM_LOCKED =0;
const int ROOM_UNLOCKED =1;
const int ROOM_COMPLETED =2;

const int MOVE_BLOCKED =0;
const int MOVE_OK =1;
const int MOVE_ROOM =2;
const int MOVE_LOCKED =3;

class maze{
    public:
            maze();
            void generate();

            bool isWall(int y, int x) const;
            int roomAt(int y, int x) const;
            void render (int playerY=-1,int playerX=-1) const;
            int getStartY() const;
            int getStartX() const;
            int getRoomState(int id) const;
            void setRoomState(int id,int state);


    private:
           char grid[MAZE_ROWS][MAZE_COLS];
           int cellRoom[MAZE_ROWS][MAZE_COLS];

           int roomTop[ROOM_COUNT];
           int roomLeft[ROOM_COUNT];
           int roomY[ROOM_COUNT];
           int roomX[ROOM_COUNT];
           int roomState[ROOM_COUNT];

           void carve(int y, int x);
           void placeRooms();
};


#endif
