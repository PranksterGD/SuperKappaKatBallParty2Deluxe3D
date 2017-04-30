
#include "pch.h"
#include "ScoreAction.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ScoreAction)

const string ScoreAction::sScoreEventSubtype = "score";
const string ScoreAction::sPlayerIDKey = "playerID";

ScoreAction::ScoreAction(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ScoreAction::~ScoreAction()
{
}

void ScoreAction::Update(WorldState& worldState)
{
	UNREFERENCED_PARAMETER(worldState);

	if(Datum* playerIDDatum = Search(sPlayerIDKey))
	{
		// assert that playerID is valid
		int32_t playerID = playerIDDatum->Get<int32_t&>();
		assert(playerID >= 0 && playerID < NUM_PLAYERS);

		if(Datum* scoresDatum = Search(HUD::sScoresKey))
		{
			// get the scores from HUD and update using player ID
			static_cast<Score*>(scoresDatum->Get<RTTI*&>(playerID))->UpdateScore();

			// get high score
			uint32_t highScore = 0;
			for(uint32_t i = 0; i < scoresDatum->Size(); ++i)
			{
				uint32_t currScore = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i))->GetScore();
				if(highScore < currScore)
				{
					highScore = currScore;
				}
			}

			// crown leader
			for(uint32_t i = 0; i < scoresDatum->Size(); ++i)
			{
				Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
				score->SetIsWinning(score->GetScore() == highScore);
			}
		}
	}
}