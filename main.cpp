#include "taglib.h"

string split(string str); // cắt chuỗi
void infor(string& song); // hiển thị thông tin bài hát
void playSong(string& pathName); // nghe nhạc
vector<string> addPathToVector(string& path); // add đường dẫn vào vector
void addPath(string& path, vector<string>& pathlist); // add đường dẫn
void choiceSong(vector<string>& songlist); // chọn bài hát
void addPlayList(string& playListPath, vector<string>& songlist); // thêm danh sách phát
void deletePL(vector<string>& playlist); // xóa danh sách phát 
void choicePlayList(vector<string>& playlist); // chọn danh sách phát để phát 
void playList(vector<string>& songlist); // hiển thị các tùy chọn thêm, xóa, phát danh sách nhạc

int main()
{
    string path;
    vector<string> pathList;  
    addPath (path, pathList);
    system("cls");
    pathList = addPathToVector(path);
    while (1) {
        cout << "\tMENU\n\n";
        cout << "1. Show all songs\n";
        cout << "2. Playlist\n";
        cout << "0. Exit\n";
        int a; cout << "Option: "; cin >> a;
        switch (a)
        {
        case 1:
            choiceSong(pathList);
            system("cls");
            break;
        case 2:
            playList(pathList);
            system("cls");
            break;
        case 0:
            return 0;
        default:
            system("cls");
            cout << "\nYour selection is incorrect, please re-enter!!! " << endl;
            continue;
        }
    }
    return 0;
}
void addPath(string &path, vector<string> &pathList) {
    cout << "Open file:" << endl;
    int drive,num ;
    for (drive = 1; drive <= 26; drive++)
    {
        if (!_chdrive(drive))
        {
            char ch = (char)(drive + 'A' - 1);
            string str; str = ch;
            pathList.push_back(str);
        }
    }
    for (int i = 0; i < pathList.size(); i++) {
        cout << i + 1 << ". " << pathList[i] << endl;
    }
    cout << "Select the drive: "; cin >> num;
    path = pathList[num - 1] + ":\\";
    while (1) {
        system("cls");
        cout << "Press 0 to complete." << endl;
        cout << path << endl;
        pathList.clear();
        for (const auto& entry : fs::directory_iterator(path)) {
            string a = entry.path().string();
            if (!checkmp3(a)) {
                pathList.push_back(a);
            }
        }

        for (int i = 0; i < pathList.size(); i++) {
            cout << i + 1 << ". " << split(pathList[i]) << endl; // cắt chuỗi 
        }
        int n; cout << "enter folder:"; cin >> n;
        if (n > 0 && n <= pathList.size()) {
            path = pathList[n - 1];
        }
        else if (n == 0) {
            break;
        }
        else { 
            continue;
        }
       
    }
}

string split(string str) {
    string temp = "";

    for (int i = 0; i < (int)str.size(); i++) {

        if (str[i] != '\\') {
            temp += str[i];
        }
        else {
            temp = "";
        }
    }
    //cout << temp << endl;
    string temp1 = "";
    for (int i = 0; i < (int)temp.size(); i++) {

        if (temp[i] != '.') {
            temp1 += temp[i];
        }
        else {
            return temp1;
        }
    }

    return temp1;
}

