#include "test.h"
#include "datastring.h"
#include "datablock.h"
#include "biglist.h"
#include "user.h"
#include "chatroom.h"
#include "parameters.h"
#include "stringbuilder.h"
#include "message.h"
#include "sha256.h"
#include <stdio.h>
#include <string.h>
#include "Debug.h"

int test::runtest(const char *test)
{
	datastring testname;
	testname = test;
	int result = 0;
	bool found = false;
	bool all = ((testname.length == 3) && (strcmp(testname.data,"all")==0));
	if ((all) || (testname == "datastring") || (testname == "1")) {
		result += test::datastring_test();
		found = true;
	}
	if ((all) || (testname == "datablock") || (testname == "2")) {
		result += test::datablock_test();
		found = true;
	}
	if ((all) || (testname == "biglist") || (testname == "3")) {
		result += test::biglist_test();
		found = true;
	}
	if ((all) || (testname == "user") || (testname == "4")) {
		result += test::user_test();
		found = true;
	}
	if ((all) || (testname == "parameters") || (testname == "5")) {
		result += test::parameters_test();
		found = true;
	}
	if ((all) || (testname == "stringbuilder") || (testname == "6")) {
		result += test::stringbuilder_test();
		found = true;
	}
	
	if ((all) || (testname == "sha256") || (testname == "7")) {
		result += test::sha256_test();
		found = true;
	}
	
	// The debug test should be the last test because it causes the program to crash.
	if ((all) || (testname == "Debug") || (testname == "8")) {
		result += test::Debug_test();
		found = true;
	}
	
	if (!found) {
		printf("Test for class %s wasn't found.\n",test);
	}
	return result;
}

int test::testcount()
{
	return 8;
}

int test::datastring_test()
{	
	printf("datastring test: ");
	datastring test1;
	datastring test2;
	test1 = "Testing1";
	test2 = test1.substr(0,7);
	if (test2.data != test1.data) {
		printf("failed test 1.\n");
		return 1;
	}
	if (test2.length != 7) {
		printf("failed test 2.\n");
		return 1;
	}
	if (test2 != "Testing") {		
		printf("failed test 3.\n");
		return 1;
	}
	if (!test1.null_terminated) {
		printf("failed test 4.\n");
		return 1;
	}	
	if (test1.substr(0,4).null_terminated) {
		printf("failed test 5.\n");
		return 1;
	}
	if (test1 == test2) {
		printf("failed test 6.\n");
		return 1;
	}
	printf("passed.\n");
	return 0;
}

int test::datablock_test()
{
	printf("datablock test: ");
	datablock block((char *)"Testing");
	if (block != "Testing") {
		printf("failed test 1.\n");
		return 1;
	}
	if (memcmp(block.data,"Testing",8)!=0) {
		printf("failed test 2.\n");
		return 1;
	}
	datablock *block2 = block.clone();
	if (block.usage != 2) {
		printf("failed test 3. block.usage=%d\n",block.usage);
		return 1;
	}
	datablock::dereference(&block2);
	if (block.usage != 1) {
		printf("failed test 4.\n");
		return 1;
	}
	printf("passed.\n");
	return 0;
}

int test::biglist_test()
{
	printf("biglist test: ");
	biglist<int> testing;
	testing.add(1);
	testing.add(2);
	testing.add(3);
	if (testing.length()!=3) {
		printf("failed test 1.\n");
		return 1;
	}
	if (!testing.find(2)) {
		printf("failed test 2.\n");
		return 1;
	}
	testing.remove(2);
	if (testing.find(2)) {
		printf("failed test 3.\n");
		return 1;
	}
	if (testing[0]->item!=1) {
		printf("failed test 4.\n");
		return 1;
	}	
	printf("passed.\n");
	return 0;
}

int test::user_test()
{
	printf("user test: ");
	user *testing = new user();
	datablock username("tester");
	datablock password("password");
	user *search;
	biglist<user *> users;
	
	testing->userid = 185;
	testing->username = username.clone();
	testing->password = password.clone();	
	users.add(testing);
	search = user::find(&users,185);
	if (search== nullptr) {
		printf("failed test 1.\n");
		return 1;
	}
	if (search->userid != 185) {
		printf("failed test 2.\n");
		return 1;
	}
	search = user::find(&users,username);
	if (search== nullptr) {
		printf("failed test 3.\n");
		return 1;
	}
	if (search->userid != 185) {
		printf("failed test 4.\n");
		return 1;
	}
	search = user::find(&users,-34);
	if (search != nullptr) {
		printf("failed test 5.\n");
		return 1;
	}
	if (username.usage != 2) {
		printf("failed test 6.\n");
		return 1;
	}
	delete testing;
	testing = nullptr;
	if (username.usage != 1) {
		printf("failed test 7.\n");
		return 1;
	}
	printf("passed.\n");
	return 0;
}

