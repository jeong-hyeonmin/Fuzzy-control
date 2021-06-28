#include <stdio.h>
#include <time.h>


#define NB -1
#define NM -0.666
#define NS -0.333
#define ZO 0
#define PS 0.333
#define PM 0.666
#define PB 1



#define dt 0.05;    //50ms

//error dot is [pre_error - now error]


/*
const double Fuzzy_Rule[7][7]= {{NB,NB,NB,NM,NM,NS,0},  //error NB
                                {NB,NM,NM,NM,NS,0,PS},  //error NM
                                {NB,NM,NS,NS,ZO,PS,PM}, //error NS
                                {NM,NS,NS,ZO,PS,PS,PM}, //error 0
                                {NM,NS,ZO,PS,PS,PM,PB}, //error PS
                                {NS,0,PS,PM,PM,PM,PB},  //error PM
                                {PB,PS,PM,PB,PB,PB,PB}};//error PB
*/

                            //  ed NB NM NS 0  PS PM PB
const double Fuzzy_Rule[7][7]= {{0,0,0,NB,0,0,0},  //error NB
                                {0,0,0,NM,0,0,0},  //error NM
                                {0,0,0,NS,0,0,0}, //error NS
                                {PM,PS,0,0,0,NS,NM}, //error 0
                                {0,0,PS,PS,0,0,0}, //error PS
                                {0,0,0,PM,0,0,0},  //error PM
                                {0,0,0,PB,0,0,0}};//error PB



// NB ~ NM
static double match_rule11[]={0,-1/(NM-NB),NM/(NM-NB)};
static double match_rule12[]={1,1/(NM-NB),-NB/(NM-NB)};

//NM ~ NS
static double match_rule21[]={1,-1/(NS-NM),NS/(NS-NM)};
static double match_rule22[]={2,1/(NS-NM),-NS/(NS-NM)};

//NS ~ ZO
static double match_rule31[]={2,1/NS,0};
static double match_rule32[]={3,-1/NS,1};

//ZO ~ PS
static double match_rule41[]={3,-1/PS,1};
static double match_rule42[]={4,1/PS,0};

//PS ~ PM
static double match_rule51[]={4,-1/(PM-PS),PM/(PM-PS)};
static double match_rule52[]={5,1/(PM-PS),-PS/(PM-PS)};

//PM~PB
static double match_rule61[]={5,-1/(PB-PM),PB/(PB-PM)};
static double match_rule62[]={6,1/(PB-PM),-PM/(PB-PM)};

//input max min-----------------------------------------------
double X_max=5;
double X_min =0;

double Y_max=5;
double Y_min =0;

double theta_max = 90;
double theta_min = -90;
//-----------------------------------------------------------


//output max min---------------------------------------------
double V_x_max = 20;        //  cm/s
double V_x_min = 0;         //  cm/s
double V_y_max = 20;        //  cm/s
double V_y_min = 0;
double W_max=20;            //  rad/s *10
double W_min=0;
//-----------------------------------------------------------


//input error, input dot error------------------------------
double X_error;
double X_delta_error;
double Y_error;
double Y_delta_error;
double theta_error;
double theta_delta_error;
//-----------------------------------------------------------

double x_now=0;
double y_now =0;
double theta_now=0;


//fuzzy, fuzzy match
double first_Fuzzy_High[2]={0,};
double first_Fuzzy_Low[2]={0,};
double second_Fuzzy_High[2]={0,};
double second_Fuzzy_Low[2]={0,};


void fuzzy_first_check(double error);
void fuzzy_second_check(double error_dot);

double U_delta();



int main(int argc, char *argv[])
{

    clock_t start, end;
    double result;
    start = clock(); // 수행 시간 측정 시작 /* 수행시간 측정하고자 하는 코드 */




    double x_goal = 0;
    double y_goal =0;
    double theta_goal = 0;


   int x_rule=0;
   int y_rule =0;
   int theta_rule=0;


    fuzzy_first_check(2); //error
    fuzzy_second_check(2);  //error dot
    double test1 = U_delta();
    printf("------------------test1 = %f----------------------------- \n",test1);

    /*
    fuzzy_first_check(0.4); //error
    fuzzy_second_check(0.5);  //error dot
    double test2 = U_delta();
    printf("------------------test2 = %f----------------------------- \n",test2);

    fuzzy_first_check(0.7); //error
    fuzzy_second_check(0.5);  //error dot
    double test3 = U_delta();
    printf("------------------test3 = %f----------------------------- \n",test3);

*/



    end = clock(); //시간 측정 끝
    result = (double)(end - start); // 결과 출력
    printf("%f", result / CLOCKS_PER_SEC);

    return 0;
}


