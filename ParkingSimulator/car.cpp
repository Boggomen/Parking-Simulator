#include "car.hpp"
#include <algorithm>

using namespace Gdiplus;

Car::Car() 
    : x(0), y(0), velocityX(0), velocityY(0), heading(0), speed(0),
      steerAngle(0), isDrifting(false), slipAngle(0), handbrakeActive(false),
      width(40), height(80), carImage(nullptr) {
}

Car::~Car() {
    if (carImage) {
        delete carImage;
        carImage = nullptr;
    }
}

void Car::Init(float startX, float startY, float startHeading) {
    x = startX;
    y = startY;
    heading = startHeading;
    velocityX = 0;
    velocityY = 0;
    speed = 0;
    steerAngle = 0;
    isDrifting = false;
    slipAngle = 0;
    handbrakeActive = false;
}

bool Car::LoadCarImage(const wchar_t* filename) {
    if (carImage) {
        delete carImage;
        carImage = nullptr;
    }
    
    carImage = Image::FromFile(filename);
    if (carImage && carImage->GetLastStatus() == Ok) {
        width = (float)carImage->GetWidth();
        height = (float)carImage->GetHeight();
        return true;
    }
    
    if (carImage) {
        delete carImage;
        carImage = nullptr;
    }
    return false;
}

// Keep angle in range -PI to PI
float Car::NormalizeAngle(float angle) {
    while (angle > PI) angle -= 2.0f * PI;
    while (angle < -PI) angle += 2.0f * PI;
    return angle;
}

// Calculate angle between where car is pointing vs where it's moving
float Car::CalculateSlipAngle() {
    float velocityMagnitude = sqrtf(velocityX * velocityX + velocityY * velocityY);
    
    if (velocityMagnitude < 1.0f) {
        return 0.0f;  // No slip when barely moving
    }
    
    float velocityAngle = atan2f(velocityX, -velocityY);
    float slip = NormalizeAngle(heading - velocityAngle);
    
    return slip;
}

// Reduce sideways velocity based on grip level
// High grip = less sliding, low grip = more sliding (drifting)
void Car::ApplyGrip(float deltaTime, float gripFactor) {
    // Forward direction (where car is pointing)
    float forwardX = sinf(heading);
    float forwardY = -cosf(heading);
    
    // Right direction (perpendicular to forward)
    float rightX = cosf(heading);
    float rightY = sinf(heading);
    
    // Split velocity into forward and sideways components
    float forwardSpeed = velocityX * forwardX + velocityY * forwardY;
    float lateralSpeed = velocityX * rightX + velocityY * rightY;
    
    // Reduce sideways velocity based on grip
    lateralSpeed *= (1.0f - gripFactor);
    
    // Recombine into velocity
    velocityX = forwardX * forwardSpeed + rightX * lateralSpeed;
    velocityY = forwardY * forwardSpeed + rightY * lateralSpeed;
}

void Car::Update(float deltaTime, bool accelerating, bool braking, 
                 bool steerLeft, bool steerRight, bool handbrake) {
    handbrakeActive = handbrake;
    
    // Forward direction
    float forwardX = sinf(heading);
    float forwardY = -cosf(heading);
    
    // Acceleration
    if (accelerating) {
        velocityX += forwardX * CAR_ACCELERATION * deltaTime;
        velocityY += forwardY * CAR_ACCELERATION * deltaTime;
    }
    
    // Braking or reverse
    if (braking) {
        float forwardSpeed = velocityX * forwardX + velocityY * forwardY;
        if (forwardSpeed > 10.0f) {
            // Braking
            velocityX -= forwardX * CAR_BRAKE_FORCE * deltaTime;
            velocityY -= forwardY * CAR_BRAKE_FORCE * deltaTime;
        } else {
            // Reverse (slower than forward)
            velocityX -= forwardX * CAR_ACCELERATION * 0.5f * deltaTime;
            velocityY -= forwardY * CAR_ACCELERATION * 0.5f * deltaTime;
        }
    }
    
    // Calculate and clamp speed
    speed = sqrtf(velocityX * velocityX + velocityY * velocityY);
    if (speed > CAR_MAX_SPEED) {
        float scale = CAR_MAX_SPEED / speed;
        velocityX *= scale;
        velocityY *= scale;
        speed = CAR_MAX_SPEED;
    }
    
    // Steering (only works when moving)
    if (speed > 5.0f) {
        float turnRate = CAR_TURN_SPEED * (speed / CAR_MAX_SPEED);
        
        // Handbrake increases turn rate to help start drifts
        if (handbrakeActive) {
            turnRate *= 1.5f;
        }
        
        if (steerLeft) {
            steerAngle = -1.0f;
            heading -= turnRate * deltaTime;
        } else if (steerRight) {
            steerAngle = 1.0f;
            heading += turnRate * deltaTime;
        } else {
            steerAngle = 0.0f;
        }
        
        heading = NormalizeAngle(heading);
    }
    
    // Drift detection
    slipAngle = CalculateSlipAngle();
    
    // Use hysteresis to prevent drift state from flickering
    if (fabsf(slipAngle) > DRIFT_SLIP_THRESHOLD && speed > 50.0f) {
        isDrifting = true;
    } else if (fabsf(slipAngle) < DRIFT_SLIP_THRESHOLD * 0.5f) {
        isDrifting = false;
    }
    
    // Apply grip based on state
    float gripFactor;
    if (handbrakeActive) {
        gripFactor = HANDBRAKE_GRIP;
    } else if (isDrifting) {
        gripFactor = DRIFT_GRIP;
    } else {
        gripFactor = NORMAL_GRIP;
    }
    ApplyGrip(deltaTime, gripFactor);
    
    // Friction when not pressing gas or brake
    if (!accelerating && !braking && speed > 0) {
        float frictionForce = CAR_FRICTION * deltaTime;
        if (frictionForce > speed) {
            velocityX = 0;
            velocityY = 0;
        } else {
            float scale = (speed - frictionForce) / speed;
            velocityX *= scale;
            velocityY *= scale;
        }
    }
    
    // Update position
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
    
    speed = sqrtf(velocityX * velocityX + velocityY * velocityY);
}

void Car::Draw(Graphics* graphics) {
    if (!graphics) return;
    
    // Save graphics state, apply rotation, then restore
    GraphicsState state = graphics->Save();
    
    graphics->TranslateTransform(x, y);
    float headingDegrees = heading * (180.0f / PI);
    graphics->RotateTransform(headingDegrees);
    
    if (carImage) {
        // Draw image centered on car position
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        graphics->DrawImage(carImage, -halfWidth, -halfHeight, width, height);
    } else {
        // Fallback: colored rectangle (red when drifting, blue normally)
        SolidBrush carBrush(isDrifting ? Color(255, 255, 100, 100) : Color(255, 100, 100, 255));
        float halfWidth = 20.0f;
        float halfHeight = 40.0f;
        graphics->FillRectangle(&carBrush, -halfWidth, -halfHeight, halfWidth * 2, halfHeight * 2);
        
        // Yellow front indicator
        SolidBrush frontBrush(Color(255, 255, 255, 0));
        graphics->FillRectangle(&frontBrush, -halfWidth + 5, -halfHeight, halfWidth * 2 - 10, 10.0f);
    }
    
    graphics->Restore(state);
}
