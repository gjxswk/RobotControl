
//#ifndef __Kine6Dof
//#define __Kine6Dof

#ifdef Dll_PPDll
#else
#define Dll_PPDll _declspec(dllimport)
#endif

#define PI 3.14159265358979324
#define   t0    0.1
#define   Ni    4000
// 全局变量
Dll_PPDll int CaptureCommand;
Dll_PPDll double ds1;  // 自主规划预捕获位置与目标位置的距离设置
Dll_PPDll int Collinformation[2]; 

//Dll_PPDll double R0c[3][3];/*变换矩阵*/
//Dll_PPDll double rca0[3];    
//Dll_PPDll double T0C[4][4];

Dll_PPDll int EndCollisionFlag;
Dll_PPDll int collflag;
Dll_PPDll int Capavoidbutton;
Dll_PPDll int collflag_jxb;
/************************************初始参数设置*********************************/
/////////////杆件基本尺寸//////////////////
//Dll_PPDll double d1,d2,d3,d4,d5,d6; 
/////////////连杆坐标系变换参数////////////
//Dll_PPDll double dhint[8][4];



/***************************************************************************************************/
/*                                            规 划 函 数                                          */
/***************************************************************************************************/
Dll_PPDll void JointMotionPlan(double Joint_now[8], double BasPE_now[6], double Joint_ini[8], double Joint_des[8], double plantime, double acctime, int plan_step, int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);
Dll_PPDll void ArcMotionplan(double Joint_now[8], double BasPE_now[6], double EndPE_ini[6], double EndPE_mid[6], double EndPE_des[6], double plantime, double acctime, int plan_step, unsigned int allarcflag, int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);
Dll_PPDll void ArcMotionplan2(double Joint_now[8], double BasPE_now[6], double EndPE_ini[6], double EndPE_mid[6], double EndPE_des[6], double plantime, double acctime, int plan_step, unsigned int allarcflag, int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);
Dll_PPDll void LinearMotionplan(double Joint_now[8], double BasPE_now[6], double EndPE_ini[6], double EndPE_des[6], double plantime, double acctime, int plan_step, int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);
Dll_PPDll void ConCurveMotionPlan(double Joint_now[8], double BasPE_now[6], double EndPE_ini[6], double EndPE_mid1[6], double EndPE_mid2[6], double EndPE_mid3[6], double EndPE_des[6], double plantime, double acctime, int plan_step, int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);  
Dll_PPDll int AutoMotionPlan_line(double Joint_now[8], double BasPE_now[6], double EndPE_des[6], double wcm[3], double SpeedLimit[6], int CapOrientFlag, int plan_step, int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);
Dll_PPDll int AutoMotionPlan_cur(double Joint_now[8], double BasPE_now[6], double EndPE_des[6], double wcm[3], double SpeedLimit[6], int CapOrientFlag, int plan_step, int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);
Dll_PPDll void HandleOperMotionPlan(double Joint_now[8], double BasPE_now[6], unsigned char ColstatusFlag, double Velvalue,  int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);
Dll_PPDll void HandleOperMotion(double Joint_now[8], double BasPE_now[6], double VWe[6], int PlanMode, double BasPE_next[6], double Base_speed[6], double Joint_next[8], double Joint_speed[8]);    



/********************************************************************************************/
/*                                         运动学函数                                       */
/********************************************************************************************/
Dll_PPDll void Forwardkine_ground( double qm[8], double T0e[4][4]);          ///除基座机器人本身正解
Dll_PPDll void Forwardkine_static(double qm[8], double PE[6]);   //固定基座运动学正解
Dll_PPDll void Forwardkine_float( double PEca[6], double qm[8], double PE[6]);  //自由漂浮运动学正解