int test::parameters_test()
{
	printf("parameters test: ");
	parameters testing;
	datastring input;
	bool success = true;
	input = "4,test7,testing1250,3,abc";
	testing.string_parameter(input,success);
	if (!success) {
		printf("failed test 8.\n");
		return 1;
	}
	if (testing.string_parameters[0] != "test") {
		printf("failed test 3.\n");
		return 1;
	}	
	testing.string_parameter(input,success);
	if (!success) {
		printf("failed test 9.\n");
		return 1;
	}	
	testing.long_parameter(input,success);
	if (!success) {
		input.print();
		printf("failed test 10.\n");
		return 1;
	}
	testing.string_parameter(input,success);
	if (!success) {
		printf("failed test 7.\n");
		return 1;
	}
	if (input.length > 0) {
		printf("failed test 1.\n");		
		return 1;
	}
	if (testing.parameter_count != 4) {
		printf("failed test 2.\n");
		return 1;
	}
	if (testing.string_parameters[1] != "testing") {
		printf("failed test 4.\n");
		return 1;
	}
	if (testing.long_parameters[2] != 1250) {
		printf("failed test 5.\n");
		return 1;
	}
	if (testing.string_parameters[3] != "abc") {
		printf("failed test 6.\n");
		return 1;
	}
	printf("passed.\n");
	return 0;
}

int test::stringbuilder_test()
{
	stringbuilder testing;
	datastring twentyfive;
	message output;
	int twelve;
	int64_t thirteen;	
	
	printf("stringbuilder test: ");	
	// Test 1.
	twelve = 12;
	thirteen = 13;
	twentyfive = "twentyfive";
	testing += twelve;
	testing += "+";
	testing += thirteen;
	testing += "=";
	testing += twentyfive;
	output = testing;
	if (output.actual_message != "12+13=twentyfive")
	{
		printf("failed test 1.\n");
		return 1;
	}
	// Test 2.
	testing.clear();
	testing += "test(";
	testing.addparameter(twelve);
	testing.addparameter(5);
	testing.addparameter(twentyfive);
	testing += ")";
	output = testing;
	if (output.actual_message != "test(12,5,10,twentyfive)") {
		printf("failed test 2.\n");
		return 1;
	}
	printf("passed.\n");	
	return 0;	
}

int test::sha256_test()
{
	printf("sha256 test: ");
	datablock input("This is a sha256 test.");
	datastring expected_output;
	datablock actual_output(SHA256_BLOCK_SIZE << 1);
	expected_output = "986fab2cb2d44c0ea7da9926c9f60bd07389bc775383d1e85f49efc4e01578af";
	input.sha_256(actual_output);
	if (actual_output != expected_output) {
		printf("failed.\n");
		return 1;
	}
	printf("passed.\n");	
	return 0;
}

int test::Debug_test()
{
	printf("Debug test.\n");
	// Start of error handling.
	error_signals::AddHandlers();
	pid_t tid = error_signals::GetThreadID();
	error_signals *error_thread = error_signals::GetThread(tid);
	if (error_thread == nullptr)
	{
	  error_thread = error_signals::AddThread(tid);
	}
	volatile int val = 0;
	if (error_thread != nullptr)
	{
	  error_thread->LineNumberStack = 0;
	  val = setjmp(error_thread->position);
	}
	if (val != 0)
	{
	  error_thread->DisplayErrorMessage(val);
	  return 0;
	}
	// End of error handling.
	// Start of testing.
	Debug debug(__FILE__,__func__,__LINE__);
	test::Debug_test2();
	
	return 0;
}

void test::Debug_test2()
{
	Debug debug(__FILE__,__func__,__LINE__);	
	test::Debug_test3(2);	
	debug = __LINE__;
	test::Debug_test3(0);	
	debug = __LINE__;
}

int test::Debug_test3(int x)
{
	Debug debug(__FILE__,__func__,__LINE__);
	int y;
	if (x < 0) {
		x = 1;
	}
	debug = __LINE__;
	y = 1/x;
	debug = __LINE__;
	return y;
}
