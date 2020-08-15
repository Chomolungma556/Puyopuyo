#include <iostream>
#include <conio.h>
#include <time.h>

static const int FIELD_WIDTH = 8;
static const int FIELD_HEIGHT= 14;

static const int PUYO_START_X = 3;
static const int PUYO_START_Y = 0;

static const int PUYO_TYPE = 4;

enum
{
    CELL_NONE,
    CELL_WALL,
    CELL_PUYO0,
    CELL_PUYO1,
    CELL_PUYO2,
    CELL_PUYO3,
    CELL_MAX
};

enum
{
    PUYO_ANGLE_0,
    PUYO_ANGLE_90,
    PUYO_ANGLE_180,
    PUYO_ANGLE_270,
    PUYO_ANGLE_MAX
};

int cells[FIELD_WIDTH][FIELD_HEIGHT];
int displayBuffer[FIELD_WIDTH][FIELD_HEIGHT];

char cellNames[][2 + 1] =
{
    "ÅE", //CELL_NONE
    "Å°", //CELL_WALL
    "ÅZ", //CELL_PUYO0
    "Å¢", //CELL_PUYO1
    "Å†", //CELL_PUYO2
    "Åô"  //CELL_PUYO3
};

int puyoX = PUYO_START_X;
int puyoY = PUYO_START_Y;
int puyoType;
int puyoAngle;

int puyoSubPositions[][2] =
{
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0},
};

void display();
bool isPuyoOverlap(int argPuyoX, int argPuyoY, int argPuyoAngle);

int main()
{
    for (int y = 0; y < FIELD_HEIGHT; ++y)
    {
        cells[0][y] = CELL_WALL;
        cells[FIELD_WIDTH - 1][y] = CELL_WALL;
    }

    for (int x = 0; x < FIELD_WIDTH; ++x)
    {
        cells[x][FIELD_HEIGHT - 1] = CELL_WALL;
    }

    time_t t = 0;
    while (true)
    {
        if (t < time(NULL))
        {
            t = time(NULL);

            if (false == isPuyoOverlap(puyoX, puyoY + 1, puyoAngle))
            {
                ++puyoY;
            }
            else
            {
                int puyoSubX = puyoX + puyoSubPositions[puyoAngle][0];
                int puyoSubY = puyoY + puyoSubPositions[puyoAngle][1];

                cells[puyoX][puyoY] = CELL_PUYO0 + puyoType;
                cells[puyoSubX][puyoSubY] = CELL_PUYO0 + puyoType;

                puyoX = PUYO_START_X;
                puyoY = PUYO_START_Y;
                puyoAngle = PUYO_ANGLE_0;
            }

            display();
        }

        if (_kbhit())
        {
            int dummyPuyoX = puyoX;
            int dummyPuyoY = puyoY;
            int dummyPuyoAngle = puyoAngle;

            switch (_getch())
            {
            case 'w':
                --dummyPuyoY;
                break;
            case 's':
                ++dummyPuyoY;
                break;
            case 'a':
                --dummyPuyoX;
                break;
            case 'd':
                ++dummyPuyoX;
                break;
            case ' ':
                dummyPuyoAngle = (++dummyPuyoAngle) % PUYO_ANGLE_MAX;
                break;
            }
            if (false == isPuyoOverlap(dummyPuyoX, dummyPuyoY, dummyPuyoAngle))
            {
                puyoX = dummyPuyoX;
                puyoY = dummyPuyoY;
                puyoAngle = dummyPuyoAngle;
            }
            display();
        }
    }
    return 0;
}

void display()
{
    system("cls");
    memcpy(displayBuffer, cells, sizeof cells);
    int puyoSubX = puyoX + puyoSubPositions[puyoAngle][0];
    int puyoSubY = puyoY + puyoSubPositions[puyoAngle][1];

    displayBuffer[puyoX][puyoY] = CELL_PUYO0 + puyoType;
    displayBuffer[puyoSubX][puyoSubY] = CELL_PUYO0 + puyoType;

    for (int y = 0; y < FIELD_HEIGHT; ++y)
    {
        for (int x = 0; x < FIELD_WIDTH; ++x)
        {
            std::cout << cellNames[displayBuffer[x][y]];
        }
        std::cout << std::endl;
    }
}

bool isPuyoOverlap(int argPuyoX, int argPuyoY, int argPuyoAngle)
{
    if (CELL_NONE != cells[argPuyoX][argPuyoY])
    {
        return true;
    }

    int puyoSubX = argPuyoX + puyoSubPositions[argPuyoAngle][0];
    int puyoSubY = argPuyoY + puyoSubPositions[argPuyoAngle][1];
    if (CELL_NONE != cells[puyoSubX][puyoSubY])
    {
        return true;
    }

    return false;
}