/********************************************************************************************/
/*                                         规划函数子函数                                   */
/********************************************************************************************/
Dll_PPDll double FisDConAcc(double t, double dist, double tf, double ts);   //加速度连续变化的位移曲线
Dll_PPDll void DrawArcPath(double Joint_now[7], double BasPE_now[6], double EndPE_ini[6], double EndPE_mid[6], double EndPE_des[6], double plantime, double acctime,int plan_setp,  unsigned int allarcflag, double ArcPathP[3]);
Dll_PPDll double SolveAngle(double p1[2], double p2[2], double p3[2], double Rad);
Dll_PPDll void TriAngleCharacter(double p1[2], double p2[2], double p3[2], double p0[2], int Character[3]);
Dll_PPDll double space_circle(double pp[3][3], double p0[3], double R[3][3]);
Dll_PPDll void InterPolnomial(double ti[4], double cord[5], double coe[7]);
Dll_PPDll void DrawConcurPath(double qm[7], double PEcaa[6], double PEint[6], double PEmid1[6], double PEmid2[6],double PEmid3[6],double PEend[6], double plantime, double ts, int plan_setp, double ConcurPathP[3]);
Dll_PPDll void Eulplan(double Rk1[3][3], double Rk2[3][3], double we[3]);  



/********************************************************************************************/
/*                                         雅克比矩阵求解                                   */
/********************************************************************************************/
Dll_PPDll void JGJN_Static(double qm[8], double Jm[6][8]);



/********************************************************************************************/
/*                                          矩阵计算                                        */
/********************************************************************************************/
Dll_PPDll void Transpose(int m, int n, double *a, double *b );  //矩阵转置
Dll_PPDll void RbtMulMtrx( int m, int n, int p, double *A, double *B, double *C );  //矩阵相乘
Dll_PPDll int RbtInvMtrx( double *C, double *IC, int n );   //矩阵求逆  
Dll_PPDll void PseudoInverse(double J[6][8], double PJ[8][6]);
Dll_PPDll void Equal(double R1[3][3], double R2[3][3]);  //相等矩阵
Dll_PPDll double Det(double a[3][3]);        //求行列式
Dll_PPDll double SDet(double *a, int n);   //求行列式符号位



/********************************************************************************************/
/*                                         基本函数、公共调用                               */
/********************************************************************************************/
Dll_PPDll void R_EulerZyx(double dTr[3][3],double dEulerZyx[3]);    //旋转矩阵转欧拉角
Dll_PPDll void EulerZyx_R( double oula[3],double a[3][3]);              //欧拉角转旋转矩阵
Dll_PPDll void T2PE(double T[4][4], double PE[6]);                      //T矩阵转末端位姿
Dll_PPDll void PE2T(double PE[6], double T[4][4]);                      //末端位姿转T矩阵
Dll_PPDll void RP_T(double P[3], double R[3][3], double T[4][4]);   //旋转矩阵和P向量转T矩阵
Dll_PPDll double  Axangle(double qint[3], double qend[3], double q[3]);   //由初始终止旋转矩阵求指向偏差
Dll_PPDll void DH2T(double dh[4], double T[4][4]);    //由DH参数求T矩阵
Dll_PPDll void Vec2DSymMtx(double vec[3], double R[3][3]);   //向量变反对称矩阵
Dll_PPDll void DHMof(double qm[8], double dh[8][4]);    //由关节角赋值DH参数
Dll_PPDll void Calc_T(double qm[8], double T[32][4]);     // 计算T矩阵
Dll_PPDll void Thpater_T(double qm[8], double T01[4][4], double T12[4][4], double T23[4][4], double T34[4][4], double T45[4][4], double T56[4][4], double T67[4][4], double T78[4][4]);
Dll_PPDll void w2Elzyxp(double Eul[3], double R[3][3]);   //角速度转欧拉角速度
Dll_PPDll void RR(double cetatemp, double R[3][3]);   //绕Z轴旋转



/********************************************************************************************/
/*                                          基础数学计算                                    */
/********************************************************************************************/
Dll_PPDll double Dotm(double a[3], double b[3]);
Dll_PPDll void Crossm(double a[], double b[], double c[]);           //X乘
Dll_PPDll int Sign(double a);                         //判定符号位
Dll_PPDll double Fmax(double a, double b);            //求最大值
Dll_PPDll double Fmin(double a, double b);            //求最小值
Dll_PPDll double Fmax3(double a, double b, double c);  //两数取大数
Dll_PPDll double Nnorm1(double a, double b, double c);
Dll_PPDll double Nnorm2(double a[3]);
Dll_PPDll double Norm(double a[3], double b[3]);



/********************************************************************************************/
/*                                               其他                                       */
/********************************************************************************************/
/// 初始化参数函数
Dll_PPDll void init_robot_size(double size[6]);//初始化机械臂基本尺寸
Dll_PPDll void init_dh(double dh[8][4]);//初始化DH参数
