//18.09.2019 - 30.12.2020

modded class BarrelHoles_ColorBase extends FireplaceBase
{
	bool Trader_IsInSafezone = false;

	//hands //mod
	override bool CanPutIntoHands( EntityAI parent )
	{
		if ( /* IsBurning() || !IsCargoEmpty() || */  DirectCookingSlotsInUse() /* || IsOpen() */ || IsHologram() || Trader_IsInSafezone)
		{
			return false;
		}

		return true;
	}
}
