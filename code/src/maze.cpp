#include <iostream>
#include <cstdlib>
#include <cctype>
#include "maze.h"
#include <algorithm>
using namespace std;

namespace
{
    const int START_Y = MAZE_ROWS/2;
    const int START_X = MAZE_COLS/2;

    const int MIN_START_DIST =12;
    const int MAX_START_DIST = 110;
    const int MIN_ROOM_GAP =8;

    const int rowChange[4] = {-1,1,0,0};
    const int colChange[4] = {0,0,-1,1};

    int absvalue(int x)
    {
        return x<0 ? -x : x;
    }
}

    maze::maze()
    {
        generate();
    }

    void maze::generate()
    {
        steps = 0;
        do
        {
        for (int i=0;i<MAZE_ROWS;i++)
            for (int j=0;j<MAZE_COLS;j++)
                grid[i][j]='#';

            int startCellY = 1+2*(rand() % ((MAZE_ROWS-1)/2));
            int startCellX = 1+2*(rand() % ((MAZE_COLS-1)/2));
            carve(startCellY, startCellX);
        }while (!placeRooms());

        playerY = START_Y;
        playerX = START_X;

        for (int i=0;i<ROOM_COUNT;i++)
            roomState[i]=ROOM_LOCKED;
        for (int i=0;i<3;i++)
            roomState[i]=ROOM_UNLOCKED;
    }

    void maze::carve(int y,int x)
    {
        grid[y][x]=' ';

        int order[4] = {0,1,2,3};
        for (int i=3;i>0;i--)
        {
            int j=rand() % (i+1);
            int temp=order[i];
            order[i]=order[j];
            order[j]=temp;
        }

        for (int k=0;k<4;k++)
        {
            int d=order[k];
            int ny=y+2*rowChange[d];
            int nx=x+2*colChange[d];

            if (ny>=1 && ny<MAZE_ROWS-1 && nx>=1 && nx<MAZE_COLS-1 && grid[ny][nx]=='#')
            {
                grid[y+rowChange[d]][x+colChange[d]]=' ';
                carve(ny,nx);
            }
        }
    }

    bool maze::placeRooms()
    {
        int startOpen =0;
        for (int d=0;d<4;d++)
            if (grid[START_Y + rowChange[d]][START_X + colChange[d]] !='#')
                startOpen++;
        if (startOpen<3)
            return false;

        int dist[MAZE_ROWS][MAZE_COLS];
        for (int i=0;i<MAZE_ROWS;i++)
            for (int j=0;j<MAZE_COLS;j++)
                dist[i][j]=-1;

        int qy[MAZE_ROWS * MAZE_COLS],qx[MAZE_ROWS * MAZE_COLS];
        int head=0,tail=0;
        qy[tail]=START_Y;
        qx[tail]=START_X;
        tail++;
        dist[START_Y][START_X]=0;

        while (head<tail)
        {
            int y=qy[head];
            int x=qx[head];
            head++;
            for (int d=0;d<4;d++)
            {
                int ny=y+rowChange[d];
                int nx=x+colChange[d];
                if (grid[ny][nx]=='#' || dist[ny][nx]!=-1)
                    continue;
                dist[ny][nx]=dist[y][x]+1;
                qy[tail]=ny;
                qx[tail]=nx;
                tail++;
            }
        }

        int candY[MAZE_ROWS*MAZE_COLS],candX[MAZE_ROWS*MAZE_COLS];
        int candCount=0;
        for (int y=1;y<MAZE_ROWS;y+=2)
        {
            for (int x=1;x<MAZE_COLS;x+=2)
            {
                if (grid[y][x]=='#')
                    continue;
                if (y==START_Y && x==START_X)
                    continue;

                int open=0;
                for (int d=0;d<4;d++)
                    if (grid[y + rowChange[d]][x + colChange[d]] !='#')
                        open++;

                if (open==1 && dist[y][x]>=MIN_START_DIST && dist[y][x]<=MAX_START_DIST)
                {
                    candY[candCount]=y;
                    candX[candCount]=x;
                    candCount++;
                }
            }
        }
        if (candCount < ROOM_COUNT)
            return false;

        for (int i=candCount-1;i>=0;i--)
        {
            int j=rand()%(i+1);
            int ty=candY[i]; candY[i]=candY[j]; candY[j]=ty;
            int tx=candX[i]; candX[i]=candX[j]; candX[j]=tx;
        }

        int pickY[ROOM_COUNT],pickX[ROOM_COUNT];
        int picked=0;
        for (int i=0;i<candCount && picked<ROOM_COUNT;i++)
        {
            bool ok=true;
            for (int k=0;k<picked;k++){
                if (absvalue(candY[i]-pickY[k]) + absvalue(candX[i]-pickX[k]) <MIN_ROOM_GAP)
                {
                    ok=false;
                    break;
                }
            }
            if (ok)
            {
                pickY[picked]=candY[i];
                pickX[picked]=candX[i];
                picked++;
            }
        }

        if (picked < ROOM_COUNT)
            return false;

        for (int i=0;i<ROOM_COUNT-1;i++)
        {
            int minIdx=i;
            for (int j=i+1;j<ROOM_COUNT;j++)
                if (dist[pickY[j]][pickX[j]]< dist[pickY[minIdx]][pickX[minIdx]])
                    minIdx = j;
            int ty=pickY[i]; pickY[i]=pickY[minIdx]; pickY[minIdx]=ty;
            int tx=pickX[i]; pickX[i]=pickX[minIdx]; pickX[minIdx]=tx;
        }

        for (int i=0;i<ROOM_COUNT;i++)
        {
            roomY[i]=pickY[i];
            roomX[i]=pickX[i];
        }
        return true;
    }

    int maze::move(char key)
    {
        key=tolower(key);
        int ny=playerY,nx=playerX;
        if (key=='w')
            ny--;
        else if (key=='s')
            ny++;
        else if (key=='a')
            nx--;
        else if (key=='d')
            nx++;
        else
            return MOVE_BLOCKED;
    if (ny<0 || ny>=MAZE_ROWS || nx<0 || nx>=MAZE_COLS)
        return MOVE_BLOCKED;
    if (grid[ny][nx]=='#')
        return MOVE_BLOCKED;

        int room = roomAt(ny,nx);
        if (room !=-1 && roomState[room] == ROOM_LOCKED)
            return MOVE_LOCKED;

        playerY=ny;
        playerX=nx;
        steps++;

        if (room !=-1 && roomState[room] == ROOM_UNLOCKED)
            return MOVE_ROOM;
        return MOVE_OK;
    }

    void maze::draw() const
    {
        cout << "\033[2J\033[H";
        cout << "Steps: " << steps << "  (W/A/S/D = move,Q=quit)\n";
        cout << "\033[94mBlue\033[0m=unlocked | \033[92mGreen\033[0m=cleared | \033[90mGrey\033[0m=locked\n\n";

        for (int i=0;i<MAZE_ROWS;i++)
        {
            for (int j=0;j<MAZE_COLS;j++)
            {
                if (i==playerY && j==playerX)
                {
                    cout << "\033[93mP\033[0m";
                }
                else
                {
                    int room=roomAt(i,j);
                    if (room !=-1)
                    {
                        if (roomState[room] == ROOM_LOCKED)
                            cout << "\033[90m" << room+1 << "\033[0m";
                        else if (roomState[room] == ROOM_UNLOCKED)
                            cout << "\033[94m" << room+1 << "\033[0m";
                        else
                            cout << "\033[92m" << room+1 << "\033[0m";
                    }
                    else
                    {
                        cout << grid[i][j];
                    }
                }
            }
            cout << "\n";
        }
    }
    int maze::roomAt(int y,int x) const
    {
        for (int i=0;i<ROOM_COUNT;i++)
            if (roomY[i]==y && roomX[i]==x)
                return i;
        return -1;
    }

    int maze::getRoomState(int id) const
    {
        return roomState[id];
    }
    void maze::setRoomState(int id,int state)
    {
        roomState[id]=state;
    }
    int maze::getSteps() const
    {
        return steps;
    }
    int maze::getPlayerY() const
    {
        return playerY;
    }
    int maze::getPlayerX() const
    {
        return playerX;
    }
