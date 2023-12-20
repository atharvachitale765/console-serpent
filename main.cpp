#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

const int width = 30;
const int height = 20;

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
        system("clear"); // For Unix-like systems

        cout << "\033[1;32m"; // Set text color to green

        cout << "==========================\n";
        cout << "         Snake Game        \n";
        cout << "==========================\n\n";

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (j == 0)
                    cout << "#";

                if (i == headY && j == headX)
                    cout << "\033[1;33mO"; // Set text color to yellow
                else if (i == fruitY && j == fruitX)
                    cout << "\033[1;31mF"; // Set text color to red
                else
                {
                    bool isTail = false;
                    for (auto &tailPos : tail)
                    {
                        if (tailPos.first == j && tailPos.second == i)
                        {
                            cout << "o";
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

        cout << "\033[1;32m"; // Set text color to green
        cout << "==========================\n";
        cout << "Score: " << score << "\n\n";
    }

    void Input()
    {
        char ch = GetChar();

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

    char GetChar()
    {
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
        return (buf);
    }
};

int main()
{
    struct termios old;
    tcgetattr(STDIN_FILENO, &old); // Get current terminal attributes

    srand(static_cast<unsigned>(time(nullptr))); // Seed for random number generation

    SnakeGame snakeGame;

    while (!snakeGame.IsGameOver())
    {
        snakeGame.Draw();
        snakeGame.Input();
        snakeGame.Logic();
        this_thread::sleep_for(chrono::milliseconds(100)); // Sleep for 0.1 seconds
    }

    cout << "\033[0m";
}