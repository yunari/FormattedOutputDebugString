# FormattedOutputDebugString

* usage:
void Test::init()
{
	std::string msg = "Hello, World!";
	WriteLog(FLOGTYPE_DEBUG, "TestModule", __FUNCTION__, "Message: %s", msg);
}

* output: 
2022-Oct-24 16:49:02 [8734][DEBUG][TestModule][Test::init] Message: Hello, World!