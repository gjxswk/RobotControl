//////////////////////////////////////////////////////////////////////////
//Datatype.h
//////////////////////////////////////////////////////////////////////////

typedef struct _VECTOR
{
  double x,y,z;
} t_Vector;

typedef struct _MODULE
{
	int ModuleIndex;
//	tVector	scale;						// CURRENT SCALE FACTORS
	t_Vector	rot;						// CURRENT ROTATION FACTORS
	t_Vector	trans;						// CURRENT TRANSLATION FACTORS
	double      JntVar_trans;
	double      JntVar_rot;
	double      JntVar_trans1;
	double      JntVar_rot1;
} t_Module;

