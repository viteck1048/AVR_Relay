#ifndef NEW_MEM

	int inic_eepr(bool start, long int br, ...); //		start/continue, br( = 2), type_1, param_1, type_2, param_2, ...
	int writeData(unsigned int p, long int vznach);
	long int readData(unsigned int p);
	void res_eepr();
	int addParam(int vtype, long int vznach);
	unsigned long int readEEPROM(int p, int vtype);
	void writeEEPROM(unsigned int p, int vtype, long int vznach);
	void load();
	void save();

#else

	void res_eepr();
	unsigned long int readEEPROM(int p, int vtype);
	void writeEEPROM(unsigned int p, int vtype, long int vznach);
	int valid_eeprom();
	bool load();
	bool save_adminset();
	bool save_time();
	bool save_itr();
	
	#ifdef WIFI8266
		void writeStringEEPROM(unsigned int p, int length, char* str);
		int readStringEEPROM(unsigned int p, int length, char* str);
	#endif

#endif