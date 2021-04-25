#include <iostream>
#include <map>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <algorithm>

#include <chrono>

struct Timer
{
public:
    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time took: " << elapsed.count()*1000 << "ms" << std::endl;
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

void guessNumber(int max = 20)
{
    int tries = 0;
    std::map<int, bool> guesses;
    srand(std::time(nullptr));
    int secret = rand() % max + 1;
    do
    {
        std::cout << "Guess what number I am thinking between 1 and " << max << std::endl;
        int input;
        std::cin >> input;
        if (guesses[input])
        {
            std::cout << "You've already guessed " << input << std::endl;
        }
        else if (input == secret)
        {
            tries++;
            break;
        }
        else
        {
            if (input > secret)
                std::cout << "Your guess is too large" << std::endl;
            else
                std::cout << "Your guess is too small" << std::endl;
            tries++;
            guesses[input] = true;
        }
    } while(true);
    std::cout << "Well done! It took you " << tries << " tries" << std::endl;
}

std::map<char, std::string> morseCode =
{
    {'A', "•–"},
    {'B', "–•••"},
    {'C', "–•–•"},
    {'D', "–••"},
    {'E', "•"},
    {'F', "••–•"},
    {'G', "––•"},
    {'H', "••••"},
    {'I', "••"},
    {'J', "•–––"},
    {'K', "–•–"},
    {'L', "•–••"},
    {'M', "––"},
    {'N', "–•"},
    {'O', "–––"},
    {'P', "•––•"},
    {'Q', "––•–"},
    {'R', "•–•"},
    {'S', "•••"},
    {'T', "–"},
    {'U', "••–"},
    {'V', "•••–"},
    {'W', "•––"},
    {'X', "–••–"},
    {'Y', "–•––"},
    {'Z', "––••"},
    {'1', "•––––"},
    {'2', "••–––"},
    {'3', "•••––"},
    {'4', "••••–"},
    {'5', "•••••"},
    {'6', "–••••"},
    {'7', "––•••"},
    {'8', "–––••"},
    {'9', "––––•"},
    {'0', "–––––"},
    {' ', "       "},
};

void lineToMorse()
{
    std::string line;
    std::cout << "Type line you wish translated to morse:" << std::endl;
    std::getline(std::cin, line);
    Timer timer;
    std::transform(line.begin(), line.end(), line.begin(), ::toupper);
    for (char c : line)
    {
        auto morse = morseCode[c];
        if (morse == "")
            morse = c;
        std::cout << morse << " ";
    }
    std::cout << std::endl;
}

int main()
{
    //guessNumber();
    lineToMorse();
    return 0;
}