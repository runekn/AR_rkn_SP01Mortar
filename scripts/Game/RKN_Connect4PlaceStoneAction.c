class RKN_Connect4PlaceStoneAction : SCR_ScriptedUserAction
{
	[Attribute(defvalue: "1")]
	int m_iPlayer;
	
	[Attribute()]
	int m_iColumn;
	
	RKN_Connect4BoardComponent m_Board;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_Board = RKN_Connect4BoardComponent.Cast(pOwnerEntity.FindComponent(RKN_Connect4BoardComponent));
	}
	
	override bool CanBeShownScript(IEntity user)
	{
		if (!super.CanBeShownScript(user) || !m_Board || !m_Board.CanPlace(m_iColumn))
			return false;
		return true;
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if (!m_Board)
			return;
		
		m_Board.PlaceStone(m_iColumn, m_iPlayer);
	}
}
