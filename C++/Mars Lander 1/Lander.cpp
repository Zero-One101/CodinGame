#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const double Gravity = 3.711;
const int MaxThrust = 4;
const double MaxAccel = MaxThrust - Gravity; 

class Point
{
public:
    int X;
    int Y;
    
    Point(int X, int Y)
    {
        this->X = X;
        this->Y = Y;
    }
};

Point FindLandingSite(const std::vector<Point> &points)
{
    for (int i = 0; i < points.size(); i++)
    {
        Point p1 = points[i];
        Point p2 = points[i+1];
        
        if (p1.Y == p2.Y)
        {
            if (abs(p2.X - p1.X) >= 1000)
            {
                int landY = p1.Y;
                int landX = (p1.X + p2.X) / 2;
                return Point(landX, landY);
            }
        }
    }
}

int CalcFullBurnStopTime(int velocity)
{
    return -velocity / MaxAccel;
}

int CalcStopPosition(int velocity, int position, int stopTime)
{
    return (0.1445 * (stopTime * stopTime)) + (velocity * stopTime) + position;
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int surfaceN; // the number of points used to draw the surface of Mars.
    std::vector<Point> points;
    cin >> surfaceN; cin.ignore();
    for (int i = 0; i < surfaceN; i++) {
        int landX; // X coordinate of a surface point. (0 to 6999)
        int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        points.push_back(Point(landX, landY));
        cin >> landX >> landY; cin.ignore();
    }
    
    Point landingSite = FindLandingSite(points);
    
    int spawnY;
    int dropDistance;
    int burnTime;
    int stopPosition;
    int time = -1;
    
    // game loop
    while (1) {
        time++;
        int X;
        int Y;
        int hSpeed; // the horizontal speed (in m/s), can be negative.
        int vSpeed; // the vertical speed (in m/s), can be negative.
        int fuel; // the quantity of remaining fuel in liters.
        int rotate; // the rotation angle in degrees (-90 to 90).
        int power; // the thrust power (0 to 4).
        cin >> X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power; cin.ignore();

        if (time == 0) // We have just spawned
        {
            spawnY = Y;
            dropDistance = spawnY - landingSite.Y;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        burnTime = CalcFullBurnStopTime(vSpeed);
        stopPosition = CalcStopPosition(vSpeed, Y, burnTime);

        // 2 integers: rotate power. rotate is the desired rotation angle (should be 0 for level 1), power is the desired thrust power (0 to 4).
        if (stopPosition < dropDistance)
        {
            rotate = 0;
            power = 1;
            
            if (vSpeed < -35)
            {
                power = 4;
            }
        }
        
        cout << rotate << " " << power << endl;
    }
}