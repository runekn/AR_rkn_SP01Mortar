[BaseContainerProps(configRoot: true)]
class RKN_MortarTargetVoiceLines
{
	[Attribute()]
	string m_sAdjustIntro;
	
	[Attribute()]
	string m_sAdjustIntroSubtitles;
	
	[Attribute()]
	string m_sOver;
	
	[Attribute(defvalue: "Add")]
	string m_sOverSubtitles;
	
	[Attribute()]
	string m_sShort;
	
	[Attribute(defvalue: "Drop")]
	string m_sShortSubtitles;
	
	[Attribute()]
	string m_sLeft;
	
	[Attribute(defvalue: "Right")] // opposite since they are directions for next shot
	string m_sLeftSubtitles;
	
	[Attribute()]
	string m_sRight;
	
	[Attribute(defvalue: "Left")] // opposite since they are directions for next shot
	string m_sRightSubtitles;
	
	[Attribute()]
	string m_sRange300Meter;
	
	[Attribute()]
	string m_sRange200Meter;
	
	[Attribute()]
	string m_sRange100Meter;
	
	[Attribute()]
	string m_sRange50Meter;
	
	[Attribute()]
	string m_sOnTarget;
	
	[Attribute()]
	string m_sOnTargetSubtitles;
}
