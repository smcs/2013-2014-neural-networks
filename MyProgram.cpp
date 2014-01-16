#include<iostream>
#include<cmath>

using namespace std;

#define InNode 4
#define HiddenNode 20
#define OutNode 1 
#define SampleSize 5

class BPNet{
public: 
      double Data[SampleSize][InNode];
      double DataTarget[SampleSize][OutNode];
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
              rate_OutWeight = 0.9;
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
           for (int Sample=0;Sample<SampleSize;Sample++){
               for (int i=0;i<InNode;i++){
                   Input[i] = Data[Sample][i]; 
               }
               for (int i=0;i<OutNode;i++){
                   Target[i] = Data[Sample][i];
               }
               for (int i=0;i<HiddenNode;i++){
                   HiddenAct[i] = 0.0;
                   for (int j=0;j<InNode;j++){
                       HiddenAct[i]=HiddenAct[i]+HiddenWeight[j][i]*Input[j]; //Get the activation value for the hidden nodes
                   }
                   Hidden[i]=1.0/(1.0-exp(-(HiddenThresh[i]-HiddenAct[i])));//Sigmoid function for Hidden output
               }
               for (int i=0;i<OutNode;i++){
                   OutAct[i]=0.0;
                   for (int j=0;j<HiddenNode;j++){
                       OutAct[i]=OutAct[i]+OutWeight[j][i]*Hidden[j]; //Get the activation value for the output nodes
                   }
                   Output[i]=1.0/(1.0-exp(-(OutThresh[i]-OutAct[i]))); //Sigmoid function for Output 
               }
               /*
               * Adjust the OutWeight
               */
               for (int i=0;i<OutNode;i++){
                   TargetDif[i]=(1-Output[i])*Output[i]*(Target[i]-Output[i]); //Find the error of single outnode
                   for (int j=0;j<HiddenNode;j++){
                       OutWeight[j][i]=OutWeight[j][i]+rate_OutWeight*TargetDif[i]*Output[i];
                   }
                   //alpha * s'(a(p,n)) * (t(p,n) - y(p,n)) * X(p,i,n).
               }
               /*
               * Adjust the HiddenWeight
               */
               for (int i=0;i<HiddenNode;i++){
                   HiddenAdjust[i]=0.0;
                   for (int j=0;j<OutNode;j++){
                       HiddenAdjust[i]=HiddenAdjust[i]+TargetDif[j]*OutWeight[i][j];
                   }
                   HiddenAdjust[i]=(1-Hidden[i])*Hidden[i]*HiddenAdjust[i];
                   for (int j=0;j<InNode;j++){
                       HiddenWeight[j][i]=HiddenWeight[j][i]+rate_HiddenWeight[i]*Input[j];
                   }
                   //alpha * s'(a(p,n)) * sum(d(j) * W(n,j)) * X(p,i,n)
               }
               for (int i=0;i<OutNode;i++){
                   error=fabs(Target[i]-Output[i])*fabs(Target[i]-Output[i])
               }
               /*
               * Adjust the Threshholds of hiddennodes and outnodes
               */
               MaxError=error/2.0;
               for (int i=0;i<HiddenNode;i++){
                   HiddenThresh[i]=HiddenThresh[i]+rate_HiddenThresh[i]*HiddenAdjust[i];
               }
               for (int i=0;i<OutNode;i++){
                   OutThresh[i]=OutThresh[i]+rate_OutThresh[i]*TargetDif[i];
               }
           }
      }
};

int main(){
    BPNet BP;
    BP.init();
    system("PAUSE");
    return 0;
} 
