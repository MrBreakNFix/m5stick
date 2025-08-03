#include <M5Unified.h>
#include <cmath>

// Screen and Level configuration
int centerX;
int centerY;
const int LEVEL_Y_OFFSET = 15; // Vertical offset for the level from center
const int BUBBLE_RADIUS = 10;
const float SENSITIVITY = 80.0f; // Higher number = more sensitive

// Variables to track the bubble's previous position for flicker-free drawing
int lastBubbleX = -100;
int lastBubbleY = -100;

// Threshold for detecting significant movement
const float CHANGE_THRESHOLD = 0.01f;

void setup() {
    M5.begin();

    centerX = M5.Display.width() / 2;
    centerY = M5.Display.height() / 2;

    // --- Draw all static elements ONCE at the start ---
    M5.Display.fillScreen(BLACK);

    // Draw the title
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(WHITE);
    M5.Display.setCursor(5, 5);
    M5.Display.println("Level");

    // Initial draw of the target circles
    M5.Display.drawCircle(centerX, centerY + LEVEL_Y_OFFSET, 50, WHITE);
    M5.Display.drawCircle(centerX, centerY + LEVEL_Y_OFFSET, 25, WHITE);
}

void loop() {
    M5.update();

    float accX, accY, accZ;
    M5.Imu.getAccelData(&accX, &accY, &accZ);

    // Calculate the new bubble position
    int bubbleX = centerX - (accX * SENSITIVITY);
    int bubbleY = centerY + LEVEL_Y_OFFSET + (accY * SENSITIVITY);

    // Only update the screen if the bubble has actually moved
    if (bubbleX != lastBubbleX || bubbleY != lastBubbleY) {

        // --- ERASE PHASE ---
        // 1. Erase the OLD bubble by drawing a black circle over it
        if (lastBubbleX != -100) {
            M5.Display.fillCircle(lastBubbleX, lastBubbleY, BUBBLE_RADIUS, BLACK);
        }
        // 2. Erase the OLD X/Y text
        M5.Display.fillRect(5, 30, M5.Display.width() - 10, 10, BLACK);

        // --- REPAIR PHASE (The Fix) ---
        // 3. Redraw the static target rings to repair any damage from the erase step.
        M5.Display.drawCircle(centerX, centerY + LEVEL_Y_OFFSET, 50, WHITE);
        M5.Display.drawCircle(centerX, centerY + LEVEL_Y_OFFSET, 25, WHITE);

        // --- REDRAW PHASE ---
        // 4. Draw the NEW bubble in its new position
        M5.Display.fillCircle(bubbleX, bubbleY, BUBBLE_RADIUS, RED);

        // 5. Draw the NEW X/Y text
        M5.Display.setTextSize(1);
        M5.Display.setTextColor(TFT_SKYBLUE);
        M5.Display.setCursor(5, 30);
        M5.Display.printf("X: %.2f  Y: %.2f", accX, accY);

        // Store the current bubble's position for the next loop
        lastBubbleX = bubbleX;
        lastBubbleY = bubbleY;
    }

    // A small delay to keep things running smoothly
    delay(20);
}