#ifndef _SAPPHIRE_I_PARTICLE_SYSTEM_SCENE_NODE_
#define _SAPPHIRE_I_PARTICLE_SYSTEM_SCENE_NODE_

#include "SapphireISceneNode.h"
#include "SapphireIParticleAnimatedMeshSceneNodeEmitter.h"
#include "SapphireIParticleEmitter.h"
#include "SapphireIParticleAffector.h"
#include "SapphireIParticleBoxEmitter.h"
#include "SapphireIParticleCylinderEmitter.h"
#include "SapphireIParticleMeshEmitter.h"
#include "SapphireIParticleRingEmitter.h"
#include "SapphireIParticleSphereEmitter.h"
#include "SapphireIParticleAttractionAffector.h"
#include "SapphireIParticleFadeOutAffector.h"
#include "SapphireIParticleGravityAffector.h"
#include "SapphireIParticleRotationAffector.h"


namespace Sapphire
{
	//! 一个用于创建雪，火焰，爆炸，烟雾...的粒子系统
	/** 一个场景节点控制一个粒子系统。 粒子的行为通过设置正确的粒子发射器和影响器来控制
	 你可以如下简单创建一个篝火

	\code
	scene::IParticleSystemSceneNode* p = scenemgr->addParticleSystemSceneNode();
	p->setParticleSize(dimension2d<Real>(20.0f, 10.0f));
	scene::IParticleEmitter* em = p->createBoxEmitter(
	aabbox3d<Real>(-5,0,-5,5,1,5),
	Vector3(0.0f,0.03f,0.0f),
	40,80, ColourValue(0,255,255,255),ColourValue(0,255,255,255), 1100,2000);
	p->setEmitter(em);
	em->drop();
	scene::IParticleAffector* paf = p->createFadeOutParticleAffector();
	p->addAffector(paf);
	paf->drop();
	\endcode

	*/
	class IParticleSystemSceneNode : public ISceneNode
	{
	public:

		 
		IParticleSystemSceneNode(ISceneNode* parent, ISceneManager* mgr, SINT32 id,
			const Vector3& position = Vector3(0, 0, 0),
			const Vector3& rotation = Vector3(0, 0, 0),
			const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		//! 设置所有粒子的大小
		virtual void setParticleSize(
			const dimension2d<Real> &size = dimension2d<Real>(5.0f, 5.0f)) = 0;

		//! 设置粒子是否应该被全局化
		/** 如果说，这些粒子会受到粒子系统场景节点的移动的影响，除法它们已经完全失效。默认为true
		*/
		virtual void setParticlesAreGlobal(bool global = true) = 0;

		//! 移动所有当前可见的粒子
		virtual void clearParticles() = 0;

		//! 受到更新粒子
		//! 这应该在当你想要在场景图外渲染这个节点时调用 ，除非这个节点是自动的
		//! 
		virtual void doParticleSystem(UINT32 time) = 0;

		//! 获取粒子发射器，它会创建粒子
		/** \return 这个粒子发射器，如果没有设置为0 */
		virtual IParticleEmitter* getEmitter() = 0;

		//! 设置粒子发射器
		/** 一个粒子发射器可以通过createEmitter方法创建。利润创建一个简单的PointEmitter，
		调用IParticleEmitter* p = createPointEmitter(); setEmitter(p); p->drop();
		\param emitter: 设置这个例子发射器。如果设置为0，将移除当前的粒子发射器并且停止粒子系统发射的粒子 */
		virtual void setEmitter(IParticleEmitter* emitter) = 0;

		//! 添加一个新的粒子影响器到这个例子系统
		/** 一个粒子影响器作用粒子群。例如，FadeOut 影响器让所有粒子在一定时间逐渐消失。
		它用如下方法创建
		\code
		IParticleAffector* p = createFadeOutParticleAffector();
		addAffector(p);
		p->drop();
		\endcode
		请注意：这个影响器对于粒子系统的工作不是必须的
		\param affector: 新的影响器. */
		virtual void addAffector(IParticleAffector* affector) = 0;

		//! 获取所有的粒子影响器的列表
		/** \return 所有关联到这个节点的粒子影响器的列表*/
		virtual const list<IParticleAffector*>::type& getAffectors() const = 0;

		//! 在这个例子系统里移除所有的的粒子影响器
		virtual void removeAllAffectors() = 0;

		//! 对一个动画网格场景节点创建一个粒子发射器
		/** \param node: 指定发射粒子的动画网格场景节点
		\param useNormalDirection: 如果为true，每个粒子的方向的将发射处顶点的法线。
		这个法线将由normalDirectionModifier参数细分，默认为100.0f
		\param direction: 粒子方向和速度
		\param normalDirectionModifier: 如果发射器使用法线方向，那么这个发射粒子的顶点法线将被分裂成这个数量
		\param mbNumber: 这允许你为发射粒子的IMesh*指定一个特定网格缓冲区meshBuffer。默认为-1，意味着从所有的
		网格缓冲区中选择一个随机的网格缓冲区，从中来选择随机的顶点。如果值为0或更大，它将从这个值指定的网格缓冲区
		中旋转随机顶点	 
		\param everyMeshVertex: 如果为true，这个例子发射器每秒将发射min/max之间个粒子，对于网格的每个顶点，如果为false
		，它将在网格的随机顶点发射min/max个粒子
		\param minParticlesPerSecond: 每秒发射的最小粒子数
		\param maxParticlesPerSecond: 每秒发射的最大粒子数
		\param minStartColor: 粒子的最小初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param maxStartColor:  粒子的最大初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param lifeTimeMin: 粒子的最小生命周期，毫秒单位
		\param lifeTimeMax: 粒子的最大生命周期，毫秒单位
		\param maxAngleDegrees: 粒子发射方向的最大角度，这个发射方向将和源方向不同
		\param minStartSize: 粒子最小初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\param maxStartSize: 粒子最大初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\return 指向创建的粒子发射器的指针。要设置这个新粒子系统的新发射器，调用setEmitter()、
	 */
		virtual IParticleAnimatedMeshSceneNodeEmitter* createAnimatedMeshSceneNodeEmitter(
			IAnimatedMeshSceneNode* node, bool useNormalDirection = true,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			Real normalDirectionModifier = 100.0f, SINT32 mbNumber = -1,
			bool everyMeshVertex = false,
			UINT32 minParticlesPerSecond = 5, UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)) = 0;

