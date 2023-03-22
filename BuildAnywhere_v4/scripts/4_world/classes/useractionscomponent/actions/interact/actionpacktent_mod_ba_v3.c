// BuildAnywhere_v3
// 11.02.2019 - 04.02.2021
// Author: BORIZZ.K (vk.com/opg_servers)

modded class ActionPackTent
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player.GetHumanInventory().GetEntityInHands()) return false;
		return super.ActionCondition(player, target, item);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		//vector offset = Vector(1, 0, 0);
		Object targetParent = action_data.m_Target.GetParent();
		TentBase tent = TentBase.Cast( targetParent );
		if (tent && tent.CanBePacked())
		{
			if (!action_data.m_Player.GetHumanInventory().GetEntityInHands())
			{
				tent.Pack(true);
				action_data.m_Player.ServerTakeEntityToHands(tent);
			}
			else
			{
				vector pos = action_data.m_Player.GetPosition() + (action_data.m_Player.GetDirection() * 0.2);
				tent.Pack( true );
				tent.SetPosition( pos );
				tent.SetOrientation( action_data.m_Player.GetOrientation() );
				tent.PlaceOnSurface();
				tent.SetOrientation(tent.GetOrientation());
			}
		}
	}
}
