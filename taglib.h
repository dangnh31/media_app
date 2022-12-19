#ifndef taglib_h
#define taglib_h

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>  
#include <string> 
#include <windows.h>

//windows media player sdk
#include <atlbase.h>
#include <atlwin.h>
#include <wmp.h>

//#include <Mmsystem.h>
//#include <mciapi.h>
//#include <cstddef>

#include <filesystem> //thư viện để lấy đường dẫn

// thư viện để lấy drive
#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <ctype.h>

#pragma comment(lib, "Winmm.lib")
namespace fs = std::filesystem;
using namespace std;

struct MP3Tag
{
	string Artist;
	string Title;
	string Album;
	string Year;
	string Comment;
};

int getsize(FILE* file)
{	
	if (file) {
		int loc = ftell(file);
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		fseek(file, loc, SEEK_SET);
		return size;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}

bool checkmp3(string filename)
{
	int filetype = filename.length() - 4;
	string filetipe = filename.substr(filetype, 4);
	if (filetipe == ".mp3")
	{
		return true;
	}
	else
	{
		return false;
	}
}

string Header(FILE* file, int readloc)
{
	if (file) {
		char magic[4];
		fseek(file, readloc, SEEK_SET);
		fread(magic, 1, 4, file);
		string str(magic);
		string mag = str.substr(0, 3);
		return mag;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
string Year(FILE* file, int readloc)
{	
	if (file) {
		char magic[4];
		fseek(file, readloc, SEEK_SET);
		fread(magic, 1, 4, file);
		string str(magic);
		return str;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
string ReadThirty(FILE* file, int readloc)
{
	if (file) {
		char magic[30];
		fseek(file, readloc, SEEK_SET);
		fread(magic, 1, 30, file);
		string str(magic);
		return str;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
void ReadMP3(FILE* file, MP3Tag tag)
{
	if (file) {
		int readloc = getsize(file) - 125;
		tag.Title = ReadThirty(file, readloc);
		readloc += 30;
		tag.Artist = ReadThirty(file, readloc);
		readloc += 30;
		tag.Album = ReadThirty(file, readloc);
		readloc += 30;
		tag.Year = Year(file, readloc);
		readloc += 4;
		tag.Comment = ReadThirty(file, readloc);
		cout << "Artist: " << tag.Artist << endl;
		cout << "Title: " << tag.Title << endl;
		cout << "Album: " << tag.Album << endl;
		cout << "Year: " << tag.Year << endl;
		cout << "Comment: " << tag.Comment << endl << endl;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
void WriteArtist(FILE* file, int filesize, char Artist[])
{
	if (file) {
		int writeloc = filesize - 95;
		fseek(file, writeloc, SEEK_SET);
		fwrite(Artist, 1, 30, file);
		cout << Artist << " set as The Artist" << endl;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
void WriteTitle(FILE* file, int filesize, char Title[])
{
	if (file) {
		int writeloc = filesize - 125;
		fseek(file, writeloc, SEEK_SET);
		fwrite(Title, 1, 30, file);
		cout << Title << " set as The Title" << endl;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
void WriteAlbum(FILE* file, int filesize, char Album[])
{
	if (file) {
		int writeloc = filesize - 65;
		fseek(file, writeloc, SEEK_SET);
		fwrite(Album, 1, 30, file);
		cout << Album << " set as The Album" << endl;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
void WriteYear(FILE* file, int filesize, char Year[])
{
	if (file) {
		int writeloc = filesize - 35;
		fseek(file, writeloc, SEEK_SET);
		fwrite(Year, 1, 4, file);
		cout << Year << " set as The Release Year" << endl;
	}
	else{
		cout << "Error! file pointer is null." << endl;
	}
}
void WriteComment(FILE* file, int filesize, char Comment[])
{
	if (file) {
		int writeloc = filesize - 31;
		fseek(file, writeloc, SEEK_SET);
		fwrite(Comment, 1, 30, file);
		cout << Comment << " set as The Comment" << endl;
	}
	else {
		cout << "Error! file pointer is null." << endl;
	}
}
#endif // !taglib_h
