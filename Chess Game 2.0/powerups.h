#ifndef POWERUPS_H
#define POWERUPS_H

typedef enum {
    TELEPORTATION,
    DOUBLE_MOVE,
    SWAP_PIECES,
    SHIELD,
    MOVE_BOOST
} PowerUp;

void power_up_triggered(void);
void activate_power_up(PowerUp power);

#endif
