#include "file_operation.h"

read_file::read_file()
{

}
void read_file::attach_run(char *filename)
{
    bin_data.clear();
    for(auto i=0;i<1024;i++)
    {
        temp_filename[i]='\000';
    }
    FILE* f1=fopen(filename,"rb");
    if(!f1)
    {
        return;
    }
    else
    {
        fclose(f1);
        int length = 0;
        qDebug()<<"filename="<<filename<<Qt::endl;
        length = strlen(filename);
        for(auto i=0;i<length;i++)
        {
            temp_filename[i]=filename[i];
        }
    }
}

read_file::~read_file()
{
    //delete temp_filename;
    bin_data.clear();
}

void read_file::stop()
{
    if(alive)
    {
        alive = !alive;
    }
}

FILE_INFO read_file::get_status()
{
    //qDebug()<<total_length<<"\\"<<current_length<<Qt::endl;
    FILE_INFO a;
    a.filename = temp_filename;
    a.current_length = current_length;
    a.total_length = total_length;
    //qDebug()<<total_length<<"\\"<<current_length<<Qt::endl;
    return a;
}

std::vector<char> read_file::get_bin_data()
{
    if(bin_data.size()==total_length)
    {
        return bin_data;
    }
    return empty_data;
}

void read_file::run()
{
    try{
    alive = true;
    ifstream infile;
    infile.open(temp_filename,ios::in | ios::binary);
    qDebug()<<temp_filename<<Qt::endl;
    if(!infile.is_open())
    {
        return;
    }
    //read file length
    struct stat statbuf;
    stat(temp_filename,&statbuf);
    size_t filesize = statbuf.st_size;
    total_length = (long long int)filesize;
    current_length = 0;
    char c;
    while(infile.read((char*)&c,sizeof(c)))
    {
        bin_data.push_back(c);
        current_length++;
        //qDebug()<<total_length<<"\\"<<current_length<<Qt::endl;
        if(!alive)
        {
            break;
        }
    }
    }
    catch(_exception e)
    {
    qDebug()<<"exception catched"<<&e<<Qt::endl;
    }
}

write_hex::write_hex()
{

}

void write_hex::attach_run(char *filename, std::vector<char> bdata)
{
    bin_data.clear();
    remove(filename);
    bin_data = bdata;
    for(auto i=0;i<1024;i++)
    {
        temp_filename[i] = '\000';
    }
    int length;
    length = strlen(filename);
    for(int j = 0;j<length;j++)
    {
        temp_filename[j] = filename[j];
    }
    qDebug()<<temp_filename<<Qt::endl;
}

write_hex::~write_hex()
{
    bin_data.clear();
}

void write_hex::stop()
{
    if(alive)
    {
        alive = !alive;
    }
}

FILE_INFO write_hex::get_status()
{
    FILE_INFO a;
    a.filename = temp_filename;
    a.current_length = current_length;
    a.total_length = total_length;
    return a;
}

void write_hex::run()
{
    total_length = bin_data.size();
    if(total_length==0)
    {
        return;
    }
    alive = true;
    FILE* f1 = fopen(temp_filename,"ab+");
    if(!f1)
    {
        return;
    }
    long long int i = 0;
    int k = 0;
    char c;
    int ch;
    char temp[8]={'\000'};
    long long int temp_num = 0;
    for(auto j=0;j<bin_data.size();j++)
    {
        current_length = i;
        if(i%8==0)
        {
            fprintf(f1,"0x%08X\t",i);
        }
        temp_num = temp_num + 1;
        ch = bin_data[i]>=0?bin_data[i]:256+bin_data[i];
        fprintf(f1,"%02X ",ch);
        temp[i%8]=bin_data[i];
        i++;
        current_length = i;
        if(i%8==0)
        {
            fprintf(f1,"\t");
            for(k = 0;k<8;k++)
            {
                if(temp[k]=='\n')
                {
                    fprintf(f1,"\\n. ");
                }
                else
                {
                    fprintf(f1,"%c. ",temp[k]);
                }
            }
            for(k=0;k<8;k++)
            {
                temp[k]='\000';
            }
            fprintf(f1,"\n");
        }
    }
    fprintf(f1,"\t\t");
    for(k=0;k<8;k++)
    {
        if(temp[k]=='\000')
        {
            break;
        }
        else
        {
            if(temp[k]=='\n')
            {
                fprintf(f1,"\\n. ");
            }
            else
            {
                fprintf(f1,"%c. ",temp[k]);
            }
        }
    }
    fclose(f1);
}


write_raw::write_raw()
{

}

void write_raw::attach_run(char *filename, std::vector<char> bdata)
{
    bin_data.clear();
    remove(filename);
    bin_data = bdata;
    for(auto i=0;i<1024;i++)
    {
        temp_filename[i] = '\000';
    }
    int length;
    length = strlen(filename);
    for(int j = 0;j<length;j++)
    {
        temp_filename[j] = filename[j];
    }
}

write_raw::~write_raw()
{
    bin_data.clear();
}

void write_raw::stop()
{
    if(alive)
    {
        alive = !alive;
    }
}

FILE_INFO write_raw::get_status()
{
    FILE_INFO a;
    a.filename = temp_filename;
    a.current_length = current_length;
    a.total_length = total_length;
    return a;
}

void write_raw::run()
{
    alive = true;
    FILE* fp = fopen(temp_filename,"ab+");
    if(!fp)
    {
        return;
    }
    else
    {
        total_length = bin_data.size();
        for(auto i=0;i<bin_data.size();i++)
        {
            current_length = i;
            fwrite((char*)&bin_data[i],sizeof(char),1,fp);
        }
        current_length = bin_data.size();
        fclose(fp);
    }
}
