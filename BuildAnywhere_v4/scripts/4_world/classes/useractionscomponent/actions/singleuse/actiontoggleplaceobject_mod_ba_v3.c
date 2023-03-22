//18.09.2019 - 05.11.2020

modded class ActionTogglePlaceObject: ActionSingleUseBase
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		// All barrels placeable //mod
		if ( item.IsInherited(Barrel_ColorBase) || item.IsInherited(BarrelHoles_ColorBase) )
		{
			return true;
		}
		return super.ActionCondition( player, target, item );
	}
}
