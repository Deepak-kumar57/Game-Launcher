#include<iostream>
#include<conio.h>
#include<fstream>
#include<windows.h>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<sstream>
#include<algorithm>
#include<limits>

using namespace std;

class Game
{
public:
    virtual void play() = 0;
};


// --------------------------------
// 1. BattleGround Game
// --------------------------------

class BattleGround : public Game
{
private:
    static const int LOCATION_COUNT = 8;

    // List of possible map locations
    const char *locations[LOCATION_COUNT] = {
        "LOGIK", "Mess", "FES", "FMCE", "FCSE", "Brabers", "Library", "Sports Complex"};

    // Player structure with basic attributes
    struct Player
    {
        char name[20];
        int health;
        int damagePower;
        int locationIndex;

        // Checks if player is still alive
        bool alive() const { return health > 0; }
    };

    Player teamPlayer[3]; // Array for player team
    Player teamEnemy[3] = {
        {"Bot_A", 0, 10, 0},
        {"Bot_B", 0, 20, 0},
        {"Bot_C", 0, 30, 0}}; // Enemy bots with preset names and damage

    const char *guns[3] = {"Pistol", "Rifle", "Sniper"}; // Gun options

    // Randomly move player to a new location
    void movePlayer(Player &p)
    {
        p.locationIndex = rand() % LOCATION_COUNT;
    }

    // Handles battle between two players
    void battle(Player &attacker, Player &defender)
    {
        defender.health -= attacker.damagePower;
        cout << attacker.name << " attacks " << defender.name
             << " causing " << attacker.damagePower << " damage.\n";

        if (defender.health <= 0)
        {
            cout << defender.name << " is defeated!\n";
        }
    }

    // Display current locations of all players
    void showPositions()
    {
        cout << "\nPlayer Positions:\n";
        for (int i = 0; i < 3; ++i)
        {
            if (teamPlayer[i].alive())
                cout << teamPlayer[i].name << " at " << locations[teamPlayer[i].locationIndex] << "\n";
        }

        cout << "\nEnemy Positions:\n";
        for (int i = 0; i < 3; ++i)
        {
            if (teamEnemy[i].alive())
                cout << teamEnemy[i].name << " at " << locations[teamEnemy[i].locationIndex] << "\n";
        }
    }

public:
    // Main gameplay loop
    void play() override
    {
        cout << "\n--- Welcome to BattleGround ---\n";

        // Setup for user-controlled players
        for (int i = 0; i < 3; ++i)
        {
            cout << "Enter name for player " << i + 1 << ": ";
            cin >> teamPlayer[i].name;

            cout << "Choose gun for " << teamPlayer[i].name << " (1)Pistol (2)Rifle (3)Sniper: ";
            int gunChoice;
            cin >> gunChoice;
            while (gunChoice < 1 || gunChoice > 3)
            {
                cout << "Invalid choice. Try again: ";
                cin >> gunChoice;
            }

            teamPlayer[i].damagePower = gunChoice * 10;
            teamPlayer[i].health = 100;
            teamPlayer[i].locationIndex = rand() % LOCATION_COUNT;
        }

        // Initialize enemy players
        for (int i = 0; i < 3; ++i)
        {
            teamEnemy[i].health = 100;
            teamEnemy[i].locationIndex = rand() % LOCATION_COUNT;
        }

        // Game continues until one team is eliminated
        while (true)
        {
            cout << "\n--- New Turn ---\n";

            // Move all alive players and enemies
            for (int i = 0; i < 3; ++i)
            {
                if (teamPlayer[i].alive())
                    movePlayer(teamPlayer[i]);
            }
            for (int i = 0; i < 3; ++i)
            {
                if (teamEnemy[i].alive())
                    movePlayer(teamEnemy[i]);
            }

            showPositions(); // Display updated positions

            // Check for battles if players and enemies meet at same location
            for (int i = 0; i < 3; ++i)
            {
                if (!teamPlayer[i].alive())
                    continue;

                for (int j = 0; j < 3; ++j)
                {
                    if (!teamEnemy[j].alive())
                        continue;

                    if (teamPlayer[i].locationIndex == teamEnemy[j].locationIndex)
                    {
                        cout << "\nFight at " << locations[teamPlayer[i].locationIndex] << " between "
                             << teamPlayer[i].name << " and " << teamEnemy[j].name << "!\n";

                        battle(teamPlayer[i], teamEnemy[j]);
                        if (teamEnemy[j].alive())
                        {
                            battle(teamEnemy[j], teamPlayer[i]); // Enemy counterattack if still alive
                        }
                    }
                }
            }

            // Check win condition
            bool playerAlive = false, enemyAlive = false;
            for (int i = 0; i < 3; ++i)
            {
                if (teamPlayer[i].alive())
                    playerAlive = true;
                if (teamEnemy[i].alive())
                    enemyAlive = true;
            }

            if (!playerAlive)
            {
                cout << "\nEnemy Team Wins!\n";
                break;
            }
            if (!enemyAlive)
            {
                cout << "\nYour Team Wins!\n";
                break;
            }
        }
    }
};

// --------------------------------
// 2. Hangman Game
// --------------------------------

class Hangman : public Game
{
private:
    // Word banks by difficulty
    const char *easyWords[30] = {
        "cat", "dog", "bat", "apple", "ball", "car", "fan", "hat", "jar", "kite",
        "lion", "mouse", "nose", "owl", "pen", "queen", "rat", "sun", "top", "van",
        "water", "xray", "yak", "zebra", "box", "cup", "dish", "ear", "frog", "goat"};

    const char *mediumWords[35] = {
        "pencil", "laptop", "guitar", "island", "jungle", "market", "nature", "orange", "parrot", "rocket",
        "school", "temple", "unicorn", "violin", "window", "yellow", "zombie", "balloon", "cactus", "desert",
        "energy", "flower", "galaxy", "helmet", "insect", "jacket", "kitten", "letter", "mirror", "notebook",
        "object", "planet", "rabbit", "saddle", "ticket"};

    const char *hardWords[40] = {
        "awkward", "bagpipes", "crypt", "dwarves", "espionage", "fjord", "gazebo", "haphazard", "ivory", "jukebox",
        "kilobyte", "larynx", "mnemonic", "nightclub", "ostracize", "pneumonia", "quartz", "rhubarb", "sphinx", "twelfth",
        "unworthy", "vaporize", "waltz", "xylophone", "yachtsman", "zephyr", "bizarre", "cobweb", "fluff", "gossip",
        "haiku", "injury", "jackpot", "knapsack", "lengths", "mystify", "nowadays", "oxidize", "puppet", "quizzes"};

    char guessedLetters[26]; // Stores already guessed letters
    int guessedCount;        // Count of guessed letters
    int difficulty;          // Difficulty level: 1 - Easy, 2 - Medium, 3 - Hard

public:
    Hangman() : guessedCount(0), difficulty(1)
    {
        for (int i = 0; i < 26; ++i)
            guessedLetters[i] = '\0';
    }

    // Check if the letter has already been guessed
    bool isAlreadyGuessed(char ch)
    {
        for (int i = 0; i < guessedCount; ++i)
        {
            if (guessedLetters[i] == ch)
                return true;
        }
        return false;
    }

    // Return a random word based on selected difficulty
    const char *getRandomWord()
    {
        if (difficulty == 1)
            return easyWords[rand() % 30];
        else if (difficulty == 2)
            return mediumWords[rand() % 35];
        else
            return hardWords[rand() % 40];
    }

    // Display hangman figure based on number of wrong attempts
    void drawHangman(int wrongAttempts)
    {
        switch (wrongAttempts)
        {
        case 1:
            cout << "   +---+\n   |   |\n       |\n       |\n       |\n       |\n  =====\n";
            break;
        case 2:
            cout << "   +---+\n   |   |\n   O   |\n       |\n       |\n       |\n  =====\n";
            break;
        case 3:
            cout << "   +---+\n   |   |\n   O   |\n   |   |\n       |\n       |\n  =====\n";
            break;
        case 4:
            cout << "   +---+\n   |   |\n   O   |\n  /|   |\n       |\n       |\n  =====\n";
            break;
        case 5:
            cout << "   +---+\n   |   |\n   O   |\n  /|\\  |\n       |\n       |\n  =====\n";
            break;
        case 6:
            cout << "   +---+\n   |   |\n   O   |\n  /|\\  |\n  /    |\n       |\n  =====\n";
            break;
        case 7:
            cout << "   +---+\n   |   |\n   O   |\n  /|\\  |\n  / \\  |\n       |\n  =====\n";
            break;
        case 8:
            cout << "   +---+\n   |   |\n   O   |\n  /|\\  |\n  / \\  |\n       |\n  HANGED!\n  =====\n";
            break;
        default:
            break;
        }
    }

    // Core game logic
    void play() override
    {
        cout << "\n--- Welcome to Hangman ---\n";
        cout << "Select Difficulty (1-Easy, 2-Medium, 3-Hard): ";
        cin >> difficulty;
        if (difficulty < 1 || difficulty > 3)
            difficulty = 1;

        const char *word = getRandomWord(); // Get random word
        int wordLen = strlen(word);
        char display[30];

        // Initialize display with underscores
        for (int i = 0; i < wordLen; ++i)
            display[i] = '_';
        display[wordLen] = '\0';

        int wrongAttempts = 0;
        guessedCount = 0;
        const int maxAttempts = 8;
        char wrongGuesses[8];
        int wrongGuessesCount = 0;

        // Game loop
        while (wrongAttempts < maxAttempts)
        {
            drawHangman(wrongAttempts);

            // Show current progress and wrong guesses
            cout << "\nWord: ";
            for (int i = 0; i < wordLen; ++i)
                cout << display[i] << " ";
            cout << "\nWrong guesses: ";
            for (int i = 0; i < wrongGuessesCount; ++i)
                cout << wrongGuesses[i] << " ";
            cout << "\nAttempts remaining: " << (maxAttempts - wrongAttempts);
            cout << "\nEnter your guess: ";
            char guess;
            cin >> guess;

            // Check if guess was already made
            if (isAlreadyGuessed(guess))
            {
                cout << "You already guessed that letter. Try a new one.\n";
                continue;
            }

            guessedLetters[guessedCount++] = guess;

            bool found = false;
            for (int i = 0; i < wordLen; ++i)
            {
                if (word[i] == guess)
                {
                    display[i] = guess;
                    found = true;
                }
            }

            // Update state based on guess
            if (found)
            {
                cout << "Correct guess!\n";
            }
            else
            {
                cout << "Wrong guess!\n";
                wrongAttempts++;
                wrongGuesses[wrongGuessesCount++] = guess;
            }

            // Check if all letters are guessed
            bool complete = true;
            for (int i = 0; i < wordLen; ++i)
            {
                if (display[i] == '_')
                {
                    complete = false;
                    break;
                }
            }
            if (complete)
            {
                cout << "\nCongratulations! You guessed the word: " << word << endl;
                return;
            }
        }

        // Player failed to guess the word
        drawHangman(maxAttempts);
        cout << "\nYou lost! The word was: " << word << endl;
    }
};

