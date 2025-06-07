[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class RKN_MortarTargetSlotClass : SCR_ScenarioFrameworkSlotBaseClass
{
}

class RKN_MortarTargetSlot : SCR_ScenarioFrameworkSlotBase
{
	[Attribute(defvalue: "100", desc: "Shell must hit within this radius to be repoted as a hit.", category: "Mortar target")]
	int m_iTargetRadius;
	
	[Attribute(defvalue: "1000", desc: "Splashes outside this radius will be ignored.", category: "Mortar target")]
	int m_iTargetIgnoreRadius;
	
	[Attribute(defvalue: "true", desc: "Display target radius as sphere.", category: "Debug")]
	bool m_bDebugTargetSphere;
	
	[Attribute(defvalue: "true", desc: "Display ignore radius as sphere.", category: "Debug")]
	bool m_bDebugIgnoreSphere;
	
	[Attribute(defvalue: "1", desc: "How many shells must land within the target radius to execute success actions. Otherwise failure actions will be executed instead.", category: "Mortar target")]
	int m_iRequiredSplashesOnTarget;
	
	[Attribute(defvalue: "1", desc: "How many shells the target will wait for during FFE stage before executing success or failure actions.", category: "Mortar target")]
	int m_iRequiredSplashes;
	
	[Attribute(defvalue: "-1", desc: "Will execute success or failure actions and disable target after this amount of seconds.", category: "Mortar target")]
	int m_iTimeoutInSeconds;
	
	[Attribute(defvalue: "-1", desc: "Will randomize timeout between 'Timeout in Seconds' and this if set", category: "Mortar target")]
	int m_iTimeoutInSecondsMax;
	
	[Attribute(typename.EnumToString(SCR_EAIArtilleryAmmoType, SCR_EAIArtilleryAmmoType.HIGH_EXPLOSIVE), UIWidgets.ComboBox, desc: "Shell ammo type which activates this slot", enumType: SCR_EAIArtilleryAmmoType, category: "Mortar target")]
	SCR_EAIArtilleryAmmoType m_eAmmoType;
	
	[Attribute(defvalue: "1", desc: "What to do once target has been hit required amount of times", UIWidgets.Auto, category: "Mortar target")]
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aActionsOnCompletion;
	
	[Attribute(defvalue: "1", desc: "What to do if target failed to be hit required amount of times", UIWidgets.Auto, category: "Mortar target")]
	ref array<ref SCR_ScenarioFrameworkActionBase> m_aActionsOnFailure;
	
	[Attribute(defvalue: "true", desc: "Observer will first request one shell that will be corrected unto target, before requesting fire for effect.", category: "Mortar target")]
	bool m_bRequestAdjustFire;
	
	[Attribute(desc: "From where adjustment are calculated relative of", UIWidgets.Auto, category: "Mortar target")]
	ref PointInfo m_oObserverPosition;
	
	[Attribute(desc: "Entity playing the voiceover lines", UIWidgets.Auto, category: "Mortar target")]
	ref SCR_ScenarioFrameworkGet m_SoundActorGetter;
	
	[Attribute(desc: "Config for the subtitles and sound event names for fire adjustments", UIWidgets.Auto, category: "Mortar target")]
	ResourceName m_sAdjustSoundConfig;
	
	int m_iSplashesOnTarget = 0;
	int m_iSplashes = 0;
	int	m_iDebugOuterShapeColor = ARGB(32, 32, 255, 255);
	bool m_bAdjustFire = true;
	ref RKN_MortarTargetVoiceLines m_sAdjustSounds;
	
	//------------------------------------------------------------------------------------------------
	override bool InitOtherThings()
	{
		Print("RKN_MortarTargetSlot.InitOtherThings");
		RKN_MortarSystem system = RKN_MortarSystem.GetInstance();
		if (system)
			system.AddTarget(this);
		m_bAdjustFire = m_bRequestAdjustFire;
		if (m_oObserverPosition)
			m_oObserverPosition.Init(GetOwner());
		
		if (m_sAdjustSoundConfig)
		{
			Resource container = BaseContainerTools.LoadContainer(m_sAdjustSoundConfig);
			m_sAdjustSounds = RKN_MortarTargetVoiceLines.Cast(BaseContainerTools.CreateInstanceFromContainer(container.GetResource().ToBaseContainer()));
		}
		
		if (m_iTimeoutInSeconds > 0)
		{
			int delay = m_iTimeoutInSeconds;
			if (m_iTimeoutInSecondsMax > m_iTimeoutInSeconds)
				delay = Math.RandomIntInclusive(m_iTimeoutInSeconds, m_iTimeoutInSecondsMax);
			SCR_ScenarioFrameworkSystem.GetCallQueuePausable().CallLater(ExecuteActions, delay * 1000, false);
		}
		
		return super.InitOtherThings();
	}

	override void EOnDeactivate(IEntity owner)
	{
		RKN_MortarSystem system = RKN_MortarSystem.GetInstance();
		if (system)
			system.RemoveTarget(this);
		
		super.EOnDeactivate();
	}
	
	override protected void DrawDebugShape(bool draw)
	{
		if (!draw)
			return;
		
		Shape dbgShapeInner = null;
		Shape dbgShapeOuter = null;

		if (m_bDebugTargetSphere)
			dbgShapeInner = Shape.CreateSphere(
				m_iDebugShapeColor,
				ShapeFlags.TRANSP | ShapeFlags.DOUBLESIDE | ShapeFlags.NOZWRITE | ShapeFlags.ONCE | ShapeFlags.NOOUTLINE,
				GetOwner().GetOrigin(),
				m_iTargetRadius
			);

		if (m_bDebugIgnoreSphere)
			dbgShapeOuter = Shape.CreateSphere(
				m_iDebugOuterShapeColor,
				ShapeFlags.TRANSP | ShapeFlags.DOUBLESIDE | ShapeFlags.NOZWRITE | ShapeFlags.ONCE | ShapeFlags.NOOUTLINE,
				GetOwner().GetOrigin(),
				m_iTargetIgnoreRadius
			);
	}
	//------------------------------------------------------------------------------------------------
	
	bool IsSplash(float dist, RKN_MortarHitHandlerComponent shellHandler)
	{
		return dist < m_iTargetIgnoreRadius && shellHandler.m_eAmmoType == m_eAmmoType;
	}
	
	void Splash(vector hit, float dist)
	{
		Print("Hit " + GetOwner().GetName() + " " + dist + " from " + m_oObserverPosition);
		if (dist < m_iTargetRadius)
		{
			Print("On target!");
			if (m_bAdjustFire)
			{
				m_bAdjustFire = false;
				SetIntroSignalValues(m_sAdjustSounds.m_iAdjustIntroIndex, m_sAdjustSounds.m_iAdjustOutroIndex);
				PlayCorrection(m_sAdjustSounds.m_sAdjustOutroSoundEvent, m_sAdjustSounds.m_sAdjustOutroSubtitles);
			}
			m_iSplashesOnTarget++;
			if (++m_iSplashes >= m_iRequiredSplashes)
			{
				ExecuteActions();
			}
		}
		else if (m_bAdjustFire)
		{
			vector target = GetOwner().GetOrigin();
			vector observerTransform[4];
			m_oObserverPosition.GetWorldTransform(observerTransform);
			vector direction = vector.Direction(observerTransform[3], target);
			vector hitDirection = vector.Direction(target, hit);
			float scale = hitDirection.Length();
			float angle = direction.ToYaw();
			float newAngle = hitDirection.ToYaw() - angle;
			vector rotated = vector.FromYaw(newAngle);
			vector rotatedHit = { rotated[0] * scale, 0, rotated[2] * scale };
			//Print("Angle : " + angle);
			//Print("HitDi : " + hitDirection);
			//Print("Rotated : " + rotatedHit);
			Print("Off target");
			SetIntroSignalValues(m_sAdjustSounds.m_iAdjustIntroIndex, m_sAdjustSounds.m_iAdjustOutroIndex);
			PlayCorrection(m_sAdjustSounds.m_sAdjustIntroSoundEvent, m_sAdjustSounds.m_sAdjustIntroSubtitles);
			int onLineDirectionIndex = 0;
			int onLineDistanceIndex = 0;
			int offsetDirectionIndex = 0;
			int offsetDistanceIndex = 0;
			string subtitles = "";
			if (rotatedHit[2] > 0)
			{
				Print("OVER " + rotatedHit[2] + " meters");
				string distanceString = GetDistanceIndex(rotatedHit[2], onLineDistanceIndex);
				if (onLineDistanceIndex > 0)
				{
					onLineDirectionIndex = m_sAdjustSounds.m_iOverIndex;
					subtitles += m_sAdjustSounds.m_sOverSubtitles + " " + distanceString + " meters.";
				}
				
			}
			else
			{
				Print("SHORT " + -rotatedHit[2] + " meters");
				string distanceString = GetDistanceIndex(-rotatedHit[2], onLineDistanceIndex);
				if (onLineDistanceIndex > 0)
				{
					onLineDirectionIndex = m_sAdjustSounds.m_iShortIndex;
					subtitles += m_sAdjustSounds.m_sShortSubtitles + " " + distanceString + " meters.";
				}
			}
			if (rotatedHit[0] > 0)
			{
				Print("RIGHT " + rotatedHit[0] + " meters");
				string distanceString = GetDistanceIndex(rotatedHit[0], offsetDistanceIndex);
				if (offsetDistanceIndex > 0)
				{
					offsetDirectionIndex = m_sAdjustSounds.m_iRightIndex;
					subtitles += " " + m_sAdjustSounds.m_sRightSubtitles + " " + distanceString + " meters.";
				}
			}
			else
			{
				Print("LEFT " + -rotatedHit[0] + " meters");
				string distanceString = GetDistanceIndex(-rotatedHit[0], offsetDistanceIndex);
				if (offsetDistanceIndex > 0)
				{
					offsetDirectionIndex = m_sAdjustSounds.m_iLeftIndex;
					subtitles += " " + m_sAdjustSounds.m_sLeftSubtitles + " " + distanceString + " meters.";
				}
			}
			SetAdjustSignalValues(onLineDirectionIndex, onLineDistanceIndex, offsetDirectionIndex, offsetDistanceIndex);
			PlayCorrection(m_sAdjustSounds.m_sAdjustSoundEvent, subtitles);
		}
		else
		{
			if (++m_iSplashes >= m_iRequiredSplashes)
			{
				ExecuteActions();
			}
		}
		
	}
	
	private void ExecuteActions()
	{
		array<ref SCR_ScenarioFrameworkActionBase> actions;
		if (m_iSplashesOnTarget >= m_iRequiredSplashesOnTarget)
			actions = m_aActionsOnCompletion;
		else
			actions = m_aActionsOnFailure;
		
		foreach (SCR_ScenarioFrameworkActionBase action : actions)
		{
			action.OnActivate(GetOwner());
		}
		
		if (m_iTimeoutInSeconds > 0)
			SCR_ScenarioFrameworkSystem.GetCallQueuePausable().Remove(ExecuteActions);
		
		Deactivate(GetOwner());
	}
	
	private string GetDistanceIndex(float d, out int index)
	{
		string subtitles = "";
		if (d <= (m_iTargetRadius * 0.8))
		{
			return subtitles;
		}
		for (int i = 0; i < m_sAdjustSounds.m_aDistances.Count(); i++)
		{
			RKN_MortarTargetDistanceConfig config = m_sAdjustSounds.m_aDistances[i];
			if (d < (config.m_iDistance * 1.1) || i == (m_sAdjustSounds.m_aDistances.Count() - 1))
			{
				if (config.m_sSubtitles && !config.m_sSubtitles.IsEmpty())
					subtitles = config.m_sSubtitles;
				else
					subtitles = string.ToString(config.m_iDistance);
				index = config.m_iIndex;
				break;
			}
		}
		return subtitles;
	}
	
	private void SetAdjustSignalValues(int onLineDirectionIndex, int onLineDistanceIndex, int offsetDirectionIndex, int offsetDistanceIndex)
	{
		SCR_ScenarioFrameworkParam<IEntity> entityWrapper = SCR_ScenarioFrameworkParam<IEntity>.Cast(m_SoundActorGetter.Get());
		SignalsManagerComponent signalManager = SignalsManagerComponent.Cast(entityWrapper.GetValue().FindComponent(SignalsManagerComponent));
		signalManager.SetSignalValue(signalManager.FindSignal("OnLineDirection"), onLineDirectionIndex);
		signalManager.SetSignalValue(signalManager.FindSignal("OnLineDistance"), onLineDistanceIndex);
		signalManager.SetSignalValue(signalManager.FindSignal("OffsetDirection"), offsetDirectionIndex);
		signalManager.SetSignalValue(signalManager.FindSignal("OffsetDistance"), offsetDistanceIndex);
	}
	
	private void SetIntroSignalValues(int introIndex, int outroIndex)
	{
		SCR_ScenarioFrameworkParam<IEntity> entityWrapper = SCR_ScenarioFrameworkParam<IEntity>.Cast(m_SoundActorGetter.Get());
		SignalsManagerComponent signalManager = SignalsManagerComponent.Cast(entityWrapper.GetValue().FindComponent(SignalsManagerComponent));
		signalManager.SetSignalValue(signalManager.FindSignal("AdjustIntro"), introIndex);
		signalManager.SetSignalValue(signalManager.FindSignal("AdjustOutro"), outroIndex);
	}
	
	private void PlayCorrection(string soundEventName, string subtitle)
	{
		if (!soundEventName)
			return;
		SCR_ScenarioFrameworkParam<IEntity> entityWrapper = SCR_ScenarioFrameworkParam<IEntity>.Cast(m_SoundActorGetter.Get());
		SCR_VoiceoverSystem.GetInstance().RegisterActor(entityWrapper.GetValue());
		SCR_VoiceoverSystem.GetInstance().PlayCustomLine(soundEventName, subtitle, entityWrapper.GetValue(), string.Empty, false);
	}
}
