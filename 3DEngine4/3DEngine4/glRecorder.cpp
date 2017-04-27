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
	m_iStep = 30;//Ĭ����С����ʱ����Ϊ30ms��Ҳ����һ������ౣ��33fps�����ݣ����ߵ�����Ҳû�������ˡ�
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
	if (m_iCurState == 1)//�����ӵ����
	{
		SaveTime(time);
	}
	else if (m_iCurState == 2)//��ȡ�ӵ����
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
		m_dNextSaveTime = time + m_iStep;	//�����´α����ʱ��
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
	if (m_iCurState == 1 && m_bCanSave)//�����ӵ����
	{
		SaveCurView();
	}
	else if (m_iCurState == 2 && m_bLoadCamera)//��ȡ�ӵ����
	{
		LoadCurView();
	}
}

void CglRecorder::MatrixRecord(int id)
{
	if (m_iCurState == 1 && m_bCanSave)//�����ӵ����
	{
		SaveCurMatrix();
	}
	else if (m_iCurState == 2)//��ȡ�ӵ����
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
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);	//�õ���ǰ��Ҫ����ľ���
	//��˾����ģ�ͱ任����
	//�����Ҫ���������˽�� m_dInverseViewMatrix��mat
	{
		glPushMatrix();
		glLoadMatrixd(m_dInverseViewMatrix);
		glMultMatrixd(mat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mat);	//�������Ժ�ľ���
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
	if (m_dLastLoadTime >= time)		//��ǰλ�þ���Ҫ��ȡ��λ�á�
		return;
	fseek(m_fpLoad, (m_iLoadRecordNo + 1)*m_iRecordSize, SEEK_SET);
	bool bFileEnd = false;
	while (!feof(m_fpLoad))
	{
		int n = fread(&nextTime, 8, 1, m_fpLoad);	//ȡ��һ��ʱ��
		if (n<1)
		{
			bFileEnd = true;
			break;
		}
		if (nextTime<time)		//��һ��ʱ����Ȼû�е���ǰ����ʱ���λ�á�
		{
			m_dLastLoadTime = nextTime;
			m_iLoadRecordNo++;
			fseek(m_fpLoad, m_iRecordSize - 8, SEEK_CUR);
			continue;
		}
		if ((nextTime - time)<(time - m_dLastLoadTime))	//��һ�������ļ��м�¼�ķ���ʱ����ӽ�����ʵ�ķ���ʱ��
		{
			m_dLastLoadTime = nextTime;
			m_iLoadRecordNo++;
			break;				//�ҵ��˵�ǰҪ��ȡ��������Ŀ������nextTime������
		}
		else										//��ǰ��Ŀ�з���ʱ����ӽ���
		{
			break;
		}
	}
	if (feof(m_fpLoad) || bFileEnd)//�ļ��Ѿ�����
		EndLoad();
}

void CglRecorder::LoadCurView()
{
	fseek(m_fpLoad, m_iLoadRecordNo*m_iRecordSize + 8, SEEK_SET);	//�ƶ����ϴζ�ȡ�ļ�¼λ��
	fread(m_dViewMatrix, 8, 16, m_fpLoad);
	glLoadMatrixd(m_dViewMatrix);			//�����ӵ����
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
		m_iLoadMatrix = id + 1;	//�´�Ҫ��ȡ�ľ���
	}
	double mat[16];
	fread(mat, 8, 16, m_fpLoad);
	glLoadMatrixd(m_dViewMatrix);			//�����ӵ����
	glMultMatrixd(mat);			//�����ӵ����
}