// --------------------------------
// 3. Tic Tac Toe (Adjustable size)
// --------------------------------

class TicTacToe : public Game
{
private:
    char board[10][10]; // Board up to 10x10
    int size;           // User-defined board size

public:
    // Main game logic
    void play() override
    {
        cout << "\n--- Welcome to Tic Tac Toe ---\n";
        cout << "Enter board size (3 to 10): ";
        cin >> size;

        // Validate board size
        while (size < 3 || size > 10)
        {
            cout << "Invalid choice!! Try again.\n";
            cout << "Enter board size (3 to 10): ";
            cin >> size;
        }

        // Initialize board with dashes
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                board[i][j] = '-';

        int moves = 0;
        char currentPlayer = 'X';

        // Main game loop
        while (true)
        {
            // Display current board
            cout << "\n";
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                    cout << board[i][j] << " ";
                cout << "\n";
            }

            // Get player's move
            int r, c;
            cout << "Player " << currentPlayer << ", enter row and column (0-based index): ";
            cin >> r >> c;

            // Validate move
            if (r < 0 || r >= size || c < 0 || c >= size || board[r][c] != '-')
            {
                cout << "Invalid move. Try again.\n";
                continue;
            }

            // Apply move
            board[r][c] = currentPlayer;
            moves++;

            // Check for win
            if (checkWin(r, c, currentPlayer))
            {
                cout << "Player " << currentPlayer << " wins!\n";
                break;
            }

            // Check for draw
            if (moves == size * size)
            {
                cout << "Game Draw!\n";
                break;
            }

            // Switch player
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    // Check if player has won after placing at (r, c)
    bool checkWin(int r, int c, char player)
    {
        bool win;

        // Check row
        win = true;
        for (int i = 0; i < size; ++i)
            if (board[r][i] != player)
                win = false;
        if (win)
            return true;

        // Check column
        win = true;
        for (int i = 0; i < size; ++i)
            if (board[i][c] != player)
                win = false;
        if (win)
            return true;

        // Check main diagonal
        if (r == c)
        {
            win = true;
            for (int i = 0; i < size; ++i)
                if (board[i][i] != player)
                    win = false;
            if (win)
                return true;
        }

        // Check anti-diagonal
        if (r + c == size - 1)
        {
            win = true;
            for (int i = 0; i < size; ++i)
                if (board[i][size - 1 - i] != player)
                    win = false;
            if (win)
                return true;
        }

        return false; // No win
    }
};

// --------------------------------
// 4. Treasure Hunt Game
// --------------------------------

class TreasureHunt : public Game
{
private:
    static const int FIELD_SIZE = 5;        // 5x5 playing field
    char field[FIELD_SIZE][FIELD_SIZE];     // Display field with underscores
    int playerX, playerY;                   // Player position
    int treasureX, treasureY;               // Treasure position
    int movesLeft;                          // Number of moves allowed

    // Display the current field with player position
    void displayField()
    {
        for (int i = 0; i < FIELD_SIZE; ++i)
        {
            for (int j = 0; j < FIELD_SIZE; ++j)
            {
                if (i == playerX && j == playerY)
                    cout << "P ";           // Player shown as P
                else
                    cout << field[i][j] << " ";
            }
            cout << "\n";
        }
    }

    // Move player based on input (W/A/S/D)
    bool movePlayer(char move)
    {
        if (move == 'W' || move == 'w')     // Up
        {
            if (playerX > 0)
                playerX--;
            else
                return false;
        }
        else if (move == 'S' || move == 's') // Down
        {
            if (playerX < FIELD_SIZE - 1)
                playerX++;
            else
                return false;
        }
        else if (move == 'A' || move == 'a') // Left
        {
            if (playerY > 0)
                playerY--;
            else
                return false;
        }
        else if (move == 'D' || move == 'd') // Right
        {
            if (playerY < FIELD_SIZE - 1)
                playerY++;
            else
                return false;
        }
        else
        {
            return false;                   // Invalid key
        }
        return true;                        // Valid move
    }

public:
    // Core game logic
    void play() override
    {
        cout << "\n--- Welcome to Treasure Hunt ---\n";

        // Initialize the field with underscores
        for (int i = 0; i < FIELD_SIZE; ++i)
            for (int j = 0; j < FIELD_SIZE; ++j)
                field[i][j] = '_';

        // Randomize player and treasure positions
        playerX = rand() % FIELD_SIZE;
        playerY = rand() % FIELD_SIZE;

        do
        {
            treasureX = rand() % FIELD_SIZE;
            treasureY = rand() % FIELD_SIZE;
        } while (playerX == treasureX && playerY == treasureY); // Ensure not same spot

        movesLeft = 15; // Total moves allowed

        // Game loop
        while (movesLeft > 0)
        {
            cout << "\nMoves left: " << movesLeft << endl;
            displayField(); // Show current state

            cout << "Move (W/A/S/D): ";
            char move;
            cin >> move;

            // Validate and perform move
            if (!movePlayer(move))
            {
                cout << "Invalid move. Try a different direction.\n";
                continue;
            }

            // Check for win condition
            if (playerX == treasureX && playerY == treasureY)
            {
                cout << "\nCongratulations! You found the treasure!\n";
                return;
            }

            movesLeft--; // Decrement moves
        }

        // If out of moves
        cout << "\nOut of moves! You lost! The treasure was at (" 
             << treasureX << ", " << treasureY << ").\n";
    }
};

// --------------------------------
// 5. Memory Game
// --------------------------------

class MemoryGame : public Game
{
private:
    static const int size = 4;                  // 4x4 board size
    int board[size][size];                      // Number grid
    bool revealed[size][size];                  // Reveal state

    // Generate board with pairs and shuffle
    void generateBoard()
    {
        int nums[size * size];

        // Fill pairs
        for (int i = 0; i < size * size / 2; i++)
        {
            nums[2 * i] = i + 1;
            nums[2 * i + 1] = i + 1;
        }

        // Shuffle using Fisher-Yates
        for (int i = size * size - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            swap(nums[i], nums[j]);
        }

        // Fill board and initialize revealed
        int index = 0;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
            {
                board[i][j] = nums[index++];
                revealed[i][j] = false;
            }
    }

    // Display board (with option to show all cards)
    void displayBoard(bool showAll = false)
    {
        cout << "   ";
        for (int j = 0; j < size; j++)
            cout << j << " ";
        cout << endl;

        for (int i = 0; i < size; i++)
        {
            cout << i << "  ";
            for (int j = 0; j < size; j++)
            {
                if (revealed[i][j] || showAll)
                    cout << board[i][j] << " ";
                else
                    cout << "_ ";
            }
            cout << endl;
        }
    }

    // Check if all cards are revealed
    bool isBoardCompleted()
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (!revealed[i][j])
                    return false;
        return true;
    }

