#include <Keyboard.h>
#include <Mouse.h>
#define BAUD_RATE 57600

#define program_pin 9
#define enable_pin 10
#define ExternSerial Serial1
#define KEY_PAUSE (76+136)

String bufferStr = "";
String last = "";
int firstSpace;
int secondSpace;
String command = "";
String arg1 = "";
String arg2 = "";
float mouseMul = 1.0;

int defaultDelay = 0;

void MouseMove(int x, int y) {
  // Use Mouse.move() for larger values
  x=(int)x*mouseMul;
  y=(int)y*mouseMul;
  for(int i=0; (x>0)?(i<x/100):(i>x/100); (x>0)?(i++):(i--))
    Mouse.move((x>0)?(100):(-100), 0); // Move in X-axis
  for(int j=0; (y>0)?(j<y/100):(j>y/100); (y>0)?(j++):(j--))
    Mouse.move(0, (y>0)?(100):(-100)); // Move in Y-axis
  Mouse.move(x%100, y%100);
}

void MouseMove(int x, int y, int w) {
  w=(int)w*mouseMul;
  for(int k=0; (w>0)?(k<w/100):(k>w/100); (w>0)?(k++):(k--))
    Mouse.move(0, 0, (w>0)?(100):(-100)); // Scroll
  Mouse.move(0, 0, w%100);
  MouseMove(x, y);
}

void Line(String _line) {
  // Split line into command and optional arguments
  firstSpace = _line.indexOf(" ");
  command = _line.substring(0, firstSpace);
  arg1 = "";
  arg2 = "";
  if (firstSpace != -1) {
    arg1 = _line.substring(firstSpace + 1);
    secondSpace = arg1.indexOf(" ");
    if (secondSpace != -1) {
      arg2 = arg1.substring(secondSpace + 1);
      arg1 = arg1.substring(0, secondSpace);
    }
  }

  if (firstSpace == -1) {
    // Not a command, so likely a single key press
    Press(_line);
  } else if (command == "STRING") {
    // Type out characters
    for (int i = firstSpace + 1; i < _line.length(); i++)
      Keyboard.write(_line[i]);
  } else if (command == "STRINGEN") {
    // Type out characters, then press enter
    for (int i = firstSpace + 1; i < _line.length(); i++)
      Keyboard.write(_line[i]);
    Keyboard.press(KEY_RETURN);
  } else if (command == "DELAY") {
    // Pause in milliseconds
    int delaytime = arg1.toInt();
    delay(delaytime);
  } else if (command == "DEFAULTDELAY" || command == "DEFAULT_DELAY") {
    // Define how long to wait between each subsequent command in ms
    defaultDelay = arg1.toInt();
  } else if (command == "REM") {
    // Line is a comment
  } else if (command == "REPLAY") {
    // Repeats the last command n times
    int replaynum = arg1.toInt();
    while (replaynum) {
      Line(last);
      --replaynum;
    }
  } else if (command == "ASCII"){
    // Enter character using ascii number
    Keyboard.write(arg1.toInt());
  } else if (command == "MOUSEMUL") {
    // Set mouse multiplier
    mouseMul = arg1.toFloat();
  } else if (command == "SCROLL") {
    // Mouse scroll wheel
    MouseMove(0, 0, arg1.toInt());
  } else if (command == "MOUSEX") {
    // Mouse X motion
    MouseMove(arg1.toInt(), 0);
  } else if (command == "MOUSEY") {
    // Mouse Y motion
    MouseMove(0, arg1.toInt());
  } else if (command == "MOUSE") {
    // Mouse X,Y motion
    MouseMove(arg1.toInt(), arg2.toInt());
  } else if (command == "MOUSEABS") {
    // Mouse absolute X,Y
    MouseMove(-3840, -2160);
    MouseMove(arg1.toInt(), arg2.toInt());
  } else {
    // Multi-key press (ex: CTRL ALT DEL)
    String remain = _line;
    while (remain.length() > 0) {
      int latest_space = remain.indexOf(" ");
      if (latest_space == -1) {
        Press(remain);
        remain = "";
      } else {
        Press(remain.substring(0, latest_space));
        remain = remain.substring(latest_space + 1);
      }
      delay(5);
    }
  }

  Keyboard.releaseAll();
  delay(defaultDelay);

  // Save this line for next loop, as it may use REPLAY
  last = _line;
}

