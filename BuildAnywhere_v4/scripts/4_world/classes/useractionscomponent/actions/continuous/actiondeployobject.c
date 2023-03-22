#ifdef SERVER

modded class ActionDeployObject: ActionContinuousBase
{			 
	bool z_itemIsTakeable;

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (item && player.GetHumanInventory().GetEntityInHands())
		{
			if (item == player.GetHumanInventory().GetEntityInHands()) z_itemIsTakeable = true;
		}
		return super.ActionCondition( player, target, item );
	}


	override void OnEndServer( ActionData action_data  )
	{
		if( !action_data || !action_data.m_MainItem ) return;

		super.OnEndServer(action_data);
		
		PlaceObjectActionData poActionData;
		poActionData = PlaceObjectActionData.Cast(action_data);
		if ( !poActionData.m_AlreadyPlaced )
		{
			if (action_data.m_MainItem && z_itemIsTakeable)
			{
				if (!action_data.m_MainItem.GetHierarchyParent() && !action_data.m_MainItem.GetParent() && !action_data.m_MainItem.GetHierarchyRootPlayer())
				{
					action_data.m_Player.ServerTakeEntityToHands( action_data.m_MainItem );
				}
			}
		}
		z_itemIsTakeable = false;
	}
}

#endif
