//繍眉了屈序崙方廬葎匯了噴序崙方

#include<iostream>
#include<cmath>
using namespace std;

#define  innode 4  //補秘潤泣方
#define  hidenode 10//咨根潤泣方
#define  outnode 1 //補竃潤泣方
#define  trainsample 81//BP儺膳劔云方
class BpNet
{
public:
    void train(double p[trainsample][innode ],double t[trainsample][outnode]);//Bp儺膳
    double p[trainsample][innode];     //補秘議劔云
    double t[trainsample][outnode];    //劔云勣補竃議

    double *recognize(double *p);//Bp紛艶

    void writetrain(); //亟儺膳頼議幡峙
    void readtrain(); //響儺膳挫議幡峙宸聞議音喘耽肝肇儺膳阻峪勣委儺膳恷挫議幡峙贋和栖祥OK

    BpNet();
    virtual ~BpNet();

public:
    void init();
    double w[innode][hidenode];//咨根潤泣幡峙
    double w1[hidenode][outnode];//補竃潤泣幡峙
    double b1[hidenode];//咨根潤泣熊峙
    double b2[outnode];//補竃潤泣熊峙

    double rate_w; //幡峙僥楼楕補秘蚊-咨根蚊)
    double rate_w1;//幡峙僥楼楕 (咨根蚊-補竃蚊)
    double rate_b1;//咨根蚊熊峙僥楼楕
    double rate_b2;//補竃蚊熊峙僥楼楕

    double e;//列餓柴麻
    double error;//塋俯議恷寄列餓
    double result[outnode];// Bp補竃
};

BpNet::BpNet()
{
    error=1.0;
    e=0.0;

    rate_w=0.9;  //幡峙僥楼楕補秘蚊--咨根蚊)
    rate_w1=0.9; //幡峙僥楼楕 (咨根蚊--補竃蚊)
    rate_b1=0.9; //咨根蚊熊峙僥楼楕
    rate_b2=0.9; //補竃蚊熊峙僥楼楕
}

BpNet::~BpNet()
{

}

void winit(double w[],int n) //幡峙兜兵晒
{
  for(int i=0;i<n;i++)
    w[i]=(2.0*(double)rand()/RAND_MAX)-1;
}

void BpNet::init()
{
    winit((double*)w,innode*hidenode);
    winit((double*)w1,hidenode*outnode);
    winit(b1,hidenode);
    winit(b2,outnode);
}

void BpNet::train(double p[trainsample][innode],double t[trainsample][outnode])
{
    double pp[hidenode];//咨根潤泣議丕屎列餓
    double qq[outnode];//錬李補竃峙嚥糞縞補竃峙議陶餓
    double yd[outnode];//錬李補竃峙

    double x[innode]; //補秘鯀
    double x1[hidenode];//咨根潤泣彜蓑峙
    double x2[outnode];//補竃潤泣彜蓑峙
    double o1[hidenode];//咨根蚊爾試峙
    double o2[hidenode];//補竃蚊爾試峙

    for(int isamp=0;isamp<trainsample;isamp++)//儉桟儺膳匯肝劔瞳
    {
        for(int i=0;i<innode;i++){
            x[i]=0;
            x[i]=p[isamp][i]; //補秘議劔云
        }
        for(int i=0;i<outnode;i++){
            yd[i]=0;
            yd[i]=t[isamp][i]; //豚李補竃議劔云
        }

        //更夛耽倖劔瞳議補秘才補竃炎彈
        for(int j=0;j<hidenode;j++)
        {
            o1[j]=0.0;
            for(int i=0;i<innode;i++)
                o1[j]=o1[j]+w[i][j]*x[i];//咨根蚊光汽圷補秘爾試峙
            x1[j]=1.0/(1+exp(-o1[j]-b1[j]));//咨根蚊光汽圷議補竃
            //    if(o1[j]+b1[j]>0) x1[j]=1;
            //else x1[j]=0;
        }

        for(int k=0;k<outnode;k++)
        {
            o2[k]=0.0;
            for(int j=0;j<hidenode;j++)
                o2[k]=o2[k]+w1[j][k]*x1[j]; //補竃蚊光汽圷補秘爾試峙
            x2[k]=1.0/(1.0+exp(-o2[k]-b2[k])); //補竃蚊光汽圷補竃
            //    if(o2[k]+b2[k]>0) x2[k]=1;
            //    else x2[k]=0;
        }

        for(int k=0;k<outnode;k++)
        {
            qq[k]=(yd[k]-x2[k])*x2[k]*(1-x2[k]); //錬李補竃嚥糞縞補竃議陶餓
            for(int j=0;j<hidenode;j++)
                w1[j][k]+=rate_w1*qq[k]*x1[j];  //和匯肝議咨根蚊才補竃蚊岻寂議仟銭俊幡
        }

        for(int j=0;j<hidenode;j++)
        {
            pp[j]=0.0;
            for(int k=0;k<outnode;k++)
                pp[j]=pp[j]+qq[k]*w1[j][k];
            pp[j]=pp[j]*x1[j]*(1-x1[j]); //咨根蚊議丕屎列餓

            for(int i=0;i<innode;i++)
                w[i][j]+=rate_w*pp[j]*x[i]; //和匯肝議補秘蚊才咨根蚊岻寂議仟銭俊幡
        }

        for(int k=0;k<outnode;k++)
        {
            e+=fabs(yd[k]-x2[k])*fabs(yd[k]-x2[k]); //柴麻譲圭餓
        }
        error=e/2.0;

        for(int k=0;k<outnode;k++)
            b2[k]=b2[k]+rate_b2*qq[k]; //和匯肝議咨根蚊才補竃蚊岻寂議仟穉峙
        for(int j=0;j<hidenode;j++)
            b1[j]=b1[j]+rate_b1*pp[j]; //和匯肝議補秘蚊才咨根蚊岻寂議仟穉峙
    }
}

