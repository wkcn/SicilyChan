#include "ActionList.h"

ActionList::ActionList(){
	actionCount = 0;
	actionTime = 0;
}

void ActionList::push(char action,int nowTime){
    if (nowTime - actionTime >= DELAY_TIME){
		actionCount = 0;
	}
	actionTime = nowTime;
    if (actionCount >= MAX_ACTION || !action)return;
	actionList[actionCount++] = action;
}

bool ActionList::match(const char* seq){
	int i = 0;
    actionList[actionCount] = '\0';
	while (seq[i] != '\0'){
        if (seq[i] != actionList[i])return false;
        ++i;
    }

	return true;
}

int ActionList::size() const{
	return actionCount;
}

void ActionList::clear(){
	actionCount = 0;
}

void ActionList::update(int nowTime){
    if (nowTime - actionTime >= DELAY_TIME){
        actionCount = 0;
    }
}