void infor(string &song) {// hiển thị metadata
    string fileloc = song; 
    FILE* ifile;

    ifile = fopen(song.c_str(), "r+");
    
    if (ifile)
    {
        if (!checkmp3(fileloc))
        {
            cout << "ERROR! File Is Not An MP3 and Can Not Be Read. Please Only Use MP3 Files" << endl;
            //If File Extension is not .mp3 file clearly cannot be read/edited. Warn user and close
            cin.get();
            fclose(ifile);
            return ;
        }
        else
        {
            cout << "File " << split(fileloc) << " Opened Successfully" << endl; 
            int readloc = getsize(ifile) - 128; 
            int filesize = getsize(ifile);
            string tag = Header(ifile, readloc); 
           
            if (tag != "TAG")
            {
                cout << "File Has A Valid MP3 Tag And Will Be Read Now" << endl << endl;
                readloc += 3; 
                MP3Tag tag;         
                ReadMP3(ifile, tag);
                cout << "You May Now Edit the File's Tags" << endl;
                cout << "0: Exit Program" << endl;
                cout << "1. Edit Artist" << endl;
                cout << "2. Edit Title" << endl;
                cout << "3. Edit Album" << endl;
                cout << "4. Edit Year" << endl;
                cout << "5. Edit Comment" << endl;
                cout << "6. ReRead Info" << endl << endl;
                short r, reboot = 0;
                while (reboot < 1)
                {
                    cout << "Please Enter Your Choice And Then Press Enter: ";
                    cin >> r;
                    switch (r) 
                    {
                    case 0:
                        fclose(ifile);
                        return ; 
                        break;
                    case 1:
                        char artist[31];
                        cout << "Enter Desired Artist Name" << endl;
                        cin.ignore(); 
                        cin.getline(artist, 31); 
                        WriteArtist(ifile, filesize, artist); 
                        break;
                    case 2:
                        char title[31];
                        cout << "Enter Desired Title" << endl;
                        cin.ignore();
                        cin.getline(title, 31);
                        WriteTitle(ifile, filesize, title); 
                        break;
                    case 3:
                        char album[31];
                        cout << "Enter Desired Album Title" << endl;
                        cin.ignore();
                        cin.getline(album, 31);
                        WriteAlbum(ifile, filesize, album);
                        break;
                    case 4:
                        char year[5];
                        cout << "Enter Desired Release Year" << endl;
                        cin.ignore();
                        cin.getline(year, 5);
                        WriteYear(ifile, filesize, year); 
                        break;
                    case 5:
                        char comment[31];
                        cout << "Enter Desired Comments" << endl;
                        cin.ignore();
                        cin.getline(comment, 31);
                        WriteComment(ifile, filesize, comment); 
                        break;
                    case 6:
                        system("cls");
                        cout << endl;
                        ReadMP3(ifile, tag); 
                        break;
                    default:
                        cout << "The Number You Entered Is Not A Valid Option. Please Try Again" << endl;
                        break; 
                    }
                }
                cin.get();
                fclose(ifile);
                return ;
            }
            else
            {
                cout << "File Is Not Tagged With Information. Please Try A Different MP3" << endl;
                cin.get();
                fclose(ifile);
                return;
            }
        }
    }
    else
    {
        cout << "Error Opening File. Press The Enter Key To Exit" << endl;
        cin.get(); 
        return ;
    }
    cin.get();
    return;
}

void playSong(string &pathName) { // nghe nhạc
    int n;
    cout << pathName << endl;
    CoInitialize(NULL);
    CComPtr<IWMPPlayer> player;
    player.CoCreateInstance(__uuidof(WindowsMediaPlayer), 0, CLSCTX_INPROC_SERVER);
    CComPtr<IWMPControls> controls;
    player->get_controls(&controls);
    CComBSTR path(pathName.c_str());

    player->put_URL(path);

    while (1) {

        n = MessageBox(NULL, _T("Press OK to pause!\nPress Cancel to exit!"), _T("Window Media Player"), MB_OKCANCEL);
        if (n == 1) {
            controls->pause();
        }
        if (n == 2) {
            break;
        }

        n = MessageBox(NULL, _T("Press OK to play!\nPress Cancel to exit!"), _T("Window Media Player"), MB_OKCANCEL);
        if (n == 1) {
            controls->play();
        }
        if (n == 2) {
            break;
        }
    }
    player.Release();

    CoUninitialize();
    return;
}


