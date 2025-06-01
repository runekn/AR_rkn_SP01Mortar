[BaseContainerProps(configRoot: true)]
class RKN_MortarTargetVoiceLines
{
	[Attribute()]
	string m_sAdjustIntro;
	
	[Attribute()]
	string m_sAdjustIntroSubtitles;
	
	[Attribute()]
	string m_sCorrectionsSoundEvent;
	
	[Attribute(defvalue: "1")]
	int m_iOverIndex;
	
	[Attribute(defvalue: "Drop")]
	string m_sOverSubtitles;
	
	[Attribute(defvalue: "2")]
	int m_iShortIndex;
	
	[Attribute(defvalue: "Add")]
	string m_sShortSubtitles;
	
	[Attribute(defvalue: "1")]
	int m_iLeftIndex;
	
	[Attribute(defvalue: "Right")] // opposite since they are directions for next shot
	string m_sLeftSubtitles;
	
	[Attribute(defvalue: "2")]
	int m_iRightIndex;
	
	[Attribute(defvalue: "Left")] // opposite since they are directions for next shot
	string m_sRightSubtitles;
	
	[Attribute(desc: "Must be in ascending order")]
	ref array<ref RKN_MortarTargetDistanceConfig> m_aDistances;
	
	[Attribute()]
	string m_sOnTarget;
	
	[Attribute()]
	string m_sOnTargetSubtitles;
}

[BaseContainerProps(configRoot: false)]
class RKN_MortarTargetDistanceConfig
{
	[Attribute()]
	int m_iDistance;
	[Attribute()]
	int m_iIndex;
	[Attribute(desc: "Leave empty to use distance")]
	string m_sSubtitles;
}