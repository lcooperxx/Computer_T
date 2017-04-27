#include"stdafx.h"
#include<vector>
#include"iostream"
#include"string.h"
#include"stdio.h"
#define PAI 3.1415926
using namespace std;
class CQuaternion;
class CEuler;
class CMatrix;
class CVector3;

class COpenGL;
class CglControl;
class CglFont;
class CglShape;
class CglPicLoad;
class CglTime;
class CglRecorder;

class AFX_CLASS_EXPORT CQuaternion
{
public:
	double x, y, z, w;
	void Set(double w, double x, double y, double z);
	operator double*(){ return &x; };
	CEuler ToEuler();//四元数转化为欧拉角
	CMatrix ToMatrix();//四元数转化为矩阵
	void SetAngle(double angle, CVector3 axis); //四元数设置
	CQuaternion& operator=(CQuaternion& p); //重载赋值
	CQuaternion CQuaternion :: operator/(double p); //重载相除
	CQuaternion operator+(CQuaternion& p); //重载‘+’
	CQuaternion operator*(double data); //重载数乘
	CQuaternion operator*(CQuaternion&p); //四元数乘法
	double dotMul(CQuaternion&p); //点乘
	double len(); //求模
	bool Normalize();	//求标准化
	CQuaternion Inverse();//求逆四元数,会改变自身。
	CQuaternion GetInverse();//求逆四元数,不改变自身，生成新的四元数
	CQuaternion Div(CQuaternion&b); //求差 当前为a,求c=a-b
	void GetAngle(double& angle, CVector3& axis); //求旋转轴和角度
	CQuaternion Slerp(CQuaternion& Vend, double t); //插值。从当前四元数插值到Vend四元数,t是参数[0,1]
	void Slerp(CQuaternion& Vend, int n, double *t, CQuaternion *Result);//插值。一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中
};
class AFX_CLASS_EXPORT CEuler
{
public:
	double h, p, b;
	void Set(double h, double p, double b);
	operator double*(){ return &h;}
	//欧拉角转换为向量
	CVector3 CEuler::ToVector3(CVector3 *updir);//如果传入updir指针不为0，则输出向量同时输出上方向量到updir中。
	CMatrix CEuler::ToMatrix();//欧拉角转换为矩阵CMarix
	CQuaternion ToQuaternion();//欧拉角转换为四元数
	void CEuler::Normal();//欧拉角规范化,h,r角度在-180~180，p角度在-90~90。万向锁时,h代替所有r的度数。
};
class AFX_CLASS_EXPORT CMatrix
{
public:
	double m00, m10, m20, m30;
	double m01, m11, m21, m31;
	double m02, m12, m22, m32;
	double m03, m13, m23, m33;
	CMatrix();
	CMatrix::~CMatrix();
	CMatrix(double *p);
	void Identity();		//设置为单位矩阵
	void Set(double *p);	//给矩阵16个元素赋值
	void SetRow(int i, CVector3 p);//设置第i行元素
	void SetCol(int j, CVector3 p);	//设置第i列元素向量
	CMatrix& operator=(const CMatrix& p);//矩阵赋值
	CMatrix& operator=(CMatrix& p);//矩阵赋值
	CMatrix operator*(double d);//矩阵数乘
	CMatrix operator*(CMatrix& p);//矩阵相乘
	CVector3 vecMul(CVector3& p); //矩阵与向量相乘
	CVector3 posMul(CVector3& p);//矩阵与点相乘
	void Transpose();//矩阵转置
	CMatrix& SetRotate(double seta, CVector3 axis);//设置为旋转矩阵
	CMatrix& SetTrans(CVector3 trans);	//设置为平移矩阵
	CMatrix& SetScale1(double k, CVector3 p);	//设置为缩放矩阵
	CMatrix& SetScale(CVector3 p);	//设置为缩放矩阵
	double Inverse(CMatrix arcs, int n);//矩阵求逆,成功返回行列式的值，否则返回0
	CMatrix GetInverseMatrix(CMatrix src, CMatrix srcinverse);//返回逆矩阵
	CMatrix run();
	CQuaternion ToQuaternion();
	CEuler ToEuler();
	operator double*(){ return &m00; }
	//operator const double*()const { return &m00; }
};
class AFX_CLASS_EXPORT CVector3
{
public:
	CVector3(){};
	double m_dX, m_dY, m_dZ, m_dT;
	CVector3(double m_dX, double m_dY, double m_dZ, double m_dT)//赋值操作
	{
		this->m_dX = m_dX;
		this->m_dY = m_dY;
		this->m_dZ = m_dZ;
		this->m_dT = m_dT;
	}
	operator double*(){ return &m_dX; }
	friend CVector3 operator +(CVector3&c1, CVector3&c2);//重载“+”操作
	friend CVector3 operator -(const CVector3&c1,const  CVector3&c2);//重载“-”操作
	friend bool operator ==(CVector3&c1, CVector3&c2);//重载“比较相等”操作
	friend bool operator !=(CVector3&c1, CVector3&c2);//重载“比较不等”操作

