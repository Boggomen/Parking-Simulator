#ifndef CAR_HPP
#define CAR_HPP

#include <windows.h>
#include <gdiplus.h>
#include <cmath>

// ============= Physics Constants =============
const float PI =       3.14159265358979f;
const float CAR_MAX_SPEED =       300.0f;
const float CAR_ACCELERATION =    200.0f;
const float CAR_BRAKE_FORCE =     300.0f;
const float CAR_FRICTION =         50.0f;
const float CAR_TURN_SPEED =        3.0f;
const float DRIFT_SLIP_THRESHOLD = 0.26f;
const float NORMAL_GRIP =          0.95f;
const float DRIFT_GRIP =            0.7f;
const float HANDBRAKE_GRIP =        0.4f;

// ============= Car Class =============
class Car {
public:
    // Position
    float x;
    float y;
    
    // Velocity
    float velocityX;
    float velocityY;
    
    // Direction car is facing (radians, 0 = up)
    float heading;
    
    // Total speed
    float speed;
    
    // Steering input (-1 left, 0 straight, 1 right)
    float steerAngle;
    
    // Drift state
    bool isDrifting;
    float slipAngle;
    
    // Handbrake state
    bool handbrakeActive;
    
    // Car dimensions
    float width;
    float height;
    
    // Car sprite image
    Gdiplus::Image* carImage;
    
    Car();
    ~Car();
    
    void Init(float startX, float startY, float startHeading);
    bool LoadCarImage(const wchar_t* filename);
    void Update(float deltaTime, bool accelerating, bool braking, 
                bool steerLeft, bool steerRight, bool handbrake);
    void Draw(Gdiplus::Graphics* graphics);
    
    float GetCenterX() const { return x; }
    float GetCenterY() const { return y; }
    
private:
    float CalculateSlipAngle();
    float NormalizeAngle(float angle);
    void ApplyGrip(float deltaTime, float gripFactor);
};

#endif
