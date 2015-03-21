#include "Sicily.h"

Trans::Trans(){
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("reload(sys)");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("sys.setdefaultencoding('utf-8')");
    pTransModule = PyImport_ImportModule("trans");//这里是要调用的Python文件名
    Translate = PyObject_GetAttrString(pTransModule, "Translate");
    finished = false;
    ing = false;
}
Trans::~Trans(){

}

void Trans::PyTrans(string text,bool eng){
    finished = false;
    ing = true;
    char textTemp[512],textTemp2[512];
    strcpy(textTemp2,text.c_str());
    static char sss[] = "sss";
    static char zh[] = "zh";
    static char en[] = "en";
    try{
      PyObject *re = 0;
      if (eng)re = PyObject_CallFunction(Translate,sss,textTemp2,zh,en);//target source
      else re = PyObject_CallFunction(Translate,sss,textTemp2,en,zh);
      if(re){
          strcpy(textTemp,PyString_AS_STRING(re));
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

    finished = true;
}


void Trans::Set(const string& text){
    if (!ing){
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
        ori = oldstr;
        ing = true;
        PyTrans(ori,eng);
    }
}

