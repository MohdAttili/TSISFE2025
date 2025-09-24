## This folder contains the control software code for all components, including the main algorithms, sensor integration, and motor control functions. It is the core that makes the vehicle autonomous.
<!-- <p>
  <img src="/other/autonomous_vehicle_software_core.png" alt="Image 8" width="600"/>
      </p> -->
<table>
  <tr>
    <td align="center" width="50%">
      <img src="/other/autonomous_vehicle_software_core.png" alt="Image 8" width="300"/>
    </td>
    <td width="50%">
      
**Cam Test Code**  

```cpp
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
      
      int obstcaleColor = cam.ccc.blocks[largestIndex].m_signature;
      
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
