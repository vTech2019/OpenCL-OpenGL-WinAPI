#include "Load_Save_Images.h"

std::vector<std::wstring> WIN_get_name_folder_files(const wchar_t* name_folder) {
	const std::filesystem::path workdir = std::filesystem::current_path();
	const wchar_t* directory = (const wchar_t*)workdir.c_str();
	std::wstring dir(directory);
	dir.push_back(L'\\');
	dir += (name_folder);
	std::vector<std::wstring> dir_files;
	for (const auto & entry : std::filesystem::directory_iterator(dir)) {
		std::wstring dir_file(entry.path());
		dir_file.push_back(0);
		dir_files.push_back(dir_file);
	}
	return dir_files;
}

dataImage WIN_load_image(const WCHAR* name) {
	dataImage image;
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartupOutput output;
	ULONG_PTR token;
	Gdiplus::GdiplusStartup(&token, &input, &output);
	Gdiplus::Color color;
	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(name);
	image.height = bitmap->GetHeight();
	image.width = bitmap->GetWidth();
	Gdiplus::BitmapData bitmapData = {};
	Gdiplus::Rect rect(0, 0, image.width, image.height);
	bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	RGBA*  pixels = (RGBA*)bitmapData.Scan0;
	UINT stride = abs(bitmapData.Stride) / 4;
	image.stride = bitmapData.Stride;
	image.pixelFormat = PixelFormat32bppRGB;
	image.data = (RGBA*)_mm_malloc(image.stride * image.height, 32);
	for (UINT row = 0; row < image.height; ++row)
		for (UINT col = 0; col < image.width; ++col)
		{
			image.data[row *	stride + col] = pixels[row * stride + col];
		}
	bitmap->UnlockBits(&bitmapData);
	delete bitmap;
	Gdiplus::GdiplusShutdown(token);

	return image;
}

void WIN_save_image(dataImage& image, const WCHAR* directory_name,  const WCHAR* name, size_t format) {


	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartupOutput output;
	ULONG_PTR token;
	Gdiplus::GdiplusStartup(&token, &input, &output);
	Gdiplus::Color color;
	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(image.width, image.height, image.stride, image.pixelFormat, (BYTE*)image.data);
	size_t length_dir;
	size_t length_file_name;

	CLSID ñlsid;
	const WCHAR* _name = name;
	for (length_dir = 0; directory_name[length_dir] != 0; length_dir++)
		length_dir++;
	for (length_file_name = 0; name[length_file_name] != 0; length_file_name++)
		length_file_name++;
	WCHAR * name_file = (WCHAR*)alloca((length_file_name + length_dir + 6) * sizeof(WCHAR));
	for (size_t i = 0; i < length_dir; i++)
		name_file[i] = directory_name[i];
	for (size_t i = length_dir; i < length_dir + length_file_name; i++)
		name_file[i] = name[i- length_dir];
	size_t length = length_dir + length_file_name;
	switch (format) {
	case BMP:
		name_file[length] = L'.';
		name_file[length + 1] = L'b';
		name_file[length + 2] = L'm';
		name_file[length + 3] = L'p';
		name_file[length + 4] = 0;
		CLSIDFromString(_BMP, &ñlsid);
		bitmap->Save(name_file, &ñlsid, NULL);
		break;
	case JPG:
		name_file[length] = L'.';
		name_file[length + 1] = L'j';
		name_file[length + 2] = L'p';
		name_file[length + 3] = L'g';
		name_file[length + 4] = 0;
		CLSIDFromString(_JPG, &ñlsid);
		bitmap->Save(name_file, &ñlsid, NULL);
		break;
	case PNG:
		name_file[length] = L'.';
		name_file[length + 1] = L'p';
		name_file[length + 2] = L'n';
		name_file[length + 3] = L'g';
		name_file[length + 4] = 0;
		CLSIDFromString(_PNG, &ñlsid);
		bitmap->Save(name_file, &ñlsid, NULL);
		break;
	case TIF:
		name_file[length] = L'.';
		name_file[length + 1] = L't';
		name_file[length + 2] = L'i';
		name_file[length + 3] = L'f';
		name_file[length + 4] = L'f';
		name_file[length + 5] = 0;
		CLSIDFromString(_TIF, &ñlsid);
		bitmap->Save(name_file, &ñlsid, NULL);
		break;
	case GIF:
		name_file[length] = L'.';
		name_file[length + 1] = L'g';
		name_file[length + 2] = L'i';
		name_file[length + 3] = L'f';
		name_file[length + 4] = 0;
		CLSIDFromString(_GIF, &ñlsid);
		bitmap->Save(name_file, &ñlsid, NULL);
		break;
	}
	delete bitmap;
	Gdiplus::GdiplusShutdown(token);
	_mm_free(image.data);
}