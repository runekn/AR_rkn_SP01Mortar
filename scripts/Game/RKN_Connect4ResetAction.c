class RKN_Connect4ResetAction : SCR_ScriptedUserAction
{	
	RKN_Connect4BoardComponent m_Board;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_Board = RKN_Connect4BoardComponent.Cast(pOwnerEntity.FindComponent(RKN_Connect4BoardComponent));
	}
	
	override bool CanBeShownScript(IEntity user)
	{
		if (!super.CanBeShownScript(user))
			return false;
		return true;
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!m_Board)
			return;
		
		m_Board.Reset();
	}
}
