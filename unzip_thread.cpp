#include "unzip_thread.h"

UnzipThread::UnzipThread(QObject *parent) : QThread(parent)
{
    // 初始化全局变量
    all_count=0;
    done_percent=0;
    done_count=0;
}
void UnzipThread::run()
{
    startUnzip();
    emit workFinished(); // 解压缩完成后，发出提示信号
}

void UnzipThread::setData(QString from, QString to, bool isText)
{
    this->isText=isText;
    file_from=from.toStdString();
    file_to=to.toStdString();
}

std::string UnzipThread::readCompressedFromFile(const std::string filename)
{
    emit readStart();
    std::ifstream myfile2;
    myfile2.open (filename.c_str(), std::ios::binary);
    struct stat64 filestatus;
    stat64(filename.c_str(), &filestatus );
    long long fsize = filestatus.st_size; // 获取文件包含的字节数

    std::string zeros = "00000000";
    char *c2=new char[fsize];
    myfile2.read(c2, fsize);

    std::string s = ""; // 存储文件中所有0、1的字符串
    long long count = 0;
    while(count<fsize) {
        unsigned char uc =  (unsigned char) c2[count];
        std::string p = ""; // 二进制数组成的字符串
        for (int j=0; j<8 && uc>0; j++) {
            // 包含二进制数的字符串表示每个字节
            if (uc%2==0)
                p="0"+p;
            else
                p="1"+p;
            uc=uc>>1;
        }
        p = zeros.substr(0, 8-p.size()) + p; // 如果需要的话，在左侧添“0”；p的长度为8
        s+= p;
        count++;
    }
    myfile2.close();
    return s;
}

std::vector<long long> UnzipThread::grabIntsFromCompressedString(std::string binString)
{
    std::vector<long long> result; // 保存每个字节对应的十进制整数
    std::string temp;
    for(long long i = 0; i < long(binString.length()); i++) {
        temp += binString[i];
        if(temp.length() >= 12) {
            result.push_back(binaryString2Int(temp));
            temp = "";
        }
    }
    if(temp.length() != 0) {
        ///only unused 0's remain
        ///do nothing
    }
    all_count=result.size();
    emit readDone();
    return result;
}

long long UnzipThread::binaryString2Int(std::string p)
{
    // 十进制转二进制
    long long code = 0;
    if (p.size()>0) {
        if (p.at(0)=='1')
            code = 1;
        p = p.substr(1);
        while (p.size()>0) {
            code = code << 1;
            if (p.at(0)=='1')
                code++;
            p = p.substr(1);
        }
    }
    return code;
}

void UnzipThread::writeAllBytes(std::string &message, std::string filename)
{
    emit writeStart();
    if(isText){
        std::ofstream fout(filename.c_str());
        fout << message;
        fout.close();
    }
    else{
        FILE * fptr = fopen(filename.c_str(), "wb");
        const char *code=message.c_str();
        for(long counter=0; counter<message.size(); counter++){
            fputc(int(code[counter]),fptr);
        }
        fclose(fptr);
    }
    emit writeDone();
}

template<typename T>
std::string UnzipThread::decompress(T begin, T end){
    emit unzipStart();
    // 构建字典
    int dictSize = 256;
    std::map<long long,std::string> dictionary;
    for (int i = 0; i < 256; i++){
        dictionary[i] = std::string(1, i);
    }
    std::string w(1, *begin++);
    std::string result = w;
    std::string entry;
    long double temp_count=0;
    for ( ; begin != end; begin++) {
        long long k = *begin;
        if (dictionary.count(k))
            entry = dictionary[k];
        else if (k == dictSize)
            entry = w + w[0];
        else
            throw "Bad compressed k";

        result += entry;

        // 添加w+entry[0]到字典
        if(dictSize < 4096){
            dictionary[dictSize++] = w + entry[0];
        }
        w = entry;
        temp_count+=1;
        if(temp_count-5000>0){
            done_count+=temp_count;
            temp_count=0;
            done_percent=int(done_count/all_count*100/2+25);
            emit progressChanged(done_percent);
        }
    }
    return result;
}

void UnzipThread::startUnzip()
{
    std::vector<long long> compressed;
    std::string binaryFileString = readCompressedFromFile(file_from);

    //把压缩好的数据放到vector
    compressed = grabIntsFromCompressedString(binaryFileString);

    //执行解压操作
    std::string decompressed = decompress(compressed.begin(), compressed.end());

    writeAllBytes(decompressed, file_to);
}

