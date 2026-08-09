#pragma once

namespace CustomSonicAPI
{
    enum class SelectSnQuillType
    {
        Default,
        Short,
        Shadow
    };

    enum class SelectSsnQuillType
    {
        Default,
        Shadow,
        Upward,
        Downward
    };

    enum class SelectEyelidType
    {
        Default,
        Lashes,
        Skin
    };

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
        int HeadAlt{};
        bool HeadHide{};

        const char* BodyName{};
        int BodyAlt{};
        bool BodyHideShoes{};
        bool BodyHideHandR{};
        bool BodyHideHandL{};

        const char* ShoesName{};
        int ShoesAlt{};

        const char* HandRName{};
        int HandRAlt{};

        const char* HandLName{};
        int HandLAlt{};
        
        SelectSnQuillType SelectSnQuill{};
        SelectSsnQuillType SelectSsnQuill{};
        SelectEyelidType SelectEyelid{};
        SelectSnMaterialType SelectSnMaterial{};
        SelectSsnMaterialType SelectSsnMaterial{};
        SelectJumpBallType SelectJumpBall{};
        SelectBounceBallType SelectBounceBall{};
    };
}