#include "CustomSonicItems.h"
#include "SonicPlayer.h"

ItemDataBody s_ItemDataBodyPrev{};
std::vector<ItemDataHead> s_ItemDataHead{};
std::vector<ItemDataBody> s_ItemDataBody{};
std::vector<ItemDataShoes> s_ItemDataShoes{};
std::vector<ItemDataHandR> s_ItemDataHandR{};
std::vector<ItemDataHandL> s_ItemDataHandL{};

int SelectHeadData{};
int SelectBodyData{};
int SelectShoesData{};
int SelectHandRData{};
int SelectHandLData{};

SelectOptionType SelectOption = SelectOptionType::SnQuill;
SelectSnQuillType SelectSnQuill = SelectSnQuillType::Default;
SelectSsnQuillType SelectSsnQuill = SelectSsnQuillType::Default;
SelectEyelidType SelectEyelid = SelectEyelidType::Default;
SelectSnMaterialType SelectSnMaterial = SelectSnMaterialType::Default;
SelectSsnMaterialType SelectSsnMaterial = SelectSsnMaterialType::Default;
SelectJumpBallType SelectJumpBall = SelectJumpBallType::Default;
SelectBounceBallType SelectBounceBall = SelectBounceBallType::BAP;

std::map<SelectOptionType, const char*> SelectOptionNameMap =
{
	{ SelectOptionType::SnQuill,		"SBSnQuill" },
	{ SelectOptionType::SsnQuill,		"SBSsnQuill" },
	{ SelectOptionType::Eyelids,		"SBEyelids" },
	{ SelectOptionType::SnMaterial,		"SBSnMaterial" },
	{ SelectOptionType::SsnMaterial,	"SBSsnMaterial" },
	{ SelectOptionType::JumpBall,		"SBJumpball" },
	{ SelectOptionType::BounceBall,		"SBBounceball" },
	{ SelectOptionType::Overflow01,		"Null" },
	{ SelectOptionType::Overflow02,		"Null" },
	{ SelectOptionType::Overflow03,		"Null" }
};

bool isRenderableCreated{};
bool isJumpBallHide{};

boost::shared_ptr<CObjCustomSonicPlayer> obj_CObjCustomSonicPlayer;

void MsgJumpModelHide(bool Enabled)
{
	isJumpBallHide = Enabled;
}

