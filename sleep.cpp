//
// Created by vbustamante on 11/27/18.
//

#include "sleep.h"

#ifdef _WIN32
  #include <windows.h>

  void lg_sleep(unsigned milliseconds){
      Sleep(milliseconds);
  }
#else
  #include <unistd.h>

  void lg_sleep(unsigned milliseconds) {
    usleep(milliseconds * 1000); // takes microseconds
  }
#endif