modded class SCR_AIStaticArtilleryBehavior
{
	// Fix bug where DisableLOD be reset after using mortar
	override void OnActionDeselected()
	{
		AIAgent agent = m_Utility.GetOwner();
		
		SCR_ChimeraAIAgent chimeraAgent = SCR_ChimeraAIAgent.Cast(agent);
		if (chimeraAgent && chimeraAgent.m_bPreventMaxLODActual)
			return;
		SCR_AIGroup aiGroup = SCR_AIGroup.Cast(agent);
		if (aiGroup && aiGroup.m_bPreventMaxLODActual)
			return;
		
		agent.AllowMaxLOD();
	}
}
