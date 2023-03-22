//18.09.2019 - 04.11.2020

modded class Barrel_ColorBase : Container_Base
{
	//mod
	override bool CanPutIntoHands( EntityAI parent )
	{
		if ( !IsOpen() )
		{
			return true;
		}
		return true;
	}
}