    // Single-player game logic
    void singlePlayerMode()
    {
        int moves = 0;

        while (!isBoardCompleted())
        {
            system("cls");
            cout << "Memory Game - Single Player Mode\n";
            displayBoard();
            cout << endl;

            int x1, y1, x2, y2;

            cout << "Enter coordinates of first card (row column): ";
            cin >> x1 >> y1;
            if (x1 < 0 || x1 >= size || y1 < 0 || y1 >= size || revealed[x1][y1])
            {
                cout << "Invalid first choice!" << endl;
                system("pause");
                continue;
            }

            cout << "Enter coordinates of second card (row column): ";
            cin >> x2 >> y2;
            if (x2 < 0 || x2 >= size || y2 < 0 || y2 >= size || revealed[x2][y2] || (x1 == x2 && y1 == y2))
            {
                cout << "Invalid second choice!" << endl;
                system("pause");
                continue;
            }

            revealed[x1][y1] = true;
            revealed[x2][y2] = true;

            system("cls");
            displayBoard();
            moves++;

            if (board[x1][y1] == board[x2][y2])
            {
                cout << "Match Found!\n";
            }
            else
            {
                cout << "No Match!\n";
                revealed[x1][y1] = false;
                revealed[x2][y2] = false;
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

        system("cls");
        displayBoard(true);
        cout << "\nCongratulations! You completed the game in " << moves << " moves!\n";
    }

    // Multiplayer game logic
    void multiplayerMode()
    {
        int moves = 0;
        int player1Score = 0;
        int player2Score = 0;
        int currentPlayer = 1;

        cout << "\nMemory Game - Multiplayer Mode!\n";
        cout << "Player 1 and Player 2 will take turns.\n";
        system("pause");

        while (!isBoardCompleted())
        {
            system("cls");
            cout << "Memory Game - Player " << currentPlayer << "'s Turn\n";
            displayBoard();
            cout << endl;

            int x1, y1, x2, y2;

            cout << "Enter coordinates of first card (row column): ";
            cin >> x1 >> y1;
            if (x1 < 0 || x1 >= size || y1 < 0 || y1 >= size || revealed[x1][y1])
            {
                cout << "Invalid first choice!" << endl;
                system("pause");
                continue;
            }

            cout << "Enter coordinates of second card (row column): ";
            cin >> x2 >> y2;
            if (x2 < 0 || x2 >= size || y2 < 0 || y2 >= size || revealed[x2][y2] || (x1 == x2 && y1 == y2))
            {
                cout << "Invalid second choice!" << endl;
                system("pause");
                continue;
            }

            revealed[x1][y1] = true;
            revealed[x2][y2] = true;

            system("cls");
            displayBoard();
            moves++;

            if (board[x1][y1] == board[x2][y2])
            {
                cout << "Match Found! Player " << currentPlayer << " gets 1 point.\n";
                if (currentPlayer == 1)
                    player1Score++;
                else
                    player2Score++;

                cout << "Player 1 Score: " << player1Score << " | Player 2 Score: " << player2Score << endl;
            }
            else
            {
                cout << "No Match!\n";
                revealed[x1][y1] = false;
                revealed[x2][y2] = false;
                currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch player
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

        system("cls");
        displayBoard(true);
        cout << "\n=== Game Over! ===\n";
        cout << "Player 1 Score: " << player1Score << endl;
        cout << "Player 2 Score: " << player2Score << endl;

        if (player1Score > player2Score)
            cout << "Player 1 Wins!\n";
        else if (player2Score > player1Score)
            cout << "Player 2 Wins!\n";
        else
            cout << "It's a Draw!\n";

        cout << "Total Moves Played: " << moves << endl;
    }

public:
    // Play mode selector
    void play() override
    {
        generateBoard(); // Initialize board

        system("cls");
        cout << "--- Memory Game ---\n";
        cout << "Select Mode:\n";
        cout << "1. Single Player\n";
        cout << "2. Multiplayer\n";
        cout << "Enter choice: ";
        int modeChoice;
        cin >> modeChoice;
        cin.ignore();

        if (modeChoice == 2)
            multiplayerMode();
        else
            singlePlayerMode();
    }
};

// --------------------------------
// 6. Typing Speed Test
// --------------------------------

class TypingSpeedTest : public Game
{
private:
    const char* sentences[5] = {
        "The quick brown fox jumps over the lazy dog",
        "Practice makes a person perfect",
        "C++ is a powerful programming language",
        "Coding is fun and rewarding",
        "Never give up on your dreams"
    };

    // Helper to convert string to lowercase
    string toLowerCase(string str)
    {
        for (char& c : str)
            c = tolower(c);
        return str;
    }

public:
    void play() override
    {
        cout << "\n--- Typing Speed Test ---\n";

        const char* sentence = sentences[rand() % 5]; // Random sentence

        cout << "Type the following sentence exactly:\n\n";
        cout << "\"" << sentence << "\"\n\n";

        cout << "When you're ready, start typing and press Enter:\n";

        time_t start = time(0); // Start timer

        string userInput;
        getline(cin >> ws, userInput); // Clear leading whitespace

        time_t end = time(0); // End timer

        double timeTaken = difftime(end, start);

        cout << "\nTime taken: " << timeTaken << " seconds.\n";

        if (toLowerCase(userInput) == toLowerCase(sentence))
        {
            cout << "Perfect typing!\n";
        }
        else
        {
            cout << "There were mistakes in your typing.\n";
        }
    }
};

// --------------------------------
// 7. Rock Paper Scissors
// --------------------------------

class RockPaperScissors : public Game
{
private:
    // Converts numeric choice to string (1 -> Rock, 2 -> Paper, 3 -> Scissors)
    string choiceToWord(int choice)
    {
        if (choice == 1)
            return "Rock";
        if (choice == 2)
            return "Paper";
        return "Scissors";
    }

public:
    void play() override
    {
        cout << "\n--- Rock Paper Scissors (Best of 3) ---\n";

        int playerWins = 0, computerWins = 0;
        int rounds = 0;

        // Continue playing until either player or computer wins 2 rounds
        while (playerWins < 2 && computerWins < 2)
        {
            cout << "\nRound " << (rounds + 1) << "\n";
            cout << "Choose: (1)Rock (2)Paper (3)Scissors: ";
            int playerChoice;
            cin >> playerChoice;

            // Input validation
            if (playerChoice < 1 || playerChoice > 3)
            {
                cout << "Invalid choice, try again.\n";
                continue;
            }

            // Random computer choice between 1 and 3
            int computerChoice = (rand() % 3) + 1;

            // Show choices
            cout << "You chose: " << choiceToWord(playerChoice) << "\n";
            cout << "Computer chose: " << choiceToWord(computerChoice) << "\n";

            // Check outcome of the round
            if (playerChoice == computerChoice)
            {
                cout << "It's a draw!\n";
            }
            else if ((playerChoice == 1 && computerChoice == 3) ||  // Rock beats Scissors
                     (playerChoice == 2 && computerChoice == 1) ||  // Paper beats Rock
                     (playerChoice == 3 && computerChoice == 2))    // Scissors beats Paper
            {
                cout << "You win this round! \n";
                playerWins++;
            }
            else
            {
                cout << "Computer wins this round! \n";
                computerWins++;
            }

            rounds++; // Increase round count
        }

        // Final result announcement
        cout << "\n=== Final Result ===\n";
        if (playerWins > computerWins)
        {
            cout << "Congratulations! You won the best of 3!\n";
        }
        else
        {
            cout << "Computer wins the best of 3. Better luck next time!\n";
        }
    }
};

// --------------------------------
// 8. KBC Game
// --------------------------------

class KBCGame : public Game
{
private:
    struct Question
    {
        string questionText;
        string options[4];
        char correctOption;
    };

    Question questions[50] = {
        {"What is the capital of India?", {"A) Mumbai", "B) Delhi", "C) Kolkata", "D) Chennai"}, 'B'},
        {"Which planet is known as the Red Planet?", {"A) Earth", "B) Venus", "C) Mars", "D) Jupiter"}, 'C'},
        {"Who wrote the national anthem of India?", {"A) Rabindranath Tagore", "B) Mahatma Gandhi", "C) Subhash Chandra Bose", "D) Jawaharlal Nehru"}, 'A'},
        {"Which is the largest mammal?", {"A) Elephant", "B) Blue Whale", "C) Giraffe", "D) Hippopotamus"}, 'B'},
        {"What is the chemical symbol for water?", {"A) H2O", "B) O2", "C) CO2", "D) H2SO4"}, 'A'},

        {"How many players are there in a cricket team?", {"A) 10", "B) 11", "C) 12", "D) 9"}, 'B'},
        {"What is the national bird of India?", {"A) Sparrow", "B) Parrot", "C) Peacock", "D) Crow"}, 'C'},
        {"Who invented the light bulb?", {"A) Newton", "B) Thomas Edison", "C) Einstein", "D) Tesla"}, 'B'},
        {"Which gas do plants absorb?", {"A) Oxygen", "B) Carbon Dioxide", "C) Nitrogen", "D) Hydrogen"}, 'B'},
        {"Which festival is called the Festival of Lights?", {"A) Eid", "B) Christmas", "C) Diwali", "D) Holi"}, 'C'},

        {"Which river is the longest in the world?", {"A) Ganga", "B) Nile", "C) Amazon", "D) Yangtze"}, 'B'},
        {"Who was the first President of India?", {"A) Dr. Rajendra Prasad", "B) Jawaharlal Nehru", "C) Sardar Patel", "D) B.R. Ambedkar"}, 'A'},
        {"Which part of the plant conducts photosynthesis?", {"A) Root", "B) Leaf", "C) Stem", "D) Flower"}, 'B'},
        {"What is 2 + 2 * 2?", {"A) 8", "B) 6", "C) 4", "D) 2"}, 'B'},
        {"Which instrument measures temperature?", {"A) Barometer", "B) Thermometer", "C) Seismograph", "D) Altimeter"}, 'B'},

        {"Which sport is P.V. Sindhu associated with?", {"A) Tennis", "B) Badminton", "C) Cricket", "D) Hockey"}, 'B'},
        {"Who discovered gravity?", {"A) Einstein", "B) Newton", "C) Galileo", "D) Hawking"}, 'B'},
        {"What currency is used in Japan?", {"A) Dollar", "B) Yen", "C) Yuan", "D) Rupee"}, 'B'},
        {"Who is known as the 'Father of Computers'?", {"A) Alan Turing", "B) Charles Babbage", "C) Bill Gates", "D) Steve Jobs"}, 'B'},
        {"What is the largest continent?", {"A) Africa", "B) Europe", "C) Asia", "D) Australia"}, 'C'},

        {"What is the boiling point of water?", {"A) 50C", "B) 90C", "C) 100C", "D) 120C"}, 'C'},
        {"Who painted the Mona Lisa?", {"A) Vincent Van Gogh", "B) Leonardo da Vinci", "C) Picasso", "D) Michelangelo"}, 'B'},
        {"Which organ pumps blood?", {"A) Brain", "B) Heart", "C) Lungs", "D) Liver"}, 'B'},
        {"Where is Taj Mahal located?", {"A) Delhi", "B) Mumbai", "C) Agra", "D) Jaipur"}, 'C'},
        {"What is the hardest substance?", {"A) Gold", "B) Diamond", "C) Iron", "D) Silver"}, 'B'},

        {"What is the largest ocean?", {"A) Atlantic", "B) Indian", "C) Arctic", "D) Pacific"}, 'D'},
        {"Which metal is liquid at room temperature?", {"A) Mercury", "B) Silver", "C) Gold", "D) Copper"}, 'A'},
        {"What is the main gas found in the air?", {"A) Oxygen", "B) Carbon Dioxide", "C) Nitrogen", "D) Hydrogen"}, 'C'},
        {"Who is known as Iron Man of India?", {"A) Nehru", "B) Sardar Patel", "C) Bose", "D) Rajaji"}, 'B'},
        {"Who was the first man on Moon?", {"A) Buzz Aldrin", "B) Neil Armstrong", "C) Yuri Gagarin", "D) Michael Collins"}, 'B'},

        {"Who wrote Harry Potter?", {"A) J.K. Rowling", "B) Tolkien", "C) Lewis Carroll", "D) Roald Dahl"}, 'A'},
        {"Who was the first woman PM of India?", {"A) Sonia Gandhi", "B) Indira Gandhi", "C) Sarojini Naidu", "D) Pratibha Patil"}, 'B'},
        {"How many colors are in a rainbow?", {"A) 5", "B) 6", "C) 7", "D) 8"}, 'C'},
        {"What is the square root of 144?", {"A) 10", "B) 11", "C) 12", "D) 13"}, 'C'},
        {"Which is the smallest prime number?", {"A) 0", "B) 1", "C) 2", "D) 3"}, 'C'},

        {"Which animal is known as the Ship of the Desert?", {"A) Horse", "B) Camel", "C) Donkey", "D) Elephant"}, 'B'},
        {"Which vitamin is gained from sunlight?", {"A) A", "B) B", "C) C", "D) D"}, 'D'},
        {"What color is chlorophyll?", {"A) Red", "B) Blue", "C) Green", "D) Yellow"}, 'C'},
        {"Where do penguins live?", {"A) Arctic", "B) Antarctica", "C) Africa", "D) Asia"}, 'B'},
        {"Which bird can mimic human speech?", {"A) Sparrow", "B) Owl", "C) Parrot", "D) Crow"}, 'C'},

        {"Who invented telephone?", {"A) Alexander Graham Bell", "B) Edison", "C) Newton", "D) Faraday"}, 'A'},
        {"Largest desert in the world?", {"A) Thar", "B) Gobi", "C) Sahara", "D) Arctic"}, 'C'},
        {"First Indian in space?", {"A) Rakesh Sharma", "B) Kalpana Chawla", "C) Sunita Williams", "D) Vikram Sarabhai"}, 'A'},
        {"Smallest state in India?", {"A) Goa", "B) Sikkim", "C) Manipur", "D) Tripura"}, 'A'},
        {"Fastest land animal?", {"A) Lion", "B) Cheetah", "C) Horse", "D) Deer"}, 'B'},

        {"Which animal is called the King of Jungle?", {"A) Tiger", "B) Lion", "C) Elephant", "D) Bear"}, 'B'},
        {"Who is the CEO of Tesla?", {"A) Bill Gates", "B) Elon Musk", "C) Jeff Bezos", "D) Sundar Pichai"}, 'B'},
        {"What does ATM stand for?", {"A) Automated Teller Machine", "B) Auto Transfer Money", "C) Auto Teller Mechanism", "D) All Time Money"}, 'A'},
        {"Which month has 28 or 29 days?", {"A) January", "B) February", "C) March", "D) April"}, 'B'},
        {"Largest island in the world?", {"A) Greenland", "B) Madagascar", "C) Australia", "D) Iceland"}, 'A'}
    };

public:
    void play() override
    {
        cout << "\n--- Welcome to Kaun Banega Crorepati! ---\n";
        cout << "Answer 13 questions correctly to become a Crorepati!\n";
        cout << "Let's start...\n\n";

        int askedIndices[13] = {0};
        int correctCount = 0;

        for (int q = 0; q < 13; ++q)
        {
            int idx;
            bool alreadyAsked;
            do
            {
                idx = rand() % 50;
                alreadyAsked = false;
                for (int i = 0; i < q; ++i)
                {
                    if (askedIndices[i] == idx)
                    {
                        alreadyAsked = true;
                        break;
                    }
                }
            } while (alreadyAsked);

            askedIndices[q] = idx;

            cout << "Q" << (q + 1) << ": " << questions[idx].questionText << "\n";
            for (int i = 0; i < 4; ++i)
            {
                cout << questions[idx].options[i] << "\n";
            }

            cout << "Lock your answer (A/B/C/D): ";
            char ans;
            cin >> ans;
            ans = toupper(ans);

            if (ans == questions[idx].correctOption)
            {
                cout << "Sahi Jawaab!\n\n";
                correctCount++;
            }
            else
            {
                cout << "Galat Jawaab!\n";
                cout << "Correct Answer was: " << questions[idx].correctOption << "\n\n";
                break;
            }
        }

        cout << "=== Game Over ===\n";
        cout << "You answered " << correctCount << " questions correctly!\n";

        if (correctCount == 13)
            cout << "Congratulations! You are now a CROREPATI!\n";
        else if (correctCount >= 7)
            cout << "Well Played! You did great!\n";
        else
            cout << "Keep practicing and try again!\n";
    }
};

// --------------------------------
// 9. Snake Game
// --------------------------------

class SnakeGame : public Game {
private:
    // Game board dimensions
    const int WIDTH = 40;
    const int HEIGHT = 20;

    // Snake head position
    int x, y;

    // Fruit position
    int fruitX, fruitY;

    // Current score
    int score;

    // Tail coordinates
    int tailX[200], tailY[200];

    // Current tail length
    int nTail;

    // Game over flag
    bool gameOver;

    // Direction enum for movement
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
    Direction dir;

    // Static variable to store high score
    static int highScore;

    // Initialize game state
    void setup() {
        x = WIDTH / 2;          // Start at center
        y = HEIGHT / 2;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        score = 0;
        nTail = 0;
        dir = STOP;
        gameOver = false;
    }

    // Hide the console cursor for a cleaner display
    void hideCursor() {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    // Load high score from file
    void loadHighScore() {
        ifstream file("snake_highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        } else {
            highScore = 0; // Set to 0 if file doesn't exist
        }
    }

    // Save high score to file
    void saveHighScore() {
        ofstream file("snake_highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

    // Draw game board
    void draw() {
        COORD coord = {0, 0}; // Reset cursor to top-left
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        // Top wall
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Board contents
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) cout << "#"; // Left wall

                if (i == y && j == x)
                    cout << "O"; // Snake head
                else if (i == fruitY && j == fruitX)
                    cout << "F"; // Fruit
                else {
                    bool print = false;

                    // Print tail segment if present
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";
                            print = true;
                        }
                    }

                    if (!print) cout << " "; // Empty space
                }

                if (j == WIDTH - 1) cout << "#"; // Right wall
            }
            cout << endl;
        }

        // Bottom wall
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Display score and high score
        cout << "Score: " << score << "  |  High Score: " << highScore << endl;
        cout << "Press X to quit\n";
    }

    // Handle keyboard input
    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a':
                    if (dir != RIGHT) dir = LEFT;  // Prevent reverse direction
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
                    gameOver = true;  // Exit game
                    break;
            }
        }
    }

    // Update game logic
    void logic() {
        // Move tail segments
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        // Move snake head
        switch (dir) {
            case LEFT: x--; break;
            case RIGHT: x++; break;
            case UP: y--; break;
            case DOWN: y++; break;
            default: break;
        }

        // Check wall collision
        if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
            gameOver = true;

        // Check self collision
        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                gameOver = true;
            }
        }

        // Check fruit collection
        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % WIDTH;
            fruitY = rand() % HEIGHT;
            nTail++; // Grow tail
        }
    }

