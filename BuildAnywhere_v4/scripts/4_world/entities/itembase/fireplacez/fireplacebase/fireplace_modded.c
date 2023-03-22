modded class Fireplace extends FireplaceBase
{
	override static bool CanIgniteEntityAsFireplace( notnull EntityAI entity )
	{
		//check ceiling (enough space for smoke)
		/*if ( MiscGameplayFunctions.IsUnderRoof( entity, FireplaceBase.MIN_CEILING_HEIGHT ) && IsEntityOnInteriorSurface( entity ) )
		{
			return false;
		}*/
		
		//check surface
		if ( FireplaceBase.IsEntityOnWaterSurface( entity ) )
		{
			return false;
		}

		//check wetness/rain/wind
		if ( FireplaceBase.IsEntityWet( entity ) )
		{
			return false;
		}

		// check if the fireplace isnt below a roof
		if ( !MiscGameplayFunctions.IsUnderRoof( entity ) )
		{
			// if not, check if there is strong rain or wind
			if ( FireplaceBase.IsRainingAboveEntity( entity ) || FireplaceBase.IsWindy() )
			{
				return false;
			}
		}

		return true;	
	}
}
