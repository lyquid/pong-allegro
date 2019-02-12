#ifndef CONFIG_H
#define CONFIG_H  

const int kFPS = 60;
const int kScreenWidth = 1024;
const int kScreenHeight = 768;

typedef struct Player { 
  int length = 150,
      width = 10,
      score = 0;
  float x = (kScreenWidth * 0.10) - (width / 2), 
        y = (kScreenHeight / 2) - (length / 2),
        dx = 0.0, 
        dy = 6.0;
} Player; 

#endif