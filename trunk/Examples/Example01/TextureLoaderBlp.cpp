//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TextureLoaderBlp.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BLP TextureLoaderBlp;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BLP::TEXTURE_LOADER_BLP()
{

}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_LOADER_BLP::~TEXTURE_LOADER_BLP()
{

}


//+-----------------------------------------------------------------------------
//| Saves a texture to a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a texture from a buffer
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer)
{
	BLP_HEADER Header;

	CurrentFileName = FileName;

	std::memcpy(reinterpret_cast<CHAR*>(&Header), &Buffer[0], sizeof(BLP_HEADER));
	if (Header.MagicNumber != '1PLB')
	{
		Error.SetMessage("The file is not a BLP texture!");
		return FALSE;
	}

	switch (Header.Compression)
	{
	case 0:
		{
			if (!LoadCompressed(Texture, Header, Buffer)) return FALSE;
			break;
		}
	case 1:
		{
			if (!LoadUncompressed(Texture, Header, Buffer)) return FALSE;
			break;
		}
	default:
		{
			Error.SetMessage("Unable to load \"" + FileName + "\", unknown compression method!");
			return FALSE;
		}

	}
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a compressed blp texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::LoadCompressed(TEXTURE& Texture, BLP_HEADER& Header, BUFFER& Buffer)
{
	BUFFER TempBuffer;
	BUFFER TempBuffer2;
	DWORD JpegHeaderSize;
	
	std::memcpy(reinterpret_cast<CHAR*>(&JpegHeaderSize), &Buffer[sizeof(BLP_HEADER)], sizeof(DWORD));

	if(!TempBuffer2.Resize(Header.Size[0] + JpegHeaderSize))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", buffer resizing failed!");
		return FALSE;
	}

	std::memcpy(&TempBuffer2[0], &Buffer[sizeof(BLP_HEADER) + sizeof(DWORD)], JpegHeaderSize);
	std::memcpy(&TempBuffer2[JpegHeaderSize], &Buffer[Header.Offset[0]], Header.Size[0]);

	if (!Jpeg.Read(TempBuffer2, TempBuffer))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", BLP reading failed!");
		return FALSE;
	}

	GLuint TextureId;
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Header.Width, Header.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TempBuffer.GetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture.TextureId = TextureId;
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads an uncompressed blp texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_LOADER_BLP::LoadUncompressed(TEXTURE &Texture, BLP_HEADER &Header, BUFFER &Buffer)
{
	INT Size;
	BUFFER TempBuffer;
	CONST INT PALETTE_SIZE = 256;
	BLP_RGBA Palette[PALETTE_SIZE];
	BLP_RGBA* TargetPixel;

	std::memcpy(reinterpret_cast<CHAR*>(Palette), &Buffer[sizeof(BLP_HEADER)], (PALETTE_SIZE * 4));

	Size = Header.Width * Header.Height;
	if (!TempBuffer.Resize(Size * 4))
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", buffer resizing failed!");
		return FALSE;
	}

	switch (Header.PictureType)
	{
	case 3:
	case 4:
		{
			BLP_PIXEL* SourcePixel;

			SourcePixel = reinterpret_cast<BLP_PIXEL*>(&Buffer[sizeof(BLP_HEADER) + (PALETTE_SIZE * 4)]);
			TargetPixel = reinterpret_cast<BLP_RGBA*>(&TempBuffer[0]);

			for (INT i=0; i<Size; i++)
			{
				TargetPixel[i].Red = Palette[SourcePixel[i].Index].Red;
				TargetPixel[i].Green = Palette[SourcePixel[i].Index].Green;
				TargetPixel[i].Blue = Palette[SourcePixel[i].Index].Blue;
			}

			for (INT i=0; i<Size; i++)
			{
				TargetPixel[i].Alpha = SourcePixel[Size + i].Index;
			}

			break;
		}

	case 5:
		{
			BLP_PIXEL* SourcePixel;

			SourcePixel = reinterpret_cast<BLP_PIXEL*>(&Buffer[sizeof(BLP_HEADER) + (PALETTE_SIZE * 4)]);
			TargetPixel = reinterpret_cast<BLP_RGBA*>(&TempBuffer[0]);

			for (INT i=0; i<Size; i++)
			{
				TargetPixel[i].Red = Palette[SourcePixel[i].Index].Red;
				TargetPixel[i].Green = Palette[SourcePixel[i].Index].Green;
				TargetPixel[i].Blue = Palette[SourcePixel[i].Index].Blue;
				TargetPixel[i].Alpha = 255 - Palette[SourcePixel[i].Index].Alpha;
			}

			break;
		}

	default:
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown picture type!");
			return FALSE;
		}
	}
		
	return TRUE;
}
