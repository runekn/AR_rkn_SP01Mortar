[BaseContainerProps(), SCR_ContainerActionTitle()]
class RKN_VoiceOverPlayFiremissionSequence : SCR_ScenarioFrameworkActionVoiceOverPlaySequence
{
	override void OnActivate(IEntity object)
	{
		int replacerIndex = GetReplacerIndex();
		if (replacerIndex != -1)
		{
			if (m_ActorGetter)
			{
				SCR_ScenarioFrameworkParam<array<IEntity>> entityArrayWrapper = SCR_ScenarioFrameworkParam<array<IEntity>>.Cast(m_ActorGetter.Get());
				if (entityArrayWrapper)
				{
					foreach (IEntity actorEntity : entityArrayWrapper.GetValue())
					{
						SetFiremissionSignalValues(replacerIndex, actorEntity);
					}
				}
				else
				{
					SCR_ScenarioFrameworkParam<IEntity> entityWrapper = SCR_ScenarioFrameworkParam<IEntity>.Cast(m_ActorGetter.Get());
					if (!entityWrapper)
					{
						if (object)
							Print(string.Format("ScenarioFramework Action: Issue with Getter detected for Action %1 attached on %2.", this, object.GetName()), LogLevel.ERROR);
						else
							Print(string.Format("ScenarioFramework Action: Issue with Getter detected for Action %1.", this), LogLevel.ERROR);
						
						return;
					}
		
					IEntity actorEntity = entityWrapper.GetValue();
					SetFiremissionSignalValues(replacerIndex, actorEntity);
				}
			}
		}
		super.OnActivate(object);
	}
	
	private void SetFiremissionSignalValues(int replacerIndex, IEntity actor)
	{
		SignalsManagerComponent signalManager = SignalsManagerComponent.Cast(actor.FindComponent(SignalsManagerComponent));
		signalManager.SetSignalValue(signalManager.FindSignal("FiremissionIndex"), replacerIndex);
	}
	
	private int GetReplacerIndex()
	{
		Resource container = BaseContainerTools.LoadContainer(m_sVoiceOverDataConfig);
		SCR_VoiceoverData data = SCR_VoiceoverData.Cast(BaseContainerTools.CreateInstanceFromContainer(container.GetResource().ToBaseContainer()));
		SCR_VoiceoverSequence sequence = data.GetSequenceByName(m_sSequenceName);
		RKN_VoiceoverLineFiremission firemissionSequence = RKN_VoiceoverLineFiremission.Cast(sequence);
		if (!firemissionSequence)
		{
			Print("Not a firemission sequence: " + m_sSequenceName, LogLevel.ERROR);
			return -1;
		}
		return firemissionSequence.m_iReplacerIndex;
	}
}