double U_delta()
{
    double f=0;
    double w=0;

    double Fuzzy[4]={0,};
    double match[4] ={0,};

    Fuzzy[0] = Fuzzy_Rule[(int)first_Fuzzy_High[0]][(int)second_Fuzzy_High[0]];
    Fuzzy[1] = Fuzzy_Rule[(int)first_Fuzzy_High[0]][(int)second_Fuzzy_Low[0]];
    Fuzzy[2] = Fuzzy_Rule[(int)first_Fuzzy_Low[0]][(int)second_Fuzzy_High[0]];
    Fuzzy[3] = Fuzzy_Rule[(int)first_Fuzzy_Low[0]][(int)second_Fuzzy_Low[0]];

    match[0] = first_Fuzzy_High[1]*second_Fuzzy_High[1];
    match[1] = first_Fuzzy_High[1]*second_Fuzzy_Low[1];
    match[2] = first_Fuzzy_Low[1]*second_Fuzzy_High[1];
    match[3] = first_Fuzzy_Low[1]*second_Fuzzy_Low[1];






    for(int i=0; i<4; i++)
    {
        printf("Fuzzy[%d] = %f, match = %f\n", i, Fuzzy[i],match[i]);

    }


    for(int i=0; i<4; i++)
    {
        w += match[i];
        f += Fuzzy[i]*match[i];
    }

    printf("f : %f, w : %f\n",f,w);

    double out = (double)(f/w);
    return out;

}


void fuzzy_first_check(double error)
{
    if(error < NB)
    {
        printf("error<NB\n");
        first_Fuzzy_High[0] = match_rule11[0];
        first_Fuzzy_High[1] = 1.0;
        first_Fuzzy_Low[0] = match_rule12[0];
        first_Fuzzy_Low[1] = 0;
    }
    else if(error >= NB && error < NM)
    {
        printf("NB<=error<NM\n");
        first_Fuzzy_High[0] = match_rule11[0];
        first_Fuzzy_High[1] = match_rule11[1]*error+match_rule11[2];
        first_Fuzzy_Low[0] = match_rule12[0];
        first_Fuzzy_Low[1] = match_rule12[1]*error+match_rule12[2];
    }
    else if(error >= NM && error < NS)
    {
        printf("NM<=error<NS\n");

        first_Fuzzy_High[0] = match_rule21[0];
        first_Fuzzy_High[1] = match_rule21[1]*error+match_rule21[2];
        first_Fuzzy_Low[0] = match_rule22[0];
        first_Fuzzy_Low[1] = match_rule22[1]*error+match_rule22[2];
    }
    else if(error >= NS && error < ZO)
    {
        printf("NS<=error<ZO\n");

        first_Fuzzy_High[0] = match_rule31[0];
        first_Fuzzy_High[1] = match_rule31[1]*error+match_rule31[2];
        first_Fuzzy_Low[0] = match_rule32[0];
        first_Fuzzy_Low[1] = match_rule32[1]*error+match_rule32[2];
    }
    else if(error >= ZO && error < PS)
    {
        printf("ZO<=error<PS\n");

        first_Fuzzy_High[0] = match_rule41[0];
        first_Fuzzy_High[1] = match_rule41[1]*error+match_rule41[2];
        first_Fuzzy_Low[0] = match_rule42[0];
        first_Fuzzy_Low[1] = match_rule42[1]*error+match_rule42[2];
    }
    else if(error >= PS && error < PM)
    {
        printf("PS<=error<PM\n");

        first_Fuzzy_High[0] = match_rule51[0];
        first_Fuzzy_High[1] = match_rule51[1]*error+match_rule51[2];
        first_Fuzzy_Low[0] = match_rule52[0];
        first_Fuzzy_Low[1] = match_rule52[1]*error+match_rule52[2];
    }
    else if(error >= PM && error < PB)
    {
        printf("PM<=error<PB\n");

        first_Fuzzy_High[0] = match_rule61[0];
        first_Fuzzy_High[1] = match_rule61[1]*error+match_rule61[2];
        first_Fuzzy_Low[0] = match_rule62[0];
        first_Fuzzy_Low[1] = match_rule62[1]*error+match_rule62[2];
    }
    else if(error >= PB)
    {
        printf("PB<=error\n");

        first_Fuzzy_High[0] = match_rule61[0];
        first_Fuzzy_High[1] = 0;
        first_Fuzzy_Low[0] = match_rule62[0];
        first_Fuzzy_Low[1] = 1;
    }

    printf("first_Fuzzy_High match rule value : %f\n",first_Fuzzy_High[0]);
    printf("first_Fuzzy_High match rule fitness : %f\n\n",first_Fuzzy_High[1]);
    printf("first_Fuzzy_low match rule value : %f\n",first_Fuzzy_Low[0]);
    printf("first_Fuzzy_low match rule fitness : %f\n\n",first_Fuzzy_Low[1]);

}

