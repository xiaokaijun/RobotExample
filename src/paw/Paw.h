#ifndef PAW_DEMO_PAW_PAW_H_
#define PAW_DEMO_PAW_PAW_H_

#include "rs232.h"
#include <stdio.h>
#include <iostream>

using namespace std;

class PawData {
 public:
  virtual void toData(u_char *data) = 0;

  virtual int getDataLength() = 0;
};

class CatchXgData : public PawData {
 public:
  CatchXgData(int speed = 500, int power = 100);

 private:
  int speed;
  int power;

 public:
  void toData(u_char *data);

  int getDataLength();
};

class ReleaseData : public PawData {

 public:
  ReleaseData(int speed = 300);

 private:
  int speed;

 public:
  void toData(u_char *data);

  int getDataLength();
};


class Paw {

 public:
  Paw(const char *port);
  ~Paw();

 private:
  const char *port;
  kfx::RS232 *rs232;

 public:
  bool catchXG(int speed = 500, int power = 100);
  bool release(int speed = 500);
  void close();

 private:
  int parseData(u_char *out, u_char cmd, PawData* data = nullptr);

};

#endif //PAW_DEMO_PAW_PAW_H_
