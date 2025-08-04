#include <M5Unified.h>

int centerX;
int centerY;
const int LEVEL_Y_OFFSET = 15;
const int BUBBLE_RADIUS = 10;

float sensitivity = 80.0f;
int lastBubbleX = -100;
int lastBubbleY = -100;

void setup() {
    M5.begin();
    centerX = M5.Display.width() / 2;
    centerY = M5.Display.height() / 2;
    M5.Display.fillScreen(BLACK);
}

void loop() {
    M5.update();

    bool sensitivityChanged = false;
    if (M5.BtnA.wasPressed()) {
        sensitivity += 10.0f;
        sensitivityChanged = true;
    }
    if (M5.BtnB.wasPressed()) {
        if (sensitivity > 10.0f) {
            sensitivity -= 10.0f;
        }
        sensitivityChanged = true;
    }

    float accX, accY, accZ;
    M5.Imu.getAccelData(&accX, &accY, &accZ);

    int bubbleX = centerX - (accX * sensitivity);
    int bubbleY = centerY + LEVEL_Y_OFFSET + (accY * sensitivity);

    if (bubbleX != lastBubbleX || bubbleY != lastBubbleY || sensitivityChanged) {

        if (lastBubbleX != -100) {
            M5.Display.fillCircle(lastBubbleX, lastBubbleY, BUBBLE_RADIUS, BLACK);
        }
        M5.Display.fillRect(0, 0, M5.Display.width(), 22, BLACK);

        M5.Display.drawCircle(centerX, centerY + LEVEL_Y_OFFSET, 50, WHITE);
        M5.Display.drawCircle(centerX, centerY + LEVEL_Y_OFFSET, 25, WHITE);

        M5.Display.fillCircle(bubbleX, bubbleY, BUBBLE_RADIUS, RED);

        M5.Display.setTextSize(1);
        M5.Display.setTextColor(TFT_SKYBLUE);

        M5.Display.setCursor(5, 2);
        M5.Display.printf("Sens: %.0f", sensitivity);

        M5.Display.setCursor(5, 12);
        M5.Display.printf("X: %.2f | Y: %.2f", accX, accY);


        lastBubbleX = bubbleX;
        lastBubbleY = bubbleY;
    }

    delay(20);
}