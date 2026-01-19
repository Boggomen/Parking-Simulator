// Car class with drift physics

#ifndef CAR_HPP
#define CAR_HPP

#include <windows.h>
#include <gdiplus.h>
#include <cmath>

// Physics constants - adjust these to change how the car feels
const float PI = 3.14159265358979f;
const float CAR_MAX_SPEED = 300.0f;       // Max speed in pixels/second
const float CAR_ACCELERATION = 200.0f;    // How fast car speeds up
const float CAR_BRAKE_FORCE = 300.0f;     // How fast car slows down when braking
const float CAR_FRICTION = 50.0f;         // Natural slowdown when not pressing gas
const float CAR_TURN_SPEED = 3.0f;        // How sharp the car turns
const float DRIFT_SLIP_THRESHOLD = 0.26f; // Slip angle to trigger drift (~15 degrees)
const float NORMAL_GRIP = 0.95f;          // Tire grip during normal driving
const float DRIFT_GRIP = 0.7f;            // Reduced grip while drifting
const float HANDBRAKE_GRIP = 0.4f;        // Very low grip when handbrake is pulled

class Car {
public:
    // Position on screen
    float x;
    float y;
    
    // Velocity components
    float velocityX;
    float velocityY;
    
    // Direction car is facing (radians, 0 = up)
    float heading;
    
    // Total speed (calculated from velocity)
    float speed;
    
    // Current steering input (-1 left, 0 straight, 1 right)
    float steerAngle;
    
    // Drift state
    bool isDrifting;
    float slipAngle;  // Angle between heading and velocity direction
    
    // True when spacebar is held
    bool handbrakeActive;
    
    // Car sprite dimensions
    float width;
    float height;
    
    // Car sprite image (GDI+ for PNG transparency and rotation)
    Gdiplus::Image* carImage;
    
    Car();
    ~Car();
    
    // Place car at starting position
    void Init(float startX, float startY, float startHeading);
    
    // Load car sprite from PNG file
    bool LoadCarImage(const wchar_t* filename);
    
    // Update physics each frame
    void Update(float deltaTime, bool accelerating, bool braking, 
                bool steerLeft, bool steerRight, bool handbrake);
    
    // Draw car rotated to current heading
    void Draw(Gdiplus::Graphics* graphics);
    
    float GetCenterX() const { return x; }
    float GetCenterY() const { return y; }
    
private:
    // Calculate angle between heading and velocity direction
    float CalculateSlipAngle();
    
    // Keep angle in range -PI to PI
    float NormalizeAngle(float angle);
    
    // Reduce sideways velocity based on grip level
    void ApplyGrip(float deltaTime, float gripFactor);
};

#endif
