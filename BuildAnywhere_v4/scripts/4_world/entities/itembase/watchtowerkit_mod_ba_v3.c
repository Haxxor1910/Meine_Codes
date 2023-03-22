modded class WatchtowerKit extends KitBase
{
	override bool DoPlacingHeightCheck()
	{
		return false;
	}
	
	override float HeightCheckOverride()
	{
		return 1.6;//9.56;
	}
}
