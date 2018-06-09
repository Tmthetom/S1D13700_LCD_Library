#include <S1D13700.h>

S1D13700 glcd;
const int displayLines = 5;
const int charsPerLine = 40;  
int currentLine = 0;
char lineNumber;
char data[displayLines][charsPerLine + 1];  // Plus EOL
bool scroll = false;

void setup()
{
	// Allocate two-dim memory
	for (int i = 0; i < displayLines; i++) {
		for (int j = 0; j < charsPerLine; j++) {
			data[i][j] = ' ';
		}
	}

	// Init display communication
	glcd.pins.d0 = 14;
	glcd.pins.d1 = 15;
	glcd.initLCD();

	// Clear display
	glcd.clearGraphic();
	glcd.clearText();

	// Init serial communication
	glcd.textGoTo(0, 0);
	glcd.writeText("Connection established!");
	Serial.begin(9600);
	Serial.println("Connection established!");
}

void loop()
{
	// If new message arrives
	if (Serial.available()) {
		saveData(Serial.readString());  // Read and save data
		writeOnScreen();  // Write on screen
		printMemoryOnSerial(); // Print memory into console (serial)
	}
}

// Print memory into console (serial)
void printMemoryOnSerial(){
	for (int i = 0; i < displayLines; i++) {
		Serial.print(i);
		Serial.print(": ");
		Serial.println(data[i]);
	}
	Serial.println("\n----------");
}

// Print memory into console char by char (serial)
void printMemoryOnSerialDetail() {
	for (int i = 0; i < displayLines; i++) {
		Serial.print(i);
		Serial.print(": ");
		Serial.println(data[i]);

		for (int j = 0; j < charsPerLine; j++) {
			Serial.print("\t[");
			Serial.print(j);
			Serial.print("]:\t");
			Serial.println(data[i][j]);
		}
		Serial.println();
	}
	Serial.println("\n----------");
}

// Write memory on screen
void writeOnScreen() {
	glcd.clearText();
	for (int i = 0; i < displayLines; i++) {
		glcd.textGoTo(0, lineNumber + i);
		glcd.writeText(&data[i][0u]);
	}
	Serial.setTimeout(10);
}

// Save data in two-dimensional array
void saveData(String message) {

	// Prepare data and display
	message.trim();  // Delete whitespaces
	Serial.print("Posledni znak: ");
	Serial.println(message.charAt(charsPerLine - 1));
	if (scroll) shiftDataVertically();  // If screen full -> scroll

	// Save also overflow lines
	if (message.length() > charsPerLine) {

		// Count number of overflow lines
		float overflow = (float)message.length() / (float)charsPerLine;
		int overflowLines = ((overflow == (int)overflow) ? overflow : overflow + 1) - 1;

		// Save first line
		message.toCharArray(data[currentLine], charsPerLine + 1);  // Save data in array

		// Save overflow lines
		for (int i = 0; i < overflowLines; i++) {
			if (scroll) shiftDataVertically(); else currentLine++;  // Scroll if display full, move cursor if not
			message.remove(0, charsPerLine);  // Cut already saved part
			message.toCharArray(data[currentLine], charsPerLine + 1);  // Save data in array
		}
	}

	// For only one line
	else {
		message.toCharArray(data[currentLine], charsPerLine + 1);  // Save data in array
	}

	// Check if display is full
	if (!scroll) {
		currentLine++;  // Increase line number for next time
		if (currentLine + 1 == displayLines) scroll = true;  // If screen full, enable scroll
	}
}

// Shift all data up, in two-dimensional array
void shiftDataVertically() {
	for (int i = 0; i < displayLines; i++) {
		strncpy(data[i], data[i + 1], charsPerLine);
	}
}
