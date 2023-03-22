modded class Watchtower extends BaseBuildingBase
{	
	override bool PerformRoofCheckForBase( string partName, PlayerBase player, out bool result )
	{
		result = false;
		return false;
	}
}
