#ifndef __TEST_H
#define __TEST_H
class test
{
	public:
	static int runtest(const char *test);
	static int datastring_test();
	static int datablock_test();
	static int biglist_test();	
	static int user_test();
	static int parameters_test();
	static int stringbuilder_test();
	static int sha256_test();
	static int file_dir_info_test(bool exact);
	static int Debug_test();
	static void Debug_test2();
	static int Debug_test3(int x);
	static int testcount();
};
#endif
