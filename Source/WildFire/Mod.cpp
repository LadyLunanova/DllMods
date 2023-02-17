#include <Sonic.h>
#include <Hedgehog.h>
#include <unordered_set>

struct Camera
{
    // Hedgehog::Universe::TStateMachine<Sonic::CCamera>
    // Sonic::CCamera
    uint8_t gap0[0x110];

    // Hedgehog::Mirage::CCamera
    // Sonic::CCamera::CMyCamera
    void* vtable110; // 0x110

    uint8_t gap114[12]; // 0x114

    Eigen::Matrix4f viewMatrix; // 0x120
    Eigen::Matrix4f projectionMatrix; // 0x160
    Eigen::Vector3f position; // 0x1A0
    uint32_t gap1AC;
    Eigen::Vector3f direction; // 0x1B0
    uint32_t gap1BC;
    float aspectRatio; // 0x1C0
    float zNear; // 0x1C4
    float zFar; // 0x1C8

    uint8_t gap1CC[0x26C];
    float fieldOfView; // 0x438
    uint8_t gap43C[0x2C];
    uint32_t field468;
    uint8_t gap46C[0x24];
};

struct MatrixNodeSingleElementNode
{
    INSERT_PADDING(0x60);
    Eigen::Matrix4f local;
    Eigen::Matrix4f world;
};

namespace Eigen
{
    template<typename Scalar>
    Matrix<Scalar, 4, 4> CreatePerspectiveMatrix(const Scalar fieldOfView, const Scalar aspectRatio, const Scalar zNear, const Scalar zFar)
    {
        const Scalar yScale = (Scalar)1 / std::tan(fieldOfView / (Scalar)2);
        const Scalar xScale = yScale / aspectRatio;

        Matrix<Scalar, 4, 4> matrix;

        matrix <<
            xScale, 0, 0, 0,
            0, yScale, 0, 0,
            0, 0, -(zFar + zNear) / (zFar - zNear), -2 * zNear * zFar / (zFar - zNear),
            0, 0, -1, 0;

        return matrix;
    }
}

bool valid()
{
    void* context = *(void**)0x1E5E2F0;
    if (!context) return false;

    void* player = *(void**)((char*)context + 0x110);
    if (!player) return false;

    return true;
}

void updateCamera(Camera* This)
{
    void* context = *(void**)0x1E5E2F0;
    if (!context) return;

    void* player = *(void**)((char*)context + 0x110);
    if (!player) return;

    boost::shared_ptr<MatrixNodeSingleElementNode> node;
    boost::shared_ptr<MatrixNodeSingleElementNode> node1;
    FUNCTION_PTR(void, __thiscall, GetNode, 0x700B70, void* This, boost::shared_ptr<MatrixNodeSingleElementNode>&node, const Hedgehog::Base::CSharedString & name);
    GetNode(*(void**)((char*)player + 0x234), node, "Head");
    GetNode(*(void**)((char*)player + 0x234), node1, "Nose");

    if (node && node1)
    {
        Eigen::Affine3f affine;
        affine = node->world;

        static Eigen::Vector3f prevPos;
        static Eigen::Quaternionf prevRot;

        Eigen::Vector3f position = ((Eigen::Affine3f)node1->world).translation();//affine.translation();
        //position.y() += 0.5f;
        Eigen::Quaternionf rotation;
        rotation = affine.rotation();
        //rotation = Eigen::AngleAxisf(3.14159265359f, Eigen::Vector3f::UnitX()) * rotation;

        Sonic::SPadState* padState = Sonic::CInputState::GetPadState();

        rotation = Eigen::AngleAxisf(-1.57079632679f, rotation * Eigen::Vector3f::UnitZ()) * rotation;

        const float pitch = padState->RightStickVertical * 1.309f;
        const float yaw = padState->RightStickHorizontal * 1.309f;

        Eigen::Vector3f yawAxis = Eigen::Vector3f::UnitY();
        const Eigen::AngleAxisf x(pitch, rotation * Eigen::Vector3f::UnitX());
        const Eigen::AngleAxisf y(-yaw, yawAxis);

        rotation = (y * x * rotation).normalized();

        //position = position * 0.1f + prevPos * 0.9f;
        rotation = prevRot.slerp(0.1f, rotation);

        position += (rotation * Eigen::Vector3f::UnitY()).normalized() * 0.04f;

        prevPos = position;
        prevRot = rotation;

        This->position = position;
        This->direction = (rotation * -Eigen::Vector3f::UnitX()).normalized();
        This->viewMatrix = (Eigen::Translation3f(position) * rotation.normalized()).inverse().matrix();
        This->fieldOfView = 1.57079632679f;
        This->zNear = 0.03f;
        This->projectionMatrix = Eigen::CreatePerspectiveMatrix(This->fieldOfView, This->aspectRatio, This->zNear, This->zFar);
        *(Eigen::Matrix4f*)((char*)This + 0x1D0) = This->viewMatrix;
    }   
}

