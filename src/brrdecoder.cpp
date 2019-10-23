#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <climits>

float reverseFloat(float inputFloat) {
 float retVal;
 char* floatToReverse = (char*) &inputFloat;
 char* floatToReturn = (char*) &retVal;
 
 floatToReturn[0] = floatToReverse[3];
 floatToReturn[1] = floatToReverse[2];
 floatToReturn[2] = floatToReverse[1];
 floatToReturn[3] = floatToReverse[0];

 return retVal;
}

std::vector<float> decodeBRR(std::vector<char> input) {
 int range, sizeOfInput = input.size();
 std::vector<float> retVal;

 for(int i = 0; i < sizeOfInput - 9; i+=9) {
  range = (0x00 | ((input[i] >> 4) & 0x0f));
  for(int j = i + 1; j < i + 9; ++j) {
   int sample1 = (0x00 | ((input[j] >> 4) & 0x0f));
   int sample2 = (0x00 | (input[j] & 0x0f));
   if((sample1 >> 3) & 0x1 == 1) {
    sample1 = ((sample1 &0x7) * (-1)) - 1;
   }
   if((sample2 >> 3) & 0x1 == 1) {
    sample2 = ((sample2 &0x7) * (-1)) - 1;
   }
   float pcmSample1 = (static_cast<float>(sample1) * (1 << range)) / 2;
   float pcmSample2 = (static_cast<float>(sample2) * (1 << range)) / 2;
   retVal.insert(retVal.end(), {reverseFloat(pcmSample1 / (1 << 15)), reverseFloat(pcmSample2 / (1 << 15))});
  }
 }
 return retVal;
}

int main(int argc, char *argv[]) {
 std::ifstream inFile(argv[1], std::fstream::binary);

 char byte;
 std::vector<char> bytes;
 if(!inFile) {
  return 1;
 } else {
  while(inFile.is_open()) {
   if(inFile.eof()) {
    break;
   }
   byte = 0x00;
   inFile.read((char*) &byte, sizeof(char));
   
   bytes.push_back(byte);
  }
 }
 inFile.close();
 std::vector<float> output = decodeBRR(bytes);
 std::ofstream ofile(argv[2], std::fstream::binary);
 ofile.write((char*) &output[0], sizeof(float) * output.size());
 ofile.close();
 return 0; 
}
