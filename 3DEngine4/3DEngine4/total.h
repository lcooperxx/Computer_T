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
	CEuler ToEuler();//��Ԫ��ת��Ϊŷ����
	CMatrix ToMatrix();//��Ԫ��ת��Ϊ����
	void SetAngle(double angle, CVector3 axis); //��Ԫ������
	CQuaternion& operator=(CQuaternion& p); //���ظ�ֵ
	CQuaternion CQuaternion :: operator/(double p); //�������
	CQuaternion operator+(CQuaternion& p); //���ء�+��
	CQuaternion operator*(double data); //��������
	CQuaternion operator*(CQuaternion&p); //��Ԫ���˷�
	double dotMul(CQuaternion&p); //���
	double len(); //��ģ
	bool Normalize();	//���׼��
	CQuaternion Inverse();//������Ԫ��,��ı�����
	CQuaternion GetInverse();//������Ԫ��,���ı����������µ���Ԫ��
	CQuaternion Div(CQuaternion&b); //��� ��ǰΪa,��c=a-b
	void GetAngle(double& angle, CVector3& axis); //����ת��ͽǶ�
	CQuaternion Slerp(CQuaternion& Vend, double t); //��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
	void Slerp(CQuaternion& Vend, int n, double *t, CQuaternion *Result);//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result��
};
class AFX_CLASS_EXPORT CEuler
{
public:
	double h, p, b;
	void Set(double h, double p, double b);
	operator double*(){ return &h;}
	//ŷ����ת��Ϊ����
	CVector3 CEuler::ToVector3(CVector3 *updir);//�������updirָ�벻Ϊ0�����������ͬʱ����Ϸ�������updir�С�
	CMatrix CEuler::ToMatrix();//ŷ����ת��Ϊ����CMarix
	CQuaternion ToQuaternion();//ŷ����ת��Ϊ��Ԫ��
	void CEuler::Normal();//ŷ���ǹ淶��,h,r�Ƕ���-180~180��p�Ƕ���-90~90��������ʱ,h��������r�Ķ�����
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
	void Identity();		//����Ϊ��λ����
	void Set(double *p);	//������16��Ԫ�ظ�ֵ
	void SetRow(int i, CVector3 p);//���õ�i��Ԫ��
	void SetCol(int j, CVector3 p);	//���õ�i��Ԫ������
	CMatrix& operator=(const CMatrix& p);//����ֵ
	CMatrix& operator=(CMatrix& p);//����ֵ
	CMatrix operator*(double d);//��������
	CMatrix operator*(CMatrix& p);//�������
	CVector3 vecMul(CVector3& p); //�������������
	CVector3 posMul(CVector3& p);//����������
	void Transpose();//����ת��
	CMatrix& SetRotate(double seta, CVector3 axis);//����Ϊ��ת����
	CMatrix& SetTrans(CVector3 trans);	//����Ϊƽ�ƾ���
	CMatrix& SetScale1(double k, CVector3 p);	//����Ϊ���ž���
	CMatrix& SetScale(CVector3 p);	//����Ϊ���ž���
	double Inverse(CMatrix arcs, int n);//��������,�ɹ���������ʽ��ֵ�����򷵻�0
	CMatrix GetInverseMatrix(CMatrix src, CMatrix srcinverse);//���������
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
	CVector3(double m_dX, double m_dY, double m_dZ, double m_dT)//��ֵ����
	{
		this->m_dX = m_dX;
		this->m_dY = m_dY;
		this->m_dZ = m_dZ;
		this->m_dT = m_dT;
	}
	operator double*(){ return &m_dX; }
	friend CVector3 operator +(CVector3&c1, CVector3&c2);//���ء�+������
	friend CVector3 operator -(const CVector3&c1,const  CVector3&c2);//���ء�-������
	friend bool operator ==(CVector3&c1, CVector3&c2);//���ء��Ƚ���ȡ�����
	friend bool operator !=(CVector3&c1, CVector3&c2);//���ء��Ƚϲ��ȡ�����

	friend CVector3 operator *(CVector3&c1, double c2);////���ء�*��������ǰ����
	friend CVector3 operator *(double c2, CVector3 &c1);//���ء�*��������ǰ����
	CVector3& operator=(const CVector3 &c2);////���ظ�ֵ��=������
	double dotMul(CVector3 &n);//���ء���ˡ�����
	CVector3 crossMul(CVector3 &n);//���ء���ˡ�����
	double len();//���ء���ģ������
	void Normalize();//���ء���׼��������
	CVector3 project(CVector3 &n);//���ء�ͶӰ������
	void Set(double x, double y, double z, double t);//������ֵ
	CVector3 lerp(CVector3 p, double t);
	CEuler ToEuler();

	friend CVector3 operator*(double *mat, CVector3& p);
	CVector3 operator-();//��
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
	CVector3 m_pos;	//�ӵ㵱ǰλ��
	CVector3 m_hpr;	//�ӵ㷽��ǣ�xΪh,yΪp,zΪr
	CVector3 m_eyedir;//���߷���
	CVector3 m_updir;	//�Ϸ�����
	double m_viewMatrix[16];	//��ͼ����
	double m_viewMatrixInverse[16];	//��ͼ����������
	double m_viewMatrixSave[16];	//�������ͼ����
	int m_type;//	������͡�0��ʾ��������(hpr�ۻ�)��1��ʾ�������Σ�2��ʾ��������
};
class AFX_CLASS_EXPORT CglControl
{
public:
	void SetSpeed(float ms, float rs);
	bool KeyboardCtrlView();