HOOK(bool, __fastcall, PlayerSpeedUpdate, 0xE6BF20, void* This, void* Edx, int a2)
{
    /*//void* shiftedPtr = (void*)((size_t)This + 40);

    FUNCTION_PTR(void, __thiscall, setVisible, 0xE765A0, void* This, void* message);
    struct MsgSetVisible
    {
        INSERT_PADDING(0x10);
        bool visible;
    } msgSetVisible { {}, false };
    setVisible(This, &msgSetVisible);*/

    bool result = originalPlayerSpeedUpdate(This, Edx, a2);
    return result;
}

std::unordered_set<Camera*> cameras;

HOOK(void, __fastcall, UpdateCamera, 0x10FB770, Camera* This, void* Edx, uint32_t a2)
{
    originalUpdateCamera(This, Edx, a2);
    if (valid())
    {
        cameras.insert(This);
        updateCamera(This);
    }
}

HOOK(void*, __fastcall, CameraDestructor, 0x10FB630, Camera* This, void* Edx)
{
    auto it = cameras.find(This);
    if (it != cameras.end())
        cameras.erase(it);

    return originalCameraDestructor(This, Edx);
}

HOOK(bool, __fastcall, CRenderDirectorFxPipelineUpdate, 0x1105F20, Sonic::CRenderDirectorFxPipeline* This, void* Edx, uint8_t* A2)
{
    char* updateCommand = *(char**)(A2 + 8);

    if (strcmp(updateCommand, "b") == 0)
    {
        for (auto& camera : cameras)
            updateCamera(camera);

        cameras.clear();
    }

    return originalCRenderDirectorFxPipelineUpdate(This, Edx, A2);
}

HOOK(bool, __fastcall, CRenderDirectorMTFxUpdate, 0xD1C480, Sonic::CRenderDirector* This, void* Edx, uint8_t* A2)
{
    char* updateCommand = *(char**)(A2 + 8);

    if (strcmp(updateCommand, "b") == 0)
    {
        for (auto& camera : cameras)
            updateCamera(camera);

        cameras.clear();
    }

    return originalCRenderDirectorMTFxUpdate(This, Edx, A2);
}

extern "C" __declspec(dllexport) void __cdecl OnFrame()
{
    *(bool*)0x1A43103 = false; // disable motion blur
}

extern "C" __declspec(dllexport) void __cdecl Init()
{
//    INSTALL_HOOK(PlayerSpeedUpdate);
    INSTALL_HOOK(UpdateCamera);
    INSTALL_HOOK(CameraDestructor);

    INSTALL_HOOK(CRenderDirectorFxPipelineUpdate);
    INSTALL_HOOK(CRenderDirectorMTFxUpdate);

    // short hop jump for normal one
    WRITE_MEMORY(0x127333B, char*, "sn_jump_d_loop");
    WRITE_MEMORY(0x1274667, char*, "sn_jump_d_loop");
    WRITE_MEMORY(0x12732D4, char*, "sn_jump_d_loop");
    WRITE_MEMORY(0x127A06D, char*, "sn_jump_d_loop");
    //WRITE_MEMORY(0x1A4C59C, uint32_t, 1);
    //WRITE_NOP(0x127333F, 6);

    // disable boost particles
    WRITE_MEMORY(0x15E9048, uint8_t, 0x00);
    WRITE_MEMORY(0x15E9060, uint8_t, 0x00);
    WRITE_MEMORY(0x15F99F8, uint8_t, 0x00);
    WRITE_MEMORY(0x15F9A10, uint8_t, 0x00);
    WRITE_MEMORY(0x164330C, uint8_t, 0x00);

    // disable jump particles
    WRITE_MEMORY(0x15F99DC, uint8_t, 0x00);
    WRITE_MEMORY(0x15E902C, uint8_t, 0x00);

    // no spin when dashpad
    WRITE_MEMORY(0xE0AC1C, uint8_t, 0xE9, 0x27, 0x01, 0x00, 0x00);
    WRITE_MEMORY(0xE0C734, uint8_t, 0xE9, 0x27, 0x01, 0x00, 0x00);

    //WRITE_MEMORY(0x10FB944, uint8_t, 0x90, 0xE9);
    //WRITE_NOP(0x10FC458, 0x13);DED0E0
}