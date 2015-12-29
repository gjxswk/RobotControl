
//#ifndef __Kine6Dof
//#define __Kine6Dof

#ifdef Dll_PPDll
#else
#define Dll_PPDll _declspec(dllimport)
#endif

#define PI 3.14159265358979324
#define   t0    0.1
#define   Ni    4000
// ȫ�ֱ���
Dll_PPDll int CaptureCommand;
Dll_PPDll double ds1;  // �����滮Ԥ����λ����Ŀ��λ�õľ�������
Dll_PPDll int Collinformation[2]; 

//Dll_PPDll double R0c[3][3];/*�任����*/
//Dll_PPDll double rca0[3];    
//Dll_PPDll double T0C[4][4];

Dll_PPDll int EndCollisionFlag;
Dll_PPDll int collflag;
Dll_PPDll int Capavoidbutton;
Dll_PPDll int collflag_jxb;
/************************************��ʼ��������*********************************/
/////////////�˼������ߴ�//////////////////
//Dll_PPDll double d1,d2,d3,d4,d5,d6; 
/////////////��������ϵ�任����////////////
//Dll_PPDll double dhint[8][4];



/***************************************************************************************************/
/*                                            �� �� �� ��                                          */
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
/*                                         �˶�ѧ����                                       */
/********************************************************************************************/
Dll_PPDll void Forwardkine_ground( double qm[8], double T0e[4][4]);          ///�����������˱�������
Dll_PPDll void Forwardkine_static(double qm[8], double PE[6]);   //�̶������˶�ѧ����
Dll_PPDll void Forwardkine_float( double PEca[6], double qm[8], double PE[6]);  //����Ư���˶�ѧ����



/********************************************************************************************/
/*                                         �滮�����Ӻ���                                   */
/********************************************************************************************/
Dll_PPDll double FisDConAcc(double t, double dist, double tf, double ts);   //���ٶ������仯��λ������
Dll_PPDll void DrawArcPath(double Joint_now[7], double BasPE_now[6], double EndPE_ini[6], double EndPE_mid[6], double EndPE_des[6], double plantime, double acctime,int plan_setp,  unsigned int allarcflag, double ArcPathP[3]);
Dll_PPDll double SolveAngle(double p1[2], double p2[2], double p3[2], double Rad);
Dll_PPDll void TriAngleCharacter(double p1[2], double p2[2], double p3[2], double p0[2], int Character[3]);
Dll_PPDll double space_circle(double pp[3][3], double p0[3], double R[3][3]);
Dll_PPDll void InterPolnomial(double ti[4], double cord[5], double coe[7]);
Dll_PPDll void DrawConcurPath(double qm[7], double PEcaa[6], double PEint[6], double PEmid1[6], double PEmid2[6],double PEmid3[6],double PEend[6], double plantime, double ts, int plan_setp, double ConcurPathP[3]);
Dll_PPDll void Eulplan(double Rk1[3][3], double Rk2[3][3], double we[3]);  



/********************************************************************************************/
/*                                         �ſ˱Ⱦ������                                   */
/********************************************************************************************/
Dll_PPDll void JGJN_Static(double qm[8], double Jm[6][8]);



/********************************************************************************************/
/*                                          �������                                        */
/********************************************************************************************/
Dll_PPDll void Transpose(int m, int n, double *a, double *b );  //����ת��
Dll_PPDll void RbtMulMtrx( int m, int n, int p, double *A, double *B, double *C );  //�������
Dll_PPDll int RbtInvMtrx( double *C, double *IC, int n );   //��������  
Dll_PPDll void PseudoInverse(double J[6][8], double PJ[8][6]);
Dll_PPDll void Equal(double R1[3][3], double R2[3][3]);  //��Ⱦ���
Dll_PPDll double Det(double a[3][3]);        //������ʽ
Dll_PPDll double SDet(double *a, int n);   //������ʽ����λ



/********************************************************************************************/
/*                                         ������������������                               */
/********************************************************************************************/
Dll_PPDll void R_EulerZyx(double dTr[3][3],double dEulerZyx[3]);    //��ת����תŷ����
Dll_PPDll void EulerZyx_R( double oula[3],double a[3][3]);              //ŷ����ת��ת����
Dll_PPDll void T2PE(double T[4][4], double PE[6]);                      //T����תĩ��λ��
Dll_PPDll void PE2T(double PE[6], double T[4][4]);                      //ĩ��λ��תT����
Dll_PPDll void RP_T(double P[3], double R[3][3], double T[4][4]);   //��ת�����P����תT����
Dll_PPDll double  Axangle(double qint[3], double qend[3], double q[3]);   //�ɳ�ʼ��ֹ��ת������ָ��ƫ��
Dll_PPDll void DH2T(double dh[4], double T[4][4]);    //��DH������T����
Dll_PPDll void Vec2DSymMtx(double vec[3], double R[3][3]);   //�����䷴�Գƾ���
Dll_PPDll void DHMof(double qm[8], double dh[8][4]);    //�ɹؽڽǸ�ֵDH����
Dll_PPDll void Calc_T(double qm[8], double T[32][4]);     // ����T����
Dll_PPDll void Thpater_T(double qm[8], double T01[4][4], double T12[4][4], double T23[4][4], double T34[4][4], double T45[4][4], double T56[4][4], double T67[4][4], double T78[4][4]);
Dll_PPDll void w2Elzyxp(double Eul[3], double R[3][3]);   //���ٶ�תŷ�����ٶ�
Dll_PPDll void RR(double cetatemp, double R[3][3]);   //��Z����ת



/********************************************************************************************/
/*                                          ������ѧ����                                    */
/********************************************************************************************/
Dll_PPDll double Dotm(double a[3], double b[3]);
Dll_PPDll void Crossm(double a[], double b[], double c[]);           //X��
Dll_PPDll int Sign(double a);                         //�ж�����λ
Dll_PPDll double Fmax(double a, double b);            //�����ֵ
Dll_PPDll double Fmin(double a, double b);            //����Сֵ
Dll_PPDll double Fmax3(double a, double b, double c);  //����ȡ����
Dll_PPDll double Nnorm1(double a, double b, double c);
Dll_PPDll double Nnorm2(double a[3]);
Dll_PPDll double Norm(double a[3], double b[3]);



/********************************************************************************************/
/*                                               ����                                       */
/********************************************************************************************/
/// ��ʼ����������
Dll_PPDll void init_robot_size(double size[6]);//��ʼ����е�ۻ����ߴ�
Dll_PPDll void init_dh(double dh[8][4]);//��ʼ��DH����
