#pragma once

namespace CustomSonicAPI
{
    enum class SelectSnMaterialType
    {
        Default,
        Magenta,
        Pink,
        Red,
        Orange,
        Yellow,
        Green,
        Cyan,
        Black,
        White,
        S4E2,
        Custom
    };

    enum class SelectEyelidType
    {
        Default,
        Lashes,
        Skin
    };

    enum class SelectSsnHeadType
    {
        Default,
        Shadow,
        Upward
    };

    enum class SelectSsnMaterialType
    {
        Default,
        Super2,
        Hyper,
        Dark
    };

    enum class SelectJumpBallType
    {
        Default,
        SWA,
        BetaSWA,
        BAP,
        SA1,
        LW,
        Forces,
        SA2,
        NoVFX,
        NoBall
    };

    enum class SelectBounceBallType
    {
        BAP,
        Original,
        SWA,
        BetaSWA,
        SA1,
        SA2,
        LW,
        Forces,
        NoVFX
    };

    struct MsgGetCustomSonicSelection
    {
        static constexpr size_t ID = 1;
        
        const char* HeadName{};
        const char* BodyName{};
        const char* ShoesName{};
        const char* HandRName{};
        const char* HandLName{};
        
        SelectSnMaterialType SelectSnMaterial{};
        SelectEyelidType SelectEyelid{};
        SelectSsnHeadType SelectSsnHead{};
        SelectSsnMaterialType SelectSsnMaterial{};
        SelectJumpBallType SelectJumpBall{};
        SelectBounceBallType SelectBounceBall{};
    };
}