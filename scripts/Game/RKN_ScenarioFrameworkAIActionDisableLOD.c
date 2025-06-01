[BaseContainerProps(), SCR_ContainerAIActionTitle()]
modded class SCR_ScenarioFrameworkAIActionDisableLOD
{
	// Remember if DisableLOD has been used. To fix bug where DisableLOD will be reset after using mortar.
	override void OnActivate()
	{
		super.OnActivate();
		
		if (!m_bDisableLOD)
			return;
		
		array<AIAgent> agents = {};
		m_AIGroup.GetAgents(agents);
		m_AIGroup.m_bPreventMaxLODActual = true;
		
		foreach (AIAgent agent : agents)
		{
			SCR_ChimeraAIAgent chimeraAgent = SCR_ChimeraAIAgent.Cast(agent);
			if (chimeraAgent)
				chimeraAgent.m_bPreventMaxLODActual = true;
		}
	}
}
