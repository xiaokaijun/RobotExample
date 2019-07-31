#ifndef kranfix_rs232_rs232_cc
#define kranfix_rs232_rs232_cc

#include "rs232.h"

static int error;
static struct termios nps;

kfx::RS232::RS232(const char *dev_name, int baudrate) : available(false) {
  // asigning device name
  strcpy(devname, dev_name);

  // Chossing baudrate
  switch (baudrate) {
    case 50 : baudr = B50;
      break;
    case 75 : baudr = B75;
      break;
    case 110 : baudr = B110;
      break;
    case 134 : baudr = B134;
      break;
    case 150 : baudr = B150;
      break;
    case 200 : baudr = B200;
      break;
    case 300 : baudr = B300;
      break;
    case 600 : baudr = B600;
      break;
    case 1200 : baudr = B1200;
      break;
    case 1800 : baudr = B1800;
      break;
    case 2400 : baudr = B2400;
      break;
    case 4800 : baudr = B4800;
      break;
    case 9600 : baudr = B9600;
      break;
    case 19200 : baudr = B19200;
      break;
    case 38400 : baudr = B38400;
      break;
    case 57600 : baudr = B57600;
      break;
    case 115200 : baudr = B115200;
      break;
    case 230400 : baudr = B230400;
      break;
    case 460800 : baudr = B460800;
      break;
    case 500000 : baudr = B500000;
      break;
    case 576000 : baudr = B576000;
      break;
    case 921600 : baudr = B921600;
      break;
    case 1000000 : baudr = B1000000;
      break;
    default      : printf("invalid baudrate\n");
      return;
  }

  port = open(devname, O_RDWR | O_NOCTTY | O_NDELAY);
  if (port == -1) {
    perror("unable to open comport ");
    return;
  }

  error = tcgetattr(port, &ops);
  if (error == -1) {
    close(port);
    perror("unable to read portsettings ");
    return;
  }
  memset(&nps, 0, sizeof(nps));  /* clear the new struct */

  nps.c_cflag = baudr | CS8 | CLOCAL | CREAD;
  nps.c_iflag = IGNPAR;
  nps.c_oflag = 0;
  nps.c_lflag = 0;
  nps.c_cc[VMIN] = 0;      /* block untill n bytes are received */
  nps.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
  error = tcsetattr(port, TCSANOW, &nps);
  if (error == -1) {
    close(port);
    perror("unable to adjust portsettings ");
    return;
  }

  available = true;
}

int kfx::RS232::Read(unsigned char byte) {
  return read(port, &byte, 1);
}

int kfx::RS232::Read(unsigned char *buf, int size) {
#ifndef __STRICT_ANSI__                       /* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
  if (size > SSIZE_MAX) size = (int) SSIZE_MAX;  /* SSIZE_MAX is defined in limits.h */
#else
  if(size>4096)  size = 4096;
#endif

  return read(port, buf, size);
}

int kfx::RS232::Write(unsigned char byte) {
  return write(port, &byte, 1);
}

int kfx::RS232::Write(unsigned char *buf, int size) {
  return write(port, buf, size);
}

void kfx::RS232::Close() {
  close(port);
  tcsetattr(port, TCSANOW, &ops);
}

/*
Constant    Description
--------------------------------------------
TIOCM_LE    DSR (data set ready/line enable)
TIOCM_DTR   DTR (data terminal ready)
TIOCM_RTS   RTS (request to send)
TIOCM_ST    Secondary TXD (transmit)
TIOCM_SR    Secondary RXD (receive)
TIOCM_CTS   CTS (clear to send)
TIOCM_CAR   DCD (data carrier detect)
TIOCM_CD    Synonym for TIOCM_CAR
TIOCM_RNG   RNG (ring)
TIOCM_RI    Synonym for TIOCM_RNG
TIOCM_DSR   DSR (data set ready)
*/
int kfx::RS232::IsCTSEnabled() {
  int status;
  status = ioctl(port, TIOCMGET, &status);
  return (status & TIOCM_CTS) ? 1 : 0;
}

// Sends a string to serial port till finding a '\0'
void kfx::RS232::Print(const char *text) {
  while (*text != 0) Write(*(text++));
}

#endif // kranfix_rs232_rs232_cc