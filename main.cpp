#include <windows.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <mmsystem.h>
using namespace std;
int width = 40, height = 10;
int ballX, ballY, ballDX, ballDY;
int paddleone, paddletwo;
struct Player {
    char name[50];
    int score;
    int misses;
};
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
Player Playerone, Playertwo;
void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Setup() {
    ballX = width / 2;
    ballY = height / 2;
    ballDX = (rand() % 2) ? 1 : -1;
    ballDY = (rand() % 2) ? 1 : -1;
    paddleone = height / 2;
    paddletwo = height / 2;
}
void ClearScreen() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(console, topLeft);
}
void Draw() {
        ClearScreen();   
    SetCursorPosition(0, 0);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int y = -1; y <= height; y++) {
        for (int x = -1; x <= width; x++) {
            if (y == -1 || y == height || x == -1 || x == width) {
                SetConsoleTextAttribute(hConsole, 11); // added blue color
                cout << '*';
            }
            else if (x == 0 && y == paddleone) {
                SetConsoleTextAttribute(hConsole, 1);
                cout << char(219); // solid paddle k lye
            }
            else if (x == width - 1 && y == paddletwo) {
                SetConsoleTextAttribute(hConsole, 1);  // Blue paddle two
                cout << char(219); // second paddle as solid
            }
            else if (x == ballX && y == ballY) {

                cout << 'O';  // ball as circle
            }
            else {
                SetConsoleTextAttribute(hConsole, 15); // Default white space
                cout << ' ';
            }
        }
        cout << endl;
    }

    SetConsoleTextAttribute(hConsole, 15); // Reset color

    cout << "Score - " << Playerone.name << ": " << Playerone.score
        << " | " << Playertwo.name << ": " << Playertwo.score << endl;
}

void HideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void Input() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 'w' && paddleone > 0) paddleone--;
        if (key == 's' && paddleone < height - 1) paddleone++;
        if (key == 'i' && paddletwo > 0) paddletwo--;
        if (key == 'k' && paddletwo < height - 1) paddletwo++;
    }
}

void Logic() {
    ballX += ballDX;
    ballY += ballDY;

    if (ballY <= 0 || ballY >= height - 1)
        ballDY *= -1;

    if (ballX == 1) {
        if (ballY == paddleone) {
            ballDX *= -1;
            Playerone.score++;
        }
        else {
            Playertwo.score++;
            Playertwo.misses++;
            Setup();
        }
    }

    if (ballX == width - 2) {
        if (ballY == paddletwo) {
            ballDX *= -1;
            Playertwo.score++;
        }
        else {
            Playerone.score++;
            Playerone.misses++;
            Setup();
        }
    }
}

void SaveScore() {
    ofstream file("score.txt", ios::app);
    file << "Final Score - " << Playerone.name << ": " << Playerone.score << ", " << Playertwo.name << ": " << Playertwo.score << endl;
    if (Playerone.misses >= 5)
        file << "ooo bhai Winner ho : " << Playertwo.name << "\n";
    else if (Playertwo.misses >= 5)
        file << "ooo bhai Winner ho : " << Playerone.name << "\n";
    file << "-----------------------------\n";
    file.close();
}
int main() {
    PlaySound(TEXT("background.wav"), NULL, SND_FILENAME | SND_ASYNC);
    srand(time(0));
    HideCursor();
    char choice;
    std::ios::sync_with_stdio(false);
    cout << "\n-----TELL ME YOUR NAMES-----" << endl;
    cout << "   0   0  \n";
    cout << "    <_> ";
    cout << "\nEnter Left Player Name: ";
    cin.getline(Playerone.name, 50);
    cout << "\nEnter Right Player Name: ";
    cin.getline(Playertwo.name, 50);
    do {
        Playerone.score = Playertwo.score = 0;
        Playerone.misses = Playertwo.misses = 0;
        Setup();
        while (Playerone.misses < 5 && Playertwo.misses < 5) {
            Draw();
            Input();
            Logic();
            Sleep(80);//speed
        }
        Draw();
        if (Playerone.misses >= 5)
        {
            cout << "   O   O  \n";
            cout << "   > _ <";
            cout << endl;
            cout << Playertwo.name << "\n Wins!\n";
        }
        else
        {
            cout << "   O   O  \n";
            cout << "   > _ <";
            cout << endl;
            cout << Playerone.name << "\n Wins!\n";
        }
        SaveScore();
        cout << "GAME OVER cutie ;) WANNA PLAY AGAIN >>3  ";
        cout << " if you want to press(y) otherwise (n) ";
        cin >> choice;
        cin.ignore(); // to safely capture name next time if restarting
    } while (choice == 'y' || choice == 'Y');

    return 0;
}  
