#include <math.h>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <climits>

float reverseFloat(float input) {
 float retVal;
 char *floatToConvert = (char*) &input;
 char *floatToReturn = (char*) &retVal;
 
 floatToReturn[0] = floatToConvert[3];
 floatToReturn[1] = floatToConvert[2];
 floatToReturn[2] = floatToConvert[1];
 floatToReturn[3] = floatToConvert[0];

 return retVal;
}

float psnr(std::vector<float> rawData, std::vector<float> decodedData) {
 int sizeOfRawData = rawData.size(), sizeOfDecoded = decodedData.size();
 float retVal = 0.0;
 if(sizeOfRawData != sizeOfDecoded) {
  sizeOfRawData > sizeOfDecoded ? sizeOfRawData = sizeOfDecoded : sizeOfRawData = sizeOfRawData;
 }
 for(int i = 0; i < sizeOfRawData; ++i) {
  retVal += (rawData[i] - decodedData[i]) * (rawData[i] - decodedData[i]);
 }
 retVal = retVal/sizeOfRawData;
 float logOfMax = log(1)/log(10);
 float logOfMSE = log(retVal)/log(10);
 retVal = (20 * logOfMax) - (10 * logOfMSE);
 return retVal;
}

int main(int argc, char* argv[]) {
 std::ifstream inFile1(argv[1], std::ifstream::in | std::ifstream::binary);
 std::ifstream inFile2(argv[2], std::ifstream::in | std::ifstream::binary);
 
 inFile1.seekg(0, std::ios::beg);
 inFile2.seekg(0, std::ios::beg);
 
 float rawSample = 0.0;
 std::vector<float> rawSamples;
 float decodedSample = 0.0;
 std::vector<float> decodedSamples;

 if(!inFile1) {
  std::cout << "This file did not read properly\n";
  return 1;
 } else {
  while(inFile1.is_open()) {
   if(inFile1.eof()) {
    std::cout << "This is the end of the file\n";
    break;
   }
   rawSample = 0.0;
   inFile1.read((char*) &rawSample, sizeof(rawSample));
   rawSamples.push_back(reverseFloat(rawSample));
  }
 }
 inFile1.close();
 if(!inFile2) {
  std::cout << "This file did not read properly\n";
  return 2;
 } else {
  while(inFile2.is_open()) {
   if(inFile2.eof()) {
    std::cout << "This is the end of the file\n";
    break;
   }
   decodedSample = 0.0;
   inFile2.read((char*) &decodedSample, sizeof(decodedSample));
   decodedSamples.push_back(reverseFloat(rawSample));
  }
 }
 inFile2.close();
 float finalPsnr = psnr(rawSamples, decodedSamples);
 std::cout << "The PSNR of file in command 1 and file in command 2 is: " << finalPsnr << "\n";
 return 0;
}
