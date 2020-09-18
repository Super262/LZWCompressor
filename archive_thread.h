#ifndef ARCHIVE_THREAD_H
#define ARCHIVE_THREAD_H

#include <QThread>

// 继承自QThread线程类，使压缩操作在子线程进行
class ArchiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ArchiveThread(QObject *parent = nullptr);

    void run(); // 重写void run()

    void setData(QString from, QString to,bool isText); // 初始化必要的变量

signals:
    // 各种信号，用以提示GUI做出响应
    void readStart();
    void readDone();
    void progressChanged(int value);
    void archiveStart();
    void writeStart();
    void writeDone();
    void workFinished();

private:
    long double all_count, done_count; // 已经处理的字节个数和总字节个数

    int done_percent; // 已经完成的百分比

    std::string file_from, file_to; // 源文件地址和目标文件地址

    bool isText; // 告知程序当前所处理的文件的类型

    // 对字符串进行压缩，并把结果保存
    template <typename T> T compress(const std::string &uncompressed, T result);

    // 将十进制数转为指定长度的二进制数
    std::string int2BinaryString(long long c, long long cl);

    // 把压缩后的内容写入文件
    void writeCompressedToFile( std::vector<long long> compressed, const char* file);

    // 读取文件内容到字符串
    std::string readAllBytes(char const* filename);

    // 分步完成压缩操作
    void startCompress();

};

#endif // ARCHIVE_THREAD_H

