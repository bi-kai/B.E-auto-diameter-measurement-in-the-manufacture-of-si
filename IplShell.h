//////////////////////////////////////////////////////////////////////////
//Opencv Extention Lib (c++)
//IplImage Shell 
//Function:use IplIamge with more fun
//author:canopus4u@gmail.com
//////////////////////////////////////////////////////////////////////////
#pragma once

template <typename Type> 
class IplShell
{
public:
	IplImage*		src;
	int				m_Step;
#ifdef _DEBUG
	int				m_Width;
	int				m_Height;
#endif
	
public:
	IplShell(IplImage* img = NULL){
		src = img;
		if ( img )
			m_Step = img->widthStep;
#ifdef _DEBUG
		if ( img )
		{
			m_Width = img->width;
			m_Height = img->height;
		}
		
#endif		
	};
	~IplShell(){};
public:
	void Init(IplImage* img){
		src = img;
		m_Step = img->widthStep;
#ifdef _DEBUG
		m_Width = img->width;
		m_Height = img->height;
#endif		
	};
	inline Type* operator [](int row){
#ifdef _DEBUG
		assert( row >= 0 && row < m_Height );
#endif
		return (Type*)(src->imageData + row * m_Step);
	};
	inline void SetPixel(int row, int col, Type val){
		((Type*)(src->imageData + row * m_Step)) = val;
#ifdef _DEBUG
		assert( row >=0 && row < m_Height );
		assert( col >=0 && col < m_Width );
#endif
	};
};

typedef struct _RGB8U
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}RGB8U;

typedef struct _RGB32F
{
	float b;
	float g;
	float r;
}RGB32F;

typedef struct _RGB32S
{
	int b;
	int g;
	int r;
}RGB32S;


typedef unsigned char Gray;
typedef int Gray32S;
typedef float Gray32F;

typedef IplShell< Gray > GrayIplShell;
typedef IplShell< RGB8U > RGB8UIplShell;
typedef IplShell< RGB32F > RGB32FIplShell;
typedef IplShell< RGB32S > RGB32SIplShell;
typedef IplShell< Gray32S > Gray32SIplShell;
typedef IplShell< Gray32F > Gray32FIplShell;
