//
// Created by nusra on 11/24/2026.
//

#ifndef PANIC_AT_THE_TERMINAL_MAZE_H
#define PANIC_AT_THE_TERMINAL_MAZE_H

#include <string>


namespace maze{
    const int ROOM_COUNT = 9;   // 3 room state: Unlocked-Blue, Locked-Grey, Cleared-Green

    void generate_maze();
    void draw_maze();
    int movePlayer(char key);
    void setRoomState(int room,int state);
    int getRoomState(int room);
    void updateWaves();
    bool allRoomsWon();
    int getSteps();
    void setMessage(const std :: string &msg);
};


#endif //PANIC_AT_THE_TERMINAL_MAZE_H
