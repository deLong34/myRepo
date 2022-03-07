#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <locale>

using namespace std;

//========== Типы данных =================
enum TCell : char {
    CROSS = 'X',
    ZERO = '0',
    EMPTY = '_'
};

enum  Tprogress {
    IN_PROGRESS,
    WIN_AI,
    WIN_HUMAN,
    IS_DRAW
};

struct Tcoord {
    size_t y{};
    size_t x{};
};

struct  TGame {
    const size_t SIZE{ 3 };             //размер поля
    TCell** ppField{ nullptr };         //указатель на указатели поля
    TCell ai;                           //ходит комп
    TCell human;                        //ходит игрок
    Tprogress progress{ IN_PROGRESS };  //состояние игры
    size_t turn{ 0 };                   //номер хода
};

// ======= Функции стандарнтые ===================

inline void  clearScr()                 //очистка экрана
{               
    // std::cout << "\x1B[2J\x1B[H";    
    system("cls");
}

int32_t __fastcall getRandomNum(int32_t min, int32_t max)  //выдает случайное число
{   const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

//======== Функции игры ========
  
void __fastcall initGame(TGame& g)     //выделяем игровое поле динамически
{                                    
    g.ppField = new TCell * [g.SIZE];
    for (size_t i = 0; i < g.SIZE; i++)
    {
        g.ppField[i] = new TCell[g.SIZE];

    }
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            g.ppField[y][x] = EMPTY;

        }
    }
    if (getRandomNum(0, 1000) > 500)
    {
        g.human = CROSS;
        g.ai = ZERO;
        g.turn = 0;

    }
    else
    {
        g.human = ZERO;
        g.ai = CROSS;
        g.turn = 1;
    }

}
 
void __fastcall deinitGame (TGame& g)   //освобождаем динамически выделенную память
{
    for (size_t i = 0; i < g.SIZE; i++)
    {
        delete[] g.ppField[i];

    }
    delete[] g.ppField;
    g.ppField = nullptr;
}

void __fastcall printGame(const TGame& g)  // печатаем игру
{
    cout << "    ";
    for (size_t x = 0; x < g.SIZE; x++)
    {
        cout << x + 1 << "   ";
    }
    cout << endl;
    for (size_t y = 0; y < g.SIZE; y++)
    {
        cout << y + 1 << " | ";
        for (size_t x = 0; x < g.SIZE; x++)
        {
            cout << g.ppField[y][x] << " | ";
        }
        cout << endl;
    }
    cout << endl << " Human: " << g.human << endl << " Computer: " << g.ai << endl;
}

// ======= Главные игровые функции =================

Tprogress __fastcall getWon(const TGame& g)     // Проверка на победу
{
    // проверяем строки
    for (size_t y = 0; y < g.SIZE; y++)
    {
        if (g.ppField[y][0] == g.ppField[y][1] && g.ppField[y][0] == g.ppField[y][2])
        {
            if (g.ppField[y][0] == g.human)
                return WIN_HUMAN;
            if (g.ppField[y][0] == g.ai)
                return WIN_AI;
        }
    }
    // проверяем столбцы
    for (size_t x = 0; x < g.SIZE; x++)
    {
        if (g.ppField[0][x] == g.ppField[1][x] && g.ppField[0][x] == g.ppField[2][x])
        {
            if (g.ppField[0][x] == g.human)
                return WIN_HUMAN;
            if (g.ppField[0][x] == g.ai)
                return WIN_AI;
        }
    }
    // проверяем диагонали
    if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2])
    {
        if (g.ppField[0][0] == g.human)
            return WIN_HUMAN;
        if (g.ppField[0][0] == g.ai)
            return WIN_AI;
    }
    if (g.ppField[0][2] == g.ppField[1][1] && g.ppField[0][2] == g.ppField[2][0])
    {
        if (g.ppField[2][0] == g.human)
            return WIN_HUMAN;
        if (g.ppField[2][0] == g.ai)
            return WIN_AI;
    }
    // Проверяем на ничью
    bool draw{ true };
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }
        }
        if (!draw)
            break;
    }
    if (draw)
    {
        return IS_DRAW;
    }

    return IN_PROGRESS;
}

