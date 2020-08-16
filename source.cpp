#include <iostream>
#include <conio.h>
#include <time.h>

static const int FIELD_WIDTH = 8;
static const int FIELD_HEIGHT= 14;

static const int PUYO_START_X = 3;
static const int PUYO_START_Y = 1;

static const int PUYO_TYPE = 4;
static const int CHAIN_NUMBER = 4;

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
bool isChecked[FIELD_WIDTH][FIELD_HEIGHT];

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
bool isLocked = false;

int puyoSubPositions[][2] =
{
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0},
};

void display();
bool isPuyoOverlap(int argPuyoX, int argPuyoY, int argPuyoAngle);
int getConnectedPuyoCount(int argX, int argY, int argType, int count);

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));
    for (int y = 0; y < FIELD_HEIGHT; ++y)
    {
        cells[0][y] = CELL_WALL;
        cells[FIELD_WIDTH - 1][y] = CELL_WALL;
    }

    for (int x = 0; x < FIELD_WIDTH; ++x)
    {
        cells[x][FIELD_HEIGHT - 1] = CELL_WALL;
    }

    puyoType = CELL_PUYO0 + rand() % PUYO_TYPE;

    time_t t = 0;
    while (true)
    {
        if (t < time(NULL))
        {
            t = time(NULL);

            if (!isLocked)
            {
                if (false == isPuyoOverlap(puyoX, puyoY + 1, puyoAngle))
                {
                    ++puyoY;
                }
                else
                {
                    int puyoSubX = puyoX + puyoSubPositions[puyoAngle][0];
                    int puyoSubY = puyoY + puyoSubPositions[puyoAngle][1];

                    cells[puyoX][puyoY] = puyoType;
                    cells[puyoSubX][puyoSubY] = puyoType;

                    puyoX = PUYO_START_X;
                    puyoY = PUYO_START_Y;
                    puyoAngle = PUYO_ANGLE_0;
                    puyoType = CELL_PUYO0 + rand() % PUYO_TYPE;

                    isLocked = true;
                }
            }

            if (isLocked)
            {
                isLocked = false;
                for (int y = FIELD_HEIGHT - 3; y >= 0; --y)
                {
                    for (int x = 1; x < FIELD_WIDTH - 1; ++x)
                    {
                        if ((CELL_NONE != cells[x][y]) && (CELL_NONE == cells[x][y + 1]))
                        {
                            cells[x][y + 1] = cells[x][y];
                            cells[x][y] = CELL_NONE;
                            isLocked = true;
                        }
                    }
                }

                if (!isLocked)
                {
                    memset(isChecked, 0, sizeof isChecked);
                    for (int y = 0; y < FIELD_HEIGHT - 1; ++y)
                    {
                        for (int x = 1; x < FIELD_WIDTH - 1; ++x)
                        {
                            if (getConnectedPuyoCount(x, y, cells[x][y], 0) >= CHAIN_NUMBER)
                            {
                                cells[x][y] = CELL_NONE;
                            }
                        }
                    }
                }
            }

            display();
        }

        if (_kbhit())
        {
            if (isLocked)
            {
                // operation is locked. 
            }
            else
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
    }
    return 0;
}

void display()
{
    system("cls");
    memcpy(displayBuffer, cells, sizeof cells);

    if (!isLocked)
    {
        int puyoSubX = puyoX + puyoSubPositions[puyoAngle][0];
        int puyoSubY = puyoY + puyoSubPositions[puyoAngle][1];

        displayBuffer[puyoX][puyoY] = puyoType;
        displayBuffer[puyoSubX][puyoSubY] = puyoType;
    }

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

int getConnectedPuyoCount(int argX, int argY, int argType, int count)
{
    if ((isChecked[argX][argY]) || (argType != cells[argX][argY]))
    {
        return count;
    }

    ++count;
    isChecked[argX][argY] = true;
    
    for (int i = 0; i < PUYO_ANGLE_MAX; ++i)
    {
        int connectedX = argX + puyoSubPositions[i][0];
        int connectedY = argY + puyoSubPositions[i][1];
        count = getConnectedPuyoCount(connectedX, connectedY, argType, count);
    }

    return count;
}
