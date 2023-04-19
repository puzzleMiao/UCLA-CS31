// rabbits.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;               // max number of rows in the arena
const int MAXCOLS = 25;               // max number of columns in the arena
const int MAXRABBITS = 100;           // max number of rabbits allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Rabbit declaration.

class Rabbit
{
  public:
      // Constructor
    Rabbit(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_posion;
    bool   m_stop = false;
      // TODO: You'll probably find that a rabbit object needs additional
      // data members to support your implementation of the behavior affected
      // by poisoned carrots.
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropPoisonedCarrot();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     rabbitCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRabbitsAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRabbit(int r, int c);
    bool addPlayer(int r, int c);
    void moveRabbits();

  private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rabbit* m_rabbits[MAXRABBITS];
    int     m_nRabbits;
    int     m_turns;

      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRabbits);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rabbit implementation
///////////////////////////////////////////////////////////////////////////

Rabbit::Rabbit(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rabbit must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Rabbit created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
}

int Rabbit::row() const
{
    return m_row;
}

int Rabbit::col() const
{
    return m_col;
}

bool Rabbit::isDead() const
{
    if (m_posion == 2)
        return true;
    else
        return false;
}

void Rabbit::move()
{
    //   Return without moving if the rabbit has eaten one poisoned
    //   carrot (so is supposed to move only every other turn) and
    //   this is a turn it does not move.
    
    //   Otherwise, attempt to move in a random direction; if can't
    //   move, don't move.  If it lands on a poisoned carrot, eat the
    //   carrot and remove it from the game (so it is no longer on that
    //   grid point).
    if (m_posion == 1)
    {
        if (m_stop == true)
        {
            m_stop = false;
            return;
        }
        else
            m_stop = true;
    }
    
    int dir = randInt(0, NUMDIRS - 1);
    switch (dir){
        case NORTH: //North
            if (m_row > 1)
                m_row--;
            break;
        case EAST: // East
            if (m_col < m_arena->cols())
                m_col++;
            break;
        case SOUTH: // South
            if (m_col < m_arena->rows())
                m_row++;
            break;
        case WEST: // West
            if (m_col > 1)
                m_col--;
            break;
    }
    
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
    {
        m_posion++;
        if (m_posion == 1)
            m_stop = true;
        m_arena->setCellStatus(m_row, m_col, EMPTY);
    }
}
///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonedCarrot()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned carrot at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned carrot has been dropped.";
}

string Player::move(int dir)
{
      //        direction.  If this fails,
      //        return "Player couldn't move; player stands."
      //        A player who moves onto a rabbit, and this
      //        returns "Player walked into a rabbit and died."
      //        Otherwise, return one of "Player moved north.",
      //        "Player moved east.", "Player moved south.", or
      //        "Player moved west."
    switch (dir) {
        case NORTH: // North
            if (m_arena->numberOfRabbitsAt(m_row - 1, m_col) > 0)
            {
                m_row--;
                setDead();
                return "Player walked into a rabbit and died.";
            }
            break;
        case EAST: // East
            if (m_arena->numberOfRabbitsAt(m_row, m_col + 1) > 0)
            {
                m_col++;
                setDead();
                return "Player walked into a rabbit and died.";
            }
            break;
        case SOUTH: // South
            if (m_arena->numberOfRabbitsAt(m_row + 1, m_col) > 0)
            {
                m_row++;
                setDead();
                return "Player walked into a rabbit and died.";
            }
            break;
        case WEST: // West
            if (m_arena->numberOfRabbitsAt(m_row, m_col - 1) > 0)
            {
                m_col--;
                setDead();
                return "Player walked into a rabbit and died.";
            }
            break;
        default:
            break;
    }
    
    switch (dir) {
        case NORTH: // North
            if (m_row > 1)
            {
                m_row--;
                return "Player moved north.";
            }
            break;
        case EAST: // East
            if (m_col < m_arena->cols())
            {
                m_col++;
                return "Player moved east";
            }
            break;
        case SOUTH: // South
            if (m_row < m_arena->rows())
            {
                m_row++;
                return "Player moved south";
            }
            break;
        case WEST: // West
            if (m_col > 1)
            {
                m_col--;
                return "Player moved west";
            }
            break;
    }
    return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRabbits = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}
/////////////////////////////////////////////
Arena::~Arena()
{
    delete m_player;
    for (int k = 0; k < m_nRabbits; k++)
        delete m_rabbits[k];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::rabbitCount() const
{
    return m_nRabbits;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r-1][c-1];
}

