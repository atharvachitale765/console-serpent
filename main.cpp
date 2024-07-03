#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

using namespace std;

const int width = 30;
const int height = 20;

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"

class SnakeGame
{
public:
    SnakeGame() : score(0), gameover(false)
    {
        headX = width / 2;
        headY = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tail.push_back({headX, headY});
        direction = ' ';
    }

    void Draw()
    {
        system("clear"); // For Unix-like systems, use "cls" for Windows

        cout << GREEN << "==========================\n";
        cout << "         Snake Game        \n";
        cout << "==========================\n\n";

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (j == 0)
                    cout << "#";

                if (i == headY && j == headX)
                    cout << YELLOW << "O"; // Head of the snake
                else if (i == fruitY && j == fruitX)
                    cout << RED << "F"; // Fruit
                else
                {
                    bool isTail = false;
                    for (auto &tailPos : tail)
                    {
                        if (tailPos.first == j && tailPos.second == i)
                        {
                            cout << GREEN << "o"; // Tail of the snake
                            isTail = true;
                        }
                    }
                    if (!isTail)
                        cout << " ";
                }

                if (j == width - 1)
                    cout << "#";
            }
            cout << "\n";
        }

        cout << GREEN << "==========================\n";
        cout << "Score: " << score << "\n\n";
        cout << RESET; // Reset color to default
    }

    void Input()
    {
        char ch = GetInput(); // Platform-independent input handling

        switch (ch)
        {
        case 'a':
            if (direction != 'R')
                direction = 'L';
            break;
        case 'd':
            if (direction != 'L')
                direction = 'R';
            break;
        case 'w':
            if (direction != 'D')
                direction = 'U';
            break;
        case 's':
            if (direction != 'U')
                direction = 'D';
            break;
        case 'q':
            gameover = true;
            break;
        }
    }

    void Logic()
    {
        int prevX = tail[0].first;
        int prevY = tail[0].second;
        int tempX, tempY;
        tail[0].first = headX;
        tail[0].second = headY;

        for (size_t i = 1; i < tail.size(); i++)
        {
            tempX = tail[i].first;
            tempY = tail[i].second;
            tail[i].first = prevX;
            tail[i].second = prevY;
            prevX = tempX;
            prevY = tempY;
        }

        switch (direction)
        {
        case 'L':
            headX--;
            break;
        case 'R':
            headX++;
            break;
        case 'U':
            headY--;
            break;
        case 'D':
            headY++;
            break;
        }

        if (headX < 0 || headX >= width || headY < 0 || headY >= height)
            gameover = true;

        for (auto &tailPos : tail)
        {
            if (tailPos.first == headX && tailPos.second == headY)
                gameover = true;
        }

        if (headX == fruitX && headY == fruitY)
        {
            score += 10;
            fruitX = rand() % width;
            fruitY = rand() % height;
            tail.push_back({0, 0}); // Dummy tail segment
        }
    }

    bool IsGameOver() const
    {
        return gameover;
    }

private:
    int headX, headY;
    int fruitX, fruitY;
    int score;
    char direction;
    vector<pair<int, int>> tail;
    bool gameover;

    char GetInput()
    {
#if defined(_WIN32) || defined(_WIN64)
        return static_cast<char>(_getch());
#else
        char buf = 0;
        struct termios old;
        fflush(stdout);
        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
            perror("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");
        return buf;
#endif
    }
};

int main()
{
    srand(static_cast<unsigned>(time(nullptr))); // Seed for random number generation

    SnakeGame snakeGame;

    while (!snakeGame.IsGameOver())
    {
        snakeGame.Draw();
        snakeGame.Input();
        snakeGame.Logic();

#if defined(_WIN32) || defined(_WIN64)
        Sleep(100); // Sleep for 0.1 seconds (Windows)
#else
        usleep(100000); // Sleep for 0.1 seconds (Unix)
#endif
    }

    return 0;
}
