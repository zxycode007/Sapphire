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
	//! һ�����ڴ���ѩ�����棬��ը������...������ϵͳ
	/** һ�������ڵ����һ������ϵͳ�� ���ӵ���Ϊͨ��������ȷ�����ӷ�������Ӱ����������
	 ��������¼򵥴���һ������

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

		//! �����������ӵĴ�С
		virtual void setParticleSize(
			const dimension2d<Real> &size = dimension2d<Real>(5.0f, 5.0f)) = 0;

		//! ���������Ƿ�Ӧ�ñ�ȫ�ֻ�
		/** ���˵����Щ���ӻ��ܵ�����ϵͳ�����ڵ���ƶ���Ӱ�죬���������Ѿ���ȫʧЧ��Ĭ��Ϊtrue
		*/
		virtual void setParticlesAreGlobal(bool global = true) = 0;

		//! �ƶ����е�ǰ�ɼ�������
		virtual void clearParticles() = 0;

		//! �ܵ���������
		//! ��Ӧ���ڵ�����Ҫ�ڳ���ͼ����Ⱦ����ڵ�ʱ���� ����������ڵ����Զ���
		//! 
		virtual void doParticleSystem(UINT32 time) = 0;

		//! ��ȡ���ӷ����������ᴴ������
		/** \return ������ӷ����������û������Ϊ0 */
		virtual IParticleEmitter* getEmitter() = 0;

		//! �������ӷ�����
		/** һ�����ӷ���������ͨ��createEmitter�������������󴴽�һ���򵥵�PointEmitter��
		����IParticleEmitter* p = createPointEmitter(); setEmitter(p); p->drop();
		\param emitter: ����������ӷ��������������Ϊ0�����Ƴ���ǰ�����ӷ���������ֹͣ����ϵͳ��������� */
		virtual void setEmitter(IParticleEmitter* emitter) = 0;

		//! ���һ���µ�����Ӱ�������������ϵͳ
		/** һ������Ӱ������������Ⱥ�����磬FadeOut Ӱ����������������һ��ʱ������ʧ��
		�������·�������
		\code
		IParticleAffector* p = createFadeOutParticleAffector();
		addAffector(p);
		p->drop();
		\endcode
		��ע�⣺���Ӱ������������ϵͳ�Ĺ������Ǳ����
		\param affector: �µ�Ӱ����. */
		virtual void addAffector(IParticleAffector* affector) = 0;

		//! ��ȡ���е�����Ӱ�������б�
		/** \return ���й���������ڵ������Ӱ�������б�*/
		virtual const list<IParticleAffector*>::type& getAffectors() const = 0;

		//! ���������ϵͳ���Ƴ����еĵ�����Ӱ����
		virtual void removeAllAffectors() = 0;

		//! ��һ���������񳡾��ڵ㴴��һ�����ӷ�����
		/** \param node: ָ���������ӵĶ������񳡾��ڵ�
		\param useNormalDirection: ���Ϊtrue��ÿ�����ӵķ���Ľ����䴦����ķ��ߡ�
		������߽���normalDirectionModifier����ϸ�֣�Ĭ��Ϊ100.0f
		\param direction: ���ӷ�����ٶ�
		\param normalDirectionModifier: ���������ʹ�÷��߷�����ô����������ӵĶ��㷨�߽������ѳ��������
		\param mbNumber: ��������Ϊ�������ӵ�IMesh*ָ��һ���ض����񻺳���meshBuffer��Ĭ��Ϊ-1����ζ�Ŵ����е�
		���񻺳�����ѡ��һ����������񻺳�����������ѡ������Ķ��㡣���ֵΪ0��������������ֵָ�������񻺳���
		����ת�������	 
		\param everyMeshVertex: ���Ϊtrue��������ӷ�����ÿ�뽫����min/max֮������ӣ����������ÿ�����㣬���Ϊfalse
		�������������������㷢��min/max������
		\param minParticlesPerSecond: ÿ�뷢�����С������
		\param maxParticlesPerSecond: ÿ�뷢������������
		\param minStartColor: ���ӵ���С��ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param maxStartColor:  ���ӵ�����ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param lifeTimeMin: ���ӵ���С�������ڣ����뵥λ
		\param lifeTimeMax: ���ӵ�����������ڣ����뵥λ
		\param maxAngleDegrees: ���ӷ��䷽������Ƕȣ�������䷽�򽫺�Դ����ͬ
		\param minStartSize: ������С��ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\param maxStartSize: ��������ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\return ָ�򴴽������ӷ�������ָ�롣Ҫ�������������ϵͳ���·�����������setEmitter()��
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

		//! ����һ���������ӷ�����
		/** \param box: ������ӷ������ĺ���
		\param direction: ���ӷ���ķ�����ٶ�
		\param minParticlesPerSecond: ÿ�뷢�����С������
		\param maxParticlesPerSecond: ÿ�뷢������������
			\param minStartColor: ���ӵ���С��ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param maxStartColor:  ���ӵ�����ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param lifeTimeMin: ���ӵ���С�������ڣ����뵥λ
		\param lifeTimeMax: ���ӵ�����������ڣ����뵥λ
		\param maxAngleDegrees: ���ӷ��䷽������Ƕȣ�������䷽�򽫺�Դ����ͬ
		\param minStartSize: ������С��ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\param maxStartSize: ��������ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\return ָ�򴴽������ӷ�������ָ�롣Ҫ�������������ϵͳ���·�����������setEmitter()��
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

		//! ����һ��Բ�������ӷ�����
		/** \param center: ���Բ��������Բ��������
		\param radius: Բ������
		\param normal: Բ����ĳ��ȷ���
		\param length: Բ���峤��
		\param outlineOnly: �Ƿ�ֻ�ѵ����Բ�����ڻ���
		\param direction: ���ӷ���ķ�����ٶ�
		\param minParticlesPerSecond: ÿ�뷢�����С������
		\param maxParticlesPerSecond: ÿ�뷢������������
			\param minStartColor: ���ӵ���С��ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param maxStartColor:  ���ӵ�����ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param lifeTimeMin: ���ӵ���С�������ڣ����뵥λ
		\param lifeTimeMax: ���ӵ�����������ڣ����뵥λ
		\param maxAngleDegrees: ���ӷ��䷽������Ƕȣ�������䷽�򽫺�Դ����ͬ
		\param minStartSize: ������С��ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\param maxStartSize: ��������ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\return ָ�򴴽������ӷ�������ָ�롣Ҫ�������������ϵͳ���·�����������setEmitter()
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

		//! ����һ���������ӷ�����
		/** \param mesh: �������ӵ�����
		\param useNormalDirection: ���Ϊtrue�� ÿ�����ӵķ����Է������ӵĶ���ķ��ߴ���
		������򽫻���normalDirectionModifier����������Ĭ��100.0f
		\param direction: ���ӷ���ķ�����ٶ�
		\param normalDirectionModifier: ���������ʹ�ö��㷨�߷�����Ϊ���ӷ��������������������������
		\param mbNumber: ��������Ϊ�������ӵ�IMesh*ָ��һ���ض����񻺳���meshBuffer��Ĭ��Ϊ-1����ζ�Ŵ����е�
		���񻺳�����ѡ��һ����������񻺳�����������ѡ������Ķ��㡣���ֵΪ0��������������ֵָ�������񻺳���
		����ת�������	 
		���Ϊtrue��������ӷ�����ÿ�뽫����min/max֮������ӣ����������ÿ�����㣬���Ϊfalse
		�������������������㷢��min/max������
		\param minParticlesPerSecond: ÿ�뷢�����С������
		\param maxParticlesPerSecond: ÿ�뷢������������
		\param minStartColor: ���ӵ���С��ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param maxStartColor:  ���ӵ�����ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param lifeTimeMin: ���ӵ���С�������ڣ����뵥λ
		\param lifeTimeMax: ���ӵ�����������ڣ����뵥λ
		\param maxAngleDegrees: ���ӷ��䷽������Ƕȣ�������䷽�򽫺�Դ����ͬ
		\param minStartSize: ������С��ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\param maxStartSize: ��������ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\return ָ�򴴽������ӷ�������ָ�롣Ҫ�������������ϵͳ���·�����������setEmitter() */
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

		//! ����һ�������ӷ�����
		/** \param direction: �������ӵķ�����ٶ�
		\param minParticlesPerSecond: ÿ�뷢�����С������
		\param maxParticlesPerSecond: ÿ�뷢������������
		\param minStartColor: ���ӵ���С��ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param maxStartColor:  ���ӵ�����ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param lifeTimeMin: ���ӵ���С�������ڣ����뵥λ
		\param lifeTimeMax: ���ӵ�����������ڣ����뵥λ
		\param maxAngleDegrees: ���ӷ��䷽������Ƕȣ�������䷽�򽫺�Դ����ͬ
		\param minStartSize: ������С��ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\param maxStartSize: ��������ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\return ָ�򴴽������ӷ�������ָ�롣Ҫ�������������ϵͳ���·�����������setEmitter() */
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

		//! ����һ���������ӷ�����
		/** \param center: ��������
		\param radius: Բ���ϵĵ������ĵľ��룬�㽫����Y����һ�����360����ת
		����ͨ����ÿ�����ṩ��ringThicknessֵ�����л�
		\param ringThickness : Բ���ĺ�ȣ����߻��Ŀ��
		\param direction: �������ӵķ�����ٶ�
		\param minParticlesPerSecond: ÿ�뷢�����С������
		\param maxParticlesPerSecond: ÿ�뷢������������
		\param minStartColor: ���ӵ���С��ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param maxStartColor:  ���ӵ�����ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param lifeTimeMin: ���ӵ���С�������ڣ����뵥λ
		\param lifeTimeMax: ���ӵ�����������ڣ����뵥λ
		\param maxAngleDegrees: ���ӷ��䷽������Ƕȣ�������䷽�򽫺�Դ����ͬ
		\param minStartSize: ������С��ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\param maxStartSize: ��������ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\return ָ�򴴽������ӷ�������ָ�롣Ҫ�������������ϵͳ���·�����������setEmitter()*/
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

		//! ����һ���������ӷ�����
		/** \param center: ��������
		\param radius: ����뾶
		\param direction: �������ӵķ�����ٶ�
		\param minParticlesPerSecond: ÿ�뷢�����С������
		\param maxParticlesPerSecond: ÿ�뷢������������
		\param minStartColor: ���ӵ���С��ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param maxStartColor:  ���ӵ�����ʼ����ɫ����ʵ��ɫ�� minStartColor �� maxStartColor֮�����
		�����ֵ����
		\param lifeTimeMin: ���ӵ���С�������ڣ����뵥λ
		\param lifeTimeMax: ���ӵ�����������ڣ����뵥λ
		\param maxAngleDegrees: ���ӷ��䷽������Ƕȣ�������䷽�򽫺�Դ����ͬ
		\param minStartSize: ������С��ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\param maxStartSize: ��������ʼ��С��ÿ�����ӵ���ʵ��С�� minStartSize �� maxStartSize֮�������ֵ����
		\return ָ�򴴽������ӷ�������ָ�롣Ҫ�������������ϵͳ���·�����������setEmitter() */
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

		//! ����һ����˥��Ӱ����
		/** ���Ӱ�����ı�����λ�ú�ÿ�����ض����ٶ��������ǵ���һ���ض���
		\param point: Ҫ�������ӵĵ�
		\param speed:  �������ض����ÿ�뵥λ�ٶ�
		point.
		\param attract: ���Ӵ������������ת��
		\param affectX: �Ƿ�Ӱ���������λ�õ�x����
		\param affectY: �Ƿ�Ӱ���������λ�õ�y����
		\param affectZ: �Ƿ�Ӱ���������λ�õ�Z����
		\return ָ���������Ӱ������ָ�롣Ҫ�������µ�Ӱ����������ϵͳ������addAffector()��
	 */
		virtual IParticleAttractionAffector* createAttractionAffector(
			const Vector3& point, Real speed = 1.0f, bool attract = true,
			bool affectX = true, bool affectY = true, bool affectZ = true) = 0;

		//! ����һ����������Ӱ����
		/** ���Ӱ��������������ӵ�����С�ı���
		\param scaleTo: ���ű���
		\return ָ���������Ӱ������ָ�롣Ҫ�������µ�Ӱ����������ϵͳ������addAffector()�� */
		virtual IParticleAffector* createScaleParticleAffector(const dimension2df& scaleTo = dimension2df(1.0f, 1.0f)) = 0;

		//! ������������Ӱ����
		/** ���Ӱ�����޸�ÿһ�����ӵ���ɫ�������������ʱ��������ɫ��
		�������Ӱ����������Ч���������EMT_TRANSPARENT_ADD_COLOR���ʱ�ʹ�ò���Ŀ����ɫ��ColourValue(0,0,0,0)��
		�����ô��������˵���Ч
		\param targetColor: ���Ӹı䵽����ɫ
		\param timeNeededToFadeOut: Ӱ�����ı���ɫ��Ŀ����ɫ�ĺ�����
		\return ָ���������Ӱ������ָ�롣Ҫ�������µ�Ӱ����������ϵͳ������addAffector() */
		virtual IParticleFadeOutAffector* createFadeOutParticleAffector(
			const ColourValue& targetColor = ColourValue(0, 0, 0, 0),
			UINT32 timeNeededToFadeOut = 1000) = 0;

		//! ����һ������Ӱ����
		/** ���Ӱ�����޸����ӵķ������������Ӵӷ������Ծ޴�������������һ��ʱ���������׽��
		\param gravity: �����ķ��������
		\param timeForceLost: ����������������ȫʧȥ��ʱ��������������Ӳ��ܽ����κ��ƶ�����ȫֻ�ܵ�����Ӱ��
		\return ָ���������Ӱ������ָ�롣Ҫ�������µ�Ӱ����������ϵͳ������addAffector() */
		virtual IParticleGravityAffector* createGravityAffector(
			const Vector3& gravity = Vector3(0.0f, -0.03f, 0.0f),
			UINT32 timeForceLost = 1000) = 0;

		//! ����һ����תӰ����
		/** ���Ӱ�����������ӵ�λ�ò�����һ���ض��ٶ��������ǵ�һ��ָ���ĵ�
		\param speed: ��ת��ÿ����ٶ�
		\param pivotPoint: ���ӻ�����ת�ĵ�
		\return ָ���������Ӱ������ָ�롣Ҫ�������µ�Ӱ����������ϵͳ������addAffector() */
		virtual IParticleRotationAffector* createRotationAffector(
			const Vector3& speed = Vector3(5.0f, 5.0f, 5.0f),
			const Vector3& pivotPoint = Vector3(0.0f, 0.0f, 0.0f)) = 0;
	};
}

#endif