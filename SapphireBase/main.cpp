

#include "SapphirePrerequisites.h"
#include "SapphireSingleton.h"
#include "SapphireLogManager.h"
#include "SapphireQMath.h"
#include "SapphireMath.h"


using namespace Sapphire;
class UserData : public AllocatedObj<GeneralAllocPolicy>
{
public:
	String name;
	int age;

};
//日志监听器
class CustomLogListener : public LogListener
{
public:
	//监听回调函数
	void messageLogged(const String& message, LogMessageLevel lml, bool maskDebug, const String &logName, bool& skipThisMessage)
	{
		StringUtil::StrStreamType ss;
		ss << "\n" << "ads";
		 
		std::cout<< ss.str() << std::endl;

		return;
	}
};

//单态模式
class  CustomSingleTest : public Singleton<CustomSingleTest>
{
public:
	int i;
	CustomSingleTest(int a)
	{
		i = a;
		StringUtil::StrStreamType ss;
		ss << "\n" << "单态:" << i;

		std::cout << ss.str() << std::endl;
	}
	 
};

//函数模板的特别化
//Singleton的静态变量msSingleton做初始化  T  class<T>::xxx = xxx;
template<> CustomSingleTest*  Singleton<CustomSingleTest>::msSingleton = 0;

int main()
{
	
	int v = SAPPHIRE_COMP_VER;

	HashMap<std::string, int> map1;
	std::string str1 = "miki";
	map1.insert(std::pair<std::string, int>(str1,55));
	HashMap<std::string, int>::iterator it =map1.find(str1);
	printf("MSVC version = %d", v);
	Sapphire::Real r = 35.11;
	const char* c = (char*)it->first.c_str();

	Sapphire::String s = "wbcdeg我的";
	Sapphire::String s2 = "wm莎拉";
	Sapphire::String s3 = s + s2;
	
	UserData* ud = SAPPHIRE_NEW_T(UserData, MEMCATEGORY_GENERAL);
	ud->name = s3;
	ud->age = 14;
	SAPPHIRE_DELETE_T(ud, UserData, MEMCATEGORY_GENERAL);

	UserData* ud2 = SAPPHIRE_NEW UserData();
	ud->name = s3;
	ud->age = 24;
	SAPPHIRE_DELETE ud2;
	//监听器
	CustomLogListener* listener = new CustomLogListener();

	 
	//日志管理器
	LogManager* lm = SAPPHIRE_NEW LogManager();
	//创建日志
	LogManager::getSingletonPtr()->createLog("sapphire.log");
	LogManager::getSingletonPtr()->getDefaultLog()->addListener(listener);	 
	LogManager::getSingleton().logMessage("\n asdkgvkg飞机");
	//LogManager::getSingletonPtr()->logMessage("asdasdasdbbb");


	LogManager::getSingletonPtr()->getDefaultLog()->removeListener(listener);
	delete listener;
 
	SAPPHIRE_DELETE lm;

	CustomSingleTest * stst1 = new CustomSingleTest(1);

	delete stst1;

	Build_Sin_Cos_Tables();
	Build_Inverse_Cos_Table(dp_inverse_cos, 360);

	int qr = FastLog2(256);
	float sin30 = Fast_Cos(60);
	int asc = FAST_INV_COS(0.5);
	
	 
	Radian rand1 = Math::ACos(0.2);
	Radian rand2 = Math::ACos(0.4);
	Radian rand3 = rand1 - rand2;

	Degree* degree1 = new Degree(Math::RadiansToDegrees(rand3.valueRadians()));
	Degree degree2 = Degree(rand3);

	StringUtil::StrStreamType ss;
	ss << rand3.valueRadians();
	std::cout << ss.str() << std::endl;


	Math* math = new Math();

	Radian* sinA = new Radian(Math::Sin(PI/2, true));
	
	Real ddd = sinA->valueDegrees();

	Real ddd2 = Fast_Sin(RAD_TO_DEG(1.0));

	Real texp = Math::Exp(8);
	
	 
	 
	 
	printf(c);
	getchar();
	return 0;
} 