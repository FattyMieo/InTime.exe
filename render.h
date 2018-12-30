#include <string>

using namespace std;

static char LINE[3][3][3][3] =
{
	//Top: 0
	{
		//Bottom: 0
		{
			//Left: 0
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				' '
			},
			//Left: 1
			{
				//Right: 0
				' ',
				//Right: 1
				char(196),
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				char(205)
			}
		},
		//Bottom: 1
		{
			//Left: 0
			{
				//Right: 0
				' ',
				//Right: 1
				char(218),
				//Right: 2
				char(213)
			},
			//Left: 1
			{
				//Right: 0
				char(191),
				//Right: 1
				char(194),
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				char(184),
				//Right: 1
				' ',
				//Right: 2
				char(209)
			}
		},
		//Bottom: 2
		{
			//Left: 0
			{
				//Right: 0
				' ',
				//Right: 1
				char(214),
				//Right: 2
				char(201)
			},
			//Left: 1
			{
				//Right: 0
				char(183),
				//Right: 1
				char(210),
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				char(187),
				//Right: 1
				' ',
				//Right: 2
				char(203)
			}
		}
	},
	//Top: 1
	{
		//Bottom: 0
		{
			//Left: 0
			{
				//Right: 0
				' ',
				//Right: 1
				char(192),
				//Right: 2
				char(212)
			},
			//Left: 1
			{
				//Right: 0
				char(217),
				//Right: 1
				char(193),
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				char(190),
				//Right: 1
				' ',
				//Right: 2
				char(207)
			}
		},
		//Bottom: 1
		{
			//Left: 0
			{
				//Right: 0
				char(179),
				//Right: 1
				char(195),
				//Right: 2
				char(198)
			},
			//Left: 1
			{
				//Right: 0
				char(180),
				//Right: 1
				char(197),
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				char(181),
				//Right: 1
				' ',
				//Right: 2
				char(216)
			}
		},
		//Bottom: 2
		{
			//Left: 0
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				' '
			},
			//Left: 1
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				' '
			}
		}
	},
	//Top: 2
	{
		//Bottom: 0
		{
			//Left: 0
			{
				//Right: 0
				' ',
				//Right: 1
				char(211),
				//Right: 2
				char(200)
			},
			//Left: 1
			{
				//Right: 0
				char(189),
				//Right: 1
				char(208),
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				char(188),
				//Right: 1
				' ',
				//Right: 2
				char(202)
			}
		},
		//Bottom: 1
		{
			//Left: 0
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				' '
			},
			//Left: 1
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				' ',
				//Right: 1
				' ',
				//Right: 2
				' '
			}
		},
		//Bottom: 2
		{
			//Left: 0
			{
				//Right: 0
				char(186),
				//Right: 1
				char(199),
				//Right: 2
				char(204)
			},
			//Left: 1
			{
				//Right: 0
				char(182),
				//Right: 1
				char(215),
				//Right: 2
				' '
			},
			//Left: 2
			{
				//Right: 0
				char(185),
				//Right: 1
				' ',
				//Right: 2
				char(206)
			}
		}
	}
};

static char FBLOCK[4] =
{
	char(176), char(177), char(178), char(219)
};

static char HBLOCK[4]
{
	char(223), char(220), char(221), char(222)
};

static char SQUARE = char(254);

static char DARROW[2]
{
	char(174), char(175)
};

static char EARROW[2]
{
	char(243), char(242)
};

static char DIV = char(246);

//Format: NAME_LENGTH_HEIGHT

static string MAIN_TITLE_34_4[4] =
{
	" __  __ _    ____  __  _  _  ____ ",
	"(  )(  ( \\  (_  _)(  )( \\/ )(  __)",
	" )( /    /    )(   )( / \\/ \\ ) _) ",
	"(__)\\_)__)   (__) (__)\\_)(_/(____)"
};

static string NUMBER_6_3[17][3] =
{
	{
		"    \\ ",
		"  (  |",
		" \\__/ "
	},
	{
		" __  |",
		"     |",
		"   __|"
	},
	{
		" __  )",
		"    / ",
		" ____|"
	},
	{
		" ___ /",
		"  __ \\",
		" ____/"
	},
	{
		"  | | ",
		" __ _|",
		"   _| "
	},
	{
		"  ___|",
		" ___ \\",
		" ____/"
	},
	{
		"   /  ",
		"   _ \\",
		" \\___/"
	},
	{
		" __  /",
		"    / ",
		"  _/  "
	},
	{
		"   _ )",
		"   _ \\",
		" \\___/"
	},
	{
		"   _ \\",
		" \\_  /",
		"   _/ "
	},
	{
		"   \\  ",
		"  _ \\ ",
		"_/  _\\"
	},
	{
		"   _ )",
		"   _ \\",
		" |___/"
	},
	{
		"   __\\",
		"  (   ",
		" \\___/"
	},
	{
		"  __\\ ",
		"  |  |",
		" ___/ "
	},
	{
		"  ___|",
		"  __| ",
		" ____|"
	},
	{
		"  ___|",
		"  __| ",
		" _|   "
	},
	{
		"   _) ",
		"      ",
		"   _) "
	}
};

