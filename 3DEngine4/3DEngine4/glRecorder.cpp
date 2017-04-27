// glRecorder.cpp: implementation of the CglRecorder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "total.h"
#include "gl\glut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CglRecorder::CglRecorder()
{
	m_fpSave = m_fpLoad = NULL;
	m_iNumMatrix = 0;
	m_iCurState = 0;
	m_bLoadCamera = false;
	m_iStep = 30;//默认最小保存时间间距为30ms，也就是一秒钟最多保存33fps的数据，更高的数据也没有意义了。
	m_bCanSave = true;
	m_iLoadRecordNo = 0;
	m_dLastLoadTime = 0;
	m_iLoadMatrix = 0;
}

CglRecorder::~CglRecorder()
{
	EndSave();
	EndLoad();
}

bool CglRecorder::BeginSave(char *filename)
{
	m_fpSave = fopen(filename, "wb");
	if (!m_fpSave)
		return false;
	m_iCurState = 1;
	return true;
}

void CglRecorder::EndSave()
{
	if (!m_fpSave)
		return;
	fwrite(&m_iNumMatrix, 4, 1, m_fpSave);
	fclose(m_fpSave);
	m_fpSave = NULL;
	m_iCurState = 0;
}

void CglRecorder::TimeRecord(double time)
{
	if (m_iCurState == 1)//保存视点矩阵
	{
		SaveTime(time);
	}
	else if (m_iCurState == 2)//读取视点矩阵
	{
		LoadTime(time);
	}
}

void CglRecorder::SaveTime(double time)
{
	if (time>m_dNextSaveTime)
	{
		fwrite(&time, 8, 1, m_fpSave);
		m_iNumMatrix = 0;
		m_dNextSaveTime = time + m_iStep;	//计算下次保存的时间
		m_bCanSave = true;
	}
	else
	{
		m_bCanSave = false;
	}
}
void CglRecorder::ViewRecord()
{
	glGetDoublev(GL_MODELVIEW_MATRIX, m_dViewMatrix);
	if (m_iCurState == 1 && m_bCanSave)//保存视点矩阵
	{
		SaveCurView();
	}
	else if (m_iCurState == 2 && m_bLoadCamera)//读取视点矩阵
	{
		LoadCurView();
	}
}

void CglRecorder::MatrixRecord(int id)
{
	if (m_iCurState == 1 && m_bCanSave)//保存视点矩阵
	{
		SaveCurMatrix();
	}
	else if (m_iCurState == 2)//读取视点矩阵
	{
		LoadCurMatrix(id);
	}
}


void CglRecorder::SaveCurView()
{
	CglMath::InverseMatrix(m_dViewMatrix, m_dInverseViewMatrix);
	fwrite(m_dViewMatrix, 8, 16, m_fpSave);
	m_iNumMatrix++;
}

void CglRecorder::SaveCurMatrix()
{
	double mat[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);	//得到当前需要保存的矩阵。
	//求此矩阵的模型变换矩阵。
	//因此需要计算矩阵相乘结果 m_dInverseViewMatrix×mat
	{
		glPushMatrix();
		glLoadMatrixd(m_dInverseViewMatrix);
		glMultMatrixd(mat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mat);	//获得相乘以后的矩阵。
		glPopMatrix();
	}
	fwrite(mat, 8, 16, m_fpSave);
	m_iNumMatrix++;
}

bool CglRecorder::BeginLoad(char *filename, bool bLoadCamera)
{
	m_fpLoad = fopen(filename, "rb");
	if (!m_fpLoad)
		return false;
	fseek(m_fpLoad, -4, SEEK_END);
	fread(&m_iNumMatrix, 4, 1, m_fpLoad);
	m_iRecordSize = 8 + m_iNumMatrix * 16 * 8;
	fseek(m_fpLoad, 0, SEEK_SET);
	m_iCurState = 2;
	m_bLoadCamera = bLoadCamera;
	m_iLoadRecordNo = 0;
	m_dLastLoadTime = 0;
	return true;
}

void CglRecorder::EndLoad()
{
	if (!m_fpLoad)
		return;
	fclose(m_fpLoad);
	m_fpLoad = NULL;
	m_iCurState = 0;
}

void CglRecorder::LoadTime(double time)
{
	double nextTime;
	if (m_bLoadCamera)
		m_iLoadMatrix = 0;
	else m_iLoadMatrix = 1;
	if (m_dLastLoadTime >= time)		//当前位置就是要读取的位置。
		return;
	fseek(m_fpLoad, (m_iLoadRecordNo + 1)*m_iRecordSize, SEEK_SET);
	bool bFileEnd = false;
	while (!feof(m_fpLoad))
	{
		int n = fread(&nextTime, 8, 1, m_fpLoad);	//取下一个时间
		if (n<1)
		{
			bFileEnd = true;
			break;
		}
		if (nextTime<time)		//下一个时间仍然没有到当前仿真时间的位置。
		{
			m_dLastLoadTime = nextTime;
			m_iLoadRecordNo++;
			fseek(m_fpLoad, m_iRecordSize - 8, SEEK_CUR);
			continue;
		}
		if ((nextTime - time)<(time - m_dLastLoadTime))	//下一个磁盘文件中记录的仿真时间更接近与真实的仿真时间
		{
			m_dLastLoadTime = nextTime;
			m_iLoadRecordNo++;
			break;				//找到了当前要读取的数据条目，就是nextTime的数据
		}
		else										//当前条目中仿真时间更接近。
		{
			break;
		}
	}
	if (feof(m_fpLoad) || bFileEnd)//文件已经读完
		EndLoad();
}

void CglRecorder::LoadCurView()
{
	fseek(m_fpLoad, m_iLoadRecordNo*m_iRecordSize + 8, SEEK_SET);	//移动到上次读取的记录位置
	fread(m_dViewMatrix, 8, 16, m_fpLoad);
	glLoadMatrixd(m_dViewMatrix);			//设置视点矩阵
	m_iLoadMatrix++;
}

void CglRecorder::LoadCurMatrix(int id)
{
	if (id<0)
	{
		if (m_iLoadMatrix<m_iNumMatrix)
		{
			fseek(m_fpLoad, m_iLoadRecordNo*m_iRecordSize + 8 + m_iLoadMatrix * 16 * 8, SEEK_SET);
			m_iLoadMatrix++;
		}
		else
			return;
	}
	else
	{
		fseek(m_fpLoad, m_iLoadRecordNo*m_iRecordSize + 8 + id * 16 * 8, SEEK_SET);
		m_iLoadMatrix = id + 1;	//下次要读取的矩阵。
	}
	double mat[16];
	fread(mat, 8, 16, m_fpLoad);
	glLoadMatrixd(m_dViewMatrix);			//设置视点矩阵
	glMultMatrixd(mat);			//设置视点矩阵
}