public:
    // Override the base class play function
    void play() override {
        hideCursor();      // Hide blinking cursor
        loadHighScore();   // Load saved high score
        setup();           // Initialize the game

        // Main game loop
        while (!gameOver) {
            draw();        // Draw game state
            input();       // Handle input
            logic();       // Update logic
            Sleep(80);     // Control speed
        }

        // Save new high score if beaten
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }

        // Game over message
        cout << "\nGame Over! Final Score: " << score << "\n";
        cout << "Highest Score: " << highScore << "\n";
        system("pause"); // Wait for user before closing
    }
};

// Static variable initialization outside the class
int SnakeGame::highScore = 0;

// --------------------------------
// 10. Flappy Bird
// --------------------------------

class FlappyBird : public Game {
private:
    static const int WIDTH = 30;      // Width of the game screen
    static const int HEIGHT = 20;     // Height of the game screen
    int birdY;                        // Vertical position of the bird
    int pipeX, gapY;                  // Pipe's horizontal position and vertical gap center
    int score;                        // Current score
    bool gameOver;                    // Game over flag
    static int highScore;            // Static variable to store highest score

    // Hides the blinking cursor in the console
    void hideCursor() {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    // Loads the highest score from file
    void loadHighScore() {
        ifstream file("flappy_bird_highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        } else {
            highScore = 0;
        }
    }

    // Saves the highest score to file
    void saveHighScore() {
        ofstream file("flappy_bird_highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

    // Initializes game variables
    void setup() {
        birdY = HEIGHT / 2;                          // Start bird in middle
        pipeX = WIDTH;                               // Pipe starts at far right
        gapY = rand() % (HEIGHT - 6) + 3;            // Random vertical gap for pipe (ensures enough space)
        score = 0;                                   // Initial score
        gameOver = false;                            // Game is not over at start
    }

    // Draws the game screen
    void draw() {
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        // Top border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Main screen
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) cout << "#"; // Left border

                if (i == birdY && j == 5)
                    cout << "O"; // Bird at fixed x = 5
                else if (j == pipeX && (i < gapY - 2 || i > gapY + 2))
                    cout << "|"; // Pipe if not in gap area
                else
                    cout << " ";

                if (j == WIDTH - 1) cout << "#"; // Right border
            }
            cout << endl;
        }

        // Bottom border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Display score and instructions
        cout << "Score: " << score << "  |  High Score: " << highScore << endl;
        cout << "Press W to jump" << endl;
    }

    // Handles user input
    void input() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'w' || key == 'W') {
                birdY -= 3; // Bird jumps up
            }
        }
    }

    // Updates game logic
    void logic() {
        pipeX--; // Move pipe to the left

        // Collision with pipe near bird's x position
        if (pipeX == 5 || pipeX == 4 || pipeX == 6) {
            if (birdY < gapY - 2 || birdY > gapY + 2)
                gameOver = true; // Bird outside the pipe gap
            else if (pipeX == 5)
                score++; // Bird passes the pipe
        }

        // Reset pipe after it leaves the screen
        if (pipeX <= 0) {
            pipeX = WIDTH;
            gapY = rand() % (HEIGHT - 6) + 3; // New gap position
        }

        birdY += 1; // Gravity pulls bird down

        // Collision with top or bottom
        if (birdY >= HEIGHT || birdY < 0) {
            gameOver = true;
        }
    }

public:
    // Main game loop
    void play() override {
        hideCursor();
        loadHighScore(); // Load saved high score

        cout << "Welcome to Flappy Bird!\n";
        cout << "Press any key to start the game...";
        _getch(); // Wait for user to start

        setup(); // Initialize game
        while (!gameOver) {
            draw();     // Draw game screen
            input();    // Process input
            logic();    // Update game logic
            Sleep(100); // Delay for smoother gameplay
        }

        // Update and save high score if needed
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }

        // Show game over screen
        cout << "\nGame Over! Final Score: " << score << endl;
        cout << "Highest Score: " << highScore << endl;
        system("pause");
    }
};

// Static variable initialization
int FlappyBird::highScore = 0;

// --------------------------------
// 11. Brick Breaker
// --------------------------------

class BrickBreaker : public Game {
private:
    static const int WIDTH = 40;     // Width of the game screen
    static const int HEIGHT = 20;    // Height of the game screen
    int ballX, ballY;                // Ball position
    int ballDirX, ballDirY;          // Ball direction (X and Y)
    int paddleX;                     // Paddle position (X coordinate)
    bool bricks[6][WIDTH];           // Brick grid (6 rows × WIDTH columns)
    bool gameOver;                   // Flag to indicate game over
    int score;                       // Current score
    static int highScore;           // Static high score

    // Hides blinking cursor in the console window
    void hideCursor() {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    // Loads the saved high score from file
    void loadHighScore() {
        ifstream file("brick_breaker_highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        } else {
            highScore = 0;
        }
    }

    // Saves the current high score to file
    void saveHighScore() {
        ofstream file("brick_breaker_highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

    // Initializes game variables and brick states
    void setup() {
        ballX = WIDTH / 2;           // Ball starts in middle
        ballY = HEIGHT / 2;
        ballDirX = -1;               // Initial ball direction
        ballDirY = -1;
        paddleX = WIDTH / 2 - 3;     // Paddle centered
        score = 0;
        gameOver = false;

        // Set all bricks to visible (true)
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < WIDTH; j++)
                bricks[i][j] = true;
    }

    // Draws the full game screen
    void draw() {
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        // Top border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Main game area
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) cout << "#"; // Left wall

                if (i < 6 && bricks[i][j])
                    cout << "#"; // Draw brick
                else if (i == ballY && j == ballX)
                    cout << "o"; // Draw ball
                else if (i == HEIGHT - 1 && j >= paddleX && j < paddleX + 7)
                    cout << "="; // Draw paddle
                else
                    cout << " "; // Empty space

                if (j == WIDTH - 1) cout << "#"; // Right wall
            }
            cout << endl;
        }

        // Bottom border
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        // Show score and controls
        cout << "Score: " << score << "  |  High Score: " << highScore << endl;
        cout << "Move with A (left) and D (right)" << endl;
    }

    // Handles player input for paddle movement
    void input() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a' || key == 'A') {
                if (paddleX > 0)
                    paddleX -= 2;
            }
            else if (key == 'd' || key == 'D') {
                if (paddleX + 7 < WIDTH)
                    paddleX += 2;
            }
        }
    }

    // Game logic: ball movement, collisions, scoring
    void logic() {
        ballX += ballDirX;
        ballY += ballDirY;

        // Bounce off left/right walls
        if (ballX <= 0 || ballX >= WIDTH - 1)
            ballDirX = -ballDirX;

        // Bounce off top wall
        if (ballY <= 0)
            ballDirY = -ballDirY;

        // Ball reaches paddle line
        if (ballY == HEIGHT - 1) {
            if (ballX >= paddleX && ballX < paddleX + 7)
                ballDirY = -ballDirY; // Bounce off paddle
            else
                gameOver = true; // Missed the paddle
        }

        // Collision with bricks
        if (ballY < 6 && bricks[ballY][ballX]) {
            bricks[ballY][ballX] = false; // Destroy brick
            ballDirY = -ballDirY;         // Bounce back
            score += 10;                  // Increase score
        }
    }

