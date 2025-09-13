// Cam Test Code


#include <Pixy2I2C.h>
#include <Wire.h>

Pixy2I2C cam;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); 
  cam.init();
  delay(500);
}

char getObstcaleColor(){
  cam.ccc.getBlocks();
  int numBlocks = cam.ccc.numBlocks;
  if (numBlocks > 0) {
    int largestIndex = 0;
    int maxArea = 0;

    
    for (int i = 0; i < numBlocks; i++) {
      int area = cam.ccc.blocks[i].m_width * cam.ccc.blocks[i].m_height;
      if (area > maxArea) {
        maxArea = area;
        largestIndex = i;
      }
    }
    
    int obstcaleColor =cam.ccc.blocks[largestIndex].m_signature;
    
    if(obstcaleColor == 1||obstcaleColor == 2||obstcaleColor == 3||obstcaleColor == 4){
      return 'R';
    }
    else if (obstcaleColor == 5||obstcaleColor == 6||obstcaleColor == 7||obstcaleColor == 8){
      return 'G';
    }
    else{
      return 'U';
    }
  }
  else{
      return 'N';
    }
}
void loop() {
  
  Serial.println(getObstcaleColor());
  delay(200);
  
}