		//! 创建一个盒子粒子发射器
		/** \param box: 这个例子发射器的盒子
		\param direction: 粒子发射的方向和速度
		\param minParticlesPerSecond: 每秒发射的最小粒子数
		\param maxParticlesPerSecond: 每秒发射的最大粒子数
			\param minStartColor: 粒子的最小初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param maxStartColor:  粒子的最大初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param lifeTimeMin: 粒子的最小生命周期，毫秒单位
		\param lifeTimeMax: 粒子的最大生命周期，毫秒单位
		\param maxAngleDegrees: 粒子发射方向的最大角度，这个发射方向将和源方向不同
		\param minStartSize: 粒子最小初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\param maxStartSize: 粒子最大初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\return 指向创建的粒子发射器的指针。要设置这个新粒子系统的新发射器，调用setEmitter()、
		*/
		virtual IParticleBoxEmitter* createBoxEmitter(
			const AxisAlignedBox& box = AxisAlignedBox(-10, 28, -10, 10, 30, 10),
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)) = 0;

		//! 创建一个圆柱体粒子发射器
		/** \param center: 这个圆柱体柱底圆环的中心
		\param radius: 圆柱体厚度
		\param normal: 圆柱体的长度方向
		\param length: 圆柱体长度
		\param outlineOnly: 是否只把点放在圆柱体内或外
		\param direction: 粒子发射的方向和速度
		\param minParticlesPerSecond: 每秒发射的最小粒子数
		\param maxParticlesPerSecond: 每秒发射的最大粒子数
			\param minStartColor: 粒子的最小初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param maxStartColor:  粒子的最大初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param lifeTimeMin: 粒子的最小生命周期，毫秒单位
		\param lifeTimeMax: 粒子的最大生命周期，毫秒单位
		\param maxAngleDegrees: 粒子发射方向的最大角度，这个发射方向将和源方向不同
		\param minStartSize: 粒子最小初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\param maxStartSize: 粒子最大初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\return 指向创建的粒子发射器的指针。要设置这个新粒子系统的新发射器，调用setEmitter()
		*/
		virtual IParticleCylinderEmitter* createCylinderEmitter(
			const Vector3& center, Real radius,
			const Vector3& normal, Real length,
			bool outlineOnly = false,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5, UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)) = 0;

		//! 创建一个网格粒子发射器
		/** \param mesh: 发射粒子的网格
		\param useNormalDirection: 如果为true， 每个粒子的方向将以发射粒子的顶点的法线创建
		这个方向将会以normalDirectionModifier参数派生，默认100.0f
		\param direction: 粒子发射的方向和速度
		\param normalDirectionModifier: 如果发射器使用顶点法线方向作为粒子方向，这个参数会从中派生多个方向
		\param mbNumber: 这允许你为发射粒子的IMesh*指定一个特定网格缓冲区meshBuffer。默认为-1，意味着从所有的
		网格缓冲区中选择一个随机的网格缓冲区，从中来选择随机的顶点。如果值为0或更大，它将从这个值指定的网格缓冲区
		中旋转随机顶点	 
		如果为true，这个例子发射器每秒将发射min/max之间个粒子，对于网格的每个顶点，如果为false
		，它将在网格的随机顶点发射min/max个粒子
		\param minParticlesPerSecond: 每秒发射的最小粒子数
		\param maxParticlesPerSecond: 每秒发射的最大粒子数
		\param minStartColor: 粒子的最小初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param maxStartColor:  粒子的最大初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param lifeTimeMin: 粒子的最小生命周期，毫秒单位
		\param lifeTimeMax: 粒子的最大生命周期，毫秒单位
		\param maxAngleDegrees: 粒子发射方向的最大角度，这个发射方向将和源方向不同
		\param minStartSize: 粒子最小初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\param maxStartSize: 粒子最大初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\return 指向创建的粒子发射器的指针。要设置这个新粒子系统的新发射器，调用setEmitter() */
		virtual IParticleMeshEmitter* createMeshEmitter(
			IMesh* mesh, bool useNormalDirection = true,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			Real normalDirectionModifier = 100.0f, SINT32 mbNumber = -1,
			bool everyMeshVertex = false,
			UINT32 minParticlesPerSecond = 5, UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)) = 0;

		//! 创建一个点粒子发射器
		/** \param direction: 发射粒子的方向和速度
		\param minParticlesPerSecond: 每秒发射的最小粒子数
		\param maxParticlesPerSecond: 每秒发射的最大粒子数
		\param minStartColor: 粒子的最小初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param maxStartColor:  粒子的最大初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param lifeTimeMin: 粒子的最小生命周期，毫秒单位
		\param lifeTimeMax: 粒子的最大生命周期，毫秒单位
		\param maxAngleDegrees: 粒子发射方向的最大角度，这个发射方向将和源方向不同
		\param minStartSize: 粒子最小初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\param maxStartSize: 粒子最大初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\return 指向创建的粒子发射器的指针。要设置这个新粒子系统的新发射器，调用setEmitter() */
		virtual IParticlePointEmitter* createPointEmitter(
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)) = 0;

		//! 创建一个环形粒子发射器
		/** \param center: 环的中心
		\param radius: 圆环上的点离中心的距离，点将会绕Y轴子一个随机360度旋转
		并且通过在每个轴提供的ringThickness值进行切换
		\param ringThickness : 圆环的厚度，或者环的宽度
		\param direction: 发射粒子的方向和速度
		\param minParticlesPerSecond: 每秒发射的最小粒子数
		\param maxParticlesPerSecond: 每秒发射的最大粒子数
		\param minStartColor: 粒子的最小初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param maxStartColor:  粒子的最大初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param lifeTimeMin: 粒子的最小生命周期，毫秒单位
		\param lifeTimeMax: 粒子的最大生命周期，毫秒单位
		\param maxAngleDegrees: 粒子发射方向的最大角度，这个发射方向将和源方向不同
		\param minStartSize: 粒子最小初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\param maxStartSize: 粒子最大初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\return 指向创建的粒子发射器的指针。要设置这个新粒子系统的新发射器，调用setEmitter()*/
		virtual IParticleRingEmitter* createRingEmitter(
			const Vector3& center, Real radius, Real ringThickness,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)) = 0;

		//! 创建一个球体粒子发射器
		/** \param center: 球体中心
		\param radius: 球体半径
		\param direction: 发射粒子的方向和速度
		\param minParticlesPerSecond: 每秒发射的最小粒子数
		\param maxParticlesPerSecond: 每秒发射的最大粒子数
		\param minStartColor: 粒子的最小初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param maxStartColor:  粒子的最大初始化颜色。真实颜色由 minStartColor 和 maxStartColor之间进行
		随机插值计算
		\param lifeTimeMin: 粒子的最小生命周期，毫秒单位
		\param lifeTimeMax: 粒子的最大生命周期，毫秒单位
		\param maxAngleDegrees: 粒子发射方向的最大角度，这个发射方向将和源方向不同
		\param minStartSize: 粒子最小初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\param maxStartSize: 粒子最大初始大小。每个粒子的真实大小由 minStartSize 和 maxStartSize之间随机插值决定
		\return 指向创建的粒子发射器的指针。要设置这个新粒子系统的新发射器，调用setEmitter() */
		virtual IParticleSphereEmitter* createSphereEmitter(
			const Vector3& center, Real radius,
			const Vector3& direction = Vector3(0.0f, 0.03f, 0.0f),
			UINT32 minParticlesPerSecond = 5,
			UINT32 maxParticlesPerSecond = 10,
			const ColourValue& minStartColor = ColourValue::getColourValue(255, 0, 0, 0),
			const ColourValue& maxStartColor = ColourValue::getColourValue(255, 255, 255, 255),
			UINT32 lifeTimeMin = 2000, UINT32 lifeTimeMax = 4000,
			SINT32 maxAngleDegrees = 0,
			const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
			const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)) = 0;

		//! 创建一个点衰减影响器
		/** 这个影响器改变粒子位置和每秒以特定的速度吸引它们到到一个特定点
		\param point: 要吸附粒子的点
		\param speed:  吸附到特定点的每秒单位速度
		point.
		\param attract: 粒子从这个点吸附或转移
		\param affectX: 是否影响这个例子位置的x坐标
		\param affectY: 是否影响这个例子位置的y坐标
		\param affectZ: 是否影响这个例子位置的Z坐标
		\return 指向这个粒子影响器的指针。要添加这个新的影响器到粒子系统，调用addAffector()。
	 */
		virtual IParticleAttractionAffector* createAttractionAffector(
			const Vector3& point, Real speed = 1.0f, bool attract = true,
			bool affectX = true, bool affectY = true, bool affectZ = true) = 0;

		//! 创建一个缩放粒子影响器
		/** 这个影响器缩放这个例子到它大小的倍数
		\param scaleTo: 缩放倍数
		\return 指向这个粒子影响器的指针。要添加这个新的影响器到粒子系统，调用addAffector()。 */
		virtual IParticleAffector* createScaleParticleAffector(const dimension2df& scaleTo = dimension2df(1.0f, 1.0f)) = 0;

		//! 创建消退粒子影响器
		/** 这个影响器修改每一个粒子的颜色和这个例子死亡时的最终颜色。
		这个例子影响器看起来效果不错，如果EMT_TRANSPARENT_ADD_COLOR材质被使用并且目标颜色是ColourValue(0,0,0,0)：
		粒子用此设置消退到无效
		\param targetColor: 粒子改变到的颜色
		\param timeNeededToFadeOut: 影响器改变颜色到目标颜色的毫秒数
		\return 指向这个粒子影响器的指针。要添加这个新的影响器到粒子系统，调用addAffector() */
		virtual IParticleFadeOutAffector* createFadeOutParticleAffector(
			const ColourValue& targetColor = ColourValue(0, 0, 0, 0),
			UINT32 timeNeededToFadeOut = 1000) = 0;

		//! 创建一个重力影响器
		/** 这个影响器修改粒子的方向。它假设粒子从发射器以巨大力量发出，但一定时间后被重力捕捉到
		\param gravity: 重力的方向和力量
		\param timeForceLost: 发射器发射力量完全失去的时间毫秒数并且粒子不能进行任何移动，完全只受到重力影响
		\return 指向这个粒子影响器的指针。要添加这个新的影响器到粒子系统，调用addAffector() */
		virtual IParticleGravityAffector* createGravityAffector(
			const Vector3& gravity = Vector3(0.0f, -0.03f, 0.0f),
			UINT32 timeForceLost = 1000) = 0;

		//! 创建一个旋转影响器
		/** 这个影响器更高粒子的位置并且以一个特定速度吸引它们到一个指定的点
		\param speed: 旋转的每秒角速度
		\param pivotPoint: 粒子环绕旋转的点
		\return 指向这个粒子影响器的指针。要添加这个新的影响器到粒子系统，调用addAffector() */
		virtual IParticleRotationAffector* createRotationAffector(
			const Vector3& speed = Vector3(5.0f, 5.0f, 5.0f),
			const Vector3& pivotPoint = Vector3(0.0f, 0.0f, 0.0f)) = 0;
	};
}

#endif