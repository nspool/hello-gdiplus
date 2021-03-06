// NativeDraw.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#include <iostream>
#include <chrono>
#include <string>

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <Pathcch.h>

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"Pathcch.lib")

int main()
{
	// GDI+ Example

	// Encoders for output. Only need one at the moment.
	// List from: https://stackoverflow.com/questions/1584202/gdi-bitmap-save-problem

	// bmp: {557cf400 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
	// jpg: {557cf401 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
	// gif: {557cf402 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
	// tif: {557cf405 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
	// png: {557cf406 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}

	CLSID pngClsid;
	CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);

	// Load a bitmap
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wchar_t result[MAX_PATH];
	GetModuleFileName(NULL, result, MAX_PATH);
	PathCchRemoveFileSpec(result, MAX_PATH);

	std::wstring path = std::wstring(result, wcslen(result));
	std::wstring inpath = path + L"\\Test.BMP";
	std::wstring outpath = path + L"\\Out.PNG";

	try
	{
		for (int i = 0; i < 100; i++)
		{
			Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromFile(inpath.c_str());
			Gdiplus::Font *f = new Gdiplus::Font(L"Times new roman", 16);

			auto start = std::chrono::steady_clock::now();

			Gdiplus::Graphics *gc = Gdiplus::Graphics::FromImage(bmp);

			Gdiplus::SolidBrush b(Gdiplus::Color(255, 0, 0, 0));
			Gdiplus::Pen blackPen(Gdiplus::Color(155, 0, 0, 0), 3);
			Gdiplus::RectF layoutRect(125.0f, 125.0f, 175.0f, 75.0f);

			gc->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			//gc->SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
			//gc->SetCompositingQuality(Gdiplus::CompositingQualityHighSpeed);
			//gc->SetPixelOffsetMode(Gdiplus::PixelOffsetModeNone);
			//gc->SetSmoothingMode(Gdiplus::SmoothingModeNone);
			//gc->SetInterpolationMode(Gdiplus::InterpolationModeDefault);
			gc->DrawString(L"Hello World", 11, f, layoutRect, &Gdiplus::StringFormat(), &b);

			gc->DrawRectangle(&blackPen, 100.0f, 100.0f, 200.0f, 100.0f);

			auto end = std::chrono::steady_clock::now();

			bmp->Save(outpath.c_str(), &pngClsid, NULL);

			delete f;
			delete gc;

			auto diff = end - start;

			std::cout << "Time Taken: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
		}
	}
	catch (...)
	{
		// 
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	std::cout << "Press any key to exit" << std::endl;
	std::cin.ignore();

	return 0;
}