	friend CVector3 operator *(CVector3&c1, double c2);////重载“*”操作，前向量
	friend CVector3 operator *(double c2, CVector3 &c1);//重载“*”操作，前常数
	CVector3& operator=(const CVector3 &c2);////重载赋值“=”操作
	double dotMul(CVector3 &n);//重载“点乘”操作
	CVector3 crossMul(CVector3 &n);//重载“叉乘”操作
	double len();//重载“求模”操作
	void Normalize();//重载“标准化”操作
	CVector3 project(CVector3 &n);//重载“投影”操作
	void Set(double x, double y, double z, double t);//设置数值
	CVector3 lerp(CVector3 p, double t);
	CEuler ToEuler();

	friend CVector3 operator*(double *mat, CVector3& p);
	CVector3 operator-();//求负
};

class AFX_CLASS_EXPORT CglMath
{
public:
	CglMath();
	virtual ~CglMath();
	static double InverseMatrix(double Matrix[], double inverseMatrix[]);
	static void CalculateNormal(CVector3 *vert, CVector3 *norm, int numRow, int numCol);
	static void Dir2HPR(CVector3 dir, CVector3 up, CVector3 &hpr);
};
class AFX_CLASS_EXPORT CglCamera
{
public:
	vector<CVector3> path;

	void SetType(int type);
	void LoadCamera();
	void SaveCamera();
	
	void Init();
	void SetCamera(const CVector3& pos, const CVector3& obj_dir, bool bObj = true, int type = 0);
	void ShowView();
	void Rotate(int dir, double len);
	void Move(int dir, double len);
	CglCamera();
	virtual ~CglCamera();
	CVector3 m_pos;	//视点当前位置
	CVector3 m_hpr;	//视点方向角，x为h,y为p,z为r
	CVector3 m_eyedir;//视线方向
	CVector3 m_updir;	//上方向量
	double m_viewMatrix[16];	//视图矩阵
	double m_viewMatrixInverse[16];	//视图矩阵的逆矩阵
	double m_viewMatrixSave[16];	//保存的视图矩阵
	int m_type;//	相机类型。0表示地面漫游(hpr累积)，1表示任意漫游，2表示环绕漫游
};
class AFX_CLASS_EXPORT CglControl
{
public:
	void SetSpeed(float ms, float rs);
	bool KeyboardCtrlView();

	bool PreTranslateMessage(unsigned int message, unsigned int wParam, unsigned int lParam);
	CglControl(COpenGL *pGL);
	virtual ~CglControl();

	//按键状态；
	enum KEYSTATUS{
		KS_M_RIGHT, KS_M_LEFT, KS_M_UP, KS_M_DOWN, KS_M_BACK, KS_M_FRONT, \
		KS_R_LEFT, KS_R_RIGHT, KS_R_UP, KS_R_DOWN, KS_R_FRONT, KS_R_BACK, \
		KS_M_STEP_N, KS_M_STEP_P, KS_R_STEP_N, KS_R_STEP_P, \
		KS_RESTORE, KS_ALL
	}KeyVal;
	bool m_keyStatus[KS_ALL];

	//鼠标按键状态
	bool m_bMouseLDown;
	bool m_bMouseRDown;
	CPoint m_MousePos;

	COpenGL *m_pOpenGL;
	CglCamera *m_pCamera;
private:
	bool SetKeyStatus(unsigned int nChar, bool bVal);
	bool MouseCtrlView(int message, CPoint pt);
	double m_fStep[2];//移动和旋转步长缩放倍率
	float m_fKeyStep;//键盘按下一个运动命令代表移动步长
};
class AFX_CLASS_EXPORT COpenGL
{
public:
	virtual void InDraw();
	void OnView();
	bool PreTranslateMessage(unsigned int message, unsigned int wParam, unsigned int lParam);
	void OnSize(int w, int h);
	void OnPaint();
	virtual void PostInit();
	bool Init(HWND hWnd);
	COpenGL();
	virtual ~COpenGL();
	virtual bool OnKey(unsigned int nChar, bool bDown);
	virtual bool OnMouse(int message, CPoint pt);
	friend class CglControl;
protected:
	HDC m_hDC;
	HGLRC m_hglrc;
	float m_PerspectiveParam[4];
	CglControl *m_pControl;
	HWND m_hWnd;
	CglCamera *m_pCamere;

