//AUTHOR BORIZZ.K (s-platoon.ru)
//19.09.2019 - 04.11.2020

modded class TentBase extends ItemBase
{
	//#define TentBaseMyDebug

	override bool CanPutIntoHands(EntityAI parent)
	{
		if (PlayerBase.Cast(parent).GetHumanInventory().GetEntityInHands())
		{	
			return false;
		}
		return super.CanPutIntoHands( parent );
	}
}
