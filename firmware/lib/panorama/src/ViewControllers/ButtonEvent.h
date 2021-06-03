//
// Created by Magnus Nordlander on 2021-02-13.
//

#ifndef PANORAMA_BUTTONEVENT_H
#define PANORAMA_BUTTONEVENT_H

/**
 * Bitmask event. An event can be both a let up and a press-and-hold
 */
enum ButtonEvent {
    BUTTON_INACTIVE,
    BUTTON_PRESS,
    BUTTON_HOLD,
    BUTTON_LET_UP,
    BUTTON_PRESS_AND_HOLD_LET_UP,
};

#endif //PANORAMA_BUTTONEVENT_H
