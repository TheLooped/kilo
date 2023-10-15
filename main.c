#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enableRawMode() {

  // This edits a terminals attributes using orig_termios
  tcgetattr(STDIN_FILENO, &orig_termios);

  // calls Disable raw at exit
  atexit(disableRawMode);

  // initalized a struct called raw that takes attributes from orig_termios
  struct termios raw = orig_termios;

  // bit shift raw so no 'ECHO' is shown
  raw.c_lflag &= ~(ECHO);

  // Set the attributes and flush
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;

  // Reading from stdin And quits upon recieving 'q'
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
  }

  return 0;
}
