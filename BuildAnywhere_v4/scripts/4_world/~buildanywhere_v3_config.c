//26.07.2021
static ref BuildAnywhere_v3 g_BuildAnywhere_v3 = new BuildAnywhere_v3();
class BuildAnywhere_v3
{
	private string modversion = "3.4";

	void BuildAnywhere_v3()
	{
		string modversionpath = "CfgMods" + " BuildAnywhere_V3 " + "version";
		if ( GetGame().ConfigIsExisting( modversionpath ) )
		{
			modversion = GetGame().ConfigGetTextOut(modversionpath);
		}
		Print("::: [BuildAnywhere_v3]: ver: " + modversion + " Init");

		#ifdef SERVER
		GetBuildAnywhere_v3_Config();
		#endif

		delete g_BuildAnywhere_v3;
	}
}

#ifdef SERVER
static ref BuildAnywhere_v3_Config g_BuildAnywhere_v3_Config;
static ref BuildAnywhere_v3_Config GetBuildAnywhere_v3_Config()
{
	if (!g_BuildAnywhere_v3_Config)
	{
		g_BuildAnywhere_v3_Config = new BuildAnywhere_v3_Config();
	}
	return g_BuildAnywhere_v3_Config;
}

class BuildAnywhere_v3_Config
{
	protected static string configfile = "$profile:BuildAnywhere.json";
	protected int 	TENT_CAN_PLACE_ANYWHERE_MODE 	= 0;
	protected float	PLACEMENT_HEIGHT_DIFF_MAX		= 1.64;
	protected float	PLACEMENT_HEIGHT_DIFF_MIN		= -1.24;
	protected float	PLACEMENT_PITCH_LIMIT_FACTOR	= 2.5;
	protected float	PLACEMENT_ROLL_LIMIT_FACTOR 	= 2.5;
	
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
			if (TENT_CAN_PLACE_ANYWHERE_MODE < 0)
			{
				Print("::: [BuildAnywhere_v3]: Loaded: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE + ": Set to mod dafault: " + 0);
				TENT_CAN_PLACE_ANYWHERE_MODE = 0; //BuildAnywhere_v3 default
				return false;
			}
			else if (TENT_CAN_PLACE_ANYWHERE_MODE > 2)
			{
				Print("::: [BuildAnywhere_v3]: Loaded: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE + ": Set to game dafault: " + 2);
				TENT_CAN_PLACE_ANYWHERE_MODE = 2; //Game default
				return false;
			}
			else
			{
				Print("::: [BuildAnywhere_v3]: Loaded: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE + " - Ok!");
			}
			return true;
		}
		else
		{
			Print("::: [BuildAnywhere_v3]: Config file not found: " + configfile);
			Print("::: [BuildAnywhere_v3]: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE + ": Set to mod dafault: " + 0);
			TENT_CAN_PLACE_ANYWHERE_MODE = 0;
			return false;
		}
	}
	
	private void SaveConfig()
	{
		Print("::: [BuildAnywhere_v3]: Save config: " + configfile);
		JsonFileLoader<BuildAnywhere_v3_Config>.JsonSaveFile(configfile, this);
	}

	void DebugConfig()
	{
		Print("::: [BuildAnywhere_v3]: TENT_CAN_PLACE_ANYWHERE_MODE: " + TENT_CAN_PLACE_ANYWHERE_MODE);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_HEIGHT_DIFF_MAX:    " + PLACEMENT_HEIGHT_DIFF_MAX);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_HEIGHT_DIFF_MIN:    " + PLACEMENT_HEIGHT_DIFF_MIN);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_PITCH_LIMIT_FACTOR: " + PLACEMENT_PITCH_LIMIT_FACTOR);
		Print("::: [BuildAnywhere_v3]: PLACEMENT_ROLL_LIMIT_FACTOR:  " + PLACEMENT_ROLL_LIMIT_FACTOR);
	}

	/**********************************************************************************************************************************************************/
	
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

	/**********************************************************************************************************************************************************/
}
#endif
