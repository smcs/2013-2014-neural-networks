#include<iostream>
#include<cmath>

using namespace std;

#define InNode 4
#define HiddenNode 20
#define OutNode 1 

class BPNet{
public:
      double HiddenThresh[HiddenNode];
      double OutThresh[OutNode];
      double HiddenWeight[InNode][HiddenNode];
      double OutWeight[HiddenNode][OutNode];
      double rate_HiddenThresh;
      double rate_HiddenWeight;
      double rate_OutThresh;
      double rate_OutWeight;
      double error;
      double MaxError;
      double result[OutNode];
      BPNet(){
              error = 0;
              MaxError = 1;
              rate_HiddenThresh = 0.9;
              rate_HiddenWeight = 0.9;
              rate_OutThresh = 0.9;
              rate_OutThresh = 0.9;
      }       
      void init(){
           for (int i=0;i<InNode;i++){
               for (int j=0;j<HiddenNode;j++){
                   HiddenWeight[i][j] = (2.0*(double)rand()/RAND_MAX)-1;
               }
           }
           for (int i=0;i<HiddenNode;i++){
               for (int j=0;j<OutNode;j++){
                   OutWeight[i][j] = (2.0*(double)rand()/RAND_MAX)-1;
               }
           }
      }
      void train(){
           double Input[InNode];
           double Hidden[HiddenNode];
           double Output[OutNode];
           double Target[OutNode];
           double TargetDif[OutNode]; 
           double HiddenAdjust[HiddenNode];
           double HiddenAct[HiddenNode];
           double OutAct[OutNode];
      }
};
int main(){
    BPNet BP;
    BP.init();
    system("PAUSE");
    return 0;
} 