	CglFont *m_pFont;
	CglShape *m_pShape;
	CglPicLoad *m_pPicLoad;
	CglTime *m_pTime;
	CglRecorder *m_pRecorder;
	float m_fps;
};
class AFX_CLASS_EXPORT CglFont
{
public:
	CglFont();
	virtual ~CglFont();
	//type == 1--7
	void Font2D(char * str, double x, double y, int type);
	bool Font3D(CString str);
	bool Font3D(CString str, int &id);
private:
	bool Create3DFont();
	LOGFONT m_logFontDefault;
	int m_iFormat;
	double m_fDeviation;
	double m_fExtrusion;
	CFont* m_pFontSelected;
	int m_iFormatDefault;
	double m_fDeviationDefault;
	double m_fExtrusionDefault;
	int m_iFirstChar;
	int m_iLastChar;
	int m_iNumberChars;
	int m_iDisplayListStart;
	int m_iDisplayListBase;
	bool m_bHaveCreate3DFont;
	CArray<int, int>m_fontlist;//保存所有的已经生成的三维字体列表
};
class AFX_CLASS_EXPORT CglShape
{
public:
	void DrawCoord(double len);
	void DrawBall(float radius, int lon, int lat);
	CglShape();
	virtual ~CglShape();
	CVector3 *m_vertArray;	//顶点
	CVector3 *m_normalArray;	//法线
	CVector3 *m_texArray;	//纹理坐标
	int		*m_indexArray;
	int m_nVert;			//需要绘制的点的个数
};
class AFX_CLASS_EXPORT CglImageData
{
public:
	int m_width;
	int m_height;
	int m_bits;
	unsigned char *m_pData;
	CglImageData()
	{
		m_width = 0;
		m_height = 0;
		m_bits = 0;
		m_pData = 0;
	}
	~CglImageData()
	{
		m_width = 0;
		m_height = 0;
		m_bits = 0;
		delete m_pData;
		m_pData = 0;
	}
};
class AFX_CLASS_EXPORT CglPicLoad
{
public:
	CglImageData* ReadBMP(char *filename);
	CglPicLoad();
	virtual ~CglPicLoad();

};
class AFX_CLASS_EXPORT CglTime
{
public:
	double GetTimeSpan();
	void SimuProcess();
	void SimuStart();	//仿真开始了
	CTime &GetSimuTimeCur();
	double GetSimuTime();
	void SetSimuTimeStartTime(CTime *SimuTimeStart);
	CglTime();
	virtual ~CglTime();
private:
	double m_dSimuTime;	//仿真时间
	double m_dTimeSpan;	//两次调用的仿真时间间隔
	CTime m_SimuTimeStart;	//仿真开始时间
	CTime m_SimuTimeCur;	//当前仿真开始时间
	double m_SimuTimeRatio;		//仿真时间与真实时间的比值
	int m_lasttime;	//记录上次系统时间
};
class AFX_CLASS_EXPORT CglRecorder
{
public:
	CglRecorder();
	virtual ~CglRecorder();

	bool BeginSave(char *filename);	//开始录制
	void EndSave();
	bool BeginLoad(char *filename, bool bLoadCamera = false);
	void EndLoad();

	//主要处理函数
	void TimeRecord(double time);	//处理时间信息
	void ViewRecord();				//处理视点信息
	void MatrixRecord(int id = -1);			//处理模型变换矩阵信息


private:
	//回放
	void LoadTime(double time);	//根据当前仿真时间查找回放文件中的记录位置	,毫秒为单位
	void LoadCurView();		//读取视点矩阵
	void LoadCurMatrix(int id = -1);		//读取第id个矩阵。id=0表示视点矩阵，从1开始是模型矩阵。如果id=-1表示从模型开始顺序读取
	//记录
	void SaveTime(double time);			//开始记录仿真时间
	void SaveCurView();					//记录视点矩阵
	void SaveCurMatrix();				//记录当前模型矩阵

	FILE *m_fpSave, *m_fpLoad;
	double m_dViewMatrix[16];			//视点矩阵
	double m_dInverseViewMatrix[16];	//逆视点矩阵
	int m_iNumMatrix;					//保存的矩阵个数
	int m_iLoadMatrix;					//当前正在读取的模型矩阵索引，,毫秒为单位
	int m_iStep;						//数据保存的间隔。以毫秒为单位。
	double m_dNextSaveTime;				//下一次保存数据的时间
	bool m_bCanSave;					//允许此次数据的保存。
	int m_iCurState;				//当前状态，1表示数据记录状态，2表示数据读取状态，其他值表示不使用数据记录。
	bool m_bLoadCamera;				//在读取状态时，是否强制视点。

	int m_iLoadRecordNo;				//当前读取第几条的记录
	double m_dLastLoadTime;				//上次载入的记录文件仿真时间
	int m_iRecordSize;					//一条记录的字节数

};