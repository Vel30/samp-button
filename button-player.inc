#if defined _INC_button_player
  #endinput
#endif

#define _INC_button_player

#include <YSI_Coding\y_hooks>

static Button: gPlayerPressedButtonID[MAX_PLAYERS];

new Iterator: PlayerNearestButton[MAX_PLAYERS]<MAX_BUTTONS>;

forward OnPlayerEnterButtonArea(playerid, Button: buttonid);
forward OnPlayerLeaveButtonArea(playerid, Button: buttonid);
forward OnPlayerPressButton(playerid, Button: buttonid);
forward OnPlayerReleaseButton(playerid, Button: buttonid);

#define ALS_DO_PlayerEnterButtonArea<%0> %0<PlayerEnterButtonArea, dd>(playerid, Button: buttonid)
#define ALS_DO_PlayerLeaveButtonArea<%0> %0<PlayerLeaveButtonArea, dd>(playerid, Button: buttonid)
#define ALS_DO_PlayerPressButton<%0> %0<PlayerPressButton, dd>(playerid, Button: buttonid)
#define ALS_DO_PlayerReleaseButton<%0> %0<PlayerReleaseButton, dd>(playerid, Button: buttonid)

hook OnScriptInit() {
  Iter_InitAndClear(PlayerNearestButton);
}

hook OnPlayerConnect(playerid) {
  gPlayerPressedButtonID[playerid] = INVALID_BUTTON_ID;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
  if (Iter_IsEmpty(PlayerNearestButton[playerid])) {
    return;
  }

  if (!IsPlayerInAnyDynamicArea(playerid)) {
    Iter_Clear(PlayerNearestButton[playerid]);
    return;
  }

  if (newkeys & BUTTON_KEY) {
    new Button: buttonid = Button: Iter_Index(PlayerNearestButton[playerid], 0);

    gPlayerPressedButtonID[playerid] = buttonid;

    call OnPlayerPressButton(playerid, _: buttonid);
    return;
  }

  if (oldkeys & BUTTON_KEY) {
    call OnPlayerReleaseButton(playerid, _: gPlayerPressedButtonID[playerid]);

    gPlayerPressedButtonID[playerid] = INVALID_BUTTON_ID;
  }
}

hook OnPlayerEnterDynArea(playerid, STREAMER_TAG_AREA: areaid) {
  new data[2];

  Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data);

  if (data[0] != BUTTON_STREAMER_AREA_ID) {
    return;
  }

  if (!Button_IsValid(Button: data[1])) {
    return;
  }

  Iter_Add(PlayerNearestButton[playerid], data[1]);

  call OnPlayerEnterButtonArea(playerid, data[1]);
}

hook OnPlayerLeaveDynArea(playerid, STREAMER_TAG_AREA: areaid) {
  new data[2];

  Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data);

  if (data[0] != BUTTON_STREAMER_AREA_ID) {
    return;
  }

  if (!Button_IsValid(Button: data[1])) {
    return;
  }

  call OnPlayerLeaveButtonArea(playerid, data[1]);

  Iter_Remove(PlayerNearestButton[playerid], data[1]);
}
