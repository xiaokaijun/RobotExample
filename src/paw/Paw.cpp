
#include <iomanip>
#include "Paw.h"

struct HexCS {
  u_char *c;
  int len;

  HexCS(u_char *_c, int len) : c(_c), len(len) {}
};

inline std::ostream &operator<<(std::ostream &o, const HexCS &hs) {
  for (int i = 0; i < hs.len; ++i) {
    o << "\\x" << setw(2) << setfill('0') << std::hex << (int) hs.c[i];
  }
  return o;
}

inline HexCS hex(u_char *c, int len = 1) {
  return HexCS(c, len);
}

Paw::Paw(const char *port) {
  this->port = port;
  int baudrate = 115200;

  rs232 = new kfx::RS232(port, baudrate);
  if (!rs232->IsAvailable()) {
    cout << "Serial port %s is not available" << endl;
    return;
  }
}

Paw::~Paw() {
  delete (rs232);
}

int Paw::parseData(u_char *out, u_char cmd, PawData *data) {

  //帧头
  u_char header[] = {0xEB, 0x90};

  //ID
  u_char id = 0x01;

  //data1
  u_char data1 = cmd;

  //data2
  u_char *data2 = nullptr;
  int data2_len = 0;

  // len
  u_char len;

  //如果有数据
  if (data) {
    data2_len = data->getDataLength();
    data2 = new u_char[data2_len];
    data->toData(data2);

    len = 1 + data2_len;
  } else {
    len = 1;
  }

  //check sum
  u_char check_sum = id + len + data1;
  if (data2) {
    for (int i = 0; i < data2_len; ++i) {
      check_sum += data2[i];
    }
  }

  cout << "====================================" << endl;
  cout << "header:    " << hex(header, 2) << endl;
  cout << "ID:        " << hex(&id) << endl;
  cout << "len:       " << hex(&len) << endl;
  cout << "data1:     " << hex(&data1) << endl;
  cout << "data2:     " << hex(data2, data2_len) << endl;
  cout << "check sum: " << hex(&check_sum) << endl;

//  out = new u_char[len + 5];
//  out = (u_char *) alloca(len + 5);
  out[0] = header[0];
  out[1] = header[1];
  out[2] = id;
  out[3] = len;
  out[4] = data1;
  memcpy(out + 5, data2, data2_len);
  out[len + 4] = check_sum;

  return len + 5;

//  cout << "---------------- write ------------------" << endl;
//  cout << "write: " << hex(buf, len + 5) << endl;
//  long int writeLen = write(this->_fd, buf, len + 5);
//  cout << "writeLen: " << writeLen << endl;
//
//  if (cmd == 0x01 || cmd == 0x12) {
//    usleep(1000000);
//  } else {
//    usleep(100000);
//  }
//
//  u_char readBuffer[1024];
//  int readLen = read(this->_fd, readBuffer, 1024);
//
//  u_char readData[readLen];
//  memcpy(readData, readBuffer, readLen);
//
//  cout << "---------------- read -------------------" << endl;
//  cout << "readLen: " << readLen << endl;
//  cout << "read: " << hex(readData, readLen) << endl;
//  if (readLen < 0) {
//    perror("read STDIN_FILENO");
//    exit(1);
//  }
//
//  cout << "====================================" << endl;


}

bool Paw::catchXG(int speed, int power) {
  u_char cmd = 0x10;
  CatchXgData *data = new CatchXgData(speed, power);
  int size = 6 + data->getDataLength();

  u_char *sendData = new u_char[size];
  parseData(sendData, cmd, data);

  rs232->Write(sendData, size);

  return true;
}

bool Paw::release(int speed) {
  u_char cmd = 0x11;
  ReleaseData *data = new ReleaseData(speed);
  int size = 6 + data->getDataLength();

  u_char *sendData = new u_char[size];
  parseData(sendData, cmd, data);

  rs232->Write(sendData, size);

  return false;
}

void Paw::close() {
  rs232->Close();
}

CatchXgData::CatchXgData(int speed, int power) : PawData() {
  if (speed > 1000) {
    speed = 1000;
  }

  if (speed < 1) {
    speed = 1;
  }
  this->speed = speed;

  if (power > 1000) {
    power = 1000;
  }

  if (power < 30) {
    power = 30;
  }
  this->power = power;
}

int CatchXgData::getDataLength() {
  return 4;
}

void CatchXgData::toData(u_char *data) {
  int speedLow = this->speed & 0x00ff;
  int speedHigh = this->speed >> 8;

  int powerLow = this->power & 0x00ff;
  int powerHigh = this->power >> 8;

  data[0] = speedLow;
  data[1] = speedHigh;
  data[2] = powerLow;
  data[3] = powerHigh;
}

ReleaseData::ReleaseData(int speed) : PawData() {
  if (speed > 1000) {
    speed = 1000;
  }

  if (speed < 1) {
    speed = 1;
  }
  this->speed = speed;
}

void ReleaseData::toData(u_char *data) {
  int low = this->speed & 0x00ff;
  int high = this->speed >> 8;

  data[0] = low;
  data[1] = high;
}

int ReleaseData::getDataLength() {
  return 2;
}