double *BpNet::recognize(double *p)
{
    double x[innode]; //補秘鯀
    double x1[hidenode]; //咨根潤泣彜蓑峙
    double x2[outnode]; //補竃潤泣彜蓑峙
    double o1[hidenode]; //咨根蚊爾試峙
    double o2[hidenode]; //補竃蚊爾試峙

    for(int i=0;i<innode;i++)
        x[i]=p[i];

    for(int j=0;j<hidenode;j++)
    {
        o1[j]=0.0;
        for(int i=0;i<innode;i++)
            o1[j]=o1[j]+w[i][j]*x[i]; //咨根蚊光汽圷爾試峙
        x1[j]=1.0/(1.0+exp(-o1[j]-b1[j])); //咨根蚊光汽圷補竃
        //if(o1[j]+b1[j]>0) x1[j]=1;
        //    else x1[j]=0;
    }

    for(int k=0;k<outnode;k++)
    {
        o2[k]=0.0;
        for(int j=0;j<hidenode;j++)
            o2[k]=o2[k]+w1[j][k]*x1[j];//補竃蚊光汽圷爾試峙
        x2[k]=1.0/(1.0+exp(-o2[k]-b2[k]));//補竃蚊光汽圷補竃
        //if(o2[k]+b2[k]>0) x2[k]=1;
        //else x2[k]=0;
    }

    for(int k=0;k<outnode;k++)
    {
        result[k]=x2[k];
    }
    return result;
}

void BpNet::writetrain()
{
    FILE *stream0;
    FILE *stream1;
    FILE *stream2;
    FILE *stream3;
    int i,j;
    //咨根潤泣幡峙亟秘
    if(( stream0 = fopen("w.txt", "w+" ))==NULL)
    {
        cout<<"幹秀猟周払移!";
        exit(1);
    }
    for(i=0;i<innode;i++)
    {
        for(j=0;j<hidenode;j++)
        {
            fprintf(stream0, "%f\n", w[i][j]);
        }
    }
    fclose(stream0);

    //補竃潤泣幡峙亟秘
    if(( stream1 = fopen("w1.txt", "w+" ))==NULL)
    {
        cout<<"幹秀猟周払移!";
        exit(1);
    }
    for(i=0;i<hidenode;i++)
    {
        for(j=0;j<outnode;j++)
        {
            fprintf(stream1, "%f\n",w1[i][j]);
        }
    }
    fclose(stream1);

    //咨根潤泣熊峙亟秘
    if(( stream2 = fopen("b1.txt", "w+" ))==NULL)
    {
        cout<<"幹秀猟周払移!";
        exit(1);
    }
    for(i=0;i<hidenode;i++)
        fprintf(stream2, "%f\n",b1[i]);
    fclose(stream2);

    //補竃潤泣熊峙亟秘
    if(( stream3 = fopen("b2.txt", "w+" ))==NULL)
    {
        cout<<"幹秀猟周払移!";
        exit(1);
    }
    for(i=0;i<outnode;i++)
        fprintf(stream3, "%f\n",b2[i]);
    fclose(stream3);

}

