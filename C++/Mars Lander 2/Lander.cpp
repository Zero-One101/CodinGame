#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

const int MaxHorizontalSpeed = 20;
const int MaxVerticalSpeed = 40;
const int MaxWidth = 7000;
const int MaxHeight = 3000;
const int SpeedBuffer = 10;
const int PadRadius = 500;
const int LandingDistance = 50;
const int PeakHeightBuffer = 500;
const float Gravity = 3.711;
const float MaxThrust = 4;
const float MaxAccel = MaxThrust - Gravity;

/**
 * Defines a point in 2D space
 **/
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
    
    // Sorts by height
    bool operator < (const Point& p) const
    {
        return (Y < p.Y);
    }
    
    bool operator > (const Point& p) const
    {
        return (Y > p.Y);
    }
    
    bool operator == (const Point& p) const
    {
        return (X == p.X && Y == p.Y);
    }
};

/**
 * Returns a point defining the target landing area
 * A landing area is defined as 2 points with the same
 * Y value, and X values at least 1000 apart
 **/
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
                //cerr << "Landing point is: " << landX << ", " << landY << endl;
                return Point(landX, landY);
            }
        }
    }
}

/**
 * Returns true if the Lander is above the landing pad
 **/
bool IsAbovePad(int posX, int padX)
{
    return posX >= padX - PadRadius && posX <= padX + PadRadius;
}

/**
 * Clamps value to between min and max
 **/
int Clamp(int value, int min, int max)
{
    return std::max(min, std::min(value, max));
}

/**
 *  Returns true if the absolute horizontal speed of the player is less than the safe landing maximum
 * and if the absolute vertical speed of the player is less than the safe landing maximum, with a buffer
 **/
bool IsTravellingAtSafeSpeeds(int HS, int VS)
{
    if (VS > MaxVerticalSpeed - SpeedBuffer || VS < -MaxVerticalSpeed + SpeedBuffer
        || HS > MaxHorizontalSpeed - SpeedBuffer || HS < -MaxHorizontalSpeed + SpeedBuffer)
        {
            return false;
        }
        
    return true;
}

/**
 * Converts radians to degrees
 **/
double RadiansToDegrees(double rad)
{
    return (rad * 180) / M_PI;
}

/**
 * Calculates the rotation required to stop the movement of the lander, assuming full burn
 **/
int RotateToStopLander(int speedX, int speedY)
{
    double speed = sqrt(pow(speedX, 2) + pow(speedY, 2));
    if ((int)speed == 0)
    {
        return 0;
    }
    double rotationRad = asin(speedX / speed);
    return (int) RadiansToDegrees(rotationRad);
}

/**
 * Determins if the lander is moving too slow to reach the landing pad
 **/
bool IsLanderMovingTooSlow(int speedX)
{
    return abs(speedX) < MaxHorizontalSpeed * 2;
}

/**
 * Determins if the lander is moving too fast to reach the landing pad
 **/
bool IsLanderMovingTooFast(int speedX)
{
    return abs(speedX) > MaxHorizontalSpeed * 4;
}

/**
 * Calculates the rotation required to propel the lander towards the pad
 **/
int RotateToMoveLander(int posX, int padX)
{
    if (posX < padX - PadRadius)
    {
        return -(int)RadiansToDegrees(acos(Gravity / MaxThrust));
    }
    else
    {
        return (int)RadiansToDegrees(acos(Gravity / MaxThrust));
    }
}

/**
 * True if the lander's Y position is lower than the Y position of the highest peak
 **/
bool IsLanderTooLow(int posY, int highestPeak)
{
    return posY < highestPeak;
}

/**
 * Represents the desired direction of travel as either 1 (left) or -1 (right)
 **/
