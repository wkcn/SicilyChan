#include "Trans.h"

QDir directoryOf(const QString &subdir)
{
    QDir dir(QApplication::applicationDirPath());

#if defined(Q_OS_WIN)
    if (dir.dirName().toLower() == "debug"
            || dir.dirName().toLower() == "release"
            || dir.dirName().toLower() == "bin")
        dir.cdUp();
#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS"){
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }
#endif
    dir.cd(subdir);
    return dir;
}


QString GetFileDir(QString file){
#if !defined(WIN32)
    file = file.replace("\\","//");
#endif
    return directoryOf("").absoluteFilePath(file);
}

QString GetQDir(QString file){
    return GetFileDir(file);
}

string GetDataDir(string file){
    return GetFileDir(QString::fromStdString(file)).toStdString();
}

TrieNode::TrieNode(){
    value = 0;
    for(auto &c:children) c = 0;
}

TrieNode::~TrieNode(){
    if(!value)delete value;
    for(auto &c:children) if(c)delete c;
}

void TrieNode::Set(const string &name){
    size_t len = name.length();
    value = new char[len + 1];
    strcpy(value,name.c_str());
}

TrieTree::TrieTree(){
    root = new TrieNode;
}

TrieTree::~TrieTree(){
    delete root;
}

void TrieTree::insert(string word, string content){
    TrieNode *node = root;
    int id;
    for(auto c:word){
        if(c >= 'a' && c <= 'z')id = c - 'a';
        else if(c >= 'A' && c <= 'Z')id = c - 'A';
        else continue;
        if(!node -> children[id])node -> children[id] = new TrieNode;
        node = node -> children[id];
    }
    node->Set(content);
}

string TrieTree::find(string word){
    TrieNode *node = root;
    int id;
    bool ok = false;
    for(auto c:word){
        if(c >= 'a' && c <= 'z')id = c - 'a';
        else if(c >= 'A' && c <= 'Z')id = c - 'A';
        else continue;

        if(node->children[id] == 0)return "";

        node = node -> children[id];
        ok = true;
    }
    if (!ok || node->value == 0)return "";
    return node -> value;
}

Trans::Trans(){
    string buf;
    for(int i=0;i<26;i++){
        string name = "Data\\Dict\\";
        name += char(i+'a');
        name += ".txt";
        ifstream fin(GetDataDir(name).c_str());
        if(fin.fail())break;
        getline(fin,buf);
        while(!fin.eof()){
            size_t q;
            string word;
            for(q=0;q<buf.size();q++){
                if(buf[q]=='~')break;
                word += buf[q];
            }
            string content;
            for(size_t u=q+1;u<buf.size();u++){
                if(buf[u]=='|')content += '\n';
                else content += buf[u];
            }
            trieTree.insert(word,content);
            getline(fin,buf);
        }
        fin.close();
    }
    finished = false;
    ing = false;
}
Trans::~Trans(){

}

void Trans::Set(const string& text){
    if (!ing){
        finished = false;
        cstr = text;
        start();
    }
}
string Trans::Get(){
    finished = false;
    ing = false;
    return res;
}
string Trans::GerOrigin(){
    return ori;
}
void Trans::run(){
    const int maxLen = 128;
    bool first = true;
    string temp;
    int count = 0;
    for(auto c:cstr){
        if(first && (c == ' ' || c == '\n' || c == '\r' || c == '\t'))continue;
        temp += c;
        count ++;
        if(count > maxLen)break;
        first = false;
    }
    if(temp != ori){
        ing = true;
        ori = temp;
        res = trieTree.find(temp);
        finished = true;
    }
}