public:
    // Main game function
    void play() override {
        hideCursor();
        loadHighScore(); // Load high score from file

        cout << "Welcome to Brick Breaker!\n";
        cout << "Press any key to start the game...";
        _getch(); // Wait for user input

        setup(); // Initialize everything

        while (!gameOver) {
            draw();   // Draw frame
            input();  // Handle input
            logic();  // Update game logic
            Sleep(70); // Small delay for smoother play
        }

        // Check if new high score was achieved
        if (score > highScore) {
            highScore = score;
            saveHighScore(); // Save it
        }

        // Game over screen
        cout << "\nGame Over! Final Score: " << score << endl;
        cout << "Highest Score: " << highScore << endl;
        system("pause");
    }
};

// Static variable initialization
int BrickBreaker::highScore = 0;

// --------------------------------
// 12. Dino Game
// --------------------------------

class DinoGame : public Game {
private:
    const int WIDTH = 50;   // Width of the screen
    const int HEIGHT = 20;  // Height of the screen
    int dinoY;              // Y position of the dinosaur
    bool isJumping;         // Is the dinosaur in the middle of a jump?
    int jumpCounter;        // Counter to manage jump duration
    int obstacleX;          // X position of the obstacle
    bool gameOver;          // Game over flag
    int score;              // Current score
    int highScore;          // High score

    // Hides the blinking console cursor
    void hideCursor() {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    // Sets cursor position in the console for overwriting the screen
    void setCursorPosition(int x, int y) {
        COORD coord = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // Loads high score from file if available
    void loadHighScore() {
        ifstream file("dino_highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        } else {
            highScore = 0;
        }
    }

    // Saves current high score to file
    void saveHighScore() {
        ofstream file("dino_highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

    // Draws the game screen
    void draw() {
        setCursorPosition(0, 0); // Reset cursor to top left

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (y == HEIGHT - 1) {
                    cout << "="; // Ground line
                } else if (x == 5 && y == dinoY) {
                    cout << "D"; // Dinosaur character
                } else if (x == obstacleX && y == HEIGHT - 2) {
                    cout << "#"; // Obstacle
                } else {
                    cout << " "; // Empty space
                }
            }
            cout << "\n";
        }

        // Display scores and controls
        cout << "Score: " << score << "  |  High Score: " << highScore << "\n";
        cout << "Press SPACE to jump, X to quit";
    }

    // Reads user input to control the game
    void input() {
        if (_kbhit()) {
            char ch = _getch();
            if ((ch == ' ' || ch == 'w') && !isJumping) {
                isJumping = true;
                jumpCounter = 0;
            } else if (ch == 'x' || ch == 'X') {
                gameOver = true;
            }
        }
    }

    // Handles game mechanics like jump, obstacle movement, collision
    void logic() {
        // Jump logic (up for 5 ticks, down for 5 ticks)
        if (isJumping) {
            if (jumpCounter < 5) {
                dinoY--; // Move up
            } else if (jumpCounter < 10) {
                dinoY++; // Move down
            } else {
                isJumping = false; // End jump
            }
            jumpCounter++;
        }

        // Move obstacle left
        obstacleX--;
        if (obstacleX < 0) {
            obstacleX = WIDTH - 1; // Reset obstacle to right
            score++;               // Increase score for each jump
        }

        // Check collision (same position as dino)
        if (obstacleX == 5 && dinoY == HEIGHT - 2) {
            gameOver = true;
        }
    }

public:
    // Main play function
    void play() override {
        hideCursor();
        loadHighScore(); // Load previously saved high score

        // Initial setup
        dinoY = HEIGHT - 2;
        isJumping = false;
        jumpCounter = 0;
        obstacleX = WIDTH - 1;
        gameOver = false;
        score = 0;

        // Game loop
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(60); // Control game speed
        }

        // Save high score if new one is set
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }

        // Show game over message
        cout << "\nGame Over! Final Score: " << score << "\n";
        cout << "Highest Score: " << highScore << "\n";
        system("pause");
    }
};

// --------------------------------
// 13. Sudoku
// --------------------------------

class SudokuGame : public Game {
private:
    static const int SIZE = 9; // 9x9 Sudoku board
    int board[SIZE][SIZE];     // Current game board
    int original[SIZE][SIZE];  // Stores original board to prevent edits

    // Checks whether it's safe to place a number in the given row and column
    bool isSafe(int row, int col, int num) {
        // Check row and column
        for (int x = 0; x < SIZE; x++) {
            if (board[row][x] == num || board[x][col] == num)
                return false;
        }

        // Check 3x3 subgrid
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i + startRow][j + startCol] == num)
                    return false;
            }
        }
        return true;
    }

    // Solves the Sudoku board using backtracking
    bool solveBoard() {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (board[row][col] == 0) { // Empty cell
                    for (int num = 1; num <= SIZE; num++) {
                        if (isSafe(row, col, num)) {
                            board[row][col] = num;
                            if (solveBoard()) // Recursive call
                                return true;
                            board[row][col] = 0; // Backtrack
                        }
                    }
                    return false;
                }
            }
        }
        return true; // Solved
    }

    // Removes digits from the solved board to create the puzzle
    void removeDigits(int count) {
        while (count) {
            int i = rand() % SIZE;
            int j = rand() % SIZE;
            if (board[i][j] != 0) {
                original[i][j] = 0; // Mark as editable
                board[i][j] = 0;    // Remove value
                count--;
            }
        }
    }

    // Displays the Sudoku board
    void printBoard() {
        system("cls");
        cout << "    ";
        for (int i = 0; i < SIZE; i++)
            cout << i + 1 << " ";
        cout << "\n  -------------------------\n";

        for (int i = 0; i < SIZE; i++) {
            cout << i + 1 << " | ";
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0)
                    cout << "_ ";
                else
                    cout << board[i][j] << " ";
            }
            cout << "|\n";
        }

        cout << "  -------------------------\n";
    }

    // Checks if the current board is completely and correctly filled
    bool isComplete() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0 || !isSafe(i, j, board[i][j]))
                    return false;
            }
        }
        return true;
    }

public:
    // Game entry point
    void play() override {
        srand(time(0)); // Random seed

        // Initialize board with zeros
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                board[i][j] = 0;

        solveBoard(); // Fill board with a complete valid solution

        // Copy solved board to original
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                original[i][j] = board[i][j];

        // Remove some numbers to make it a playable puzzle
        removeDigits(40); // Adjust the number for difficulty

        int row, col, val;

        // Main game loop
        while (true) {
            printBoard();

            // Check if puzzle is complete
            if (isComplete()) {
                cout << "\nCongratulations! You solved the Sudoku!\n";
                break;
            }

            // Ask user for input
            cout << "\nEnter row (1-9), column (1-9), and value (1-9): ";
            cin >> row >> col >> val;

            // Convert to 0-based indexing
            row--; col--;

            // Input validation
            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || val < 1 || val > 9) {
                cout << "Invalid input. Try again.\n";
                system("pause");
                continue;
            }

            // Prevent changing original values
            if (original[row][col] != 0) {
                cout << "You cannot change this original value!\n";
                system("pause");
                continue;
            }

            // Check if the move is valid
            if (!isSafe(row, col, val)) {
                cout << "Invalid move!\n";
                system("pause");
                continue;
            }

            // Place value
            board[row][col] = val;
        }
    }
};

class PlantsVsZombie : public Game {
public:
    PlantsVsZombie(int zombiesToDefeatLevel = 15) :
        zombiesToSpawnTotal(zombiesToDefeatLevel) {
        if (console_handle == INVALID_HANDLE_VALUE) {
             console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    }

    //~PlantsVsZombie() override {}

    void play() override {
        srand((unsigned int)time(0));
        setCursorVisibility(false);
        system("cls");

        activePlantCount = 0;
        activeZombieCount = 0;
        activeBulletCount = 0;
        zombiesSpawnedSoFar = 0;
        zombiesDefeated = 0;
        zombiesReachedHouse = 0;
        gameTimer = 0;
        spawnTimer = ZOMBIE_SPAWN_DELAY / 3;
        gameOver = false;
        gameWon = false;

        for (int i = 0; i < MAX_PLANTS_IN_POOL; ++i) plants_pool[i].active = false;
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) zombies_pool[i].active = false;
        for (int i = 0; i < MAX_BULLETS_POOL; ++i) bullets_pool[i].active = false;

        while (!gameOver && !gameWon) {
            handleInputInternal();
            updateStateInternal();
            renderInternal();
            Sleep(50);
        }

        system("cls");
        int msgX = GRID_COLS / 2 - 10;
        if (msgX < 1) msgX = 1;
        int msgY = GRID_ROWS / 2;
        gotoRawXY(msgX, msgY);
        SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

        if (gameWon) {
            gotoRawXY(msgX, msgY++); std::cout << "********************";
            gotoRawXY(msgX, msgY++); std::cout << "* YOU WON!        *";
            gotoRawXY(msgX, msgY++); std::cout << "* Zombies Defeated!  *";
            gotoRawXY(msgX, msgY++); std::cout << "********************";
        } else {
            gotoRawXY(msgX, msgY++); std::cout << "********************";
            gotoRawXY(msgX, msgY++); std::cout << "* GAME OVER!      *";
            if (zombiesReachedHouse > 0) {
                gotoRawXY(msgX, msgY++); std::cout << "* A zombie reached!  *";
            } else if (gameOver) {
                gotoRawXY(msgX, msgY++); std::cout << "* You quit the game! *";
            } else {
                gotoRawXY(msgX, msgY++); std::cout << "* *";
            }
            gotoRawXY(msgX, msgY++); std::cout << "********************";
        }
        gotoRawXY(0, GRID_ROWS + 6);
        SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        setCursorVisibility(true);
        std::cout << "\nPlants vs Zombies Game Session Ended.";
        Sleep(1000);

        while (_kbhit()) {
            _getch();
        }
    }

private:
    static const int GRID_ROWS = 5;
    static const int GRID_COLS = 10;
    static const int MAX_PLANTS_IN_POOL = 10;
    static const int MAX_ACTIVE_PLANTS = 3;
    static const int MAX_ZOMBIES_POOL = 20;
    static const int MAX_BULLETS_POOL = 50;

    static const int ZOMBIE_START_COL = GRID_COLS - 1;
    static const int PLANT_COLUMN = 1;
    static const int HOUSE_COL = 0;

    static const int PEASHOOTER_SHOOT_DELAY = 15;

    static const int ZOMBIE_HEALTH = 3;
    static const int ZOMBIE_MOVE_DELAY = 25;
    static const int ZOMBIE_SPAWN_DELAY = 60;

    static const int BULLET_MOVE_DELAY = 2;
    static const int BULLET_DAMAGE = 1;

    static HANDLE console_handle;
    static COORD cursor_pos;

    static void gotoGridXY(int gridX, int gridY) {
        if (gridX < 0 || gridX >= GRID_COLS || gridY < 0 || gridY >= GRID_ROWS) {
            return;
        }
        cursor_pos.X = 1 + gridX * 2;
        cursor_pos.Y = 1 + gridY;
        SetConsoleCursorPosition(console_handle, cursor_pos);
    }

