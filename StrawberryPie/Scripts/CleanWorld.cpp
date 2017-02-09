#include <Common.h>
#include <Scripts/CleanWorld.h>

#include <shv/main.h>
#include <shv/natives.h>

#include <Enums/GameControl.h>

void scriptCleanWorld()
{
	while (true) {
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) {
			continue;
		}

		PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(player, 0.0f);
		PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
		PLAYER::SET_AUTO_GIVE_PARACHUTE_WHEN_ENTER_PLANE(player, false);
		PLAYER::_0xD2B315B6689D537D(player, false);

		if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 0, false);
		}

		VEHICLE::SET_RANDOM_BOATS(false);
		VEHICLE::SET_RANDOM_TRAINS(false);
		VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		VEHICLE::SET_NUMBER_OF_PARKED_VEHICLES(-1);
		VEHICLE::SET_ALL_LOW_PRIORITY_VEHICLE_GENERATORS_ACTIVE(false);
		VEHICLE::SET_FAR_DRAW_VEHICLES(false);
		VEHICLE::_DISPLAY_DISTANT_VEHICLES(false);

		PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);
		PED::SET_CAN_ATTACK_FRIENDLY(playerPed, true, true);
		PED::SET_CAN_ATTACK_FRIENDLY(playerPed, true, true);
		PED::SET_PED_CAN_BE_TARGETTED(playerPed, true);

		MOBILE::DESTROY_MOBILE_PHONE();

		CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::EnterCheatCode, true);
		//CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::FrontendPause, true);
		//CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::FrontendPauseAlternate, true);
		//CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::FrontendSocialClub, true);
		//CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::FrontendSocialClubSecondary, true);
		CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::SpecialAbility, true);
		CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::SpecialAbilityPC, true);
		CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::SpecialAbilitySecondary, true);
		CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::CharacterWheel, true);
		CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::Phone, true);
		CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::Duck, true);

		if (PED::GET_PED_PARACHUTE_STATE(playerPed) == 2) {
			CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::Aim, true);
			CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::Attack, true);
		}

		if (PED::IS_PED_RAGDOLL(playerPed)) {
			CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::Attack, true);
			CONTROLS::DISABLE_CONTROL_ACTION(0, GameControl::Attack2, true);
		}

		UI::HIDE_HELP_TEXT_THIS_FRAME();
		UI::DISPLAY_CASH(false);

		DECISIONEVENT::SUPPRESS_SHOCKING_EVENTS_NEXT_FRAME();
		DECISIONEVENT::SUPPRESS_AGITATION_EVENTS_NEXT_FRAME();

		if (GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS()) {
			GAMEPLAY::CANCEL_STUNT_JUMP();
		}

		WAIT(0);
	}
}
