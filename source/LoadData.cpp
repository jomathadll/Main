// LoadData.cpp: implementation of the CLoadData class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadData.h"
#include "GlobalBitmap.h"
#include "ZzzBMD.h"
#include "ZzzTexture.h"

CLoadData gLoadData;

static GLuint LoadUniriaTextureFallback(const char* filename, int type, int wrap)
{
	if (_stricmp(filename, "unicon.jpg") == 0 || _stricmp(filename, "le.jpg") == 0)
	{
		return Bitmaps.LoadImage("Data\\Item\\unicon.jpg", type, wrap);
	}

	if (_stricmp(filename, "unicon01.tga") == 0)
	{
		return Bitmaps.LoadImage("Data\\Item\\unicon01.tga", GL_NEAREST, wrap);
	}

	return BITMAP_UNKNOWN;
}

CLoadData::CLoadData() // OK
{

}

CLoadData::~CLoadData() // OK
{
}

void CLoadData::AccessModel(int Type,char *Dir,char *FileName,int i)
{
	char Name[64];
	if(i == -1)
     	sprintf(Name,"%s.bmd",FileName);
	else if(i < 10)
     	sprintf(Name,"%s0%d.bmd",FileName,i);
	else
     	sprintf(Name,"%s%d.bmd",FileName,i);

    bool Success = false;

	Models[Type].m_iBMDSeqID = Type;

	Success = Models[Type].Open2(Dir,Name);

    if ( Success==false && ( strcmp(FileName,"Monster")==NULL || strcmp(FileName,"Player")==NULL || strcmp(FileName,"PlayerTest")==NULL || strcmp(FileName,"Angel")==NULL ) )
    {
		char Text[256];
    	sprintf(Text,"%s file does not exist.",Name );
		MessageBox(g_hWnd,Text,NULL,MB_OK);
		SendMessage(g_hWnd,WM_DESTROY,0,0);
    }
}

void CLoadData::AccessModelCustom(int Type, char* Dir, char* FileName, int i)
{
	char Name[64];
	if (i == -1)
		sprintf(Name, "%s.%s", FileName, BMD_EXT);
	else if (i < 10)
		sprintf(Name, "%s0%d.%s", FileName, i, BMD_EXT);
	else
		sprintf(Name, "%s%d.%s", FileName, i, BMD_EXT);

	bool Success = false;

	Models[Type].m_iBMDSeqID = Type;

	Success = Models[Type].OpenCustom(Dir, Name);

	if (Success == false && (strcmp(FileName, "Monster") == NULL || strcmp(FileName, "Player") == NULL || strcmp(FileName, "PlayerTest") == NULL || strcmp(FileName, "Angel") == NULL))
	{
		char Text[256];
		sprintf(Text, "%s file does not exist.", Name);
		MessageBox(g_hWnd, Text, NULL, MB_OK);
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);
	}
}

void CLoadData::OpenTexture(int Model,char *SubFolder, int Wrap, int Type,bool Check)
{
	BMD* pModel = &Models[Model];

	for(int i=0;i<pModel->NumMeshs;i++)
	{
		Texture_t* pTexture = &pModel->Textures[i];
		
		char szFullPath[256] = {0, };
		if (SubFolder[0] == 'D' && SubFolder[1] == 'a' && SubFolder[2] == 't' && SubFolder[3] == 'a') {

		}
		else {
			strcpy(szFullPath,"Data\\");
		}
		strcat(szFullPath, SubFolder);
		strcat(szFullPath, pTexture->FileName);
		
		char __ext[_MAX_EXT] = {0, };
		_splitpath(pTexture->FileName, NULL, NULL, NULL, __ext);
		if(pTexture->FileName[0]=='s' && pTexture->FileName[1]=='k' && pTexture->FileName[2]=='i')
			pModel->IndexTexture[i] = BITMAP_SKIN;
		else if(pTexture->FileName[0]=='l' && pTexture->FileName[1]=='e' && pTexture->FileName[2]=='v' && pTexture->FileName[3]=='e' && pTexture->FileName[4]=='l')
			pModel->IndexTexture[i] = BITMAP_SKIN;
		else if(pTexture->FileName[0]=='h' && pTexture->FileName[1]=='i' && pTexture->FileName[2]=='d')
			pModel->IndexTexture[i] = BITMAP_HIDE;
		else if (pTexture->FileName[0]=='h' && pTexture->FileName[1]=='a' && pTexture->FileName[2]=='i' && pTexture->FileName[3]=='r' )
			pModel->IndexTexture[i] = BITMAP_HAIR;
		else if(tolower(__ext[1])=='t')
			pModel->IndexTexture[i] = Bitmaps.LoadImage(szFullPath,GL_NEAREST,Wrap);
		else if(tolower(__ext[1])=='j')
			pModel->IndexTexture[i] = Bitmaps.LoadImage(szFullPath,Type,Wrap);

		if (pModel->IndexTexture[i] == BITMAP_UNKNOWN)
		{
			pModel->IndexTexture[i] = LoadUniriaTextureFallback(pTexture->FileName, Type, Wrap);
		}

		if(pModel->IndexTexture[i] == BITMAP_UNKNOWN)
		{
			BITMAP_t* pBitmap = Bitmaps.FindTextureByName(pTexture->FileName);
			if(pBitmap)
			{
				Bitmaps.LoadImage(pBitmap->BitmapIndex, pBitmap->FileName);
				pModel->IndexTexture[i] = pBitmap->BitmapIndex;
			}
			else
			{
				char szErrorMsg[256] = {0, };
				sprintf(szErrorMsg, "%s of %s", pTexture->FileName, pModel->Name);
				PopUpErrorCheckMsgBox("OpenTexture Failed", szErrorMsg);
				break;
				
			}
		}
	}
}

