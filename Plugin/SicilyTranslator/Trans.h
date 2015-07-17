#ifndef TRANS_H
#define TRANS_H
#define _CRT_SECURE_NO_WARNINGS

#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QThread>

#include <fstream>
#include <string>
#include <cstring>
using namespace std;

struct TrieNode{
    char *value;
    TrieNode *children[26];
    TrieNode();
    ~TrieNode();
    void Set(const string &name);
};

class TrieTree{
private:
    TrieNode *root;
public:
    TrieTree();
    ~TrieTree();
    void insert(string word,string content);
    string find(string word);
};

class Trans : public QThread
{
    Q_OBJECT

public:
    Trans();
    ~Trans();
private:
    string res;
    string ori;
    string cstr;
    TrieTree trieTree;
protected:
    void run();

public:

    void Set(const string &text);
    string Get();
    string GerOrigin();
    //ing代表正在翻译
    //finished代表翻译完成
    volatile bool ing;
    volatile bool finished;
};
#endif // TRANS_H