	bool PreTranslateMessage(unsigned int message, unsigned int wParam, unsigned int lParam);
	CglControl(COpenGL *pGL);
	virtual ~CglControl();

	//����״̬��
	enum KEYSTATUS{
		KS_M_RIGHT, KS_M_LEFT, KS_M_UP, KS_M_DOWN, KS_M_BACK, KS_M_FRONT, \
		KS_R_LEFT, KS_R_RIGHT, KS_R_UP, KS_R_DOWN, KS_R_FRONT, KS_R_BACK, \
		KS_M_STEP_N, KS_M_STEP_P, KS_R_STEP_N, KS_R_STEP_P, \
		KS_RESTORE, KS_ALL
	}KeyVal;
	bool m_keyStatus[KS_ALL];

	//��갴��״̬
	bool m_bMouseLDown;
	bool m_bMouseRDown;
	CPoint m_MousePos;

	COpenGL *m_pOpenGL;
	CglCamera *m_pCamera;
private:
	bool SetKeyStatus(unsigned int nChar, bool bVal);
	bool MouseCtrlView(int message, CPoint pt);
	double m_fStep[2];//�ƶ�����ת�������ű���
	float m_fKeyStep;//���̰���һ���˶���������ƶ�����
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
	CArray<int, int>m_fontlist;//�������е��Ѿ����ɵ���ά�����б�
};
class AFX_CLASS_EXPORT CglShape
{
public:
	void DrawCoord(double len);
	void DrawBall(float radius, int lon, int lat);
	CglShape();
	virtual ~CglShape();
	CVector3 *m_vertArray;	//����
	CVector3 *m_normalArray;	//����
	CVector3 *m_texArray;	//��������
	int		*m_indexArray;
	int m_nVert;			//��Ҫ���Ƶĵ�ĸ���
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
	void SimuStart();	//���濪ʼ��
	CTime &GetSimuTimeCur();
	double GetSimuTime();
	void SetSimuTimeStartTime(CTime *SimuTimeStart);
	CglTime();
	virtual ~CglTime();
private:
	double m_dSimuTime;	//����ʱ��
	double m_dTimeSpan;	//���ε��õķ���ʱ����
	CTime m_SimuTimeStart;	//���濪ʼʱ��
	CTime m_SimuTimeCur;	//��ǰ���濪ʼʱ��
	double m_SimuTimeRatio;		//����ʱ������ʵʱ��ı�ֵ
	int m_lasttime;	//��¼�ϴ�ϵͳʱ��
};
class AFX_CLASS_EXPORT CglRecorder
{
public:
	CglRecorder();
	virtual ~CglRecorder();

	bool BeginSave(char *filename);	//��ʼ¼��
	void EndSave();
	bool BeginLoad(char *filename, bool bLoadCamera = false);
	void EndLoad();

	//��Ҫ������
	void TimeRecord(double time);	//����ʱ����Ϣ
	void ViewRecord();				//�����ӵ���Ϣ
	void MatrixRecord(int id = -1);			//����ģ�ͱ任������Ϣ


private:
	//�ط�
	void LoadTime(double time);	//���ݵ�ǰ����ʱ����һط��ļ��еļ�¼λ��	,����Ϊ��λ
	void LoadCurView();		//��ȡ�ӵ����
	void LoadCurMatrix(int id = -1);		//��ȡ��id������id=0��ʾ�ӵ���󣬴�1��ʼ��ģ�;������id=-1��ʾ��ģ�Ϳ�ʼ˳���ȡ
	//��¼
	void SaveTime(double time);			//��ʼ��¼����ʱ��
	void SaveCurView();					//��¼�ӵ����
	void SaveCurMatrix();				//��¼��ǰģ�;���

	FILE *m_fpSave, *m_fpLoad;
	double m_dViewMatrix[16];			//�ӵ����
	double m_dInverseViewMatrix[16];	//���ӵ����
	int m_iNumMatrix;					//����ľ������
	int m_iLoadMatrix;					//��ǰ���ڶ�ȡ��ģ�;���������,����Ϊ��λ
	int m_iStep;						//���ݱ���ļ�����Ժ���Ϊ��λ��
	double m_dNextSaveTime;				//��һ�α������ݵ�ʱ��
	bool m_bCanSave;					//����˴����ݵı��档
	int m_iCurState;				//��ǰ״̬��1��ʾ���ݼ�¼״̬��2��ʾ���ݶ�ȡ״̬������ֵ��ʾ��ʹ�����ݼ�¼��
	bool m_bLoadCamera;				//�ڶ�ȡ״̬ʱ���Ƿ�ǿ���ӵ㡣

	int m_iLoadRecordNo;				//��ǰ��ȡ�ڼ����ļ�¼
	double m_dLastLoadTime;				//�ϴ�����ļ�¼�ļ�����ʱ��
	int m_iRecordSize;					//һ����¼���ֽ���

};