#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H
#include<QThread>
#include<QDebug>
#include<vector>
#include<sys/stat.h>
#include<iostream>
#include<fstream>
using namespace std;
struct FILE_INFO
{
    char* filename;
    long long int total_length;
    long long int current_length;
};

class read_file : public QThread
{
public :
    read_file();
    ~read_file();
    void stop();
    void attach_run(char* filename);
    FILE_INFO get_status();
    std::vector<char> get_bin_data();
protected:
    void run();
    bool alive = false;
    char temp_filename[1024] = {'\000'};
    std::vector<char> bin_data;
    std::vector<char> empty_data;
    long long int total_length;
    long long int current_length;
};

class write_hex : public QThread
{
public:
    write_hex();
    ~write_hex();
    void stop();
    void attach_run(char* filename,std::vector<char> bdata);
    FILE_INFO get_status();
protected:
    void run();
    bool alive = false;
    char temp_filename[1024]={'\000'};
    std::vector<char> bin_data;
    long long int total_length;
    long long int current_length;
};

class write_raw : public QThread
{
public:
    write_raw();
    ~write_raw();
    void stop();
    void attach_run(char* filename,std::vector<char> bdata);
    FILE_INFO get_status();
protected:
    void run();
    bool alive = false;
    char temp_filename[1024]={'\000'};
    std::vector<char> bin_data;
    long long int total_length;
    long long int current_length;
};

#endif // FILE_OPERATION_H
