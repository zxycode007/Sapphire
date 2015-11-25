

#include "SapphirePrerequisites.h"
#include "SapphireSingleton.h"
#include "SapphireLogManager.h"
#include "SapphireQMath.h"
#include "SapphireMath.h"
#include "SapphireAny.h"
#include "SapphireVertex.h"


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

class CustomAny : public AnyNumeric
{
public:

	CustomAny()
	{}
};

//函数模板的特别化
//Singleton的静态变量msSingleton做初始化  T  class<T>::xxx = xxx;
template<> CustomSingleTest*  Singleton<CustomSingleTest>::msSingleton = 0;


void TestSVertex();

void TestSTL()
{
	std::list<int> lis;
	lis.push_back(5);
	lis.push_back(8);
	lis.push_back(1);
	std::list<int>::iterator it0 = lis.begin();
	while (it0 != lis.end())
	{
		int a = *it0;
		it0++;
		std::cout << a << std::endl;
	}
	std::vector<UserData> v;
	UserData u1;
	u1.age = 16;
	u1.name = "dasd";
	v.push_back(u1);
	v.push_back(u1);
	v.push_back(u1);
	v.push_back(u1);
	std::vector<UserData>::iterator it = v.begin()+3;
	UserData u2 = v.at(0);
	std::cout << u2.name << std::endl;
	std::cout << "TEST STL"<<v.size() << std::endl;
 
}

int main()
{
	TestSTL();
	
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
	
	CustomAny any;
	Any aint(12);
	Any afloat(24.25);
	std::cout << "CLASS ANY: aint type: " << aint.getType().name() << " afloat type :"<<afloat.getType().name()<<std::endl;
	std::cout << "aint value = " << aint.get<int>() << std::endl;
	
	double f = 12.51;
	AnyNumeric anum = f;
	AnyNumeric anum2(12);
	AnyNumeric anum3(anum+anum2);
	AnyNumeric* anump = new AnyNumeric(f);
	std::cout << " anum names :" << anum.getType().name() << std::endl;
	std::cout << " anum2 names :" << anum2.getType().name() << std::endl;
	std::cout << " anum3 names :" << anum3.getType().name() << std::endl;

	TestSVertex();
	int ara[2];
	ara[1] = 5;
	ara[0] = 1;
	printf(c);
	if (SAPPHIRE_3D_API == SAPPHIRE_WITH_OPENGL)
	{
		cout << "\n SAPPHIRE_3D_API=" << SAPPHIRE_3D_API << endl;
	}
	
	getchar();
	
	
	
	SAPPHIRE_DELETE lm;
	return 0;
} 



void TestSVertex()
{
	SVertex vs;
	vs.setTexCoord(0, Vector2(0.15, 0.56));
	LogManager::getSingletonPtr()->getDefaultLog()->logMessage("vs Test");
	
}