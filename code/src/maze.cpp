#include <iostream>
#include <cstdlib>
#include <ctime>
#include "maze.h"
using namespace std;

namespace
{
    const int START_Y = MAZE_ROWS/2;
    const int START_X = MAZE_COLS/2;

    const int rowChange[4] = {-1,1,0,0};
    const int colChange[4] = {0,0,-1,1};

    const char* stateColor(int state)
    {
        if (state==ROOM_UNLOCKED)
            return "\033[94m";
        if (state==ROOM_COMPLETED)
            return "\033[92m";
        return "\033[90m";
    }
}

    maze::maze()
{
    static bool seeded=false;
    if (!seeded)
    {
        srand(unsigned(time(0)));
        seeded=true;
    }
    generate();
}

    void maze::generate()
{
    for (int i=0;i<MAZE_ROWS;i++)
        for (int j=0;j<MAZE_COLS;j++)
        {
            grid[i][j]='#';
            cellRoom[i][j]=-1;
        }
    placeRooms();

    carve(START_Y,START_X);

        for (int i=0;i<ROOM_COUNT;i++)
            roomState[i]=ROOM_LOCKED;
        for (int i=0;i<3;i++)
            roomState[i]=ROOM_UNLOCKED;
    }


    void maze::placeRooms()
{
    static const int top[ROOM_COUNT]={12,0,16,0,0,24,24,24,8};
    static const int left[ROOM_COUNT]={0,12,24,0,24,0,12,24,24};
    static const int doorY[ROOM_COUNT]={15,6,19,6,6,24,24,24,11};
    static const int doorX[ROOM_COUNT]={10,17,24,5,29,5,17,29,24};

    bool mirror=(rand()%2==1);

    for (int id=0;id<ROOM_COUNT;id++)
    {
        int t=top[id];
        int l=mirror ? (MAZE_COLS-BOX_W-left[id]):left[id];
        int dy=doorY[id];
        int dx=mirror?(MAZE_COLS-1-doorX[id]):doorX[id];

        roomTop[id]=t;
        roomLeft[id]=l;

        for (int r=t;r<t+BOX_H;r++)
            for (int c=l;c<l+BOX_W;c++)
            {
                cellRoom[r][c]=id;
                bool border=(r==t || r==t+BOX_H-1 || c==l || c==l+BOX_W-1);
                grid[r][c]=border ? '#' : 'R';
            }

        grid[dy][dx]='D';

        roomY[id]=dy;
        roomX[id]=dx;
    }
}


     void maze::carve(int y,int x)
{
    grid[y][x]=' ';

    int order[4] ={0,1,2,3};
    for (int i=3;i>=0;i--)
    {
        int j=rand()%(i+1);
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


    bool maze::isWall(int y,int x) const
   {
    if (y<0 || y>=MAZE_ROWS || x<0 || x>=MAZE_COLS)
        return true;
    return grid[y][x] !=' ' && grid[y][x] != 'D';

   }
    int maze::roomAt(int y,int x) const
    {
        for (int i=0;i<ROOM_COUNT;i++)
            if (roomY[i]==y && roomX[i]==x)
                return i;
        return -1;
    }

    void maze::render(int playerY,int playerX) const
    {
        cout << "\033[2J\033[H";
        cout << "\033[94mBlue\033[0m= unlocked | \033[92mGreen\033[0m=cleared | \033[90mGrey\033[0m=locked | \033[93m++\033[0m=door | \033[96m@\033[0m=you\n";
        for (int i=0;i<MAZE_ROWS;i++)
        {
            for (int j=0;j<MAZE_COLS;j++)
            {
                if (i==playerY && j==playerX)
                {
                    cout << "\033[96m@ \033[0m";
                }
                else if (grid[i][j]=='D')
                {
                    cout << "\033[93m++\033[0m";
                }
                else if (cellRoom[i][j]!=-1)
                {
                    int id=cellRoom[i][j];
                    if (grid[i][j]=='#')
                    {
                        cout << stateColor(roomState[id]) << "##\033[0m";
                    }
                    else if (i==roomTop[id]+3 && j>=roomLeft[id]+4 && j<=roomLeft[id]+6)
                    {
                        char label[7]={'R','O','O','M',' ',(char)('1' + id), '\0'};
                        int pos=(j-(roomLeft[id]+4))*2;
                        cout << stateColor(roomState[id]) << label[pos] << label[pos+1] << "\033[0m";
                    }
                    else
                    {
                        cout << "  ";
                    }
                }
                else if (grid[i][j]=='#')
                {
                    cout << "##";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << "\n";
        }
}

    int maze::getStartY() const
    {
        return START_Y;
    }
    int maze::getStartX() const
    {
    return START_X;
    }
    int maze::getRoomState(int id) const
    {
        return roomState[id];
    }
    void maze::setRoomState(int id,int state)
    {
        roomState[id]=state;
    }
