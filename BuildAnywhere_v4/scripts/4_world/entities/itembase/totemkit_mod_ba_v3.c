modded class TerritoryFlagKit extends KitBase
{
	override bool DoPlacingHeightCheck()
	{
		return false;
	}
	
	override float HeightCheckOverride()
	{
		return 1.6;
	}
}