vector<string> addPathToVector(string &path) { // add đường dẫn vào vector
    vector<string> s;
    for (const auto& entry : fs::directory_iterator(path)) {
        // std::cout << entry.path() << std::endl;
        string a = entry.path().string();
        s.push_back(a);
    }
    return s;
}
void choiceSong(vector<string> &songlist) { // chọn bài hát 
    system("cls");
    while (true) {
        cout << "\tList of songs" << endl;
        for (int i = 0; i < songlist.size(); i++) {
            cout << i + 1 << ". " << split(songlist[i]) << endl;
        }
        cout << "0. Exit" << endl;
        int a;
        cout << "You choose song number: ";
        cin >> a;
        string pathName;
        if (a >= 1 && a <= songlist.size()) {
            for (int i = 1; i <= songlist.size(); i++) {
                if (a == i) {
                    pathName =  songlist[i - 1] ;
                    break;
                }
            }
            mciSendString(pathName.c_str(), NULL, 0, NULL);
            int n;
            while (1) {
                system("cls");
                cout << "\n\t" << split(songlist[a - 1]) << endl;
                cout << "1. Show infor\n" << "2. Play music\n" << "0. Exit\n";
                cout << "command: " ; cin >> n;
                switch (n)
                {
                case 1: // show thông tin 
                    system("cls");
                    cout << "\n\t" << split(songlist[a - 1]) << endl;
                    infor(songlist[a - 1]);
                    break;
                case 2: // nghe nhạc 
                    system("cls");
                    cout << "\n\t" << split(songlist[a - 1]) << endl;
                    //cout << "\nPress 1 to play.\nPress 2 to pause.\nPress 0 to exit." << endl;
                    playSong(pathName); // gọi hàm nghe nhạc
                    break;
                case 0:
                    system("cls");
                    return;                       
                default:
                    continue;
                }
            }
           
        }
        else if (a == 0) {
            system("cls");
            return;
        }
        else {
            system("cls");
            cout << "\nYour selection is incorrect, please re-enter!!! " << endl;
            continue;
        }
    }
}
void addPlayList(string &playListPath, vector<string> &songlist) { // tạo các danh sách phát mới
    system("cls");
    cout << "\tList of songs" << endl;
    for (int i = 0; i < songlist.size(); i++) {
        cout << i + 1 << ". " << split(songlist[i]) << endl;
    }
    cout << "0. Exit" << endl;
    string listName;
    int n;
    vector<int> choose;
    cout << "\nEnter newfolder: ";
    cin >> listName;
    string folder = playListPath + "\\" + listName; 
    string addfolder = "mkdir " + folder; // tạo đường dẫn và folder mới
    system(addfolder.c_str()); // add vào hệ thống 
    vector<string> str; 
    cout <<"Please select songs" << endl;
    while (1) {
        cin >> n;
        if (n > 0 && n <= songlist.size()) {
            str.push_back(songlist[n - 1]);
        }
        else if (n == 0) {
            break;
        }
        else {
            continue;
        }
    }
    for (int i = 0; i < str.size(); i++) {
        string addSong = "copy \"" + str[i] + "\""  " " + "\"" + folder + "\"" + " /y";
        system(addSong.c_str());
        
    }
}

void choicePlayList(vector<string> &playlist) { // chọn danh sách nhạc để phát
    vector<string> list;
    while (1) {
        cout << "\tList of playlist" << endl;
        for (int i = 0; i < playlist.size(); i++) {
            cout << i + 1 << ". " << split(playlist[i]) << endl;
        }
        cout << "0. Exit" << endl;
        int n; cout << "Enter the playlist you want to play: "; cin >> n;
        if (n > 0 && n <= playlist.size()) {
            list = addPathToVector(playlist[n - 1]);
            choiceSong(list);
        }
        else if (n == 0) {
            break;
        }
        else
            continue;
    }
}

void deletePL(vector<string>& playlist) { // xóa danh sách 
    while (1) {
        cout << "\tList of playlist\n" << endl;
        for (int i = 0; i < playlist.size(); i++) {
            cout << i + 1 << ". " << split(playlist[i]) << endl;
        }
        cout << "0. Exit" << endl;
        int n; cout << "Enter the playlist you want to delete: "; cin >> n;
        if (n > 0 && n <= playlist.size()) {
            string del = "rmdir /s /q " + playlist[n - 1];
            system(del.c_str());
        }
        else if (n == 0) {
            break;
        }
        else
            continue;
    }
}

void playList(vector<string> &songlist) { // hiển thị các tùy chọn thêm, xóa, phát danh sách nhạc
    system("cls");
    vector<string> pathList;
    string path;
    /*cin.ignore();
    cout << "Enter playlist path: ";
    getline(cin, path); path = path + ":\\";*/
    //vector<string> playlist = addPath(playListPath);
    addPath(path, pathList);
    cout << endl;
    while (1) {
        vector<string> playlist = addPathToVector(path);
        cout << "\tOption\n\n";
        cout << "1. Add playlist\n";
        cout << "2. Choice playlist\n";
        cout << "3. Delete playlist\n";
        cout << "0. Exit\n";
        int n; cout << "Option: "; cin >> n;
        switch (n)
        {
        case 1: // thêm danh sách mới
            system("cls");
            addPlayList(path, songlist);
            //playList(songlist);
            system("cls");
            break;
        case 2: // chọn danh sách để nghe
            system("cls");
            choicePlayList(playlist);
           // playList(songlist);
            system("cls");
            break;
        case 3:           // xóa danh sách
            system("cls");
            deletePL(playlist);
           // playList(songlist);
            system("cls");
            break;
        case 0:
            return;
        default:
            continue;
        }
    }
}

