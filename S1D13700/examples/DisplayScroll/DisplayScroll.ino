#include <S1D13700.h>

S1D13700 Lcd;  // S1D13700 display object
const int displayLines = 30;  // Total display lines
const int charsPerLine = 40;  // Visible display characters per line
int currentLine = 0;  // Cursor position
char lineNumber;  // Special character for display
char memory[displayLines][charsPerLine + 1];  // Array for text shift (+1 -> EOL)
bool scroll = false;  // If curson on bottom of display, scroll enabled

void setup()
{
	// Clear two-dim memory
	for (int i = 0; i < displayLines; i++) {
		for (int j = 0; j < charsPerLine; j++) {
			memory[i][j] = ' ';
		}
	}

	// Init display communication
	Lcd.pins.d0 = 14;
	Lcd.pins.d1 = 15;
	Lcd.initLCD();

	// Clear display
	Lcd.clearGraphic();
	Lcd.clearText();

	// Init serial communication
	Lcd.textGoTo(0, 0);
	Lcd.writeText("Connection established!");
	Serial.begin(9600);
	Serial.println("Connection established!");
}

void loop()
{
	// If new message arrives
	if (Serial.available()) {
		saveData(Serial.readString());  // Read and save data
		printMemoryOnScreen();  // Write on screen
		printMemoryOnSerial();  // Write into console (serial)
	}
}

// Save data in two-dimensional array
void saveData(String message) {

	// Prepare data
	message.trim();  // Delete whitespaces
	if (scroll) shiftDataVertically();  // If screen full -> scroll

	// Save more lines
	if (message.length() > charsPerLine) {

		// Count number of overflow lines
		float overflow = (float)message.length() / (float)charsPerLine;
		int overflowLines = ((overflow == (int)overflow) ? overflow : overflow + 1) - 1;

		// Save first line
		message.toCharArray(memory[currentLine], charsPerLine + 1);  // Save data in array

		// Save overflow lines
		for (int i = 0; i < overflowLines; i++) {
			if (scroll) shiftDataVertically(); else currentLine++;  // Scroll if display full, move cursor if not
			message.remove(0, charsPerLine);  // Cut already saved part
			message.toCharArray(memory[currentLine], charsPerLine + 1);  // Save data in array
		}
	}

	// Save one line only
	else {
		message.toCharArray(memory[currentLine], charsPerLine + 1);  // Save data in array
	}

	// Check if display is full
	if (!scroll) {
		if (currentLine + 1 == displayLines) scroll = true;  // If screen full, enable scroll
		else currentLine++;  // Increase line number for next time
	}
}

// Shift all data up, in two-dimensional array
void shiftDataVertically() {
	for (int i = 0; i < displayLines - 1; i++) {
		strncpy(memory[i], memory[i + 1], charsPerLine);
	}
}

// Write memory on screen
void printMemoryOnScreen() {
	Lcd.clearText();
	for (int i = 0; i < displayLines; i++) {
		Lcd.textGoTo(0, lineNumber + i);
		Lcd.writeText(&memory[i][0u]);
	}
	Serial.setTimeout(10);
}

// Print memory into console (serial)
void printMemoryOnSerial() {
	for (int i = 0; i < displayLines; i++) {
		Serial.print(i);
		Serial.print(": ");
		Serial.println(memory[i]);
	}
	Serial.println("\n----------");
}

// Print memory into console char by char (serial)
void printMemoryOnSerialDetail() {
	for (int i = 0; i < displayLines; i++) {
		Serial.print(i);
		Serial.print(": ");
		Serial.println(memory[i]);

		for (int j = 0; j < charsPerLine; j++) {
			Serial.print("\t[");
			Serial.print(j);
			Serial.print("]:\t");
			Serial.println(memory[i][j]);
		}
		Serial.println();
	}
	Serial.println("\n----------");
}
