#ifndef UNZIP_THREAD_H
#define UNZIP_THREAD_H

#include <QObject>
#include <QThread>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sys/stat.h>

class UnzipThread :public QThread
        // 继承自QThread线程类，使解压缩操作在子线程中进行
{
    Q_OBJECT
public:
    explicit UnzipThread(QObject *parent = nullptr);

    void run(); // 重写void run();

    void setData(QString from, QString to, bool isText); // 初始化必要的变量

signals:
    // 各种信号，用以提示GUI做出响应
    void readStart();
    void readDone();
    void progressChanged(int value);
    void unzipStart();
    void writeStart();
    void writeDone();
    void workFinished();

private:
    long double all_count,done_count; // 已经处理的字节个数和总字节个数

    int done_percent; // 已经完成的百分比

    std::string file_from, file_to; // 源文件地址和目标文件的地址

    bool isText; // 文件类型（True：文本文件，False：图像文件）

    // 解压缩
    template <typename T> std::string decompress(T begin, T end);

    // 读取压缩文件
    std::string readCompressedFromFile(const std::string filename);

    // 从压缩文件的二进制序列中得到整数序列
    std::vector<long long> grabIntsFromCompressedString(std::string binString);

    // 把由二进制数组成的字符串转换为十进制整数
    long long binaryString2Int(std::string p);

    // 向文件中写入字节
    void writeAllBytes(std::string &message, std::string filename);

    // 分布完成解压缩操作
    void startUnzip();

};

#endif // UNZIP_THREAD_H
