#include <fstream>
#include <vector>
#include <iostream>
#include <climits>
#include <stdlib.h>
#include <math.h>

float reverseFloat(float inFloat) {
 float retVal;
 char *floatToConvert = (char*) &inFloat;
 char *returnFloat = (char*) &retVal;

 returnFloat[0] = floatToConvert[3];
 returnFloat[1] = floatToConvert[2];
 returnFloat[2] = floatToConvert[1];
 returnFloat[3] = floatToConvert[0];

 return retVal;
}

std::vector<char> encodeBRR(std::vector<float> inputSamples) {
 int range, sizeOfVector = inputSamples.size(), max = INT_MIN, min = INT_MAX, next;
 float error1 = 0.0, error2 = 0.0, error3 = 0.0, error4 = 0.0;
 std::vector<int> samplesAsInt, errors;
 std::vector<char> retVal;
 for(int i = 0; i <= sizeOfVector - 16; i+=16) {
  for(int j = i; j < i + 16; ++j) {
   samplesAsInt.push_back(static_cast<int>(inputSamples[j] * 32768));
   if(samplesAsInt[j] > max) {
    max = samplesAsInt[j];
   } else if(samplesAsInt[j] < min) {
    min = samplesAsInt[j];
   }
  }
  if(abs(min) > abs(max)) {
   next = static_cast<int>(ceil(log(abs(min)) / log(2)));
   range = next - 3; 
  } else {
   next = static_cast<int>(ceil(log(abs(max)) / log(2)));
   range = next - 2; 
  }
  char header = 0x0 | ((range << 4) & 0xf0);
  retVal.push_back(header);
  for(int j = i; j < i + 16; j+=2) {
   int nibble1 = static_cast<int>(round(((static_cast<float>(samplesAsInt[j])/(1 << range)) * 2)));
   int nibble2 = static_cast<int>(round(((static_cast<float>(samplesAsInt[j + 1])/(1 << range)) * 2)));
   char sample = 0x0; 
   sample = sample | ((nibble1 << 4) & 0xf0);
   sample |= (nibble2 & 0x0f);
   retVal.push_back(sample);
  }
  max = INT_MIN;
  min = INT_MAX;
 }
 return retVal;
}


int main(int argc, char* argv[]) {
 std::ifstream file(argv[1], std::ifstream::in | std::ifstream::binary);
 
 file.seekg(0, std::ios::beg);
 float sample;
 std::vector<float> samples;
 if(!file) {
  std::cout << "This file did not read properly\n";
  return 1;
 } else {
  while(file.is_open()) {
   if(file.eof()) {
    std::cout << "This is the end of the file\n";
    break;
   } 
   sample = 0.0;
   file.read((char*) &sample, sizeof(sample)); 
   float bigEndSample = reverseFloat(sample);
   samples.push_back(bigEndSample);
  }
 }
 file.close();
 std::vector<char> output = encodeBRR(samples);
 std::ofstream ofile(argv[2], std::ios::binary);
 ofile.write((char*) &output[0], sizeof(char) * output.size());
 ofile.close();
}
