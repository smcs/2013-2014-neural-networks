#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;

#define InNode 4
#define HiddenNode 10
#define OutNode 1
#define SampleSize 256


double Data[SampleSize][InNode];
double DataTarget[SampleSize][OutNode];
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
               HiddenThresh[i]=(2.0*(double)rand()/RAND_MAX)-1;
           }
           for (int i=0;i<OutNode;i++){
               OutThresh[i]=(2.0*(double)rand()/RAND_MAX)-1;
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
                   Input[i]=0;
                   Input[i] = Data[Sample][i]; 
               }
               for (int i=0;i<OutNode;i++){
                   Target[i]=0;
                   Target[i] = DataTarget[Sample][i];
               }
               for (int i=0;i<HiddenNode;i++){
                   HiddenAct[i] = 0.0;
                   for (int j=0;j<InNode;j++){
                       HiddenAct[i]=HiddenAct[i]+HiddenWeight[j][i]*Input[j]; //Get the activation value for the hidden nodes
                   }
                   Hidden[i]=(double)1.0/(1.0+exp(-HiddenThresh[i]-HiddenAct[i]));//Sigmoid function for Hidden output
               }
               for (int i=0;i<OutNode;i++){
                   OutAct[i]=0.0;
                   for (int j=0;j<HiddenNode;j++){
                       OutAct[i]=OutAct[i]+OutWeight[j][i]*Hidden[j]; //Get the activation value for the output nodes
                   }
                   Output[i]=(double)1.0/(1.0+exp(-OutThresh[i]-OutAct[i])); //Sigmoid function for Output 
               }
               /*
               * Adjust the OutWeight
               */
               for (int i=0;i<OutNode;i++){
                   TargetDif[i]=(1-Output[i])*Output[i]*(Target[i]-Output[i]); //Find the error of single outnode
                   for (int j=0;j<HiddenNode;j++){
                       OutWeight[j][i]=OutWeight[j][i]+rate_OutWeight*TargetDif[i]*Hidden[i];
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
                       HiddenWeight[j][i]=HiddenWeight[j][i]+rate_HiddenWeight*HiddenAdjust[i]*Input[j];
                   }
                   //alpha * s'(a(p,n)) * sum(d(j) * W(n,j)) * X(p,i,n)
               }
               for (int i=0;i<OutNode;i++){
                   error=error+fabs(Target[i]-Output[i])*fabs(Target[i]-Output[i]);
               }
               /*
               * Adjust the Threshholds of hiddennodes and outnodes
               */
               MaxError=error/2.0;
               for (int i=0;i<HiddenNode;i++){
                   HiddenThresh[i]=HiddenThresh[i]+rate_HiddenThresh*HiddenAdjust[i];
               }
               for (int i=0;i<OutNode;i++){
                   OutThresh[i]=OutThresh[i]+rate_OutThresh*TargetDif[i];
                   //cout<<OutThresh[i];
               }
           }
      }
      void recognize(double *data){
             double Input[InNode];
             double Hidden[HiddenNode];
             double Output[OutNode];
             double actHidden[HiddenNode];
             double actOut[OutNode];
             for (int i=0;i<InNode;i++){
                 Input[i]=data[i];
             }
             for (int i=0;i<HiddenNode;i++){
                 actHidden[i]=0.0;
                 for (int j=0;j<InNode;j++){
                     actHidden[i]=actHidden[i]+HiddenWeight[j][i]*Input[j];
                 } 
                 Hidden[i]=(double)1.0/(1.0+exp(-HiddenThresh[i]-actHidden[i]));
             }
             for (int i=0;i<OutNode;i++){
                 actOut[i]=0.0;
                 for (int j=0;j<HiddenNode;j++){
                     actOut[i]=actOut[i]+OutWeight[j][i]*Hidden[j];
                 }
                 Output[i]=(double)1.0/(1.0+exp(-OutThresh[i]-actOut[i]));
             }
             for (int i=0;i<OutNode;i++){
                 result[i]=Output[i];
             }
      } 
      ~BPNet(){
      }
};

int main(){
    ofstream myFile("OutputWeights.txt");
    BPNet BP;
    BP.init();
    int count=0;
    int ActualAnswer[SampleSize];
    double temp[4]={2,3,3,1};
    for (int a=0;a<4;a++){
        for (int b=0;b<4;b++){
            for (int c=0;c<4;c++){
                for (int d=0;d<4;d++){
                    Data[count][0]=a;
                    Data[count][1]=b;
                    Data[count][2]=c;
                    Data[count][3]=d;
                    ActualAnswer[count]=a*d-b*c;
                    cout<<ActualAnswer[count]<<endl;
                    count++;
                }
            }
        }
    }
    for (int i=0;i<SampleSize;i++){
        DataTarget[i][0]=(double)i*((double)1.0/((double)SampleSize-1.0));
    } 
    system("PAUSE");
    
    count=0;
    while(BP.MaxError>0.000085)
    {
        count++;
        BP.error=0.0;
        BP.train();
        cout<<count<<" "<<"error="<<BP.MaxError<<endl;
    }
    BP.recognize(temp);
    double min=10000;
    double diff[SampleSize][OutNode];
    int index=0;
    //cout<<BP.result[0]<<endl;
    for (int i=0;i<SampleSize;i++){
        for (int j=0;j<OutNode;j++){
            diff[i][j]=(double)fabs(DataTarget[i][j]-BP.result[j]);
            cout<<diff[i][j]<<endl;
            if (min>diff[i][j]){
                          min=diff[i][j];
                          index=i;
            }
        }
    }
    if (myFile.is_open()){
        myFile<<"Weights from input to hidden:"<<endl;
        for (int i=0;i<InNode;i++){
            for (int j=0;j<HiddenNode;j++){
                myFile<<BP.HiddenWeight[i][j]<<" ";
            }
            myFile<<endl;
        }
        myFile<<endl;
        myFile<<"Wights from hidden to output:"<<endl;
        for (int i=0;i<HiddenNode;i++){
            for (int j=0;j<OutNode;j++){
                myFile<<BP.OutWeight[i][j]<<" ";
            }
            myFile<<endl;
        }
        myFile<<endl<<"Input-output matching:"<<endl;
        myFile<<"Input: ";
        for (int i=0;i<SampleSize;i++){
            myFile<<"{";
            for (int j=0;j<InNode;j++){
                myFile<<Data[i][j];
                if (j!=InNode-1) myFile<<",";
            }
            myFile<<"} ";
        }
        myFile<<endl<<"Target: ";
        for (int i=0;i<SampleSize;i++){
            myFile<<DataTarget[i][0]<<" | ";
        }
        myFile<<endl;
    }
    cout<<index<<" "<<ActualAnswer[index]<<endl;
    system("PAUSE");
    return 0;
} 
