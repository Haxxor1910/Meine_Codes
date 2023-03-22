//10.07.2020.2159 - 23.07.2021

modded class FireplaceBase extends ItemBase
{
	//FIX IN 1.06.152.829 - 10.12.2019 // Old value: PARAM_FIRE_CONSUM_RATE_AMOUNT = 1;
	//const float	PARAM_FIRE_CONSUM_RATE_AMOUNT		= 0.5;		//! base value of fire consumption rate (how many base energy will be spent on each update)
		
	/***
	const float TEMPERATURE_LOSS_MP_DEFAULT			= 0.51;		//Default 1.0 //0.41
	const float TEMPERATURE_LOSS_MP_STONES			= 0.46;		//20% boost //Default 0.83 //0.36
	const float TEMPERATURE_LOSS_MP_OVEN			= 0.41;		//50% boost //Default 0.67 //0.31

	const float FUEL_BURN_RATE_DEFAULT				= 0.51;		//Default 1.0 //0.41
	const float FUEL_BURN_RATE_STONES				= 0.46;		//Default 0.83  //0.36
	const float FUEL_BURN_RATE_OVEN					= 0.41;		//Default 0.67 //0.31
	***/

	const float	MIN_CEILING_HEIGHT 					= 1.7;

	override static bool IsEntityOnInteriorSurface( notnull EntityAI entity_ai )
	{
		if (entity_ai.IsInherited(Fireplace)) return false;
			
		string surface_type;
		vector fireplace_pos = entity_ai.GetPosition();
		GetGame().SurfaceGetType3D( fireplace_pos[0], fireplace_pos[1] + 1.0, fireplace_pos[2], surface_type );
		if ( ( GetGame().ConfigGetInt( "CfgSurfaces " + surface_type + " interior" ) ) == 1 )
			return true;
		else return false;
	}
}
