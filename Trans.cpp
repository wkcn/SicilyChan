#include "Trans.h"

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
        ifstream fin(GetDataDir(name));
        //qDebug("%c===",name.c_str());
        getline(fin,buf,'\n');
        while(!fin.eof()){
            //qDebug("--%s",buf.c_str());
            int q;
            string word;
            for(q=0;q<buf.size();q++){
                if(buf[q]=='~')break;
                word += buf[q];
            }
            //string content = buf.substr(q+1);
            string content;
            for(int u=q+1;u<buf.size();u++){
                if(buf[u]=='|')content += '\n';
                else content += buf[u];
            }
            //qDebug("%s -- %s",word.c_str(),content.c_str());
            trieTree.insert(word,content);
            getline(fin,buf,'\n');
        }
        fin.close();
    }
    /*
    pycan = false;

    Py_Initialize();

    if(!Py_IsInitialized()){
        return;
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("reload(sys)");
    PyRun_SimpleString("sys.path.append('./')");
    string setPath = "sys.path.append('";
    setPath += GetDataDir("Data");
    setPath += "')";
    qDebug("!!!%s",setPath.c_str());
    PyRun_SimpleString(setPath.c_str());
    PyRun_SimpleString("sys.setdefaultencoding('utf-8')");
    pTransModule = PyImport_ImportModule("trans");//这里是要调用的Python文件名
    if(!pTransModule){
        return;
    }
    Translate = PyObject_GetAttrString(pTransModule, "Translate");

    if(!Translate || !PyCallable_Check(Translate))return;
    */
    finished = false;
    ing = false;
    //pycan = true;
}
Trans::~Trans(){

}

void Trans::PyTrans(string text){
    /*
    finished = false;
    ing = true;
    char textC[256],resC[256] = {'\0'};
    strcpy(textC,text.c_str());
    static char s[] = "s";
    //qDebug("---%s",textC);
    try{
        PyObject *re = PyObject_CallFunction(Translate,s,textC);
        if(re)strcpy(resC,PyString_AS_STRING(re));
    }catch(...){
        resC[0] = '\0';
    }
    res = resC;


    ////

    char textTemp[512],textTemp2[512];
    strcpy(textTemp2,text.c_str());
    static char sss[] = "sss";
    static char zh[] = "zh";
    static char en[] = "en";
    try{
      PyObject *re = 0;
      if (eng)re = PyObject_CallFunction(Translate,sss,textTemp2,zh,en);//target source
      else re = PyObject_CallFunction(Translate,sss,textTemp2,en,zh);
      qDebug("tr:%s %d",textTemp2,eng);
      if(re){
          strcpy(textTemp,PyString_AS_STRING(re));
          qDebug("ok%s",textTemp);
      }else{
          textTemp[0] = 0;
      }
    }catch(...){
        textTemp[0] = 0;
    }

    if(!finished)res = textTemp;

    size_t count = 0;
    for (size_t i=0;i<res.size();i++){
        if(res[i] == -1)count++;
    }
    if(count == res.size()){
        res = "";
    }
    */
    finished = true;

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
        //PyTrans(temp);
    }
    /*
    const int maxLen = 256;
    static char oldstr[maxLen + 32];
    size_t len = cstr.length();
    bool lastBlank = false;
    int count = 0;
    bool can = false;
    bool dif = false;
    bool eng = true;
    int cnCount = 0;
    int blankCount = 0;
    bool first = true;
    unsigned int t;
    static size_t lastLen;
    if(lastLen != len){
        lastLen = len;
        dif = true;
    }
    if(len < maxLen * 2){
        can = true;
        for(t=0;t<len;t++){
            if(cstr[t] == '\n' || cstr[t] == '\t' || cstr[t] == '\r'){
                cstr[t] = ' ';
            }
            if(cstr[t] == ' '){
                if(first){
                    continue;
                }
                if(lastBlank){
                    continue;
                }
                lastBlank = true;
                blankCount ++;
            }else{
                lastBlank = false;
            }
            first = false;

            if(cstr[t]<0)
                cnCount ++;

            //下标关系，当前处理的值下标count
            if(oldstr[count]!=cstr[t]){
                dif = true;
            }

            oldstr[count++] = cstr[t];
            if(count>=maxLen){
                can = false;
                break;
            }
        }
        if(blankCount == count){
            can  = false;
        }
        while(t < len && count < maxLen + 32 - 8 && count > 0 && oldstr[count-1] < 0){
            oldstr[count++] = cstr[t++];
        }
        oldstr[count++] = 0;
    }
    //qDebug("qq%d",count);


    if(can && dif){
        if(cnCount > count - cnCount * 2){
            eng = false;
        }
        if(cstr[0] == '!'){
            ori = cstr.substr(1);
            ing = true;
            PyTrans(cstr,true);
        }else{
            ori = oldstr;
            ing = true;
            PyTrans(ori,eng);
        }
    }
    */
}