int Arena::numberOfRabbitsAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nRabbits; k++)
    {
        if (m_rabbits[k] == nullptr)
            continue;
        if (m_rabbits[k]->row() == r && m_rabbits[k]->col() == c)
            count++;
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (poisoned carrots)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
    
    // Indicate each rabbit's position
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
        {
            if (numberOfRabbitsAt(r, c) == 1)
                displayGrid[r-1][c-1] = 'R';
            else if (numberOfRabbitsAt(r, c) > 1)
                displayGrid[r-1][c-1] =
                (numberOfRabbitsAt(r, c) > 9 ? '9' : '0' + numberOfRabbitsAt(r, c));
        }
            
      // Indicate the player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, rabbit, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << rabbitCount() << " rabbits remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addRabbit(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
      // Don't add a rabbit on a spot with a poisoned carrot
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a rabbit on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

      // If there are MAXRABBITS existing rabbits, return false.  Otherwise,
      // dynamically allocate a new rabbit at coordinates (r,c).  Save the
      // pointer to newly allocated rabbit and return true.

    if (m_nRabbits == MAXRABBITS)
        return false;
    else
    {
        m_rabbits[m_nRabbits] = new Rabbit(this, r, c);
        m_nRabbits++;
        return true;
    }
    return false;  // This implementation compiles, but is incorrect.
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a rabbit
    if (numberOfRabbitsAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRabbits()
{
    // Move all rabbits
    // Deallocate any dead dynamically allocated rabbit.
    for (int k = 0; k < m_nRabbits; k++)
        {
            if (m_rabbits[k] == nullptr)
                continue;
            m_rabbits[k]->move();
            
            // Check if the player dead
            if (numberOfRabbitsAt(m_player->row(), m_player->col() > 0))
                m_player->setDead();
            
            // Deallocate any dead dynamically allocated rabbit.
            if (m_rabbits[k]->isDead())
            {
                m_nRabbits--;
                delete m_rabbits[k];
                m_rabbits[k] = nullptr;
            }
        }
    
    // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRabbits)
{
    if (nRabbits < 0)
    {
        cout << "***** Cannot create Game with negative number of rabbits!" << endl;
        exit(1);
    }
    if (nRabbits > MAXRABBITS)
    {
        cout << "***** Trying to create Game with " << nRabbits
             << " rabbits; only " << MAXRABBITS << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nRabbits - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nRabbits << " rabbits!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with rabbits
    while (nRabbits > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRabbit(r, c);
        nRabbits--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/c or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonedCarrot();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'c')
                return player->dropPoisonedCarrot();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/c." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->rabbitCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveRabbits();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    switch (dir) {
        case NORTH:
            if (r > 1)
            {
                r--;
                return true;
            }
            break;
        case EAST:
            if (c < a.cols())
            {
                c++;
                return true;
            }
            break;
        case SOUTH:
            if (r < a.rows())
            {
                r++;
                return true;
            }
            break;
        case WEST:
            if (c > 1)
            {
                c--;
                return true;
            }
            break;
    }
    return false; 
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poisoned carrot and
  // not move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
int findSurroundRabbit(const Arena& a, int r, int c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    int numNorth = MAXRABBITS + 1;
    int numSouth = MAXRABBITS + 1;
    int numWest = MAXRABBITS + 1;
    int numEast = MAXRABBITS + 1;
    
    bool hasRabbitN = false;
    bool hasRabbitE = false;
    bool hasRabbitS = false;
    bool hasRabbitW = false;
    
    if (r > 1)
    {
        numNorth = findSurroundRabbit(a, r - 1, c);
        if (numNorth > 0)
            hasRabbitN = true;
    }
    if (r < a.rows())
    {
        numSouth = findSurroundRabbit(a, r + 1, c);
        if (numSouth > 0)
            hasRabbitS = true;
    }
    if (c > 1)
    {
        numWest = findSurroundRabbit(a, r , c - 1);
        if (numWest > 0)
            hasRabbitW = true;
    }
    if (r > 1)
    {
        numEast = findSurroundRabbit(a, r , c + 1);
        if (numEast > 0)
            hasRabbitE = true;
    }
    
    if (numNorth <= numSouth && numNorth <= numEast && numNorth <= numWest && !hasRabbitN)
        bestDir = NORTH;
    if (numSouth <= numNorth && numSouth <= numEast && numSouth <= numWest && !hasRabbitS)
        bestDir = SOUTH;
    if (numEast <= numNorth  && numEast <= numSouth && numEast <= numWest  && !hasRabbitE)
        bestDir = EAST;
    if (numWest <= numNorth  && numWest <= numSouth && numWest <= numEast  && !hasRabbitW)
        bestDir = WEST;
    
    return false;

}

int findSurroundRabbit(const Arena& a, int r, int c)
{
    int rabbitTotal = 0;
    
    if (r > 1)
        rabbitTotal += a.numberOfRabbitsAt(r - 1, c);
    if (r < a.rows())
        rabbitTotal += a.numberOfRabbitsAt(r + 1, c);
    if (c > 1)
        rabbitTotal += a.numberOfRabbitsAt(r, c - 1);
    if (c < a.cols())
        rabbitTotal += a.numberOfRabbitsAt(r, c + 1);
    
    rabbitTotal += a.numberOfRabbitsAt(r, c);
    return rabbitTotal;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(8, 8, 2);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