    static void gotoRawXY(int x, int y) {
        cursor_pos.X = x;
        cursor_pos.Y = y;
        SetConsoleCursorPosition(console_handle, cursor_pos);
    }

    static void setCursorVisibility(bool visible) {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(console_handle, &cursorInfo);
        cursorInfo.bVisible = visible;
        SetConsoleCursorInfo(console_handle, &cursorInfo);
    }

    class Bullet {
    public:
        int x, y;
        bool active;
        int moveTimer;
        COORD prevPos;

        Bullet() : x(0), y(0), active(false), moveTimer(0), prevPos({-1, -1}) {}

        void activate(int startGridX, int startGridY) {
            x = startGridX + 1;
            y = startGridY;
            active = true;
            moveTimer = BULLET_MOVE_DELAY;
            prevPos = {-1, -1};
            if (x >= GRID_COLS) {
                active = false;
            }
        }

        void update() {
            if (!active) return;
            prevPos = {(SHORT)(1 + x * 2), (SHORT)(1 + y)};
            moveTimer--;
            if (moveTimer <= 0) {
                x++;
                moveTimer = BULLET_MOVE_DELAY;
                if (x >= GRID_COLS) {
                    active = false;
                }
            }
        }

        void draw() {
            if (active) {
                gotoGridXY(x, y);
                std::cout << "-";
            }
        }

        void clearDraw() {
            if (prevPos.X != -1) {
                gotoRawXY(prevPos.X, prevPos.Y);
                cout << " ";
                prevPos = {-1, -1};
            } else if (!active) {
                gotoGridXY(x, y);
                cout << " ";
            }
        }
    };

    class Peashooter {
    public:
        int x, y;
        bool active;
        int shootTimer;
        bool hasZombieInRow;
        COORD currentPos;

        Peashooter() : x(0), y(0), active(false), shootTimer(PEASHOOTER_SHOOT_DELAY), hasZombieInRow(false), currentPos({-1,-1}) {}

        void place(int col, int row) {
            x = col;
            y = row;
            active = true;
            shootTimer = PEASHOOTER_SHOOT_DELAY / 2;
            hasZombieInRow = false;
            currentPos = {(SHORT)(1 + x * 2), (SHORT)(1 + y)};
        }

        void detectZombie(bool zombiePresent) {
            hasZombieInRow = zombiePresent;
        }

        bool update() {
            if (!active) return false;
            currentPos = {(SHORT)(1 + x * 2), (SHORT)(1 + y)};
            if (hasZombieInRow) {
                shootTimer--;
                if (shootTimer <= 0) {
                    shootTimer = PEASHOOTER_SHOOT_DELAY;
                    return true;
                }
            } else {
                if (shootTimer < PEASHOOTER_SHOOT_DELAY) {
                    shootTimer = PEASHOOTER_SHOOT_DELAY;
                }
            }
            return false;
        }

        void draw() {
            if (active) {
                gotoGridXY(x, y);
                SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << "P";
                SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
        void clearDraw() {
            if (!active && currentPos.X != -1) {
                gotoRawXY(currentPos.X, currentPos.Y);
                cout << " ";
                currentPos = {-1,-1};
            }
        }
    };

    class Zombie {
    public:
        int x, y;
        bool active;
        int health;
        int moveTimer;
        COORD prevPos;

        Zombie() : x(0), y(0), active(false), health(0), moveTimer(0), prevPos({-1,-1}) {}

        void spawn(int row) {
            x = ZOMBIE_START_COL;
            y = row;
            active = true;
            health = ZOMBIE_HEALTH;
            moveTimer = ZOMBIE_MOVE_DELAY;
            prevPos = {-1,-1};
        }

        bool update() {
            if (!active) return false;
            prevPos = {(SHORT)(1 + x * 2), (SHORT)(1 + y)};
            moveTimer--;
            if (moveTimer <= 0) {
                x--;
                moveTimer = ZOMBIE_MOVE_DELAY;
                if (x < HOUSE_COL) {
                    active = false;
                    return true;
                }
            }
            return false;
        }

        void takeDamage(int damage) {
            if (!active) return;
            health -= damage;
            if (health <= 0) {
                active = false;
            }
        }

        void draw() {
            if (active) {
                gotoGridXY(x, y);
                SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << "Z";
                SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
        void clearDraw() {
            if (prevPos.X != -1) {
                gotoRawXY(prevPos.X, prevPos.Y);
                cout << " ";
                prevPos = {-1,-1};
            } else if (!active) {
                gotoGridXY(x,y);
                cout << " ";
            }
        }
    };

    Peashooter plants_pool[MAX_PLANTS_IN_POOL];
    Zombie zombies_pool[MAX_ZOMBIES_POOL];
    Bullet bullets_pool[MAX_BULLETS_POOL];

    int activePlantCount;
    int activeZombieCount;
    int activeBulletCount;

    int zombiesToSpawnTotal;
    int zombiesSpawnedSoFar;
    int zombiesDefeated;
    int zombiesReachedHouse;

    int gameTimer;
    int spawnTimer;

    bool gameOver;
    bool gameWon;

    int findInactivePlantSlot() {
        for (int i = 0; i < MAX_PLANTS_IN_POOL; ++i) {
            if (!plants_pool[i].active) return i;
        }
        return -1;
    }

    int findInactiveZombieSlot() {
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) {
            if (!zombies_pool[i].active) return i;
        }
        return -1;
    }

    int findInactiveBulletSlot() {
        for (int i = 0; i < MAX_BULLETS_POOL; ++i) {
            if (!bullets_pool[i].active) return i;
        }
        return -1;
    }

    int getPlantIndexAt(int gridX, int gridY) {
        for (int i = 0; i < MAX_PLANTS_IN_POOL; ++i) {
            if (plants_pool[i].active && plants_pool[i].x == gridX && plants_pool[i].y == gridY) {
                return i;
            }
        }
        return -1;
    }
    
    bool isZombieInRow(int gridY) {
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) {
            if (zombies_pool[i].active && zombies_pool[i].y == gridY) {
                return true;
            }
        }
        return false;
    }

    void spawnNewZombie() {
        if (zombiesSpawnedSoFar >= zombiesToSpawnTotal) return;
        if (activeZombieCount >= MAX_ZOMBIES_POOL) return;

        spawnTimer--;
        if (spawnTimer <= 0) {
            int i = findInactiveZombieSlot();
            if (i != -1) {
                int row = rand() % GRID_ROWS;
                zombies_pool[i].spawn(row);
                zombiesSpawnedSoFar++;
                spawnTimer = ZOMBIE_SPAWN_DELAY + (rand() % (ZOMBIE_SPAWN_DELAY / 2)) - (ZOMBIE_SPAWN_DELAY / 4);
            } else {
                spawnTimer = ZOMBIE_SPAWN_DELAY / 2;
            }
        }
    }

    void spawnNewBullet(int plantGridX, int plantGridY) {
        if (activeBulletCount >= MAX_BULLETS_POOL) return;

        int i = findInactiveBulletSlot();
        if (i != -1) {
            bullets_pool[i].activate(plantGridX, plantGridY);
        }
    }

    void handleInputInternal() {
        if (_kbhit()) {
            char key = _getch();

            if (key >= '1' && key <= '0' + GRID_ROWS) {
                int row = key - '1';
                int targetCol = PLANT_COLUMN;

                int plantIndexAtTarget = getPlantIndexAt(targetCol, row);

                if (plantIndexAtTarget != -1) {
                    plants_pool[plantIndexAtTarget].active = false;
                } else {
                    if (activePlantCount < MAX_ACTIVE_PLANTS) {
                        int newPlantSlot = findInactivePlantSlot();
                        if (newPlantSlot != -1) {
                            plants_pool[newPlantSlot].place(targetCol, row);
                        }
                    }
                }
            } else if (key == 'q' || key == 27) {
                gameOver = true;
            }
        }
    }

    void clearOldPositionsInternal() {
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) {
            if (zombies_pool[i].active) zombies_pool[i].clearDraw();
        }
        for (int i = 0; i < MAX_BULLETS_POOL; ++i) {
            if (bullets_pool[i].active) bullets_pool[i].clearDraw();
        }

        for (int i = 0; i < MAX_PLANTS_IN_POOL; ++i) {
            if (!plants_pool[i].active) plants_pool[i].clearDraw();
        }
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) {
            if (!zombies_pool[i].active) zombies_pool[i].clearDraw();
        }
        for (int i = 0; i < MAX_BULLETS_POOL; ++i) {
            if (!bullets_pool[i].active) bullets_pool[i].clearDraw();
        }
    }

    void updateStateInternal() {
        gameTimer++;

        int currentActivePlants = 0;
        for (int i = 0; i < MAX_PLANTS_IN_POOL; ++i) if (plants_pool[i].active) currentActivePlants++;
        activePlantCount = currentActivePlants;

        int currentActiveZombies = 0;
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) if (zombies_pool[i].active) currentActiveZombies++;
        activeZombieCount = currentActiveZombies;

        int currentActiveBullets = 0;
        for (int i = 0; i < MAX_BULLETS_POOL; ++i) if (bullets_pool[i].active) currentActiveBullets++;
        activeBulletCount = currentActiveBullets;

        for (int i = 0; i < MAX_BULLETS_POOL; ++i) {
            if (bullets_pool[i].active) bullets_pool[i].update();
        }

