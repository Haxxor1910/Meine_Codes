//AUTHOR: BORIZZ.K aka [G.P.]NOIZZ(MC2)
//19.09.2019 - 23.07.2021

modded class PlayerBase extends ManBase
{
	private		bool	TENT_CAN_PLACE_ANYWHERE_MODEDebug 	= false;	// Debug
	protected	int		TENT_CAN_PLACE_ANYWHERE_MODE		= 0;		// Synchronized variable. 0 - soft restrictions (default), 1 - no restrictions (exclude water and much higher or lower than player), 2 - default game restrictions
	protected	float	PLACEMENT_HEIGHT_DIFF_MAX			= 1.61;  	// Maximum positive height difference between player and placement position - only for tents
	protected	float	PLACEMENT_HEIGHT_DIFF_MIN			= -1.21; 	// Maximum negative height difference between player and placement position - only for tents
	protected	float	PLACEMENT_PITCH_LIMIT_FACTOR		= 2.43;		// Maximum pitch angle factor - only for tents and gardenplots - can be placed on a slope
	protected	float	PLACEMENT_ROLL_LIMIT_FACTOR 		= 2.43;		// Maximum roll angle factor - only for tents and gardenplots - can be placed on a slope

	/*** SERVER **********************************************************************************************************************************************************************************************/

	#ifdef SERVER
	void PlayerBase()
	{
		TENT_CAN_PLACE_ANYWHERE_MODE	= GetBuildAnywhere_v3_Config().GetTentCanBePlaceAnywhereLevel();
		PLACEMENT_HEIGHT_DIFF_MAX		= GetBuildAnywhere_v3_Config().GetPlacementHeightDiffMaxLevel();
		PLACEMENT_HEIGHT_DIFF_MIN		= GetBuildAnywhere_v3_Config().GetPlacementHeightDiffMinLevel();
		PLACEMENT_PITCH_LIMIT_FACTOR	= GetBuildAnywhere_v3_Config().GetPlacementPitchLimitFactorLevel();
		PLACEMENT_ROLL_LIMIT_FACTOR		= GetBuildAnywhere_v3_Config().GetPlacementRollLimitFactorLevel();
	}

	override void OnConnect()
	{
		super.OnConnect();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(SendTentCanPlaceAnywhere);
	}

	void SendTentCanPlaceAnywhere()
	{
		ref Param5<int,float,float,float,float> parameters = new Param5<int,float,float,float,float>(TENT_CAN_PLACE_ANYWHERE_MODE,PLACEMENT_HEIGHT_DIFF_MAX,PLACEMENT_HEIGHT_DIFF_MIN,PLACEMENT_PITCH_LIMIT_FACTOR,PLACEMENT_ROLL_LIMIT_FACTOR);
		GetGame().RPCSingleParam(this, BUILDANYWHERE_ERPCs.PARAMETERS, parameters, true, GetIdentity());
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SendTentCanPlaceAnywhere);
	}
	#endif

	/*** SERVER END ******************************************************************************************************************************************************************************************/

	/*** CLIENT **********************************************************************************************************************************************************************************************/

	#ifndef SERVER
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type == BUILDANYWHERE_ERPCs.PARAMETERS)
		{
			ref Param5<int,float,float,float,float> parameters = new Param5<int,float,float,float,float>(TENT_CAN_PLACE_ANYWHERE_MODE,PLACEMENT_HEIGHT_DIFF_MAX,PLACEMENT_HEIGHT_DIFF_MIN,PLACEMENT_PITCH_LIMIT_FACTOR,PLACEMENT_ROLL_LIMIT_FACTOR);
			if (ctx.Read(parameters))
			{
				if (parameters.param1 > -1 && parameters.param1 < 3)
				{
					TENT_CAN_PLACE_ANYWHERE_MODE = parameters.param1;
					Print("[BuildAnywhere_v3]: Received from server: parameters: TENT_CAN_PLACE_ANYWHERE_MODE: " + parameters.param1 + ":" + TENT_CAN_PLACE_ANYWHERE_MODE);
				}
				else
				{
					Print("[BuildAnywhere_v3]: Received from server: TENT_CAN_PLACE_ANYWHERE_MODE: " + parameters.param1 + ": ERROR! Set default: " + 0);
					TENT_CAN_PLACE_ANYWHERE_MODE = 0;
				}
				PLACEMENT_HEIGHT_DIFF_MAX 		= parameters.param2;
				PLACEMENT_HEIGHT_DIFF_MIN 		= parameters.param3;
				PLACEMENT_PITCH_LIMIT_FACTOR 	= parameters.param4;
				PLACEMENT_ROLL_LIMIT_FACTOR 	= parameters.param5;
			}
			else
			{
				Print("[BuildAnywhere_v3]: RPC ERROR! Use default: TENT_CAN_PLACE_ANYWHERE_MODE: " + 0);
				TENT_CAN_PLACE_ANYWHERE_MODE = 0;
			}

			Print("::: [BuildAnywhere_v3]: PLACEMENT_HEIGHT_DIFF_MAX:    " + PLACEMENT_HEIGHT_DIFF_MAX);
			Print("::: [BuildAnywhere_v3]: PLACEMENT_HEIGHT_DIFF_MIN:    " + PLACEMENT_HEIGHT_DIFF_MIN);
			Print("::: [BuildAnywhere_v3]: PLACEMENT_PITCH_LIMIT_FACTOR: " + PLACEMENT_PITCH_LIMIT_FACTOR);
			Print("::: [BuildAnywhere_v3]: PLACEMENT_ROLL_LIMIT_FACTOR:  " + PLACEMENT_ROLL_LIMIT_FACTOR);
		}
	}
	#endif

	/*** CLIENT END ******************************************************************************************************************************************************************************************/

	/*********************************************************************************************************************************************************************************************************/

	int GetTentCanBePlaceAnywhere()
	{
		return TENT_CAN_PLACE_ANYWHERE_MODE;
	}
	
	float GetPlacementHeightDiffMax()
	{
		return PLACEMENT_HEIGHT_DIFF_MAX;
	}
	
	float GetPlacementHeightDiffMin()
	{
		return PLACEMENT_HEIGHT_DIFF_MIN;
	}
	
	float GetPlacementPitchLimitFactor()
	{
		return PLACEMENT_PITCH_LIMIT_FACTOR;
	}
	
	float GetPlacementRollLimitFactor()
	{
		return PLACEMENT_ROLL_LIMIT_FACTOR;
	}

	/*********************************************************************************************************************************************************************************************************/
}
