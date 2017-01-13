#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int MaxX = 39;
const int MaxY = 17;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 * ---
 * Hint: You can use the debug stream to print initialTX and initialTY, if Thor seems not follow your orders.
 **/
int main()
{
    int thorX;
    int thorY;
    int lightX; // the X position of the light of power
    int lightY; // the Y position of the light of power
    int initialTX; // Thor's starting X position
    int initialTY; // Thor's starting Y position
    cin >> lightX >> lightY >> initialTX >> initialTY; cin.ignore();
    
    thorX = initialTX;
    thorY = initialTY;

    // game loop
    while (1) {    
        string dirX = "";
        string dirY = "";
    
        int remainingTurns; // The remaining amount of turns Thor can move. Do not remove this line.
        cin >> remainingTurns; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        if (thorX < lightX)
        {
            dirX = "E";
            thorX++;
        }
        else if (thorX > lightX)
        {
            dirX = "W";
            thorX--;
        }

        if (thorY < lightY)
        {
            dirY = "S";
            thorY++;
        }
        else if (thorY > lightY)
        {
            dirY = "N";
            thorY--;
        }
        // A single line providing the move to be made: N NE E SE S SW W or NW
        cout << dirY << dirX << endl;
    }
}