Hedgehog::Base::CSharedString ArchiveHeadString()
{
	char result[512]{};

	auto mapChar = s_ItemDataHead[SelectHeadData].Name;
	auto mapAltCount = s_ItemDataHead[SelectHeadData].AltCount;
	auto mapAltSelect = s_ItemDataHead[SelectHeadData].AltSelect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Head/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Head/%s", mapChar.c_str());

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelHeadString(bool in_isSuper)
{
	char result[512]{};

	auto mapChar = s_ItemDataHead[SelectHeadData].Name;
	auto mapAltCount = s_ItemDataHead[SelectHeadData].AltCount;
	auto mapAltSelect = s_ItemDataHead[SelectHeadData].AltSelect;
	const char* texExtForm = "";
	const char* texExtSn = "_Sn_";
	const char* texExtSsn = "_Ssn_";

	if (!in_isSuper)
		texExtForm = texExtSn;
	else
		texExtForm = texExtSsn;

	if (mapAltCount >= 1)
		sprintf(result, "chr%s%s_%02d", texExtForm, mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr%s%s", texExtForm, mapChar.c_str());


	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveBaseHeadString()
{
	char result[512]{};

	auto mapHeadHideHead = s_ItemDataHead[SelectHeadData].HideHead;
	const char* texExt = "BdyHead";

	if (mapHeadHideHead)
		sprintf(result, "");
	else
		sprintf(result, "Customize/bdy_Body/%s", texExt);

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelBaseHeadString(bool in_isSuper)
{
	char result[512]{};

	auto mapHeadHideHead = s_ItemDataHead[SelectHeadData].HideHead;
	const char* texExtMesh = "BdyHead";
	const char* texExtType = "";
	const char* texExtOption = "";
	const char* texExtSn = "_Sn_";
	const char* texExtSsn = "_Ssn_";
	const char* texExtAlt0 = "_00";
	const char* texExtAlt1 = "_01";
	const char* texExtAlt2 = "_02";
	const char* texExtAlt3 = "_03";
	const char* texExtAlt4 = "_04";

	if (!in_isSuper)
	{
		texExtType = texExtSn;
		switch (SelectSnQuill)
		{
		case SelectSnQuillType::Default:
			texExtOption = texExtAlt0;
			break;
		case SelectSnQuillType::Short:
			texExtOption = texExtAlt1;
			break;
		case SelectSnQuillType::Shadow:
			texExtOption = texExtAlt2;
			break;
		}
	}
	else
	{
		texExtType = texExtSsn;
		switch (SelectSsnQuill)
		{
		case SelectSsnQuillType::Default:
			texExtOption = texExtAlt0;
			break;
		case SelectSsnQuillType::Shadow:
			texExtOption = texExtAlt1;
			break;
		case SelectSsnQuillType::Upward:
			texExtOption = texExtAlt2;
			break;
		case SelectSsnQuillType::Darkspine:
			texExtOption = texExtAlt3;
			break;
		case SelectSsnQuillType::Downward:
			texExtOption = texExtAlt4;
			break;
		}
	}

	if (mapHeadHideHead)
		sprintf(result, "");
	else
		sprintf(result, "chr%s%s%s", texExtType, texExtMesh, texExtOption);

	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveMouthString()
{
	char result[512]{};

	auto mapHeadHideMouth = s_ItemDataHead[SelectHeadData].HideMouth;
	const char* texExt = "BdyMouth";

	if (mapHeadHideMouth)
		sprintf(result, "");
	else
		sprintf(result, "Customize/bdy_Body/%s", texExt);

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelMouthString(bool in_isLeft)
{
	char result[512]{};

	auto mapHeadHideMouth = s_ItemDataHead[SelectHeadData].HideMouth;
	const char* texExtSn = "_Sn_";
	const char* texExtMesh = "BdyMouth";
	const char* texExtOption = "";
	const char* texExtMthR = "_R";
	const char* texExtMthL = "_L";

	if (in_isLeft)
		texExtOption = texExtMthL;
	else
		texExtOption = texExtMthR;

	if (mapHeadHideMouth)
		sprintf(result, "");
	else
		sprintf(result, "chr%s%s%s", texExtSn, texExtMesh, texExtOption);

	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveEyelidString()
{
	char result[512]{};

	const char* texExtVar = "";
	const char* texExt0 = "BdyEyeDefault";
	const char* texExt1 = "BdyEyeLash";
	const char* texExt2 = "BdyEyeSkin";

	switch (SelectEyelid)
	{
	case SelectEyelidType::Default:
		texExtVar = texExt0;
		break;
	case SelectEyelidType::Lashes:
		texExtVar = texExt1;
		break;
	case SelectEyelidType::Skin:
		texExtVar = texExt2;
		break;
	}

	sprintf(result, "Customize/bdy_Body/%s", texExtVar);

	return result;
}
Hedgehog::Base::CSharedString CModelEyelidString()
{
	char result[512]{};

	const char* texExtVar = "";
	const char* texExt0 = "BdyEyeDefault";
	const char* texExt1 = "BdyEyeLash";
	const char* texExt2 = "BdyEyeSkin";

	switch (SelectEyelid)
	{
	case SelectEyelidType::Default:
		texExtVar = texExt0;
		break;
	case SelectEyelidType::Lashes:
		texExtVar = texExt1;
		break;
	case SelectEyelidType::Skin:
		texExtVar = texExt2;
		break;
	}

	sprintf(result, "chr_Sn_%s", texExtVar);
	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveBodyString()
{
	char result[512]{};

	auto mapChar = s_ItemDataBody[SelectBodyData].Name;
	auto mapAltCount = s_ItemDataBody[SelectBodyData].AltCount;
	auto mapAltSelect = s_ItemDataBody[SelectBodyData].AltSelect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Body/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Body/%s", mapChar.c_str());

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelBodyString()
{
	char result[512]{};

	auto mapChar = s_ItemDataBody[SelectBodyData].Name;
	auto mapAltCount = s_ItemDataBody[SelectBodyData].AltCount;
	auto mapAltSelect = s_ItemDataBody[SelectBodyData].AltSelect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveShoeString()
{
	char result[512]{};

	auto mapBodyHideShoes = s_ItemDataBody[SelectBodyData].HideShoes;
	auto mapChar = s_ItemDataShoes[SelectShoesData].Name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].AltCount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].AltSelect;

	if (!mapBodyHideShoes)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Shoes/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Shoes/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelShoeString()
{
	char result[512]{};

	auto mapBodyHideShoes = s_ItemDataBody[SelectBodyData].HideShoes;
	auto mapChar = s_ItemDataShoes[SelectShoesData].Name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].AltCount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].AltSelect;

	if (!mapBodyHideShoes)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveHandRString()
{
	char result[512]{};

	auto mapBodyHideHandR = s_ItemDataBody[SelectBodyData].HideHandR;
	auto mapChar = s_ItemDataHandR[SelectHandRData].Name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].AltCount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].AltSelect;

	if (!mapBodyHideHandR)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Hand_R/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Hand_R/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelHandRString()
{
	char result[512]{};

	auto mapBodyHideHandR = s_ItemDataBody[SelectBodyData].HideHandR;
	auto mapChar = s_ItemDataHandR[SelectHandRData].Name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].AltCount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].AltSelect;

	if (!mapBodyHideHandR)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveHandLString()
{
	char result[512]{};

	auto mapBodyHideHandL = s_ItemDataBody[SelectBodyData].HideHandL;
	auto mapChar = s_ItemDataHandL[SelectHandLData].Name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].AltCount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].AltSelect;

	if (!mapBodyHideHandL)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Hand_L/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Hand_L/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelHandLString()
{
	char result[512]{};

	auto mapBodyHideHandL = s_ItemDataBody[SelectBodyData].HideHandL;
	auto mapChar = s_ItemDataHandL[SelectHandLData].Name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].AltCount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].AltSelect;

	if (!mapBodyHideHandL)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString CMaterialBodyString(bool in_isSuper)
{
	char result[512]{};

	const char* texExtVar = "chr_sn_body_original";
	const char* texExtOG = "chr_sn_body_original"; //OG
	const char* texExtMagenta = "chr_sn_body_magenta"; //Magenta
	const char* texExtPink = "chr_sn_body_pink"; //Pink
	const char* texExtRed = "chr_sn_body_red"; //Red
	const char* texExtOrange = "chr_sn_body_orange"; //Orange
	const char* texExtYellow = "chr_sn_body_yellow"; //Yellow
	const char* texExtGreen = "chr_sn_body_green"; //Green
	const char* texExtCyan = "chr_sn_body_cyan"; //Cyan
	const char* texExtBlack = "chr_sn_body_black"; //Black
	const char* texExtWhite = "chr_sn_body_white"; //White
	const char* texExtS4E2 = "chr_sn_body_s4e2"; //S4E2
	const char* texExtCustom = "chr_sn_body_custom"; //Custom

	const char* texExtSuper = "chr_ssn_body_original"; //Super
	const char* texExtSuper2 = "chr_ssn_body_ss2"; //Super2
	const char* texExtHyper = "chr_ssn_body_hyper"; //Hyper
	const char* texExtDark = "chr_ssn_body_dark"; //Dark
	const char* texExtDarkspine = "chr_ssn_body_darkspine"; //Darkspine

	switch (SelectSnMaterial)
	{
	case SelectSnMaterialType::Default:
		texExtVar = texExtOG;
		break;
	case SelectSnMaterialType::Magenta:
		texExtVar = texExtMagenta;
		break;
	case SelectSnMaterialType::Pink:
		texExtVar = texExtPink;
		break;
	case SelectSnMaterialType::Red:
		texExtVar = texExtRed;
		break;
	case SelectSnMaterialType::Orange:
		texExtVar = texExtOrange;
		break;
	case SelectSnMaterialType::Yellow:
		texExtVar = texExtYellow;
		break;
	case SelectSnMaterialType::Green:
		texExtVar = texExtGreen;
		break;
	case SelectSnMaterialType::Cyan:
		texExtVar = texExtCyan;
		break;
	case SelectSnMaterialType::Black:
		texExtVar = texExtBlack;
		break;
	case SelectSnMaterialType::White:
		texExtVar = texExtWhite;
		break;
	case SelectSnMaterialType::S4E2:
		texExtVar = texExtS4E2;
		break;
	case SelectSnMaterialType::Custom:
		texExtVar = texExtCustom;
		break;
	}

	if (in_isSuper)
	{
		switch (SelectSsnMaterial)
		{
		case SelectSsnMaterialType::Default:
			texExtVar = texExtSuper;
			break;
		case SelectSsnMaterialType::Super2:
			texExtVar = texExtSuper2;
			break;
		case SelectSsnMaterialType::Hyper:
			texExtVar = texExtHyper;
			break;
		case SelectSsnMaterialType::Dark:
			texExtVar = texExtDark;
			break;
		//case SelectSsnMaterialType::Darkspine:
		//	texExtVar = texExtDarkspine;
		//	break;
		}
	}

	sprintf(result, "%s", texExtVar);
	return Hedgehog::Base::CSharedString(result);
}

void CreateCustomizeSonicPlayerRenderable()
{
	if (!isRenderableCreated)
	{
		obj_CObjCustomSonicPlayer = boost::make_shared<CObjCustomSonicPlayer>();
		Sonic::CGameDocument::GetInstance()->AddGameObject(obj_CObjCustomSonicPlayer);
		isRenderableCreated = true;
	}
}

void KillCustomizeSonicPlayerRenderable()
{
	if (isRenderableCreated)
	{
		obj_CObjCustomSonicPlayer->Kill();
		obj_CObjCustomSonicPlayer = nullptr;
		isRenderableCreated = false;
	}
}

void OnFrameHandler()
{
	//Send Wildfire VFX
	if (s_ItemDataBody[SelectBodyData].Name == "BdWildFire")
		MsgWildFire(1);
	else
		MsgWildFire(0);

	MsgJumpBall(SelectJumpBall);
}

void ReadJson(std::string jsonFilePath, int cCat)
{
	std::ifstream stream(jsonFilePath);
	if (stream.is_open())
	{
		json json;
		stream >> json;

		switch (cCat)
		{
		case 0:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHead.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.HideHead = obj["HideHead"];
				itemData.HideMouth = obj["HideMouth"];
				itemData.AltSelect = 0;
			}
			break;
		case 1:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataBody.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.HideShoes = obj["HideShoes"];
				itemData.HideHandR = obj["HideHandR"];
				itemData.HideHandL = obj["HideHandL"];
				itemData.AltSelect = 0;
			}
			break;
		case 2:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataShoes.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.AltSelect = 0;
			}
			break;
		case 3:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandR.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.AltSelect = 0;
			}
			break;
		case 4:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandL.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.AltSelect = 0;
			}
			break;
		}
	}
}