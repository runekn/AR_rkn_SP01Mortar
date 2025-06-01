[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class RKN_Connect4BoardComponentClass : ScriptComponentClass
{
}

class RKN_Connect4BoardComponent : ScriptComponent
{
	static int HEIGHT = 6;
	static int WIDTH = 7;
	
	[Attribute()]
	ref PointInfo m_LowerLeftSlot;
	
	[Attribute()]
	ref PointInfo m_UpperRightSlot;
	
	[Attribute(params: "et")]
	ResourceName m_Player1;
	
	[Attribute(params: "et")]
	ResourceName m_Player2;
	
	IEntity m_aStones[7][6];
	int m_aState[7][6];
	
	ref ScriptInvoker m_OnPlayerMove = new ScriptInvoker();
	ref ScriptInvoker m_OnReset = new ScriptInvoker();
	
	int m_iWinner;
	
	void PlaceStone(int x, int player)
	{
		PlaceStoneImpl(x, player);
		m_OnPlayerMove.Invoke();
	}
	
	void Reset()
	{
		m_iWinner = 0;
		for (int x; x < HEIGHT; x++)
		{
			for (int y; y < WIDTH; y++)
			{
				m_aState[x][y] = 0;
				RplComponent.DeleteRplEntity(m_aStones[x][y], false);
				m_aStones[x][y] = null;
			}
		}
		m_OnReset.Invoke();
	}
	
	void PlaceStoneImpl(int x, int player)
	{
		if (!CanPlace(x))
			return;
		Print("Player " + player + " places stone at " + x);
		for (int y; y < HEIGHT; y++)
		{
			if (m_aState[x][y] == 0)
			{
				m_aState[x][y] = player;
				SpawnStoneAt(player, x, y);
				break;
			}
		}
		m_iWinner = GetWinner(m_aState);
		if (m_iWinner > 0)
		{
			Print("The winner is " + player)
		}
	}
	
	bool CanPlace(int x)
	{
		return m_aState[x][HEIGHT-1] == 0;
	}
	
	private int GetWinner(int state[7][6])
	{
		for (int x; x < 7; x++)
		{
			for (int y; y < 6; y++)
			{
				int p = state[x][y];
				if (p == 0)
					continue;
				// Vertical (counting down)
				if (y > 2 && state[x][y-1] == p && state[x][y-2] == p && state[x][y-3] == p)
					return p;
				// Horizontal (counting left)
				if (x > 2 && state[x-1][y] == p && state[x-2][y] == p && state[x-3][y] == p)
					return p;
				// Diagonal (counting left and down)
				if (x > 2 && y > 2 && state[x-1][y-1] == p && state[x-2][y-2] == p && state[x-3][y-3] == p)
					return p;
				// Diagonal (counting left and up)
				if (x > 2 && y < 4 && state[x-1][y+1] == p && state[x-2][y+2] == p && state[x-3][y+3] == p)
					return p;
			}
		}
		return 0;
	}
	
	private void SpawnStoneAt(int player, int x, int y)
	{
		ResourceName prefab = m_Player1;
		if (player == 2)
			prefab = m_Player2;
		Resource p = Resource.Load(prefab);
		vector pos = GetSlotPosition(x, y);
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform[3] = pos;
		params.Parent = GetOwner();
		IEntity entity = GetGame().SpawnEntityPrefab(p, null, params);
		m_aStones[x][y] = entity;
	}
	
	private vector GetSlotPosition(int x, int y)
	{
		vector lowerLeft = GetPositionOfPoint(m_LowerLeftSlot);
		vector upperRight = GetPositionOfPoint(m_UpperRightSlot);
		float xAxisLength = upperRight[2] - lowerLeft[2];
		float xStep = xAxisLength / 6;
		float xPos = xStep * x + lowerLeft[2];
		float yAxisLength = upperRight[1] - lowerLeft[1];
		float yStep = yAxisLength / 5;
		float yPos = yStep * y + lowerLeft[1];
		return {0, yPos, xPos};
	}
	
	private vector GetPositionOfPoint(PointInfo point)
	{
		vector transform[4];
		point.GetLocalTransform(transform);
		return transform[3];
	}
}
