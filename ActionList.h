class ActionList{
	public:
		ActionList();
		void push(char action,int nowTime);
		bool match(const char* seq);
		int size() const;
		void clear();
        void update(int nowTime);
        enum{MAX_ACTION = 64,DELAY_TIME = 400};
	private:
        char actionList[MAX_ACTION];
		int actionCount;
		int actionTime;
};
