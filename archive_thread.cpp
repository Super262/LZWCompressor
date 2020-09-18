#include "archive_thread.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sys/stat.h>

ArchiveThread::ArchiveThread(QObject *parent) :QThread(parent)
{
    // 初始化全局变量
    all_count=0;
    done_percent=0;
}
void ArchiveThread::run()
{
    startCompress();
    emit workFinished(); //在压缩完成后，发出完成信号，指示主线程刷新GUI
}

void ArchiveThread::setData(QString from, QString to,bool isText)
{
    // 指定输入路径、输出路径
    file_from=from.toStdString();
    file_to=to.toStdString();
    this->isText=isText;
}


template <typename T>
T ArchiveThread::compress(const std::string &uncompressed, T result) {

    long long dictSize = 256; // 字典当前长度
    long long maxSize =4096 ; // 字典最大长度；超过此长度后，程序重建字典

    std::map<std::string, long long> dictionary; // 字典，使用map实现

    for (int i = 0; i < 256; i++){
        // 初始化字典，添加全部的ASCII码作为初始编码
        dictionary[std::string(1, i)] = i;
    }
    std::string w; // 前缀
    all_count=uncompressed.size(); // 确定待压缩字符的总数
    done_count=0; // 当前已被压缩的字符数
    long double temp_count=0; // 临时计数器
    if(all_count>0){
        done_percent=int(done_count/all_count*100); // 初始化百分比
    }
    emit archiveStart(); //在压缩开始后，发出完成信号，指示主线程刷新GUI
    for (std::string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it) {
        char c = *it; // 后缀
        std::string wc = w + c;
        if (dictionary.count(wc)) // 字典中有wc
            w = wc;
        else { // 字典中没有wc
            *result++ = dictionary[w];
            // 把wc及其编码添加到字典
            if(dictSize < maxSize){
                dictionary[wc] = dictSize++;
            }
            w = std::string(1, c); // 更新前缀
        }
        temp_count+=1;
        if((long long)temp_count % 5000==0){ // 每处理5000个字符后
            done_count+=temp_count;
            temp_count=0;
            done_percent=int(done_count/all_count*100/2+25);
            emit progressChanged(done_percent); // 发出完成信号，指示主线程刷新GUI
        }
    }

    // 输出w的编码
    if (!w.empty())
        *result++ = dictionary[w];
    return result;
}

std::string ArchiveThread::int2BinaryString(long long c, long long cl) {
    std::string p = "";
    while (c>0) { // 十进制整数转二进制字符串
        if (c%2==0)
            p="0"+p;
        else
            p="1"+p;
        c=c>>1;
    }
    long long zeros = cl-p.size(); // 判断当前的长度是否达标
    if (zeros<0) {
        std::cout << "\nWarning: Overflow. code is too big to be coded by " << cl <<" bits!\n";
        p = p.substr(p.size()-cl);
    }
    else {
        for (long long i=0; i<zeros; i++)  //在左边补0
            p = "0" + p;
    }
    return p;
}


void ArchiveThread::writeCompressedToFile(std::vector<long long> compressed, const char* file) {

    std::string bcode= ""; // 压缩后的二进制数据
    std::string p= ""; // 临时变量
    long long bits; // 编码长度

    // 遍历存储压缩后的字节数据的vector，累加其中的所有的元素，组成压缩后的二进制数据
    for (std::vector<long long>::iterator it = compressed.begin() ; it != compressed.end(); ++it) {
        bits = 12; // 12位编码
        p = int2BinaryString(*it, bits); // 二进制数据
        bcode+=p; // 保存二进制数据
    }
    std::ofstream myfile; // 初始化输出流
    myfile.open(file,  std::ios::binary);
    emit writeStart(); // 发出提示信号
    std::string zeros = "00000000";
    if (bcode.size()%8!=0)
    {
        // 确保二进制字符串的长度可以被8整除
        bcode += zeros.substr(0, 8-bcode.size()%8);
    }
    int b;
    for (unsigned int i = 0; i < bcode.size(); i+=8) {
        b = 1;
        for (unsigned long j = 0; j < 8; j++) {
            b = b<<1;
            if (bcode.at(i+j) == '1')
                b+=1;
        }
        char c = (char) (b & 255); // 逐字节地保存由二进制数构成的字符串
        myfile.write(&c, 1);
    }
    myfile.close(); // 关闭文件，释放资源
    emit writeDone(); // 发出提示信号
}

std::string ArchiveThread::readAllBytes(char const* filename) {
    emit readStart(); // 发出提示信号
    if(isText){
        // 是文本文件
        std::ifstream f(filename);
        std::string theString((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();
        emit readDone(); // 发出提示信号
        return theString;
    }
    else{
        // 不是文本
        FILE * fptr = fopen(filename, "rb");
        std::string file_str="";
        fseek(fptr, 0, SEEK_END);
        long fileLen=ftell(fptr); // 得到图像文件的大小
        fseek(fptr, 0, SEEK_SET);
        for(long counter=0; counter<fileLen; counter++){
            std::string s="a";
            s[0]=char(fgetc(fptr));
            file_str.append(s);
        }
        emit readDone(); // 发出提示信号
        return file_str;
    }

}

void ArchiveThread::startCompress()
{
    std::vector<long long> compressed;

    // 保存文件名到局部变量
    const char* cstrFilename = file_from.c_str();
    const char* cstrNewFilename = file_to.c_str();

    //保存文件内容到字符串
    std::string message = readAllBytes(cstrFilename);

    //开始压缩
    compress(message, std::back_inserter(compressed));

    // 保存压缩文件
    writeCompressedToFile(compressed, cstrNewFilename);

}
