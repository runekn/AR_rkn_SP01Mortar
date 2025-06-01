[BaseContainerProps(configRoot: true)]
class RKN_MortarTargetVoiceLines
{
	
	[Attribute("SOUND_FIREMISSION_ADJUST_INTRO")]
	string m_sAdjustIntroSoundEvent;
	
	[Attribute("0")]
	int m_iAdjustIntroIndex;
	
	[Attribute()]
	string m_sAdjustIntroSubtitles;
	
	[Attribute("SOUND_FIREMISSION_ADJUST_OUTRO")]
	string m_sAdjustOutroSoundEvent;
	
	[Attribute("0")]
	int m_iAdjustOutroIndex;
	
	[Attribute()]
	string m_sAdjustOutroSubtitles;
	
	[Attribute("SOUND_FIREMISSION_ADJUST")]
	string m_sAdjustSoundEvent;
	
	[Attribute(defvalue: "1", params: "0 inf")]
	int m_iOverIndex;
	
	[Attribute(defvalue: "Drop")]
	string m_sOverSubtitles;
	
	[Attribute(defvalue: "2", params: "0 inf")]
	int m_iShortIndex;
	
	[Attribute(defvalue: "Add")]
	string m_sShortSubtitles;
	
	[Attribute(defvalue: "1", params: "0 inf")]
	int m_iLeftIndex;
	
	[Attribute(defvalue: "Right")] // opposite since they are directions for next shot
	string m_sLeftSubtitles;
	
	[Attribute(defvalue: "2", params: "0 inf")]
	int m_iRightIndex;
	
	[Attribute(defvalue: "Left")] // opposite since they are directions for next shot
	string m_sRightSubtitles;
	
	[Attribute(desc: "Must be in ascending order")]
	ref array<ref RKN_MortarTargetDistanceConfig> m_aDistances;
}

[BaseContainerProps(configRoot: false)]
class RKN_MortarTargetDistanceConfig
{
	[Attribute(params: "0 inf")]
	int m_iDistance;
	[Attribute()]
	int m_iIndex;
	[Attribute(desc: "Leave empty to use distance")]
	string m_sSubtitles;
}