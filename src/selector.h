#include <M5Stack.h>

class Selector
{
public:
    bool selecting = false;

    int chosenIndex = 0;
    char chosenField[33];

    char *start(char list[][33], int listLength)
    {
        // Setup
        M5.Speaker.begin();
        M5.Lcd.setTextSize(1);
        M5.Lcd.clear(); // Clear the screen.
        M5.Lcd.setCursor(0, 0);

        // Pre-fill chosenField with null terminator
        memset(chosenField, 0x00, 33);

        // Buttons
        M5.Lcd.drawRect(0, 208, 106, 32, BLUE);
        M5.Lcd.drawRect(106, 208, 107, 32, BLUE);
        M5.Lcd.drawRect(213, 208, 106, 32, BLUE);
        M5.Lcd.drawString("Op", 50, 215, 2);
        M5.Lcd.drawString("Vaelg", 143, 215, 2);
        M5.Lcd.drawString("Ned", 245, 215, 2);

        M5.Lcd.println("Selecting:");

        M5.Lcd.printf("List contains %d elements", listLength);
        M5.Lcd.println();
        updateList(list, listLength);

        // During startup, update/reset button-clicks to avoid selecting the first item in the list
        if (!selecting)
        {
            delay(1);
            M5.update();
            selecting = true;
        }

        // While selector is running
        while (selecting)
        {
            if (M5.BtnA.wasPressed())
            {
                chosenIndex--;
                if (chosenIndex < 0)
                {
                    chosenIndex = listLength - 1;
                }

                updateList(list, listLength);
            }
            else if (M5.BtnB.wasPressed())
            {
                selecting = false;

                int i;
                for (i = 0; i < strlen(list[chosenIndex]); i++)
                {
                    chosenField[i] = list[chosenIndex][i];
                }
            }
            else if (M5.BtnC.wasPressed())
            {
                chosenIndex++;
                if (chosenIndex == listLength)
                {
                    chosenIndex = 0;
                }

                updateList(list, listLength);
            }
            delay(1);
            M5.update();
        }

        M5.Speaker.tone(900, 120);
        return chosenField;
    }

    void updateList(char list[][33], int listLength)
    {
        int x, y;
        int listItemWidth = 320;
        int numberOfItemsToDisplay = 5;
        int displayIndex = 0;
        M5.Lcd.fillRect(0, 20, 320, 160, BLACK);

        if (chosenIndex >= 2 && chosenIndex < listLength - 2)
        {
            for (int i = chosenIndex - 2; i <= chosenIndex + 2; i++)
            {
                x = 0;
                y = (20 + (displayIndex * 32));
                if (i == chosenIndex)
                {
                    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
                    M5.Lcd.fillRect(x, y, listItemWidth, 32, WHITE);
                }
                else
                {
                    M5.Lcd.drawRect(x, y, listItemWidth, 32, BLUE);
                }
                M5.Lcd.setCursor(x + 5, y + 12);
                M5.Lcd.printf("%d: ", i + 1);
                M5.Lcd.drawString(list[i], x + 20, y + 7, 2);
                M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
                displayIndex++;
            }
        }
        else if (chosenIndex < 2)
        {
            for (int i = 0; i < numberOfItemsToDisplay; i++)
            {
                x = 0;
                y = (20 + (displayIndex * 32));
                if (i == chosenIndex)
                {
                    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
                    M5.Lcd.fillRect(x, y, listItemWidth, 32, WHITE);
                }
                else
                {
                    M5.Lcd.drawRect(x, y, listItemWidth, 32, BLUE);
                }
                M5.Lcd.setCursor(x + 5, y + 12);
                M5.Lcd.printf("%d: ", i + 1);
                M5.Lcd.drawString(list[i], x + 20, y + 7, 2);
                M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
                displayIndex++;
            }
        }
        else if (chosenIndex >= listLength - 2)
        {
            for (int i = listLength - numberOfItemsToDisplay; i < listLength; i++)
            {
                x = 0;
                y = (20 + (displayIndex * 32));
                if (i == chosenIndex)
                {
                    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
                    M5.Lcd.fillRect(x, y, listItemWidth, 32, WHITE);
                }
                else
                {
                    M5.Lcd.drawRect(x, y, listItemWidth, 32, BLUE);
                }
                M5.Lcd.setCursor(x + 5, y + 12);
                M5.Lcd.printf("%d: ", i + 1);
                M5.Lcd.drawString(list[i], x + 20, y + 7, 2);
                M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
                displayIndex++;
            }
        }
    }
};