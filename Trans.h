#ifndef TRANS_H
#define TRANS_H
#include "Defines.h"

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

    //PyObject *pTransModule;
    //PyObject *Translate;
    //之前用py出现乱码的原因，没有将字符串转str!还要encodeutf8!

    void PyTrans(string text);
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
    //bool pycan;
};
#endif // TRANS_H