void CLoadData::OpenTextureCustom(int Model, char* SubFolder, int Wrap, int Type, bool Check)
{
	BMD* pModel = &Models[Model];

	for (int i = 0; i < pModel->NumMeshs; i++)
	{
		Texture_t* pTexture = &pModel->Textures[i];

		char szFullPath[256] = { 0, };
		if (SubFolder[0] == 'D' && SubFolder[1] == 'a' && SubFolder[2] == 't' && SubFolder[3] == 'a') {

		}
		else {
			strcpy(szFullPath, "Data\\");
		}
		strcat(szFullPath, SubFolder);
		strcat(szFullPath, pTexture->FileName);

		char __ext[_MAX_EXT] = { 0, };
		_splitpath(pTexture->FileName, NULL, NULL, NULL, __ext);
		if (pTexture->FileName[0] == 's' && pTexture->FileName[1] == 'k' && pTexture->FileName[2] == 'i')
			pModel->IndexTexture[i] = BITMAP_SKIN;
		else if (pTexture->FileName[0] == 'l' && pTexture->FileName[1] == 'e' && pTexture->FileName[2] == 'v' && pTexture->FileName[3] == 'e' && pTexture->FileName[4] == 'l')
			pModel->IndexTexture[i] = BITMAP_SKIN;
		else if (pTexture->FileName[0] == 'h' && pTexture->FileName[1] == 'i' && pTexture->FileName[2] == 'd')
			pModel->IndexTexture[i] = BITMAP_HIDE;
		else if (pTexture->FileName[0] == 'h' && pTexture->FileName[1] == 'a' && pTexture->FileName[2] == 'i' && pTexture->FileName[3] == 'r')
			pModel->IndexTexture[i] = BITMAP_HAIR;
		else if (tolower(__ext[1]) == 't')
			pModel->IndexTexture[i] = Bitmaps.LoadImageCustom(szFullPath, GL_NEAREST, Wrap);
		else if (tolower(__ext[1]) == 'j')
			pModel->IndexTexture[i] = Bitmaps.LoadImageCustom(szFullPath, Type, Wrap);

		if (pModel->IndexTexture[i] == BITMAP_UNKNOWN)
		{
			pModel->IndexTexture[i] = LoadUniriaTextureFallback(pTexture->FileName, Type, Wrap);
		}

		if (pModel->IndexTexture[i] == BITMAP_UNKNOWN)
		{
			BITMAP_t* pBitmap = Bitmaps.FindTextureByName(pTexture->FileName);
			if (pBitmap)
			{
				Bitmaps.LoadImageCustom(pBitmap->BitmapIndex, pBitmap->FileName);
				pModel->IndexTexture[i] = pBitmap->BitmapIndex;
			}
			else
			{
				char szErrorMsg[256] = { 0, };
				sprintf(szErrorMsg, "%s of %s", pTexture->FileName, pModel->Name);
				PopUpErrorCheckMsgBox("OpenTextureCustom Failed", szErrorMsg);
				break;

			}
		}
	}
}