void fuzzy_second_check(double error_dot)
{
    if(error_dot < NB)
    {
        printf("error<NB\n");

        second_Fuzzy_High[0] = match_rule11[0];
        second_Fuzzy_High[1] = 1.0;
        second_Fuzzy_Low[0] = match_rule12[0];
        second_Fuzzy_Low[1] = 0;
    }
    else if(error_dot >= NB && error_dot < NM)
    {
        printf("NB<=error<NM\n");

        second_Fuzzy_High[0] = match_rule11[0];
        second_Fuzzy_High[1] = match_rule11[1]*error_dot+match_rule11[2];
        second_Fuzzy_Low[0] = match_rule12[0];
        second_Fuzzy_Low[1] = match_rule12[1]*error_dot+match_rule12[2];
    }
    else if(error_dot >= NM && error_dot < NS)
    {
        printf("NM<=error<NS\n");

        second_Fuzzy_High[0] = match_rule21[0];
        second_Fuzzy_High[1] = match_rule21[1]*error_dot+match_rule21[2];
        second_Fuzzy_Low[0] = match_rule22[0];
        second_Fuzzy_Low[1] = match_rule22[1]*error_dot+match_rule22[2];
    }
    else if(error_dot >= NS && error_dot < ZO)
    {
        printf("NS<=error<ZO\n");

        second_Fuzzy_High[0] = match_rule31[0];
        second_Fuzzy_High[1] = match_rule31[1]*error_dot+match_rule31[2];
        second_Fuzzy_Low[0] = match_rule32[0];
        second_Fuzzy_Low[1] = match_rule32[1]*error_dot+match_rule32[2];
    }
    else if(error_dot >= ZO && error_dot < PS)
    {
        printf("ZO<=error<PS\n");

        second_Fuzzy_High[0] = match_rule41[0];
        second_Fuzzy_High[1] = match_rule41[1]*error_dot+match_rule41[2];
        second_Fuzzy_Low[0] = match_rule42[0];
        second_Fuzzy_Low[1] = match_rule42[1]*error_dot+match_rule42[2];
    }
    else if(error_dot >= PS && error_dot < PM)
    {
        printf("PS<=error<PM\n");

        second_Fuzzy_High[0] = match_rule51[0];
        second_Fuzzy_High[1] = match_rule51[1]*error_dot+match_rule51[2];
        second_Fuzzy_Low[0] = match_rule52[0];
        second_Fuzzy_Low[1] = match_rule52[1]*error_dot+match_rule52[2];
    }
    else if(error_dot >= PM && error_dot < PB)
    {
        printf("PM<=error<PB\n");

        second_Fuzzy_High[0] = match_rule61[0];
        second_Fuzzy_High[1] = match_rule61[1]*error_dot+match_rule61[2];
        second_Fuzzy_Low[0] = match_rule62[0];
        second_Fuzzy_Low[1] = match_rule62[1]*error_dot+match_rule62[2];
    }
    else if(error_dot >= PB)
    {
        printf("PB<=error\n");

        second_Fuzzy_High[0] = match_rule61[0];
        second_Fuzzy_High[1] = 0;
        second_Fuzzy_Low[0] = match_rule62[0];
        second_Fuzzy_Low[1] = 1;
    }



    printf("second_Fuzzy_High match rule value : %f\n",second_Fuzzy_High[0]);
    printf("second_Fuzzy_High match rule fitness : %f\n\n",second_Fuzzy_High[1]);
    printf("second_Fuzzy_low match rule value : %f\n",second_Fuzzy_Low[0]);
    printf("second_Fuzzy_low match rule fitness : %f\n\n",second_Fuzzy_Low[1]);

}

