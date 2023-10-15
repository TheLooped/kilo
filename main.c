/*** Includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** Data ***/

struct termios orig_termios;

/*** Terminal ***/

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() { /*{{{*/
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  die("tcsetattr");
} /*}}}*/

void enableRawMode() {

  // This edits a terminals attributes using orig_termios
  if (tcgetattr(STDIN_FILENO, &orig_termios))
    die("tcgetattr");

  // calls Disable raw at exit
  atexit(disableRawMode);

  // initalized a struct called raw that takes attributes from orig_termios
  struct termios raw = orig_termios;

  // Disable some terminal defaults
  // bit shift raw so no 'ECHO' is shown
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_cflag |= (CS8);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  // Set the attributes and flush
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/*** Init ***/

int main() {
  enableRawMode();

  // Reading from stdin And quits upon recieving 'q'
  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
      die("read");
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q')
      break;
  }
  return 0;
}
