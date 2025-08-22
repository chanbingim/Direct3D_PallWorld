#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CBinaryHelper : public CBase
{
private :
	CBinaryHelper();
	virtual ~CBinaryHelper() = default;

public :
	// ======================================================
	//|   ███████╗██╗  ██╗██████╗  ██████╗ ██████╗ ████████╗ |
	//|   ██╔════╝╚██╗██╔╝██╔══██╗██╔═══██╗██╔══██╗╚══██╔══╝ |
	//|   █████╗   ╚███╔╝ ██████╔╝██║   ██║██████╔╝   ██║    |
	//|   ██╔══╝   ██╔██╗ ██╔═══╝ ██║   ██║██╔══██╗   ██║    |
	//|   ███████╗██╔╝ ██╗██║     ╚██████╔╝██║  ██║   ██║    |
	//|   ╚══════╝╚═╝  ╚═╝╚═╝      ╚═════╝ ╚═╝  ╚═╝   ╚═╝    |
	// ======================================================
	template <typename T>
	void	ExportBinadry(const char* FileName, T* In);

	template <typename T>
	void	ExportBinadry(const char* FileName, T* In, _uInt iLength);

	template <typename T>
	void	ExportBinadry(const char* FileName, vector<T*>* In);
	
	// ====================================================
	//|   ██╗███╗   ███╗██████╗  ██████╗ ██████╗ ████████╗ |
	//|   ██║████╗ ████║██╔══██╗██╔═══██╗██╔══██╗╚══██╔══╝ |
	//|   ██║██╔████╔██║██████╔╝██║   ██║██████╔╝   ██║    |
	//|   ██║██║╚██╔╝██║██╔═══╝ ██║   ██║██╔═══╝    ██║    |
	//|   ██║██║ ╚═╝ ██║██║     ╚██████╔╝██║        ██║    |
	//|   ╚═╝╚═╝     ╚═╝╚═╝      ╚═════╝ ╚═╝        ╚═╝    |
	// ====================================================
	template <typename T>
	void	ImportBinadry(const char* FileName, T* pOut);

	template <typename T>
	void	ImportBinadry(const char* FileName, T** pOut, _uInt iLength, _bool bDyanmic);

	template <typename T>
	void	ImportBinadry(const char* FileName, vector<T*>* pOut);
};
NS_END

template<typename T>
inline void CBinaryHelper::ExportBinadry(const char* FileName, T* In)
{
	ofstream file;
	ios_base::openmode flag;
	flag = ios::out | ios::trunc | ios::binary;

	open(FileName, flag);
	if (file.is_open())
		file.write(reinterpret_cast<char*>(In), sizeof(T));

	file.close();
}

template<typename T>
inline void CBinaryHelper::ExportBinadry(const char* FileName, T* In, _uInt iLength)
{
	ofstream file;
	ios_base::openmode flag;
	flag = ios::out | ios::trunc | ios::binary;

	open(FileName, flag);
	if (file.is_open())
	{
		for (_uInt i = 0; i < iLength; ++i)
		{
			file.write(reinterpret_cast<char*>(&In[i]), sizeof(T));
		}
	}

	file.close();
}

template<typename T>
inline void CBinaryHelper::ExportBinadry(const char* FileName, vector<T*>* In)
{
	ofstream file;
	ios_base::openmode flag;
	flag = ios::out | ios::trunc | ios::binary;

	open(FileName, flag);
	if (file.is_open())
	{
		for (auto iter : *In)
		{
			file.write(reinterpret_cast<char*>(iter), sizeof(T));
		}
	}

	file.close();
}

template<typename T>
inline void CBinaryHelper::ImportBinadry(const char* FileName, vector<T*>* pOut)
{
	ifstream file;
	ios_base::openmode flag;
	flag = ios::In | ios::binary;

	open(FileName, flag);

	file.seekg(0, ios::end);
	streamsize size = file.tellg();
	file.seekg(0, ios::beg);

	_longlong DataCnt = size / sizeof(T);
	(*pOut).reserve(DataCnt);

	if (file.is_open())
	{
		T ReadData;
		file.
		while (!file.eof())
		{
			file.read(reinterpret_cast<char*>(ReadData), sizeof(T));
			(*pOut).push_back(T);
		}
	}

	file.close();
}

template<typename T>
inline void CBinaryHelper::ImportBinadry(const char* FileName, T* pOut)
{
	ifstream file;
	ios_base::openmode flag;
	flag = ios::In | ios::binary;

	open(FileName, flag);
	if (file.is_open())
		file.read(reinterpret_cast<char*>(pOut), sizeof(T));

	file.close();
}

template<typename T>
inline void CBinaryHelper::ImportBinadry(const char* FileName, T** pOut, _uInt iLength, _bool bDyanmic)
{
	ifstream file;
	ios_base::openmode flag;
	flag = ios::In | ios::binary;

	if(bDyanmic)
		(*pOut) = new T[iLength];

	open(FileName, flag);
	if (file.is_open())
	{
		_uInt i = 0;
		while (!file.eof())
		{
			if (iLength <= i)
				break;

			file.read(reinterpret_cast<char*>(&(*pOut)[i]), sizeof(T));
			i++;
		}
	}

	file.close();
}