void Press(String b) {
  if (b.length() == 1) {
    Keyboard.press(char(b[0]));
    return;
  }
  if (b.equals("ENTER")) Keyboard.press(KEY_RETURN);
  else if (b.equals("CTRL") || b.equals("CONTROL")) Keyboard.press(KEY_LEFT_CTRL);
  else if (b.equals("SHIFT")) Keyboard.press(KEY_LEFT_SHIFT);
  else if (b.equals("ALT")) Keyboard.press(KEY_LEFT_ALT);
  else if (b.equals("ALTGR")) Keyboard.press(KEY_RIGHT_ALT);
  else if (b.equals("GUI") || b.equals("WINDOWS")) Keyboard.press(KEY_LEFT_GUI);
  else if (b.equals("UP") || b.equals("UPARROW")) Keyboard.press(KEY_UP_ARROW);
  else if (b.equals("DOWN") || b.equals("DOWNARROW")) Keyboard.press(KEY_DOWN_ARROW);
  else if (b.equals("LEFT") || b.equals("LEFTARROW")) Keyboard.press(KEY_LEFT_ARROW);
  else if (b.equals("RIGHT") || b.equals("RIGHTARROW")) Keyboard.press(KEY_RIGHT_ARROW);
  else if (b.equals("DELETE") || b.equals("DEL")) Keyboard.press(KEY_DELETE);
  else if (b.equals("PAGEUP")) Keyboard.press(KEY_PAGE_UP);
  else if (b.equals("PAGEDOWN")) Keyboard.press(KEY_PAGE_DOWN);
  else if (b.equals("HOME")) Keyboard.press(KEY_HOME);
  else if (b.equals("ESC") || b.equals("ESCAPE")) Keyboard.press(KEY_ESC);
  else if (b.equals("BACKSPACE")) Keyboard.press(KEY_BACKSPACE);
  else if (b.equals("INSERT")) Keyboard.press(KEY_INSERT);
  else if (b.equals("TAB")) Keyboard.press(KEY_TAB);
  else if (b.equals("END")) Keyboard.press(KEY_END);
  else if (b.equals("CAPSLOCK") || b.equals("CAPS")) Keyboard.press(KEY_CAPS_LOCK);
  else if (b.equals("F1")) Keyboard.press(KEY_F1);
  else if (b.equals("F2")) Keyboard.press(KEY_F2);
  else if (b.equals("F3")) Keyboard.press(KEY_F3);
  else if (b.equals("F4")) Keyboard.press(KEY_F4);
  else if (b.equals("F5")) Keyboard.press(KEY_F5);
  else if (b.equals("F6")) Keyboard.press(KEY_F6);
  else if (b.equals("F7")) Keyboard.press(KEY_F7);
  else if (b.equals("F8")) Keyboard.press(KEY_F8);
  else if (b.equals("F9")) Keyboard.press(KEY_F9);
  else if (b.equals("F10")) Keyboard.press(KEY_F10);
  else if (b.equals("F11")) Keyboard.press(KEY_F11);
  else if (b.equals("F12")) Keyboard.press(KEY_F12);
  else if (b.equals("SPACE")) Keyboard.press(' ');
  else if (b.equals("PAUSE") || b.equals("BREAK")) Keyboard.press(KEY_PAUSE);
  else if (b.equals("APP") || b.equals("MENU")) {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F10);
  }
  // Mouse support
  else if (b.equals("CLICK") || b.equals("CLICK_LEFT")|| b.equals("MOUSE_CLICK_LEFT")|| b.equals("MOUSE_CLICK")) Mouse.click();
  else if (b.equals("CLICK_RIGHT") || b.equals("MOUSE_CLICK_RIGHT")) Mouse.click(MOUSE_RIGHT);
  else if (b.equals("CLICK_MIDDLE") || b.equals("MOUSE_CLICK_MIDDLE")) Mouse.click(MOUSE_MIDDLE);
  else if (b.equals("PRESS") || b.equals("PRESS_LEFT") || b.equals("MOUSE_PRESS_LEFT")|| b.equals("MOUSE_PRESS")) Mouse.press();
  else if (b.equals("PRESS_RIGHT") || b.equals("MOUSE_PRESS_RIGHT")) Mouse.press(MOUSE_RIGHT);
  else if (b.equals("PRESS_MIDDLE") || b.equals("MOUSE_PRESS_MIDDLE")) Mouse.press(MOUSE_MIDDLE);
  else if (b.equals("RELEASE") || b.equals("RELEASE_LEFT") || b.equals("MOUSE_RELEASE_LEFT") || b.equals("MOUSE_RELEASE")) Mouse.release();
  else if (b.equals("RELEASE_RIGHT") || b.equals("MOUSE_RELEASE_RIGHT")) Mouse.release(MOUSE_RIGHT);
  else if (b.equals("RELEASE_MIDDLE") || b.equals("MOUSE_RELEASE_MIDDLE")) Mouse.release(MOUSE_MIDDLE);
  else Serial.println("[A] not found :'"+b+"'("+String(b.length())+")");
}

void setup() {
  Serial.begin(BAUD_RATE);
  ExternSerial.begin(BAUD_RATE);

  pinMode(enable_pin, OUTPUT);
  pinMode(program_pin, OUTPUT);
  digitalWrite(program_pin, HIGH);
  digitalWrite(enable_pin, HIGH);

  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  if (ExternSerial.available()) {
    // 0x0D = carriage return
    bufferStr = ExternSerial.readStringUntil(0x0D);
    bufferStr.trim();
    // Serial.println("[A] ---bufferStr---");
    // Serial.print("[A]");
    // Serial.println(bufferStr);
    // Serial.println("[A] ---------------");
  }

  int bufLen = bufferStr.length();
  if (bufLen > 0) {
    // 0x02/0x03 Start/end of text
    if (bufferStr.charAt(0) == 0x02 && bufferStr.charAt(bufLen - 1) == 0x03) {
      // Strip off verification chars
      bufferStr = bufferStr.substring(1, bufLen - 1);
      Serial.print("[A] run: ");
      Serial.println(bufferStr);
      Line(bufferStr);
      // Serial.println("[A] line done");
      ExternSerial.write(0x06); // acknowledge
    } else if (bufferStr.substring(0, 3) == "[E]") {
      // Print passed debug messages from the ESP
      Serial.println(bufferStr);
    } else {
      // Something else... just ignore it, but show what it was
      Serial.print("[A] ignore:");
      Serial.println(bufferStr);
    }
    bufferStr = "";
  }
}
