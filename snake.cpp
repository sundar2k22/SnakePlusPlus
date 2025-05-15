#include <bits/stdc++.h>
#include <iostream>
#include <ncurses.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <string>
using namespace std;

// Console dimensions
const int width = 20;
const int height = 20;

// Color pairs
#define BORDER_PAIR 1
#define SNAKE_PAIR 2
#define FOOD_PAIR 3
#define TEXT_PAIR 4
#define TITLE_PAIR 5

// Snake and food positions
vector<pair<int, int> > snake;
pair<int, int> food;

// Game variables
int score = 0;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;

// Function to initialize colors
void initColors() {
    start_color();
    init_pair(BORDER_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SNAKE_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(FOOD_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(TEXT_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(TITLE_PAIR, COLOR_MAGENTA, COLOR_BLACK);
}

// Function to display welcome screen and get user name
string getUserName() {
    clear();
    
    // Display title with color
    attron(COLOR_PAIR(TITLE_PAIR) | A_BOLD);
    printw("\n\n");
    printw("  *****************************\n");
    printw("  *                           *\n");
    printw("  *   COMMAND LINE COBRA      *\n");
    printw("  *                           *\n");
    printw("  *****************************\n\n");
    attroff(COLOR_PAIR(TITLE_PAIR) | A_BOLD);
    
    // Display instructions with color
    attron(COLOR_PAIR(TEXT_PAIR));
    printw("  Use WASD keys to navigate your snake\n");
    printw("  Eat food (F) to grow and earn points\n");
    printw("  Avoid hitting walls or yourself\n\n");
    attroff(COLOR_PAIR(TEXT_PAIR));
    
    // Get player name with color
    attron(COLOR_PAIR(TEXT_PAIR) | A_BOLD);
    printw("  Enter your name: ");
    attroff(COLOR_PAIR(TEXT_PAIR) | A_BOLD);
    
    char name[50];
    echo(); // Enable echo to show user input
    getnstr(name, 49);
    noecho(); // Disable echo after input
    
    // Show welcome message
    clear();
    attron(COLOR_PAIR(TITLE_PAIR) | A_BOLD);
    printw("\n\n  Welcome, %s!\n", name);
    printw("  Get ready to play...\n");
    attroff(COLOR_PAIR(TITLE_PAIR) | A_BOLD);
    refresh();
    sleep(2); // Pause for 2 seconds
    
    return string(name);
}

// Initialize the game
void Setup() {
    gameOver = false;
    dir = STOP;
    
    // Initial snake position (middle of screen)
    snake.clear();
    snake.push_back(make_pair(width / 2, height / 2));
    
    // Generate random food position
    srand(time(0));
    food.first = rand() % width;
    food.second = rand() % height;
    
    score = 0;
}

// Draw the game board
void Draw(const string& playerName) {
    clear(); // Clear the console using ncurses
    
    // Display player name and score at the top
    attron(COLOR_PAIR(TEXT_PAIR) | A_BOLD);
    printw("Player: %s | Score: %d\n", playerName.c_str(), score);
    attroff(COLOR_PAIR(TEXT_PAIR) | A_BOLD);
    
    // Draw top border
    attron(COLOR_PAIR(BORDER_PAIR));
    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\n");
    
    // Draw game area
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                printw("#"); // Left border
                
            // Check if current position has snake
            bool isSnakePart = false;
            for (auto& segment : snake) {
                if (segment.first == j && segment.second == i) {
                    attroff(COLOR_PAIR(BORDER_PAIR));
                    attron(COLOR_PAIR(SNAKE_PAIR));
                    printw("O");
                    attroff(COLOR_PAIR(SNAKE_PAIR));
                    attron(COLOR_PAIR(BORDER_PAIR));
                    isSnakePart = true;
                    break;
                }
            }
            
            // Check if current position has food
            if (!isSnakePart) {
                if (j == food.first && i == food.second) {
                    attroff(COLOR_PAIR(BORDER_PAIR));
                    attron(COLOR_PAIR(FOOD_PAIR));
                    printw("F");
                    attroff(COLOR_PAIR(FOOD_PAIR));
                    attron(COLOR_PAIR(BORDER_PAIR));
                } else {
                    printw(" ");
                }
            }
            
            if (j == width - 1)
                printw("#"); // Right border
        }
        printw("\n");
    }
    
    // Draw bottom border
    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\n");
    attroff(COLOR_PAIR(BORDER_PAIR));
    
    // Display controls
    attron(COLOR_PAIR(TEXT_PAIR));
    printw("Controls: WASD to move, X to quit\n");
    attroff(COLOR_PAIR(TEXT_PAIR));
    refresh();
}

// Handle user input
void Input() {
    // Use nodelay for continuous input detection
    nodelay(stdscr, TRUE);
    
    int ch = getch();
    if (ch != ERR) {
        switch (ch) {
            case 'a':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                if (dir != UP) dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

// Update game state
void Logic() {
    // If no direction set, don't move
    if (dir == STOP)
        return;
        
    // Calculate new head position based on direction
    pair<int, int> newHead = snake.front();
    
    switch (dir) {
        case LEFT:
            newHead.first--;
            break;
        case RIGHT:
            newHead.first++;
            break;
        case UP:
            newHead.second--;
            break;
        case DOWN:
            newHead.second++;
            break;
        default:
            return;
    }
    
    // Check for collision with walls
    if (newHead.first < 0 || newHead.first >= width ||
        newHead.second < 0 || newHead.second >= height) {
        gameOver = true;
        return;
    }
    
    // Check for collision with self
    for (auto& segment : snake) {
        if (newHead.first == segment.first && newHead.second == segment.second) {
            gameOver = true;
            return;
        }
    }
    
    // Add new head
    snake.insert(snake.begin(), newHead);
    
    // Check if snake ate food
    if (newHead.first == food.first && newHead.second == food.second) {
        // Increase score
        score += 10;
        
        // Generate new food
        food.first = rand() % width;
        food.second = rand() % height;
    } else {
        // Remove tail if no food was eaten
        snake.pop_back();
    }
}

int main() {
    // Initialize ncurses with proper settings
    initscr();
    cbreak();  // Line buffering disabled
    keypad(stdscr, TRUE);  // Enable function keys
    noecho();  // Don't echo input characters
    curs_set(0);  // Hide cursor
    
    // Initialize colors if terminal supports them
    if (has_colors()) {
        initColors();
    }
    
    // Get player name from welcome screen
    string playerName = getUserName();
    
    Setup();
    
    while (!gameOver) {
        Draw(playerName);
        Input();
        Logic();
        usleep(100000);  // 100ms delay for game speed
    }
    
    // Display final score on screen
    clear();
    attron(COLOR_PAIR(TITLE_PAIR) | A_BOLD);
    printw("\n\n");
    printw("  *****************************\n");
    printw("  *                           *\n");
    printw("  *        GAME OVER!         *\n");
    printw("  *                           *\n");
    printw("  *****************************\n\n");
    attroff(COLOR_PAIR(TITLE_PAIR) | A_BOLD);
    
    attron(COLOR_PAIR(TEXT_PAIR));
    printw("  Player: %s\n", playerName.c_str());
    printw("  Final Score: %d\n\n", score);
    printw("  Press any key to exit...");
    attroff(COLOR_PAIR(TEXT_PAIR));
    
    refresh();
    getch();
    
    // Save player name and score for console output
    string finalPlayerName = playerName;
    int finalScore = score;
    
    // Clean up ncurses
    endwin();
    
    // Print final score to console after exiting ncurses mode
    cout << "\n========================================\n";
    cout << "           SNAKE GAME RESULTS           \n";
    cout << "========================================\n";
    cout << "Player: " << finalPlayerName << endl;
    cout << "Final Score: " << finalScore << endl;
    cout << "========================================\n";
    
    return 0;
}