int GetTravelDirection(int posX, int padX)
{
    if (posX < padX - PadRadius)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * Returns true if the lander is just above the landing pad
 **/
bool IsAboutToLand(int posY, int padY)
{
    return posY < padY + LandingDistance;
}

/**
 * Represents the direction of the pad relative to the player's spawn
 * 1 represents left, -1 represents right
 **/
int DirectionToPad(int posX, int padX)
{
    if (posX < padX)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * Returns true if the lander has passed the highest peak in its way
 **/
bool IsPastHighestPeak(int padDirection, int posX, int peakX)
{
    if (padDirection == -1) // Moving right
    {
        return posX >= peakX;
    }
    else
    {
        return posX <= peakX;
    }
}

/**
 * Finds the next highest peak of the map, ignoring ones the Lander has already passed
 **/
Point FindNextHighestPeak(int padDirection, Point currentPeak, const std::vector<Point> &points)
{
    Point comparePeak = currentPeak;
    Point peak = currentPeak;
    
    if (padDirection == -1) // Moving right
    {
        for (auto& p : points)
        {            
            if (comparePeak == peak) // If we haven't gotten the first peak
            {
                if (p.X >= comparePeak.X && p.Y < MaxHeight)
                {
                    peak = p;
                }
            }
            else
            {
                if (p.X >= comparePeak.X && p.Y > peak.Y && p.Y < MaxHeight) // If the peak is ahead of us and is taller
                {
                    peak = p;
                }
            }
        }
    }
    else
    {
        if (currentPeak.X == 0 && currentPeak.Y == 0)
        {
            comparePeak = Point(MaxWidth, 0);
            peak = Point(MaxWidth, 0);
        }
        
        for (auto& p : points)
        {
            
            if (comparePeak == peak) // If we haven't gotten the first peak
            {
                if (p.X <= comparePeak.X && p.Y < MaxHeight)
                {
                    peak = p;
                }
            }
            else
            {
                if (p.X <= comparePeak.X && p.Y > peak.Y && p.Y < MaxHeight) // If the peak is ahead of us and is taller
                {
                    peak = p;
                }
            }
        }
    }
    
    return peak;
}

int CalcThrust(int speedY)
{
    if (speedY < 0)
    {
        return 4;
    }
    
    return 0;
}

/**
 * Save the Planet.
 * Use less Fossil Fuel.
 **/
int main()
{
    int N; // the number of points used to draw the surface of Mars.
    std::vector<Point> points;
    cin >> N; cin.ignore();
    for (int i = 0; i < N; i++) {
        int landX; // X coordinate of a surface point. (0 to 6999)
        int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        points.push_back(Point(landX, landY));
        cin >> landX >> landY; cin.ignore();
    }
    
    
    Point landingSite = FindLandingSite(points);
    
    // Sort the map points by height for height comparisons
    std::sort(points.begin(), points.end(), greater<Point>());
        
    //for (auto& i : points)
    //{
    //    cerr << i.X << ", " << i.Y << endl;
    //}
    Point highestPeak = Point(0, 0);

    int time = -1;
    int padDirection;
    // game loop
    while (1) {
        time++;
        int X;
        int Y;
        int HS; // the horizontal speed (in m/s), can be negative.
        int VS; // the vertical speed (in m/s), can be negative.
        int F; // the quantity of remaining fuel in liters.
        int R; // the rotation angle in degrees (-90 to 90).
        int P; // the thrust power (0 to 4).
        cin >> X >> Y >> HS >> VS >> F >> R >> P; cin.ignore();

        if (time == 0) // If we have just spawned
        {
            padDirection = DirectionToPad(X, landingSite.X);
            highestPeak = FindNextHighestPeak(padDirection, highestPeak, points);
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        //cerr << "Burn time = " << fullBurnTime << endl;
        //cerr << "Stop Position = " << stopPosition << endl;
        //cerr << "Highest Peak = " << highestPeak.X << ", " << highestPeak.Y << endl;

        // R P. R is the desired rotation angle. P is the desired thrust power.
        if (IsAbovePad(X, landingSite.X))
        {
            //cerr << "Above pad" << endl;          
            if (IsAboutToLand(Y, landingSite.Y))
            {
                R = 0;
                P = CalcThrust(VS);
                
                if (!IsTravellingAtSafeSpeeds(HS, VS))
                {
                    R = 0;
                    P = 4;
                }
            }
            else if (!IsTravellingAtSafeSpeeds(HS, VS))
            {
                R = RotateToStopLander(HS, VS);
                P = 4;
            }  
            else
            {
                R = 0;
                P = 1;
            }
        }
        else
        {
            //cerr << "Not above pad" << endl;
            
            if (IsPastHighestPeak(padDirection, X, highestPeak.X))
            {
                //cerr << "Past highest peak" << endl;
                highestPeak = FindNextHighestPeak(padDirection, highestPeak, points);
            }
            
            if (IsLanderTooLow(Y, highestPeak.Y) && !IsPastHighestPeak(padDirection, X, highestPeak.X))
            {
                //cerr << "Lander is too low" << endl;
                R = RotateToStopLander(HS, VS);
                P = 4;
            }
            else if (IsLanderMovingTooSlow(HS))
            {
                R = RotateToMoveLander(X, landingSite.X);
                P = 3;
            }
            else if (IsLanderMovingTooFast(HS))
            {
                R = RotateToStopLander(HS, VS);
                P = 4;
            }
            else
            {
                R = 0;
                P = CalcThrust(VS);
            }
            
            if (Y >= 2900)
            {
                P = 2;
            }
        }
        
        cout << R << " " << P << endl;
    }
}