Tcoord __fastcall getHumanCoord(const TGame& g) // Ход человека 
{
    Tcoord c{ 0 };
    do
    {
        cout << " Введите Х (1, 2 или 3): ";
        cin >> c.x;
        cout << " Введите Y (1, 2 или 3): ";
        cin >> c.y;
        c.x--;
        c.y--;

    } while (c.x > 2 || c.y > 2 || g.ppField[c.y][c.x] != EMPTY);
    return c;
}

Tcoord __fastcall getComputerCoord(TGame& g) // Ход компа ИИ
{
    // Предвыигрышная ситуация
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                g.ppField[y][x] = g.ai;
                if (getWon(g) == WIN_AI)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                g.ppField[y][x] = EMPTY;
            }
        }
    }
    // Предпроигрышная ситуация
   // for (size_t y = 0; y < g.SIZE; y++)
  //  {
    //    for (size_t x = 0; x < g.SIZE; x++)
    //    {
     //       if (g.ppField[y][x] == EMPTY)
      //      {
      //          g.ppField[y][x] = g.human;
      //          if (getWon(g) == WIN_HUMAN)
       //         {
       //             g.ppField[y][x] = EMPTY;
      //              return { y, x };
      //          }
      //          g.ppField[y][x] = EMPTY;
      //      }
     //   }
   // }
    //Ход по приоритету + ранд

    if (g.ppField[1][1] == EMPTY)
    {
        return { 1, 1 }; // центр поля
    }
    // углы
    Tcoord buf[4];
    size_t num = 0;
    if (g.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0, 0 };  // Если угол пустой - записываем в буф его координаты
        num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2, 2 };  // Если угол пустой - записываем в буф его координаты
        num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2, 0 };  // Если угол пустой - записываем в буф его координаты
        num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0, 2 };  // Если угол пустой - записываем в буф его координаты
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];   // Случайный пустой угол
    }
    // не углы
    num = 0;
    if (g.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };  // Если не угол пустой - записываем в буф его координаты
        num++;
    }
    if (g.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };  // Если не угол пустой - записываем в буф его координаты
        num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };  // Если не угол пустой - записываем в буф его координаты
        num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };  // Если не  угол пустой - записываем в буф его координаты
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
}

void congrats(const TGame& g)
{
    if (g.progress == WIN_HUMAN)
    {
        cout << "Вы победили! Поздравляю!!!" << endl;
    }
    else if (g.progress == WIN_AI)
    {
        cout << "Компъютер победил! Да, он умнее =)" << endl;
    }
    else if (g.progress == IS_DRAW)
    {
        cout << "Ничья!" << endl;
    }
}

//=======================================================================

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    TGame g;                     // создаем переменную игры
    initGame(g);                 // инициируем игру
    clearScr();                  // очищаем экран
    printGame(g);                // печатаем игру
    
    do                           // Главный игровой цикл
    {
        if (g.turn % 2 == 0)     // ход игрока человека
        {
            Tcoord c = getHumanCoord(g);
            g.ppField[c.y][c.x] = g.human;
        }
        else                     // ход игрока компьютера
        {
            Tcoord c = getComputerCoord(g);
            g.ppField[c.y][c.x] = g.ai;
        }

        g.turn++;               // следующий ход
        clearScr();             // очистка экрана
        printGame(g);           // печатаем игру
        g.progress = getWon(g); // кто-то выиграл? или ничья?
    } while (g.progress == IN_PROGRESS);     // пока игра в состоянии прогресса
    congrats(g);                // поздравление
    deinitGame(g);              // деинициализация игры   
    return 0;
}
