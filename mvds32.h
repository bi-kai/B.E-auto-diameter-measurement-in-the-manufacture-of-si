/**************************************************************************

	
**************************************************************************/

#ifndef _MVDS32_
#define _MVDS32_

#include <vfw.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	_BRIGHTNESS                     = 0,
	_CONTRAST                       = 1,
	_HUE		                    = 2,
	_SATURATION                     = 3,
	_SHARPNESS                      = 4,
		
}PROPERY;

long WINAPI MV_GetCardNumber();

HANDLE WINAPI MV_Opencard( long Index );
BOOL WINAPI MV_Closecard( HANDLE hCap );

BOOL WINAPI MV_SnapShot( HANDLE hCap, BYTE* lpBuf, long BufLen );
BOOL WINAPI MV_StartCap( HANDLE hCap, HWND hWnd );
BOOL WINAPI MV_StopCap( HANDLE hCap );
BOOL WINAPI MV_SetCallBack( HANDLE hCap, void* pCall, PVOID pContext );

BOOL WINAPI MV_SetOutSize( HANDLE hCap, long resmode );
BOOL WINAPI MV_GetOutSize( HANDLE hCap, long* Width,long* High );
BOOL WINAPI MV_SetPropery( HANDLE hCap, PROPERY Propery,long value );
BOOL WINAPI MV_GetPropery( HANDLE hCap, PROPERY Propery,long* value );
BOOL WINAPI MV_SetVideoMode( HANDLE hCap, long bInterlace, long bColor );
BOOL WINAPI MV_SetVideoflip( HANDLE hCap, BOOL isHflip, BOOL isVflip );
BOOL WINAPI MV_SetVideoSource( HANDLE hCap, long pin );
BOOL WINAPI MV_SetVideoStandard( HANDLE hCap, long mode );
BOOL WINAPI MV_GetNumberOfLines( HANDLE hCap, long* lines);

BOOL WINAPI MV_SaveBMPFile(char* lpFileName, BITMAPINFO* pBmpInfo, BYTE* pImageBuffer);
BOOL WINAPI MV_SaveJPEGFile(char* lpFileName, BYTE* pBuffer, long nWidth, long nHeight, BOOL bcolor, long nQuality);

BOOL WINAPI MV_AVICreate(HANDLE hCap,char* lpFileName,long nWidth, long nHeight, long bColor, double dFramerate);
BOOL WINAPI MV_AVIClose(HANDLE hCap);
BOOL WINAPI MV_AVIWrite(HANDLE hCap,BYTE* pBuffer);
BOOL WINAPI MV_ChooseCompressor( HANDLE hCap,long compress);

#ifdef __cplusplus
}
#endif

#endif