void BpNet::readtrain()
{
    FILE *stream0;
    FILE *stream1;
    FILE *stream2;
    FILE *stream3;
    int i,j;

    //咨根潤泣幡峙響竃
    if(( stream0 = fopen("w.txt", "r" ))==NULL)
    {
        cout<<"嬉蝕猟周払移!";
        exit(1);
    }
    float  wx[innode][hidenode];
    for(i=0;i<innode;i++)
    {
        for(j=0;j<hidenode;j++)
        {
            fscanf(stream0, "%f", &wx[i][j]);
            w[i][j]=wx[i][j];
        }
    }
    fclose(stream0);

    //補竃潤泣幡峙響竃
    if(( stream1 = fopen("w1.txt", "r" ))==NULL)
    {
        cout<<"嬉蝕猟周払移!";
        exit(1);
    }
    float  wx1[hidenode][outnode];
    for(i=0;i<hidenode;i++)
    {
        for(j=0;j<outnode;j++)
        {
            fscanf(stream1, "%f", &wx1[i][j]);
            w1[i][j]=wx1[i][j];
        }
    }
    fclose(stream1);

    //咨根潤泣熊峙響竃
    if(( stream2 = fopen("b1.txt", "r" ))==NULL)
    {
        cout<<"嬉蝕猟周払移!";
        exit(1);
    }
    float xb1[hidenode];
    for(i=0;i<hidenode;i++)
    {
        fscanf(stream2, "%f",&xb1[i]);
        b1[i]=xb1[i];
    }
    fclose(stream2);

    //補竃潤泣熊峙響竃
    if(( stream3 = fopen("b2.txt", "r" ))==NULL)
    {
        cout<<"嬉蝕猟周払移!";
        exit(1);
    }
    float xb2[outnode];
    for(i=0;i<outnode;i++)
    {
        fscanf(stream3, "%f",&xb2[i]);
        b2[i]=xb2[i];
    }
    fclose(stream3);
}


//補秘劔云
double X[trainsample][innode];
//豚李補竃劔云
double Y[trainsample][outnode];

int main()
{

    BpNet bp;
    bp.init();
    for (int i=0;i<trainsample;i++){
        Y[i][0]=(double)i*(1.0/80.0);
        //cout<<Y[i][0]<<endl;
    }
    int count=0;
    for (int a=0;a<3;a++){
            for (int b=0;b<3;b++){
                for (int c=0;c<3;c++){
                    for (int d=0;d<3;d++){
                    X[count][0]=0.0;
                    X[count][0]=a;
                    //cout<<X[count][0]<<" ";
                    X[count][1]=0.0;
                    X[count][1]=b;
                    //cout<<X[count][1]<<" ";
                    X[count][2]=0.0;
                    X[count][2]=c;
                    //cout<<X[count][2]<<" "<<endl;
                    X[count][3]=0.0;
                    X[count][3]=d;
                    count++;
                    }
                }
            }
    }
    for(int i=0;i<trainsample;i++){
            for(int j=0;j<innode;j++){
                    cout<<X[i][j]<<" ";
            }
            cout<<endl;
    }
    system("PAUSE");
    int times=0;
    while(bp.error>0.0001)
    {
        bp.e=0.0;
        times++;
        bp.train(X,Y);
        cout<<"Times="<<times<<" error="<<bp.error<<endl;
    }
    cout<<"trainning complete..."<<endl;
    double m[innode]={1,0,0,1};
    double *r=bp.recognize(m);
    for(int i=0;i<outnode;++i)
       cout<<bp.result[i]<<" ";
    double cha[trainsample][outnode];
    double mi=100;
    double index;
    for(int i=0;i<trainsample;i++)
    {
        for(int j=0;j<outnode;j++)
        {
            //孀餓峙恷弌議椎倖劔云
            cha[i][j]=(double)(fabs(Y[i][j]-bp.result[j]));
            if(cha[i][j]<mi)
            {
                mi=cha[i][j];
                index=i;
            }
        }
    }
    for(int i=0;i<innode;++i)
       cout<<m[i];
    cout<<" is "<<index<<endl;
    cout<<endl;
    system("PAUSE");
    return 0;
}
