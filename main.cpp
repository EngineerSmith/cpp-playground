#include <iostream>
#include <map>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <chrono>
#include <functional>
#include <regex>
#include <memory>

template<typename T>
using ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr ref<T> makeref(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

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

inline void ToUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
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
    ToUpper(line);
    for (char c : line)
    {
        auto morse = morseCode[c];
        if (morse == "")
            morse = c;
        std::cout << morse << " ";
    }
    std::cout << std::endl;
}

namespace BattleShip
{
    // Ship Lengths
    namespace Length {
        int Carrier    = 5;
        int Battleship = 4;
        int Destroyer  = 3;
        int Submarine  = 3;
        int PatrolBoat = 2;
    }
    
    using Row = std::vector<char>;
    
    class Board
    {
        private:
            std::vector<Row> map;
        public:
            Board(const char& base)
                : map(10)
            {
                for (int y = 0; y < 10; y++)
                {
                    map[y] = Row(10);
                    for (int x = 0; x < 10; x++)
                    {
                        map[y][x] = base;
                    }
                }
            }
            
            const Row& getRow(int row) const
            {
                return map[row];
            }
            
            const void setChar(int row, int column, char c)
            {
                map[row][column] = c;
            }
    };
    
    struct Point 
    {
        char X;
        int Y;
        
        int getX() const
        {
            return (int)X - (int)'A';
        }
        
        friend std::ostream & operator<<(std::ostream & os, Point const& p) 
        {
            os << p.X << ":" << p.Y;
            return os;
        }
    };
    
    
    
    void drawRows(int index, const Row& row1, const Row& row2)
    {
        // Draw first board's row
        printf(" %2d", index);
        for (auto c : row1)
            std::cout << " " << c;
        // Draw second board's row
        printf("\t %2d", index);
        for (auto c : row2)
            std::cout << " " << c;
        std::cout << "\n";
    }
    
    using drawBoardFn = std::function<void()>;
    
    void drawBoard(const ref<Board>& own, const ref<Board>& fire)
    {
        printf("\033c"); // Clear console
        printf(" Own Board             \tFiring Board\n");
        printf("    A B C D E F G H I J\t    A B C D E F G H I J\n");
        for (int i = 0; i < 10; i++)
            drawRows(i+1, own->getRow(i), fire->getRow(i));
        std::cout << std::endl;
    }
    
    Point StringToPoint(std::string coord)
    {
        ToUpper(coord);
        Point p;
        p.X = coord[0];
        constexpr int beginning = (int)'0';
        if (coord.size() == 5)
            p.Y = 10; //((int)coord[2] - beginning * 10) + (int)coord[3] - beginning;
            // 10 is the only number that is two characters long
        else
            p.Y = (int)coord[2] - beginning;
        return p;
    }
    
    std::string getLine()
    {
        std::string line;
        std::getline(std::cin, line);
        line += ' ';
        return line;
    }
    
    const std::regex pointRegex("([A-Ja-j]):([1-9]|10)[^1-9\\S]");
    
    void InitBoard(drawBoardFn draw, ref<Board> own)
    {
        bool finished = false;
        auto rbegin = std::sregex_iterator(), rend = std::sregex_iterator();
        std::string warning;
        do
        {
            draw();
            std::cout << " First you must place your own ships! Format: \"A:1 A:5\"\n\tCarrier (5)\n\t" << warning << std::endl;
            
            std::string line = getLine();
            
            rbegin = std::sregex_iterator(line.begin(), line.end(), pointRegex);
            int c = std::distance(rbegin, rend);
            
            if (c != 2)
            {
                warning = "Couldn't match 2 points, please try again.";
                continue;
            }
            
            finished = true;
            
        } while(!finished);
        
        std::cout << "TWO MATCHES" << std::endl;
        for (auto it = rbegin; it != rend; ++it)
        {
            std::smatch match = *it;
            Point p = StringToPoint(match.str());
            own->setChar(p.Y-1, p.getX(), '#');  
        }
        
        draw();
        
    }
    
    void StartGame()
    {
        ref<Board> own = makeref<Board>(' '), enemy = makeref<Board>('~');
        drawBoardFn draw = std::bind(drawBoard, own, enemy);
        InitBoard(draw, own);
    }
}

int main()
{
    //guessNumber();
    //lineToMorse();
    ::BattleShip::StartGame();
    return 0;
}