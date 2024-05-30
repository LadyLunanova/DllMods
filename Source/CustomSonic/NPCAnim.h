#include <Sonic\Object\ObjectBase.h>
#include <Sonic\ObjectSystem\SetObjectListener.h>
#include <Hedgehog\Animation\hhAnimationPose.h>

#include <Sonic/System/MatrixNodeTransform.h>

// TODO: Move
// Takes in a CAnimationPose::SMotionInfo array created on the stack, evaluating its count so you don't have to manually log it.
#define NPC_ADD_ANIM_LIST(list) \
    AddAnimationList(list, sizeof(list) / sizeof(Hedgehog::Animation::SMotionInfo))

namespace Sonic
{
	class CNPCAnimation;  

	static inline BB_FUNCTION_PTR(void, __stdcall, fpCNPCAnimationInitialize, 0xB677A0,
		CNPCAnimation* This, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase, const Hedgehog::Base::CSharedString& in_rSkeletonName, float in_Scale);

	// TODO: Migrate to another file

	static inline uint32_t pCNPCAnimationAddAnimationList = 0xB678F0;
	static void fCNPCAnimationAddAnimationList(CNPCAnimation* This, const Hedgehog::Animation::SMotionInfo* in_pList, int in_Count, bool in_Condition)
	{
		__asm
		{
			mov al, in_Condition
			mov ecx, in_Count
			push in_pList
			push This
			call [pCNPCAnimationAddAnimationList]
		}
	}

	class CNPCAnimation : public IAnimationContext
	{
	public:
		boost::shared_ptr<CAnimationStateMachine> m_spAnimationStateMachine;
		boost::shared_ptr<Hedgehog::Animation::CAnimationPose> m_spAnimationPose;
		Hedgehog::Math::CVector m_Velocity;

		virtual boost::shared_ptr<Hedgehog::Animation::CAnimationPose>
		GetSharedAnimationPose()
		{
			return m_spAnimationPose;
		}

		virtual boost::shared_ptr<Hedgehog::Animation::CAnimationPose>*
		GetPtrSharedAnimationPose()
		{
			return &m_spAnimationPose;
		}

		virtual ~CNPCAnimation() = default;


		void Initialize(const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase, const Hedgehog::Base::CSharedString& in_rSkeletonName, float in_Scale = 1.0f)
		{
			fpCNPCAnimationInitialize(this, in_spDatabase, in_rSkeletonName, in_Scale);
		}

		void AddAnimationList(const Hedgehog::Animation::SMotionInfo* in_pList, int in_Count, bool in_Condition = false)
		{
			fCNPCAnimationAddAnimationList(this, in_pList, in_Count, in_Condition);
		}

		// Intellisense trick
#pragma push_macro("NPC_ADD_ANIM_LIST")
#undef NPC_ADD_ANIM_LIST
		void NPC_ADD_ANIM_LIST(Hedgehog::Animation::SMotionInfo* in_pList) const;
#pragma pop_macro("NPC_ADD_ANIM_LIST")

		Hedgehog::Animation::CAnimationPose* GetAnimationPose() override
		{
			return m_spAnimationPose.get();
		};
		Hedgehog::Math::CVector GetVelocityForAnimationSpeed() override
		{
			return m_Velocity;
		};
		Hedgehog::Math::CVector GetVelocityForAnimationChange() override
		{
			return m_Velocity;
		};
	};
	BB_ASSERT_OFFSETOF(CNPCAnimation, m_spAnimationStateMachine, 0x04);
	BB_ASSERT_OFFSETOF(CNPCAnimation, m_spAnimationPose, 0x0C);
	BB_ASSERT_OFFSETOF(CNPCAnimation, m_Velocity, 0x20);

}