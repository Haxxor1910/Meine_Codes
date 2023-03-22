class BuildAnywhere_v3
{
	private string modversion;

	private ref BuildAnywhere_v3_Config config;

	void BuildAnywhere_v3()
	{
		modversion = GetModVersion();

		Print("::: [BuildAnywhere_v3]: ver: " + modversion + " Init");

		#ifdef SERVER
		config = new BuildAnywhere_v3_Config();
		#endif
	}

	private string GetModVersion()
	{
		string modversionpath = "CfgMods BuildAnywhere_V3 version";
		if (GetGame().ConfigIsExisting(modversionpath))
		{
			return GetGame().ConfigGetTextOut(modversionpath);
		}
		return "3.4";
	}
}

#ifdef SERVER
class BuildAnywhere_v3_Config
{
	private static string configfile = "$profile:BuildAnywhere.json";

	private int TENT_CAN_PLACE_ANYWHERE_MODE = 0;
	private float PLACEMENT_HEIGHT_DIFF_MAX = 1.64;
	private float PLACEMENT_HEIGHT_DIFF_MIN = -1.24;
	private float PLACEMENT_PITCH_LIMIT_FACTOR = 2.5;
	private float PLACEMENT_ROLL_LIMIT_FACTOR = 2.5;

	void BuildAnywhere_v3_Config()
	{
		Init();
	}

	private void Init()
	{
		if (!LoadConfig())
		{
			SaveConfig();
		}

		DebugConfig();
	}

	private bool LoadConfig()
	{
		if (FileExist(configfile))
		{
			Print("::: [BuildAnywhere_v3]: Load config: " + configfile);
			JsonFileLoader<BuildAnywhere_v3_Config>.JsonLoadFile(configfile, this);
			CheckTentPlacementMode();
			return true;
		}
		else
		{
			Print("::: [BuildAnywhere_v3]: Config file not found: " + configfile);
			return false;
		}
	}

	private void SaveConfig()
	{
		Print("::: [BuildAnywhere_v3]: Save config: " + configfile);
		JsonFileLoader<BuildAnywhere_v3_Config>.JsonSaveFile(configfile, this);
	}

	private void DebugConfig()
	{
		Print("::: [BuildAnywhere_v3]: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_HEIGHT_DIFF_MAX: " + PLACEMENT_HEIGHT_DIFF_MAX);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_HEIGHT_DIFF_MIN: " + PLACEMENT_HEIGHT_DIFF_MIN);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_PITCH_LIMIT_FACTOR: " + PLACEMENT_PITCH_LIMIT_FACTOR);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_ROLL_LIMIT_FACTOR: " + PLACEMENT_ROLL_LIMIT_FACTOR);
	}

	private void CheckTentPlacementMode()
	{
		if (TENT_CAN_PLACE_ANYWHERE_MODE < 0)
		{
			Print("::: [BuildAnywhere_v3]: Loaded: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE + ": Set to mod default: " + 0);
			TENT_CAN_PLACE_ANYWHERE_MODE = 0;
		}
		else if (TENT_CAN_PLACE_ANYWHERE_MODE > 2)
		{
			Print("::: [BuildAnywhere_v3]: Loaded: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE + ": Set to game default: " + 2);
			TENT_CAN_PLACE_ANYWHERE_MODE = 2;
		}
		else
		{
			Print("::: [BuildAnywhere_v3]: Loaded: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE + " - Ok!");
		}
		int GetTentCanBePlaceAnywhereLevel()
		{
			return TENT_CAN_PLACE_ANYWHERE_MODE;
		}

		float GetPlacementHeightDiffMaxLevel()
		{
			return PLACEMENT_HEIGHT_DIFF_MAX;
		}

		float GetPlacementHeightDiffMinLevel()
		{
			return PLACEMENT_HEIGHT_DIFF_MIN;
		}

		float GetPlacementPitchLimitFactorLevel()
		{
			return PLACEMENT_PITCH_LIMIT_FACTOR;
		}

		float GetPlacementRollLimitFactorLevel()
		{
			return PLACEMENT_ROLL_LIMIT_FACTOR;
		}
	}
}