static string TIMER_WINDOW_50_5[5] =
{
	string()+LINE[0][1][0][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][2][1][0],
	string()+LINE[1][1][0][0]+"                                                "+LINE[2][2][0][0],
	string()+LINE[1][1][0][0]+"                                                "+LINE[2][2][0][0],
	string()+LINE[1][1][0][0]+"                                                "+LINE[2][2][0][0],
	string()+LINE[1][0][0][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[0][0][2][2]+LINE[2][0][2][0]
};

static string PLAYER_6_3[3] =
{
	string()+LINE[0][1][0][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][0][1][1]+LINE[0][1][1][0],
	string()+LINE[1][1][0][0]+FBLOCK[3]+HBLOCK[2]+HBLOCK[3]+FBLOCK[3]+LINE[1][1][0][0],
	string()+LINE[1][0][0][1]+LINE[2][2][1][0]+LINE[0][2][1][1]+LINE[0][2][1][1]+LINE[2][2][0][1]+LINE[1][0][1][0]
};

static string BEAM_CANNON_6_3[3] =
{
	string()+' '+LINE[0][2][1][0]+' '+HBLOCK[1]+HBLOCK[1]+LINE[0][1][1][0],
	string()+DARROW[1]+LINE[2][2][0][2]+LINE[0][0][1][1]+LINE[1][1][1][0]+FBLOCK[3]+HBLOCK[2],
	string()+' '+LINE[2][0][1][0]+' '+HBLOCK[0]+HBLOCK[0]+LINE[1][0][1][0]
};

static string ID_WALL_6_3[3] =
{
	string()+LINE[0][2][0][1]+LINE[0][1][0][1]+FBLOCK[1]+FBLOCK[2]+LINE[0][1][1][0]+LINE[0][1][1][0],
	string()+LINE[1][1][0][1]+FBLOCK[2]+FBLOCK[0]+FBLOCK[0]+FBLOCK[2]+LINE[1][1][1][0],
	string()+LINE[1][0][0][1]+LINE[1][0][0][1]+FBLOCK[2]+FBLOCK[1]+LINE[1][0][1][0]+LINE[2][0][1][0]
};

static string BETTLE_6_3[3] =
{
	string()+' '+' '+HBLOCK[3]+HBLOCK[2]+' '+' ',
	string()+' '+LINE[2][0][0][1]+FBLOCK[3]+FBLOCK[3]+LINE[2][0][1][0]+' ',
	string()+' '+LINE[0][1][0][1]+HBLOCK[0]+HBLOCK[0]+LINE[0][1][1][0]+' '
};

static string SHOCKER_6_3[3] =
{
	string()+LINE[0][2][0][1]+LINE[0][0][1][1]+'>'+'>'+FBLOCK[0]+FBLOCK[0],
	string()+LINE[2][0][1][1]+' '+EARROW[0]+EARROW[1]+' '+LINE[0][2][1][1],
	string()+FBLOCK[0]+FBLOCK[0]+'<'+'<'+LINE[0][0][1][1]+LINE[2][0][1][0]
};

static string BATTERING_RAM_6_3[3] =
{
	string()+' '+HBLOCK[1]+HBLOCK[1]+HBLOCK[1]+HBLOCK[1]+' ',
	string()+' '+LINE[2][2][0][1]+DIV+'0'+LINE[2][2][1][0]+' ',
	string()+' '+HBLOCK[0]+HBLOCK[0]+HBLOCK[0]+HBLOCK[0]+' '
};

static string TURRET_6_3[3] =
{
	string()+' '+LINE[0][1][1][0]+' '+HBLOCK[1]+HBLOCK[1]+LINE[0][1][1][0],
	string()+DARROW[0]+LINE[2][2][0][2]+LINE[0][0][2][2]+LINE[1][1][1][0]+FBLOCK[0]+LINE[2][2][1][1],
	string()+' '+LINE[1][0][1][0]+' '+HBLOCK[0]+HBLOCK[0]+LINE[1][0][1][0]
};

static string GHOST_6_3[3] =
{
	string()+' '+LINE[0][1][1][0]+' '+HBLOCK[1]+HBLOCK[1]+LINE[0][1][1][0],
	string()+DARROW[0]+LINE[2][2][0][2]+LINE[0][0][2][2]+LINE[1][1][1][0]+SQUARE+LINE[2][2][1][1],
	string()+' '+LINE[1][0][1][0]+' '+HBLOCK[0]+HBLOCK[0]+LINE[1][0][1][0]
};

static string BLACK_ICE_6_3[3] =
{
	string()+LINE[0][1][0][1]+LINE[0][1][0][1]+FBLOCK[2]+FBLOCK[1]+LINE[0][1][1][0]+LINE[0][1][1][0],
	string()+LINE[1][1][0][0]+FBLOCK[1]+FBLOCK[3]+FBLOCK[3]+FBLOCK[1]+LINE[1][1][0][0],
	string()+LINE[1][0][0][1]+LINE[1][0][0][1]+FBLOCK[1]+FBLOCK[2]+LINE[1][0][1][0]+LINE[1][0][1][0]
};

static string ISO_GENERATOR_6_3[3] =
{
	string()+HBLOCK[1]+HBLOCK[1]+HBLOCK[1]+HBLOCK[1]+HBLOCK[1]+HBLOCK[1],
	string()+FBLOCK[3]+LINE[2][2][2][2]+LINE[2][2][2][2]+LINE[2][2][2][2]+LINE[2][2][2][2]+FBLOCK[3],
	string()+HBLOCK[0]+HBLOCK[0]+HBLOCK[0]+HBLOCK[0]+HBLOCK[0]+HBLOCK[0]
};
