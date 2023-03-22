class CfgPatches
{
	class BuildAnywhere_V3
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgMods
{
	class BuildAnywhere_V3
	{
		dir = "BuildAnywhere_V3";
		picture = "BuildAnywhere_V3/images/logos/modlogohover.edds";
		action = "https://steamcommunity.com/sharedfiles/filedetails/?id=1854626456";
		hideName = 0;
		hidePicture = 0;
		name = "BuildAnywhere_V3";
		credits = "Cleetus, Equadro";
		author = "Borizz.K aka [G.P]NOlZZ(MC2)";
		authorID = "76561198156927007";
		version = "3.4.26.07.2021.1";
		extra = 0;
		type = "mod";
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"BuildAnywhere_V3/definitions","BuildAnywhere_V3/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"BuildAnywhere_V3/definitions","BuildAnywhere_V3/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"BuildAnywhere_V3/definitions","BuildAnywhere_V3/scripts/5_Mission"};
			};
		};
	};
};