        for (int i = 0; i < MAX_PLANTS_IN_POOL; ++i) {
            if (plants_pool[i].active) {
                plants_pool[i].detectZombie(isZombieInRow(plants_pool[i].y));
                if (plants_pool[i].update()) {
                    spawnNewBullet(plants_pool[i].x, plants_pool[i].y);
                }
            }
        }

        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) {
            if (zombies_pool[i].active) {
                if (zombies_pool[i].update()) {
                    gameOver = true;
                    zombiesReachedHouse++;
                }
            }
        }

        for (int b = 0; b < MAX_BULLETS_POOL; ++b) {
            if (!bullets_pool[b].active) continue;
            for (int z = 0; z < MAX_ZOMBIES_POOL; ++z) {
                if (!zombies_pool[z].active) continue;

                if (bullets_pool[b].x == zombies_pool[z].x && bullets_pool[b].y == zombies_pool[z].y) {
                    zombies_pool[z].takeDamage(BULLET_DAMAGE);
                    bullets_pool[b].active = false;

                    if (!zombies_pool[z].active) {
                        zombiesDefeated++;
                    }
                    gotoGridXY(bullets_pool[b].x, bullets_pool[b].y);
                    std::cout << " ";
                    break;
                }
            }
        }

        spawnNewZombie();

        if (gameOver) return;

        currentActiveZombies = 0;
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) if (zombies_pool[i].active) currentActiveZombies++;
        activeZombieCount = currentActiveZombies;

        if (zombiesDefeated >= zombiesToSpawnTotal && activeZombieCount == 0) {
            if (zombiesSpawnedSoFar >= zombiesToSpawnTotal) {
                 gameWon = true;
            }
        }
    }

    void drawBackgroundAndUIInternal() {
        SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        gotoRawXY(0, 0);
        cout << "+";
        for (int j = 0; j < GRID_COLS; ++j) std::cout << "--";
        cout << "+";

        int remainingToSpawn = zombiesToSpawnTotal - zombiesSpawnedSoFar;
        if (remainingToSpawn < 0) remainingToSpawn = 0;
        int totalZombiesLeftInLevel = activeZombieCount + remainingToSpawn;

        cout << " Zombies: " << totalZombiesLeftInLevel << "/" << zombiesToSpawnTotal << "   ";
        cout << " Defeated: " << zombiesDefeated << "   ";
        cout << " Plants: " << activePlantCount << "/" << MAX_ACTIVE_PLANTS << "    \n";

        for (int i = 0; i < GRID_ROWS; ++i) {
            gotoRawXY(0, i + 1);
            std::cout << "|";
            for (int j = 0; j < GRID_COLS; ++j) {
                gotoGridXY(j, i);
                cout << " ";
            }
            gotoRawXY(1 + GRID_COLS * 2, i + 1);
            cout << "|";
        }

        gotoRawXY(0, GRID_ROWS + 1);
        cout << "+";
        for (int j = 0; j < GRID_COLS; ++j) std::cout << "--";
        cout << "+\n";

        gotoRawXY(0, GRID_ROWS + 2);
        cout << "Controls: '1'-'" << GRID_ROWS << "' for Peashooter (P) in Col " << (PLANT_COLUMN + 1)
                  << ". Max: " << MAX_ACTIVE_PLANTS << ". 'q' to Quit.     \n";
        gotoRawXY(0, GRID_ROWS + 3);
        cout << "Objective: Defeat " << zombiesToSpawnTotal << " Zombies (Z). Don't let them reach the house!         \n";
        gotoRawXY(0, GRID_ROWS + 4);
        cout << "                                                                                  \n";
    }

    void renderInternal() {
        drawBackgroundAndUIInternal();
        clearOldPositionsInternal();

        for (int i = 0; i < MAX_PLANTS_IN_POOL; ++i) {
            if (plants_pool[i].active) plants_pool[i].draw();
        }
        for (int i = 0; i < MAX_ZOMBIES_POOL; ++i) {
            if (zombies_pool[i].active) zombies_pool[i].draw();
        }
        for (int i = 0; i < MAX_BULLETS_POOL; ++i) {
            if (bullets_pool[i].active) bullets_pool[i].draw();
        }
        gotoRawXY(0, GRID_ROWS + 5);
    }
};

HANDLE PlantsVsZombie::console_handle = INVALID_HANDLE_VALUE;
COORD PlantsVsZombie::cursor_pos = {0,0};

// --------------------------------
// Play Store
// --------------------------------

class PlayStore {
private:
    vector<string> availableGames;

public:
    PlayStore() {
        availableGames.push_back("BattleGround");
        availableGames.push_back("Hangman");
        availableGames.push_back("Tic Tac Toe");
        availableGames.push_back("Treasure Hunt");
        availableGames.push_back("Memory Game");
        availableGames.push_back("Typing Speed");
        availableGames.push_back("Rock Paper Scissors");
        availableGames.push_back("KBC");
        availableGames.push_back("Snake Game");
        availableGames.push_back("Flappy Bird");
        availableGames.push_back("Brick Breaker");
        availableGames.push_back("Dino Game");
        availableGames.push_back("Sudoku");
        availableGames.push_back("Plants vs Zombie");
    }

    void showGames() const {
        cout << "--- PlayStore Games ---\n";
        for (size_t i = 0; i < availableGames.size(); ++i) {
            std::cout << i + 1 << ". " << availableGames[i] << "\n";
        }
        cout << availableGames.size() + 1 << ". Back to Main Menu\n";
        cout << "-----------------------\n";
    }

    string getGame(int choice) const {
        if (choice > 0 && choice <= static_cast<int>(availableGames.size())) {
            return availableGames[choice - 1];
        }
        return ""; // Invalid choice
    }

    int getReturnOption() const {
        return static_cast<int>(availableGames.size() + 1);
    }
};

// --------------------------------
// Game Launcher
// --------------------------------

class GameLauncher
{
private:
    // Structure representing an installed game with its name
    struct InstalledGame
    {
        Game *game;
        string name;
    };

    // Array to store installed games (max 10)
    InstalledGame installedGames[10];
    int installedCount = 0; // Current number of installed games

    // Account credentials
    string username, email, password;
    bool isLoggedIn = false;
    bool shouldExit = false; // Flag to control main application loop

    // File paths for data storage
    const string USERS_FILE = "users.txt";
    const string SESSION_FILE = "session.txt";

    // Structure for user account data
    struct UserAccount
    {
        string username;
        string email;
        string password;
    };

    // Checks if email contains '@' to validate format
    bool validEmail(const string &email)
    {
        return email.find('@') != string::npos;
    }

    // Save user account to file
    void saveUserAccount(const UserAccount &user)
    {
        ofstream file(USERS_FILE, ios::app);
        if (file.is_open())
        {
            file << user.email << "|" << user.username << "|" << user.password << endl;
            file.close();
        }
    }

    // Load all user accounts from file
    vector<UserAccount> loadUserAccounts()
    {
        vector<UserAccount> users;
        ifstream file(USERS_FILE);
        string line;
        
        while (getline(file, line))
        {
            if (!line.empty())
            {
                UserAccount user;
                stringstream ss(line);
                getline(ss, user.email, '|');
                getline(ss, user.username, '|');
                getline(ss, user.password, '|');
                users.push_back(user);
            }
        }
        file.close();
        return users;
    }

    // Check if email already exists
    bool emailExists(const string &email)
    {
        vector<UserAccount> users = loadUserAccounts();
        for (const auto &user : users)
        {
            if (user.email == email)
                return true;
        }
        return false;
    }

    // Validate login credentials
    bool validateLogin(const string &email, const string &password, string &outUsername)
    {
        vector<UserAccount> users = loadUserAccounts();
        for (const auto &user : users)
        {
            if (user.email == email && user.password == password)
            {
                outUsername = user.username;
                return true;
            }
        }
        return false;
    }

    // Save current session
    void saveSession()
    {
        ofstream file(SESSION_FILE);
        if (file.is_open())
        {
            file << email << "|" << username << "|" << password << endl;
            file.close();
        }
    }

    // Load previous session
    bool loadSession()
    {
        ifstream file(SESSION_FILE);
        if (file.is_open())
        {
            string line;
            if (getline(file, line) && !line.empty())
            {
                stringstream ss(line);
                getline(ss, email, '|');
                getline(ss, username, '|');
                getline(ss, password, '|');
                file.close();
                
                // Validate session data
                string tempUsername;
                if (validateLogin(email, password, tempUsername))
                {
                    isLoggedIn = true;
                    return true;
                }
            }
            file.close();
        }
        return false;
    }

    // Clear session file
    void clearSession()
    {
        ofstream file(SESSION_FILE, ios::trunc);
        file.close();
    }

    // Get user-specific games file path
    string getUserGamesFile()
    {
        // Ensure the email is clean for filename (replace special chars for safety)
        string cleanedEmail = email;
        replace(cleanedEmail.begin(), cleanedEmail.end(), '@', '_');
        replace(cleanedEmail.begin(), cleanedEmail.end(), '.', '_');
        return cleanedEmail + "_games.txt";
    }

    // Save user's installed games
    void saveUserGames()
    {
        if (email.empty())
        {
            return;
        }
        
        string gamesFile = getUserGamesFile();
        ofstream file(gamesFile);
        if (file.is_open())
        {
            for (int i = 0; i < installedCount; i++)
            {
                file << installedGames[i].name << endl;
            }
            file.close();
        }
        else
        {
            cout << "Error: Could not save games to file: " << gamesFile << "\n";
        }
    }

    // Load user's installed games
    void loadUserGames()
    {
        if (email.empty())
        {
            return;
        }
        
        // Clear current games first to avoid memory leaks and old games persisting
        for (int i = 0; i < installedCount; i++)
        {
            delete installedGames[i].game;
            installedGames[i].game = nullptr; // Set to nullptr after deleting
        }
        installedCount = 0;

        string gamesFile = getUserGamesFile();
        
        ifstream file(gamesFile);
        
        if (file.is_open())
        {
            string gameName;
            while (getline(file, gameName) && installedCount < 10)
            {
                if (!gameName.empty())
                {
                    // Recreate game objects
                    Game* gameObj = createGameObject(gameName);
                    if (gameObj != nullptr)
                    {
                        installedGames[installedCount].game = gameObj;
                        installedGames[installedCount].name = gameName;
                        installedCount++;
                    }
                    else
                    {
                        cout << "Error: Failed to create game object for: " << gameName << "\n";
                    }
                }
            }
            file.close();
        }
        // No "Could not open file" message here, as it's normal for new users
    }

    // Helper function to create game objects
    Game* createGameObject(const string &gameName)
    {
        if (gameName == "BattleGround")
            return new BattleGround();
        else if (gameName == "Hangman")
            return new Hangman();
        else if (gameName == "Tic Tac Toe")
            return new TicTacToe();
        else if (gameName == "Treasure Hunt")
            return new TreasureHunt();
        else if (gameName == "Memory Game")
            return new MemoryGame();
        else if (gameName == "Typing Speed")
            return new TypingSpeedTest();
        else if (gameName == "Rock Paper Scissors")
            return new RockPaperScissors();
        else if (gameName == "KBC")
            return new KBCGame();
        else if (gameName == "Snake Game")
            return new SnakeGame();
        else if (gameName == "Flappy Bird")
            return new FlappyBird();
        else if (gameName == "Brick Breaker")
            return new BrickBreaker();
        else if (gameName == "Dino Game")
            return new DinoGame();
        else if (gameName == "Sudoku")
            return new SudokuGame();
        else if (gameName == "Plants vs Zombie")
            return new PlantsVsZombie();
        else
            return nullptr; // Unknown game
    }

    // Creates a new account with basic validation
    void createAccount()
    {
        string tempEmail, tempUsername, tempPassword; // Use temp variables for input
        while (true)
        {
            system("cls");
            cout << "--- Create Your GameLauncher Account ---\n";
            cout << "Enter Email: ";
            getline(cin, tempEmail);

            // Email must contain '@'
            if (!validEmail(tempEmail))
            {
                cout << "Invalid email format! Must contain '@'. Try again.\n";
                system("pause");
                continue;
            }

            // Check if email already exists
            if (emailExists(tempEmail))
            {
                cout << "Email already registered! Please use a different email or sign in.\n";
                system("pause");
                continue;
            }

            // Input username and password
            cout << "Enter username: ";
            getline(cin, tempUsername);
            cout << "Enter Password: ";
            getline(cin, tempPassword);

            string confirmPassword;
            cout << "Confirm Password: ";
            getline(cin, confirmPassword);

            // Confirm both passwords match
            if (tempPassword != confirmPassword)
            {
                cout << "Passwords do not match! Try again.\n";
                system("pause");
            }
            else
            {
                // Save new user account
                UserAccount newUser = {tempUsername, tempEmail, tempPassword};
                saveUserAccount(newUser);
                
                // Set current session details
                this->email = tempEmail;
                this->username = tempUsername;
                this->password = tempPassword;
                saveSession(); // Auto-login after registration
                isLoggedIn = true;
                
                // Initialize empty games for new user
                installedCount = 0; 
                saveUserGames(); // Ensure an empty games file is created for a new user

                cout << "Account Created Successfully! You are now logged in.\n";
                system("pause");
                break;
            }
        }
    }

