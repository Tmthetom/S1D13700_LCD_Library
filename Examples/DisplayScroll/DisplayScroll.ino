#include <S1D13700.h>

S1D13700 glcd;
const int displayLines = 30;
const int charsPerLine = 40;
int line = 0;
char lineNumber;
char data[displayLines][charsPerLine];
bool scroll = false;

void setup()
{
	// Init communication
	glcd.pins.d0 = 14;
	glcd.pins.d1 = 15;
	glcd.initLCD();
	Serial.begin(9600);

	// Clear display
	glcd.clearGraphic();
	glcd.clearText();
}

void loop()
{
	// If new message arrives
	if (Serial.available()) {

		// Save data
		saveData(Serial.readString());

		// Write on screen
		glcd.clearText();
		for (int i = 0; i < displayLines; i++) {
			glcd.textGoTo(0, lineNumber + i);
			glcd.writeText(&data[i][0u]);
		}
		Serial.setTimeout(10);

		// Show memory into console
		for (int i = 0; i < displayLines; i++) {
			Serial.print(i);
			Serial.print(": ");
			Serial.println(data[i]);
		}
		Serial.println("\n----------");
	}
}

// Save data in two-dimensional array
void saveData(String text) {

	// Prepare data and display
	text.trim();  // Delete whitespaces
	if (scroll) shiftDataVertically();  // If screen full -> scroll
	
	// Save also overflow lines
	if (text.length() > charsPerLine) {

		// Count number of overflow lines
		float overflow = (float)text.length() / (float)charsPerLine;
		int overflowLines = ((overflow == (int)overflow) ? overflow : overflow + 1) - 1;

		// Save first line
		text.toCharArray(data[line], charsPerLine);  // Save data in array

		// Save overflow lines
		for (int i = 0; i < overflowLines; i++) {
			if (scroll) shiftDataVertically(); else line++;  // Scroll if display full, move cursor if not
			text.remove(0, charsPerLine);  // Cut already saved part
			text.toCharArray(data[line], charsPerLine);  // Save data in array
		}
	}

	// For only one line
	else {
		text.toCharArray(data[line], charsPerLine+1);  // Save data in array
	}

	// Check if display is full
	if (!scroll) {
		line++;  // Increase line number for next time
		if (line + 1 == displayLines) scroll = true;  // If screen full, enable scroll
	}
}

// Shift all data up, in two-dimensional array
void shiftDataVertically() {
	for (int i = 0; i < displayLines; i++) {
		strncpy(data[i], data[i + 1], charsPerLine+1);
	}
}
