// BuildAnywhere_v3.4

modded class Construction
{
	override bool IsColliding(string part_name)
	{
		return false;
	}

	override bool IsCollidingEx( CollisionCheckData check_data )
	{
		return false;
	}
}