    // Handle user login
    void loginUser()
    {
        string inputEmail, inputPassword; // Use temp variables for input
        while (true)
        {
            system("cls");
            cout << "--- Sign In to GameLauncher ---\n";
            cout << "Enter Email: ";
            getline(cin, inputEmail);
            cout << "Enter Password: ";
            getline(cin, inputPassword);

            string tempUsername;
            if (validateLogin(inputEmail, inputPassword, tempUsername))
            {
                // Set current session details
                this->email = inputEmail;
                this->username = tempUsername;
                this->password = inputPassword; // Store password to save session later
                saveSession(); // Save session for future logins
                isLoggedIn = true;
                loadUserGames(); // Load user's games specific to this email
                
                cout << "Login Successful! Welcome back, " << username << "!\n";
                cout << "Loaded " << installedCount << " games from your account.\n";
                system("pause");
                break;
            }
            else
            {
                cout << "Invalid credentials! Email not registered or password incorrect.\n";
                cout << "1. Try Again\n2. Create New Account\n3. Exit\n";
                cout << "Choose option: ";
                
                int choice;
                cin >> choice;
                // Clear the input buffer after reading an integer
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (choice == 2)
                {
                    createAccount();
                    // After creating account, the user is logged in, and their games are handled
                    break; 
                }
                else if (choice == 3)
                {
                    shouldExit = true; // Set flag to exit main run loop
                    return; // Return from loginUser
                }
                // choice == 1 continues the loop
            }
        }
    }

    // Authentication menu
    void authenticationMenu()
    {
        while (!isLoggedIn && !shouldExit) // Also check shouldExit to break out if user chooses to exit
        {
            system("cls");
            cout << "===== Welcome to GameLauncher =====\n";
            cout << "1. Sign In\n";
            cout << "2. Create New Account\n";
            cout << "3. Exit\n";
            cout << "Select an option: ";

            int choice;
            cin >> choice;
            // Clear the input buffer after reading an integer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1:
                loginUser();
                // If loginUser resulted in exit choice, break authenticationMenu loop
                if (shouldExit) break; 
                break; // Break from switch, then while condition will be re-evaluated
            case 2:
                createAccount();
                // If createAccount resulted in exit choice, break authenticationMenu loop
                if (shouldExit) break; 
                break; // Break from switch, then while condition will be re-evaluated
            case 3:
                shouldExit = true; // Set flag to exit main run loop
                return; // Return from authenticationMenu
            default:
                cout << "Invalid choice! Try again.\n";
                system("pause");
            }
            // If loginUser or createAccount succeeded, isLoggedIn will be true, loop will exit
            // If user chooses to exit, shouldExit will be true, loop will exit
        }
    }

    // Shows the main menu of GameLauncher
    void showMenu()
    {
        int choice;
        do
        {
            system("cls");
            cout << "===== Welcome to GameLauncher, " << username << " =====\n";

            cout << "1. View Installed Games\n";
            cout << "2. Install Games from PlayStore\n";
            cout << "3. Switch Account\n";
            cout << "4. Exit Launcher\n";
            cout << "Select an option: ";

            cin >> choice;
            // Clear the input buffer after reading an integer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1:
                showInstalledGames();
                break;
            case 2:
                installGames();
                break;
            case 3:
                switchAccount();
                // After switching, isLoggedIn will be false, and the outer loop in run() will re-call authenticationMenu.
                return; // Exit this showMenu, so run() can re-evaluate and call authenticationMenu
            case 4:
                cout << "Exiting Launcher. Goodbye!\n";
                shouldExit = true; // Set flag to exit main run loop
                return; // Exit this showMenu, and the main run loop will also terminate
            default:
                cout << "Invalid choice! Try again.\n";
                system("pause");
            }
        } while (true); // Loop indefinitely until an exit condition or switch account is met
    }

    // Switch to another account
    void switchAccount()
    {
        // Save current user's games before switching
        if (isLoggedIn) { // Only save if a user is currently logged in
            saveUserGames();
        }
        
        // Clear current session
        clearSession();
        isLoggedIn = false;
        
        // Clear current games from memory to prevent mixing games between users
        for (int i = 0; i < installedCount; i++)
        {
            delete installedGames[i].game;
            installedGames[i].game = nullptr; // Set to nullptr after deleting
        }
        installedCount = 0;
        
        // Clear user data (email and username) - crucial to ensure new login sets them
        username.clear();
        email.clear();
        password.clear(); // Clear password for security

        cout << "Account switched. You will be redirected to the login/signup screen.\n";
        system("pause");
        
        // `isLoggedIn` is now false. The `run()` loop will detect this
        // and call `authenticationMenu()` again.
        // We do NOT call authenticationMenu() directly here, as `run()` handles it.
    }

    // Displays the installed games and allows playing or uninstalling
    void showInstalledGames()
    {
        if (installedCount == 0)
        {
            cout << "No games installed yet. Please install from PlayStore.\n";
            system("pause");
            return;
        }

        while (true)
        {
            system("cls");
            cout << "--- Installed Games ---\n";
            for (int i = 0; i < installedCount; ++i)
            {
                cout << i + 1 << ". " << installedGames[i].name << "\n";
            }

            cout << installedCount + 1 << ". Uninstall a Game\n";
            cout << installedCount + 2 << ". Back to Menu\n";
            cout << "Select an option: ";

            int choice;
            cin >> choice;
            // Clear the input buffer after reading an integer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == installedCount + 2)
            {
                break;
            }
            else if (choice == installedCount + 1)
            {
                uninstallGame();
            }
            else if (choice < 1 || choice > installedCount)
            {
                cout << "Invalid choice. Try again.\n";
                system("pause");
            }
            else
            {
                installedGames[choice - 1].game->play();
                system("pause");
            }
        }
    }

    // Handle game uninstallation
    void uninstallGame()
    {
        cout << "Enter the number of the game to uninstall: ";
        int deleteChoice;
        cin >> deleteChoice;
        // Clear the input buffer after reading an integer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (deleteChoice < 1 || deleteChoice > installedCount)
        {
            cout << "Invalid choice. Try again.\n";
            system("pause");
            return;
        }

        cout << "Are you sure you want to uninstall \"" << installedGames[deleteChoice - 1].name << "\"? (Y/N): ";
        char confirm;
        cin >> confirm;
        // Clear the input buffer after reading a character
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirm == 'Y' || confirm == 'y')
        {
            delete installedGames[deleteChoice - 1].game;
            installedGames[deleteChoice - 1].game = nullptr; // Set to nullptr

            // Shift remaining games
            for (int i = deleteChoice - 1; i < installedCount - 1; ++i)
            {
                installedGames[i] = installedGames[i + 1];
            }
            installedCount--;

            // Save updated games list
            saveUserGames();
            
            cout << "Game deleted successfully!\n";
        }
        else
        {
            cout << "Deletion cancelled.\n";
        }
        system("pause");
    }

    // Allows installation of games from the PlayStore
    void installGames()
    {
        PlayStore store;
        while (true)
        {
            system("cls");
            store.showGames();
            cout << "Select a game to install: ";
            int choice;
            cin >> choice;
            // Clear the input buffer after reading an integer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == store.getReturnOption())
            {
                break;
            }

            string gameName = store.getGame(choice);
            if (gameName.empty())
            {
                cout << "Invalid choice! Try again.\n";
                system("pause");
                continue;
            }

            if (installedCount >= 10)
            {
                cout << "Max Storage Reached. Delete old games to install new.\n";
                system("pause");
                continue;
            }

            // Check for duplicates
            bool alreadyInstalled = false;
            for (int i = 0; i < installedCount; i++)
            {
                if (installedGames[i].name == gameName)
                {
                    alreadyInstalled = true;
                    break;
                }
            }

            if (alreadyInstalled)
            {
                cout << gameName << " is already installed!\n";
                system("pause");
                continue;
            }

            // Install the game
            Game* gameObj = createGameObject(gameName);
            if (gameObj != nullptr)
            {
                installedGames[installedCount].game = gameObj;
                installedGames[installedCount].name = gameName;
                installedCount++;
                
                // Save updated games list
                saveUserGames();
                
                cout << gameName << " installed successfully!\n";
            }
            else
            {
                cout << "Error: Unable to create game object for " << gameName << "\n";
            }
            system("pause");
        }
    }

public:
    // Entry point for the launcher
    void run()
    { 
        // Main application loop: keeps the launcher running until shouldExit is true
        while (!shouldExit)
        {
            // If not logged in, manage authentication (login or create account)
            if (!isLoggedIn)
            {
                // First try to load a previous session
                if (loadSession())
                {
                    cout << "Welcome back, " << username << "!\n";
                    loadUserGames(); // Load games for the loaded session
                    system("pause");
                    // isLoggedIn is already true from loadSession()
                }
                else
                {
                    // If no session, prompt for sign in/create account
                    authenticationMenu();
                    if (shouldExit) // If authenticationMenu sets shouldExit, break the main loop
                    {
                        break;
                    }
                    // After authenticationMenu, if not exited, isLoggedIn should be true
                    // and loadUserGames() has been called within loginUser/createAccount.
                }
            }

            // If we are logged in (either by loading session or fresh login/creation)
            // and the user has not chosen to exit the application
            if (isLoggedIn && !shouldExit)
            {
                showMenu(); // This will loop until "Exit Launcher" or "Switch Account" is chosen
            }
            // If showMenu returns, it means:
            // 1. User chose "Exit Launcher" -> shouldExit is true, loop will terminate in next iteration.
            // 2. User chose "Switch Account" -> isLoggedIn is false, loop will re-enter `if (!isLoggedIn)` block.
        }

        // Clean up all dynamically allocated games on exit
        // This is only called once the main application loop exits (when shouldExit is true)
        if (isLoggedIn) { // Only save games if a user is still logged in when exiting
            saveUserGames(); 
        }

        for (int i = 0; i < installedCount; ++i)
        {
            delete installedGames[i].game;
            installedGames[i].game = nullptr; // Set to nullptr to avoid double deletion issues
        }
    }
    
    // Destructor to ensure resources are freed when GameLauncher object is destroyed
    ~GameLauncher() {
        // This is a safety net; `run()` should handle most cleanup on exit
        // but if GameLauncher object is destroyed before `run()` completes or if `run()` is not called
        for (int i = 0; i < installedCount; ++i) {
            delete installedGames[i].game;
            installedGames[i].game = nullptr;
        }
    }
};

// --------------------------------
// Main Launcher
// --------------------------------

int main()
{
    srand(time(0)); // Seed random generator (if games need it)

    GameLauncher launcher;
    launcher.run(); // Start the game launcher

    return 0;
}



