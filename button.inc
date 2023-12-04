#if defined _INC_button
  #endinput
#endif

#define _INC_button

#include <streamer>

#include <YSI_Data\y_iterate>

#define CreateButton Button_Create
#define DestroyButton Button_Destroy
#define IsValidButton Button_IsValid
#define SetButtonPos Button_SetPos
#define SetButtonSize Button_SetSize
#define SetButtonVirtualWorld Button_SetVirtualWorld
#define SetButtonInterior Button_SetInterior
#define SetButtonExtraData Button_SetExtraData
#define SetButtonExtraDataSize Button_SetExtraDataSize
#define GetButtonPos Button_GetPos
#define GetButtonSize Button_GetSize
#define GetButtonVirtualWorld Button_GetVirtualWorld
#define GetButtonInterior Button_GetInterior
#define GetButtonExtraData Button_GetExtraData
#define GetButtonExtraDataSize Button_GetExtraDataSize
#define GetButtonAreaID Button_GetAreaID

#if !defined MAX_BUTTONS
  #define MAX_BUTTONS 8192
#endif

#if !defined MAX_BUTTON_EXTRA_DATA
  #define MAX_BUTTON_EXTRA_DATA 256
#endif

#if !defined BUTTON_STREAMER_AREA_ID
  #define BUTTON_STREAMER_AREA_ID cellmax
#endif

#if !defined BUTTON_KEY
  #define BUTTON_KEY KEY_SECONDARY_ATTACK
#endif

const Button: INVALID_BUTTON_ID = Button: cellmin;

static enum E_BUTTON_DATA {
  E_BUTTON_EXTRA_DATA[MAX_BUTTON_EXTRA_DATA],
  E_BUTTON_EXTRA_DATA_SIZE,
  STREAMER_TAG_AREA: E_BUTTON_AREA_ID
};

static gButtonData[MAX_BUTTONS][E_BUTTON_DATA];

new Iterator: Button<MAX_BUTTONS>;

forward Button: Button_Create(Float: x, Float: y, Float: z, Float: size, worldid = 0, interiorid = 0);
forward bool: Button_Destroy(Button: buttonid);
forward bool: Button_IsValid(Button: buttonid);
forward bool: Button_SetPos(Button: buttonid, Float: x, Float: y, Float: z);
forward bool: Button_SetSize(Button: buttonid, Float: size);
forward bool: Button_SetVirtualWorld(Button: buttonid, worldid);
forward bool: Button_SetInterior(Button: buttonid, interiorid);
forward Button_SetExtraData(Button: buttonid, const data[], size = sizeof(data));
forward Button_SetExtraDataSize(Button: buttonid, size);
forward bool: Button_GetPos(Button: buttonid, &Float: x, &Float: y, &Float: z);
forward Float: Button_GetSize(Button: buttonid);
forward Button_GetVirtualWorld(Button: buttonid);
forward Button_GetInterior(Button: buttonid);
forward bool: Button_GetExtraData(Button: buttonid, data[]);
forward Button_GetExtraDataSize(Button: buttonid);
forward STREAMER_TAG_AREA: Button_GetAreaID(Button: buttonid);

stock Button: Button_Create(Float: x, Float: y, Float: z, Float: size, worldid = 0, interiorid = 0) {
  new id = Iter_Free(Button);

  if (id == ITER_NONE) {
    return INVALID_BUTTON_ID;
  }

  gButtonData[id][E_BUTTON_AREA_ID] = CreateDynamicSphere(x, y, z, size, worldid, interiorid);

  new data[2];

  data[0] = BUTTON_STREAMER_AREA_ID;
  data[1] = id;

  Streamer_SetArrayData(STREAMER_TYPE_AREA, gButtonData[id][E_BUTTON_AREA_ID], E_STREAMER_EXTRA_ID, data);

  Iter_Add(Button, id);
  return Button: id;
}

stock bool: Button_Destroy(Button: buttonid) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  Iter_Remove(Button, _: buttonid);

  for (new i; i < gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA_SIZE]; i++) {
    gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA][i] = cellmin;
  }

  gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA_SIZE] = 0;

  DestroyDynamicArea(gButtonData[_: buttonid][E_BUTTON_AREA_ID]);
  return true;
}

stock bool: Button_IsValid(Button: buttonid) {
  return Iter_Contains(Button, _: buttonid);
}

stock bool: Button_SetPos(Button: buttonid, Float: x, Float: y, Float: z) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  Streamer_SetItemPos(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], x, y, z);
  return true;
}

stock bool: Button_SetSize(Button: buttonid, Float: size) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  Streamer_SetFloatData(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], E_STREAMER_SIZE, size);
  return true;
}

stock bool: Button_SetVirtualWorld(Button: buttonid, worldid) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  Streamer_SetFloatData(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], E_STREAMER_WORLD_ID, worldid);
  return true;
}

stock bool: Button_SetInterior(Button: buttonid, interiorid) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  Streamer_SetFloatData(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], E_STREAMER_INTERIOR_ID, interiorid);
  return true;
}

stock Button_SetExtraData(Button: buttonid, const data[], size = sizeof(data)) {
  if (!Button_IsValid(buttonid)) {
    return 0;
  }

  if (size > MAX_BUTTON_EXTRA_DATA) {
    return -1;
  }

  gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA_SIZE] = 0;

  for (new i; i < size; i++) {
    gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA][gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA_SIZE]++] = data[i];
  }
  return 1;
}

stock Button_SetExtraDataSize(Button: buttonid, size) {
  if (!Button_IsValid(buttonid)) {
    return 0;
  }

  if (size > MAX_BUTTON_EXTRA_DATA) {
    return -1;
  }

  gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA_SIZE] = size;
  return 1;
}

stock bool: Button_GetPos(Button: buttonid, &Float: x, &Float: y, &Float: z) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  Streamer_GetItemPos(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], x, y, z);
  return true;
}

stock Float: Button_GetSize(Button: buttonid) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  new Float: size;

  Streamer_GetFloatData(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], E_STREAMER_SIZE, size);
  return size;
}

stock Button_GetVirtualWorld(Button: buttonid) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  new worldid;

  Streamer_GetFloatData(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], E_STREAMER_WORLD_ID, worldid);
  return worldid;
}

stock Button_GetInterior(Button: buttonid) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  new interiorid;

  Streamer_GetFloatData(STREAMER_TYPE_AREA, gButtonData[_: buttonid][E_BUTTON_AREA_ID], E_STREAMER_INTERIOR_ID, interiorid);
  return interiorid;
}

stock bool: Button_GetExtraData(Button: buttonid, data[]) {
  if (!Button_IsValid(buttonid)) {
    return false;
  }

  for (new i; i < gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA_SIZE]; i++) {
    data[i] = gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA][i];
  }
  return true;
}

stock Button_GetExtraDataSize(Button: buttonid) {
  return !Button_IsValid(buttonid) ? 0 : gButtonData[_: buttonid][E_BUTTON_EXTRA_DATA_SIZE];
}

stock STREAMER_TAG_AREA: Button_GetAreaID(Button: buttonid) {
  return !Button_IsValid(buttonid) ? INVALID_STREAMER_ID : gButtonData[_: buttonid][E_BUTTON_AREA_ID];
}

#include "button-player"
